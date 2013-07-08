# - Find CppUnit
# Sets the following variables:
#   CPPUNIT_INCLUDE_DIRS - path to the CppUnit include directory
#   CPPUNIT_LIBRARIES    - path to the CppUnit libraries to be linked against
#   CPPUNIT_DEFINITIONS  - specific CppUnit definitions to be added
#
#  The header cppunit/extensions/HelperMacros.h is looked for.
#  The following libraries are searched  
#        cppunit_dll, or cppunitd_dll (Windows) 
#        cppunit (Linux)
#

#########################################################################
# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

IF(NOT CppUnit_FIND_QUIETLY)
    MESSAGE(STATUS "Looking for CppUnit ...")
ENDIF()

# Headers
SET(CPPUNIT_ROOT_DIR $ENV{CPPUNIT_ROOT_DIR} CACHE PATH "Path to the CPPUNIT.")
IF(CPPUNIT_ROOT_DIR)
  LIST(APPEND CMAKE_INCLUDE_PATH "${CPPUNIT_ROOT_DIR}/include")
  LIST(APPEND CMAKE_PROGRAM_PATH "${CPPUNIT_ROOT_DIR}/bin")
ENDIF(CPPUNIT_ROOT_DIR)

SET(CPPUNIT_INCLUDE_TO_FIND cppunit/extensions/HelperMacros.h)
FIND_PATH(CPPUNIT_INCLUDE_DIRS ${CPPUNIT_INCLUDE_TO_FIND})

# Libraries
IF(WIN32)
  IF(CMAKE_BUILD_TYPE STREQUAL Debug)
    FIND_LIBRARY(CPPUNIT_LIBRARIES cppunitd_dll)
  ELSE(CMAKE_BUILD_TYPE STREQUAL Debug)
    FIND_LIBRARY(CPPUNIT_LIBRARIES cppunit_dll)
  ENDIF(CMAKE_BUILD_TYPE STREQUAL Debug)
ELSE(WIN32)
  FIND_PROGRAM(CPPUNIT_CONFIG_BIN cppunit-config)
  IF(NOT CPPUNIT_CONFIG_BIN)
    MESSAGE(FATAL_ERROR "Error in CPPUNIT detection ! cppunit-config executable not found !")
  ENDIF(NOT CPPUNIT_CONFIG_BIN)
  EXECUTE_PROCESS(COMMAND ${CPPUNIT_CONFIG_BIN} --libs OUTPUT_VARIABLE CPPUNIT_LDFLAGS)
  STRING( REGEX MATCHALL "-L([^\" ]+|\"[^\"]+\")" CPPUNIT_TMP4 "${CPPUNIT_LDFLAGS}" )
  IF(CPPUNIT_TMP4)
    STRING( REGEX REPLACE "^-L" "" CPPUNIT_LIBRARY_DIRS ${CPPUNIT_TMP4})
    LIST(APPEND CMAKE_LIBRARY_PATH ${CPPUNIT_LIBRARY_DIRS})
  ENDIF(CPPUNIT_TMP4)
  STRING( REGEX MATCHALL "-l([^\", \n]+)" CPPUNIT_TMP5 "${CPPUNIT_LDFLAGS}" )
  FOREACH(LIB ${CPPUNIT_TMP5})
    STRING(REGEX REPLACE "^-l" "" LIB2 ${LIB})
    FIND_LIBRARY(CPPUNIT_SUBLIB_${LIB2} ${LIB2})
    IF(NOT CPPUNIT_SUBLIB_${LIB2})
      MESSAGE(FATAL_ERROR "Error in CPPUNIT detection ! Fail to locate the needed library ${LIB2} !")
    ENDIF(NOT CPPUNIT_SUBLIB_${LIB2})
    LIST(APPEND CPPUNIT_LIBRARIES ${CPPUNIT_SUBLIB_${LIB2}})
  ENDFOREACH(LIB ${CPPUNIT_TMP5})
#  MESSAGE("**** ${CPPUNIT_LIBRARIES}")
ENDIF(WIN32)

# Global variables
SET(CPPUNIT_DEFINITIONS)
IF(WIN32)
  SET(CPPUNIT_DEFINITIONS -DCPPUNIT_DLL)
ENDIF(WIN32)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CppUnit REQUIRED_VARS CPPUNIT_INCLUDE_DIRS CPPUNIT_LIBRARIES)


