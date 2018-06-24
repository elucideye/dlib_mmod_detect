set(dlib_cmake_args
  DLIB_HEADER_ONLY=OFF  #all previous builds were header on, so that is the default
  DLIB_ENABLE_ASSERTS=OFF #must be set on/off or debug/release build will differ and config will not match one
  DLIB_NO_GUI_SUPPORT=ON
  DLIB_ISO_CPP_ONLY=OFF # needed for directory navigation code (loading training data)
  DLIB_JPEG_SUPPORT=OFF  # https://github.com/hunter-packages/dlib/blob/eb79843227d0be45e1efa68ef9cc6cc187338c8e/dlib/CMakeLists.txt#L422-L432
  DLIB_LINK_WITH_SQLITE3=OFF
  DLIB_USE_BLAS=OFF
  DLIB_USE_LAPACK=OFF
  DLIB_USE_CUDA=ON
  DLIB_PNG_SUPPORT=ON
  DLIB_JPEG_SUPPORT=ON
  DLIB_GIF_SUPPORT=OFF
  DLIB_USE_MKL_FFT=OFF  
  HUNTER_INSTALL_LICENSE_FILES=dlib/LICENSE.txt
)

hunter_config(dlib VERSION ${HUNTER_dlib_VERSION} CMAKE_ARGS ${dlib_cmake_args} DUMMY_SOURCES=2)
