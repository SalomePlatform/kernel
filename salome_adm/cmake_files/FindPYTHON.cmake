
IF(WINDOWS)
  FIND_PROGRAM(PYTHON_EXECUTABLE python_d)
ELSE(WINDOWS)
  FIND_PROGRAM(PYTHON_EXECUTABLE python)
ENDIF(WINDOWS)

EXECUTE_PROCESS(
  COMMAND ${PYTHON_EXECUTABLE} -c "import sys; sys.stdout.write(sys.version[:3])"
  OUTPUT_VARIABLE PYTHON_VERSION
  )

SET(PYTHON_ROOT ${PYTHON_EXECUTABLE})
GET_FILENAME_COMPONENT(PYTHON_ROOT ${PYTHON_ROOT} PATH)
GET_FILENAME_COMPONENT(PYTHON_ROOT ${PYTHON_ROOT} PATH)

FIND_PATH(PYTHON_INCLUDE_PATH Python.h PATHS ${PYTHON_ROOT}/include ${PYTHON_ROOT}/include/python${PYTHON_VERSION})

IF(WINDOWS)
  STRING(REPLACE "." "" PYTHON_VERSION_WITHOUT_DOT ${PYTHON_VERSION})
  FIND_LIBRARY(PYTHON_LIBRARIES python${PYTHON_VERSION_WITHOUT_DOT}_d ${PYTHON_ROOT}/libs)
ELSE(WINDOWS)
  FIND_LIBRARY(PYTHON_LIBRARIES python${PYTHON_VERSION} ${PYTHON_ROOT}/lib)
ENDIF(WINDOWS)

SET(PYTHON_INCLUDES -I${PYTHON_INCLUDE_PATH})
SET(PYTHON_LIBS ${PYTHON_LIBRARIES})

IF(WINDOWS)
  SET(PYTHON_INCLUDES ${PYTHON_INCLUDES} -DHAVE_DEBUG_PYTHON)
ENDIF(WINDOWS)