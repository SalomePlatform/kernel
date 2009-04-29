#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
# ------

MESSAGE(STATUS "Check for python ...")

# ------

IF(PYTHON_IS_MANDATORY STREQUAL 0)
  SET(PYTHON_IS_MANDATORY 0)
  SET(PYTHON_IS_OPTIONAL 1)
ENDIF(PYTHON_IS_MANDATORY STREQUAL 0)
IF(PYTHON_IS_OPTIONAL STREQUAL 0)
  SET(PYTHON_IS_MANDATORY 1)
  SET(PYTHON_IS_OPTIONAL 0)
ENDIF(PYTHON_IS_OPTIONAL STREQUAL 0)
IF(NOT PYTHON_IS_MANDATORY AND NOT PYTHON_IS_OPTIONAL)
  SET(PYTHON_IS_MANDATORY 1)
  SET(PYTHON_IS_OPTIONAL 0)
ENDIF(NOT PYTHON_IS_MANDATORY AND NOT PYTHON_IS_OPTIONAL)

# ------

SET(PYTHON_STATUS 1)
IF(WITHOUT_PYTHON OR WITH_PYTHON STREQUAL 0)
  SET(PYTHON_STATUS 0)
  MESSAGE(STATUS "python disabled from command line.")
ENDIF(WITHOUT_PYTHON OR WITH_PYTHON STREQUAL 0)

# ------

IF(PYTHON_STATUS)
  IF(WITH_PYTHON)
    SET(PYTHON_ROOT_USER ${WITH_PYTHON})
  ENDIF(WITH_PYTHON)
  IF(NOT PYTHON_ROOT_USER)
    SET(PYTHON_ROOT_USER $ENV{PYTHON_ROOT})
  ENDIF(NOT PYTHON_ROOT_USER)
  IF(NOT PYTHON_ROOT_USER)
    SET(PYTHON_ROOT_USER $ENV{PYTHONHOME})
  ENDIF(NOT PYTHON_ROOT_USER)
ENDIF(PYTHON_STATUS)

# ------

IF(PYTHON_STATUS)
  IF(PYTHON_ROOT_USER)
    SET(PYTHON_FIND_PATHS_OPTION NO_DEFAULT_PATH)
  ELSE(PYTHON_ROOT_USER)
    SET(PYTHON_FIND_PATHS_OPTION)
  ENDIF(PYTHON_ROOT_USER)
ENDIF(PYTHON_STATUS)

# ------

IF(PYTHON_STATUS)
  IF(WINDOWS)
    IF(CMAKE_BUILD_TYPE STREQUAL Release)
      SET(PYTHON_EXECUTABLE_TO_FIND python)
    ELSE(CMAKE_BUILD_TYPE STREQUAL Release)
      SET(PYTHON_EXECUTABLE_TO_FIND python_d)
    ENDIF(CMAKE_BUILD_TYPE STREQUAL Release)
  ELSE(WINDOWS)
    SET(PYTHON_EXECUTABLE_TO_FIND python)
  ENDIF(WINDOWS)
  IF(NOT PYTHON_ROOT_USER)
    SET(PYTHON_EXECUTABLE_PATHS)
  ELSE(NOT PYTHON_ROOT_USER)
    IF(WINDOWS)
      SET(PYTHON_EXECUTABLE_PATHS ${PYTHON_ROOT_USER})
    ELSE(WINDOWS)
      SET(PYTHON_EXECUTABLE_PATHS ${PYTHON_ROOT_USER}/bin)
    ENDIF(WINDOWS)
  ENDIF(NOT PYTHON_ROOT_USER)
  FIND_PROGRAM(PYTHON_EXECUTABLE ${PYTHON_EXECUTABLE_TO_FIND} PATHS ${PYTHON_EXECUTABLE_PATHS} ${PYTHON_FIND_PATHS_OPTION})
  IF(PYTHON_EXECUTABLE)
    MESSAGE(STATUS "python found: ${PYTHON_EXECUTABLE}")
    IF(PYTHON_EXECUTABLE STREQUAL /usr/bin/python)
    ELSE(PYTHON_EXECUTABLE STREQUAL /usr/bin/python)
      SET(PYTHON_ROOT_USER ${PYTHON_EXECUTABLE})
      GET_FILENAME_COMPONENT(PYTHON_ROOT_USER ${PYTHON_ROOT_USER} PATH)
      IF(NOT WINDOWS)
	GET_FILENAME_COMPONENT(PYTHON_ROOT_USER ${PYTHON_ROOT_USER} PATH)
      ENDIF(NOT WINDOWS)
    ENDIF(PYTHON_EXECUTABLE STREQUAL /usr/bin/python)
  ELSE(PYTHON_EXECUTABLE)
    IF(PYTHON_ROOT_USER)
      MESSAGE(STATUS "${PYTHON_EXECUTABLE_TO_FIND} not found in ${PYTHON_EXECUTABLE_PATHS}, check your PYTHON installation.")
    ELSE(PYTHON_ROOT_USER)
      MESSAGE(STATUS "${PYTHON_EXECUTABLE_TO_FIND} not found on system, try to use WITH_PYTHON option or PYTHON_ROOT environment variable.")
    ENDIF(PYTHON_ROOT_USER)
    SET(PYTHON_STATUS 0)
  ENDIF(PYTHON_EXECUTABLE)
ENDIF(PYTHON_STATUS)

# ------

IF(PYTHON_STATUS)
  EXECUTE_PROCESS(
    COMMAND ${PYTHON_EXECUTABLE} -c "import sys; sys.stdout.write(sys.version[:3])"
    OUTPUT_VARIABLE PYTHON_VERSION
    )
ENDIF(PYTHON_STATUS)

