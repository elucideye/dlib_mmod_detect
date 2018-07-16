cmake_minimum_required(VERSION 3.8)

option(HUNTER_KEEP_PACKAGE_SOURCES "Keep" ON)

include("cmake/HunterGate.cmake")

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.22.29.tar.gz"
    SHA1 "61e9583824b956b064aa8819c56c4b134bae86bc"
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

set(dlib_mmod_detect_release_url "https://github.com/elucideye/dlib_mmod_detect/releases/download/v0.0.1")

hunter_private_data(
  URL "${dlib_mmod_detect_release_url}/dlib_mmod_eye_detector_v2.dat"
  SHA1 "89a93bfd770259fbf415b67501c665ad6934c537"
  FILE "dlib_mmod_eye_detector.dat"
  LOCATION DLIB_DNN_MMOD_EYE_DETECTOR
)
message("DLIB_DNN_MMOD_EYE_DETECTOR = ${DLIB_DNN_MMOD_EYE_DETECTOR}")
install(FILES ${DLIB_DNN_MMOD_EYE_DETECTOR} DESTINATION share)
