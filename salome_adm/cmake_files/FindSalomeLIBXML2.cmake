
# LIBXML2 detection for Salome

set(LIBXML2_ROOT_DIR $ENV{LIBXML2_ROOT_DIR} CACHE PATH "Path to LibXml2 directory")
if(EXISTS ${LIBXML2_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${LIBXML2_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${LIBXML2_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${LIBXML2_ROOT_DIR}/bin)
endif(EXISTS ${LIBXML2_ROOT_DIR})
find_package(LibXml2 REQUIRED)
#if(${LIBXML2_VERSION_STRING} VERSION_LESS "2.7.6")
#  message(FATAL_ERROR "LIbXml2 required version is 2.7.6, version is ${LIBXML2_VERSION_STRING} , please defined LIBXML2_ROOT_DIR as environment or cmake variable")
#endif(${LIBXML2_VERSION_STRING} VERSION_LESS "2.7.6")
