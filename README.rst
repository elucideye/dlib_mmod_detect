dlib_mmod_detect
================

A `dlib  <https://github.com/davisking/dlib>`__ mmod object detection sample with a pre-trained eye detector built with the `Hunter <https://github.com/ruslo/hunter>`__ package manager.

Quick Start
-----------

-  Install compiler:
   http://cgold.readthedocs.io/en/latest/first-step.html
-  Install `CMake <https://github.com/kitware/CMake>`__ (and add to ``PATH``)
-  Install Python (for Polly)
-  Clone `Polly <https://github.com/ruslo/polly>`__ and add ``<polly>/bin`` to ``PATH``


.. code-block:: bash

    export TOOLCHAIN=xcode # for more options: polly.py --help 
    export CONFIG=Release
    polly.py --toolchain ${TOOLCHAIN} --config-all ${CONFIG} --install --verbose --reconfig
    find ${HOME}/path/to/images -name "*.jpg" > images.txt
    _install/${TOOLCHAIN}/bin/dnn_mmod_find_thing_ex -i images.txt -m _install/xcode/share/dlib_mmod_eye_detector.dat -o /tmp/
    
::
