// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*
 
    Modified from original source here:
 
    https://github.com/davisking/dlib/blob/master/examples/dnn_mmod_find_cars2_ex.cpp
 
    This example shows how to run a CNN based eye detector using dlib.  The
    example loads a pretrained model and uses it to find eyes in an image.
    It does not rely on a first stage face detector step.  This can be useful
    in case where the iamges contain partial faces due to FOV, cropping or occlusion.
 
    The model used by this example was trained by dnn_mmod_train_find_things_ex.cpp
    example.  Also, since this is a CNN, you really should use a GPU to get the
    best execution speed.  For instance, when run on a NVIDIA 1080ti, this detector 
    runs at 98fps when run on the provided test image.  That's more than an order 
    of magnitude faster than when run on the CPU.

    Users who are just learning about dlib's deep learning API should read
    the dnn_introduction_ex.cpp and dnn_introduction2_ex.cpp examples to learn
    how the API works.  For an introduction to the object detection method you
    should read dnn_mmod_ex.cpp.

*/


#include <iostream>
#include <dlib/dnn.h>
#include <dlib/image_io.h>
#include <dlib/image_processing.h>
#include <dlib/cmd_line_parser.h>

#if !defined(DLIB_NO_GUI_SUPPORT)
#  include <dlib/gui_widgets.h>
#endif

using namespace std;
using namespace dlib;

#include "dnn_mmod_sample_detector.h"

// https://stackoverflow.com/a/1567703
class Line
{
    std::string data;
    
public:
    friend std::istream& operator>>(std::istream& is, Line& l)
    {
        std::getline(is, l.data);
        return is;
    }
    operator std::string() const { return data; }
};


static void expand(const std::string& filename, std::vector<std::string>& filenames)
{
    // Create input file list (or single image)
    filenames = { filename };
    if (filename.find(".txt") != std::string::npos)
    {
        std::ifstream ifs(filename);
        if (ifs)
        {
            filenames.clear();
            std::copy(std::istream_iterator<Line>(ifs), std::istream_iterator<Line>(), std::back_inserter(filenames));
        }
        else
        {
            throw std::runtime_error("Unable to open file: " + filename);
        }
    }
}

static std::string basename(const std::string& name, const std::string& ext=".")
{
    size_t pos = name.rfind("/");
    
    if (pos != std::string::npos)
    {
        pos += 1;
    }
    else
    {
        pos = 0;
    }
    
    std::string base = name.substr(pos);
    return base.substr(0, std::min(base.size(), base.rfind(ext)));
};

// ----------------------------------------------------------------------------------------

