
# Swig detection for Salome

set(SWIG_ROOT_DIR $ENV{SWIG_ROOT_DIR} CACHE PATH "Path to Swig directory")
if(EXISTS ${SWIG_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${SWIG_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${SWIG_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${SWIG_ROOT_DIR}/bin)
endif(EXISTS ${SWIG_ROOT_DIR})
find_package(SWIG REQUIRED)