# ------

IF(PYTHON_STATUS)
  IF(PYTHON_ROOT_USER)
    SET(PYTHON_FIND_PATHS_OPTION NO_DEFAULT_PATH)
  ELSE(PYTHON_ROOT_USER)
    SET(PYTHON_FIND_PATHS_OPTION)
  ENDIF(PYTHON_ROOT_USER)
ENDIF(PYTHON_STATUS)

# ------

IF(PYTHON_STATUS)
  IF(PYTHON_ROOT_USER)
    SET(PYTHON_INCLUDE_PATHS ${PYTHON_ROOT_USER}/include ${PYTHON_ROOT_USER}/include/python${PYTHON_VERSION})
  ELSE(PYTHON_ROOT_USER)
    SET(PYTHON_INCLUDE_PATHS /usr/include/python${PYTHON_VERSION})
  ENDIF(PYTHON_ROOT_USER)
  SET(PYTHON_INCLUDE_TO_FIND Python.h)
  FIND_PATH(PYTHON_INCLUDE_DIR ${PYTHON_INCLUDE_TO_FIND} PATHS ${PYTHON_INCLUDE_PATHS} ${PYTHON_FIND_PATHS_OPTION})
  IF(PYTHON_INCLUDE_DIR)
    SET(PYTHON_INCLUDES -I${PYTHON_INCLUDE_DIR})
    IF(WINDOWS)
      IF(CMAKE_BUILD_TYPE STREQUAL Release)
      ELSE(CMAKE_BUILD_TYPE STREQUAL Release)
	SET(PYTHON_INCLUDES ${PYTHON_INCLUDES} -DHAVE_DEBUG_PYTHON)
      ENDIF(CMAKE_BUILD_TYPE STREQUAL Release)
    ENDIF(WINDOWS)
    MESSAGE(STATUS "${PYTHON_INCLUDE_TO_FIND} found in ${PYTHON_INCLUDE_DIR}")
  ELSE(PYTHON_INCLUDE_DIR)
    SET(PYTHON_STATUS 0)
    IF(PYTHON_ROOT_USER)
      MESSAGE(STATUS "${PYTHON_INCLUDE_TO_FIND} not found in ${PYTHON_INCLUDE_PATHS}, check your PYTHON installation.")
    ELSE(PYTHON_ROOT_USER)
      MESSAGE(STATUS "${PYTHON_INCLUDE_TO_FIND} not found on system, try to use WITH_PYTHON option or PYTHON_ROOT environment variable.")
    ENDIF(PYTHON_ROOT_USER)
  ENDIF(PYTHON_INCLUDE_DIR)
ENDIF(PYTHON_STATUS)

# ----

IF(PYTHON_STATUS)
  IF(PYTHON_ROOT_USER)
    IF(WINDOWS)
      SET(PYTHON_LIB_PATHS ${PYTHON_ROOT_USER}/libs)
    ELSE(WINDOWS)
      SET(PYTHON_LIB_PATHS ${PYTHON_ROOT_USER}/lib ${PYTHON_ROOT_USER}/lib/python${PYTHON_VERSION}/config)
    ENDIF(WINDOWS)
  ELSE(PYTHON_ROOT_USER)
    SET(PYTHON_LIB_PATHS)
  ENDIF(PYTHON_ROOT_USER)
  IF(WINDOWS)
    STRING(REPLACE "." "" PYTHON_VERSION_WITHOUT_DOT ${PYTHON_VERSION})
    IF(CMAKE_BUILD_TYPE STREQUAL Release)
      FIND_LIBRARY(PYTHON_LIB python${PYTHON_VERSION_WITHOUT_DOT} PATHS ${PYTHON_LIB_PATHS} ${PYTHON_FIND_PATHS_OPTION})
    ELSE(CMAKE_BUILD_TYPE STREQUAL Release)
      FIND_LIBRARY(PYTHON_LIB python${PYTHON_VERSION_WITHOUT_DOT}_d PATHS ${PYTHON_LIB_PATHS} ${PYTHON_FIND_PATHS_OPTION})
    ENDIF(CMAKE_BUILD_TYPE STREQUAL Release)
  ELSE(WINDOWS)
    FIND_LIBRARY(PYTHON_LIB python${PYTHON_VERSION} PATHS ${PYTHON_LIB_PATHS} ${PYTHON_FIND_PATHS_OPTION})
  ENDIF(WINDOWS)
  SET(PYTHON_LIBS)
  IF(PYTHON_LIB)
    SET(PYTHON_LIBS ${PYTHON_LIBS} ${PYTHON_LIB})
    MESSAGE(STATUS "python lib found: ${PYTHON_LIB}")
  ELSE(PYTHON_LIB)
    SET(PYTHON_STATUS 0)
    IF(PYTHON_ROOT_USER)
      MESSAGE(STATUS "python lib not found in ${PYTHON_LIB_PATHS}, check your PYTHON installation.")
    ELSE(PYTHON_ROOT_USER)
      MESSAGE(STATUS "python lib not found on system, try to use WITH_PYTHON option or PYTHON_ROOT environment variable.")
    ENDIF(PYTHON_ROOT_USER)
  ENDIF(PYTHON_LIB)
ENDIF(PYTHON_STATUS)

# ----

IF(PYTHON_STATUS)
ELSE(PYTHON_STATUS)
  IF(PYTHON_IS_MANDATORY)
    MESSAGE(FATAL_ERROR "python not found ... mandatory ... abort")
  ELSE(PYTHON_IS_MANDATORY)
    MESSAGE(STATUS "python not found ... optional ... disabled")
  ENDIF(PYTHON_IS_MANDATORY)
ENDIF(PYTHON_STATUS)
