cmake_minimum_required(VERSION 3.8)

option(HUNTER_KEEP_PACKAGE_SOURCES "Keep" ON)

include("cmake/HunterGate.cmake")

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.22.16.tar.gz"
    SHA1 "84153076a3cebf4869c904fa5c93ea309386b583"
    LOCAL # cmake/Hunter/config.cmake
)

project(dlib_dnn_mmod_detect VERSION 0.0.1)

hunter_add_package(dlib)
find_package(dlib CONFIG REQUIRED)

set(app_list dnn_mmod_find_thing_ex dnn_mmod_train_find_thing_ex)

foreach(name ${app_list})
  add_executable(${name} ${name}.cpp dnn_mmod_sample_detector.h)
  target_link_libraries(${name} PUBLIC dlib::dlib)
  install(TARGETS ${name} DESTINATION bin)
endforeach()
