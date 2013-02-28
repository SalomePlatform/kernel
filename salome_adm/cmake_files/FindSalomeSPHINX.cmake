
# Sphinx detection for Salome

set(SPHINX_ROOT_DIR $ENV{SPHINX_ROOT_DIR} CACHE PATH "Path to Sphinx directory")
if(EXISTS ${SPHINX_ROOT_DIR})
  set(CMAKE_PROGRAM_PATH ${SPHINX_ROOT_DIR}/bin)
endif(EXISTS ${SPHINX_ROOT_DIR})
find_package(SPHINX)