int main(int argc, char **argv) try
{
    command_line_parser parser;
    parser.add_option("h", "Display this help message.");
    parser.add_option("i", "Input image filename or list of filenames with *.txt extension.", 1);
    parser.add_option("m", "Detector model layout (must match dnn_mmod_sample_detector.h)", 1);
    parser.add_option("o", "Output directory for annotated images.", 1);
    parser.add_option("e", "Eye crop (if two eyes found)", 1);
    
    parser.parse(argc, argv);

    if (parser.option("h") || (argc == 1))
    {
        cout << "Usage: dnn_mmod_find_things_ex -i <input_image> -m <input_model> -o <output_directory>\n";
        parser.print_options();
        return EXIT_SUCCESS;
    }
        
    if(!parser.option("i"))
    {
        std::cout << "Must specify input image (or *.txt list of images)" << std::endl;
        return EXIT_SUCCESS;
    }

    std::string in_file = parser.option("i").argument();
    std::vector<std::string> filenames;
    expand(in_file, filenames);
        
    if(!parser.option("m"))
    {
        std::cout << "Must specify network model weights (*.dat)" << std::endl;
        return EXIT_SUCCESS;
    }
        
    const std::string network_filename = parser.option("m").argument();

    std::string output_directory;
    if(parser.option("o"))
    {
        output_directory = parser.option("o").argument();
    }
    
    net_type net;
    deserialize(network_filename) >> net;

    dlib::pipe<std::size_t> jobs(filenames.size());
    for(std::size_t i = 0; i < filenames.size(); i++)
    {
        auto index = i; // need copy here
        jobs.enqueue(index);
    }

    bool do_eye_crop = parser.option("e");
        
    struct fs_image
    {
        std::size_t index;
        matrix<rgb_pixel> image;
        bool sentinel;
    };
        
    dlib::pipe<fs_image> images(4);
        
    auto data_loader = [&images, &jobs, &filenames](int value)
    {
    
        std::size_t index;
        while(jobs.dequeue_or_timeout(index, 0))
        {
            try
            {
                fs_image image { index };
                load_image(image.image, filenames[image.index]);
                images.enqueue(image);
            }
            catch(std::exception& e)
            {
                cerr << e.what() << endl;
            }
        }
        
        fs_image sentinel { 0, {}, true };
        images.enqueue(sentinel);
        
        cout << "DONE" << std::endl;
    };
        
    std::thread data_loader1([data_loader](){ data_loader(1); });
    std::thread data_loader2([data_loader](){ data_loader(2); });
    std::thread data_loader3([data_loader](){ data_loader(3); });
    std::thread data_loader4([data_loader](){ data_loader(4); });
     
    int sentinels = 0;
        
    // Run network on single thread
    fs_image image;
    while(images.dequeue(image))
    {
        if(image.sentinel)
        {
            if(++sentinels == 4)
            {
                break;
            }
            else
            {
                continue;
            }
        }
        
#if !defined(DLIB_NO_GUI_SUPPORT)
        image_window win;
        win.set_image(image.image);
#endif
        
        std::string filename = filenames[image.index];

        // Run the detector on the image and show us the output.
        std::vector<dlib::mmod_rect> detections;
        try
        {
            detections = net(image.image);
        }
        catch(std::exception &e)
        {
            cerr << e.what() << std::endl;
            cerr << "Skipping image ..." << filename << std::endl;
            continue;
        }
        
        std::cout << image.index << ' ' << filename << ' ' << detections.size();

        if(do_eye_crop)
        {
            // Assume two detections == right + left eye:
            if(detections.size() == 2)
            {
                dlib::matrix<rgb_pixel> details;
            
                auto c0 = dlib::center(detections[0].rect);
                auto c1 = dlib::center(detections[1].rect);
            
                if(c1.x() < c0.x())
                {
                    std::swap(c0, c1);
                }
            
                const float iod =  dlib::length(c0 - c1);
                const float width = iod * 0.6;
                std::vector<dlib::chip_details> rois =
                    {
                        dlib::centered_rect(c0, width, width * 3.0f/4.0f),
                        dlib::centered_rect(c1, width, width * 3.0f/4.0f)
                    };
            
                dlib::array<dlib::matrix<rgb_pixel>> chips;
                dlib::extract_image_chips(image.image, rois, chips);
            
                for(std::size_t i = 0; i < chips.size(); i++)
                {
                    std::string output_filename;
                    output_filename += output_directory;
                    output_filename += "/";
                    output_filename += basename(filename);
                    output_filename += "_";
                    output_filename += std::to_string(i);
                    output_filename += ".jpg";
                    dlib::save_jpeg(chips[i], output_filename);
                }
            }
        }

        for (const auto& d : detections)
        {
            const auto &roi = d.rect;
            std::cout << ' ' << roi.left() << ' ' << roi.top() << ' ' << roi.width() << ' ' << roi.height();
            
            if(!output_directory.empty())
            {
                draw_rectangle(image.image, d, rgb_pixel(0,255,0), 4);
            }

#if !defined(DLIB_NO_GUI_SUPPORT)
            win.add_overlay(rect, rgb_pixel(255,0,0));
#endif
        }


        std::cout << std::endl;

        if(!output_directory.empty())
        {
            std::string output_filename;
            output_filename += output_directory;
            output_filename += "/";
            output_filename += basename(filename);
            output_filename += ".jpg";
            try
            {
                save_jpeg(image.image, output_filename);
            }
            catch(image_save_error &e)
            {
                cerr << e.what() << endl;
                cerr << "Failed to write image at path : " << output_filename << std::endl;
            }
        }
    }
        
    
    jobs.disable();
    images.disable();
        
    data_loader1.join();
    data_loader2.join();
    data_loader3.join();
    data_loader4.join();
        
    return EXIT_SUCCESS;
}
catch(image_load_error& e)
{
    cerr << e.what() << endl;
    cerr << "The test image is located in the examples folder.  So you should run this program from a sub folder so that the relative path is correct." << endl;
}
catch(serialization_error& e)
{
    cerr << e.what() << endl;
    cerr << "The correct model file can be obtained from: http://dlib.net/files/mmod_rear_end_vehicle_detector.dat.bz2" << endl;
}
catch(std::exception& e)
{
    cerr << e.what() << endl;
}




