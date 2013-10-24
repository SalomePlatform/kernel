# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

## Detect architecture
IF(WIN32)
  SET(MACHINE WINDOWS)
ELSE()
  SET(MACHINE PCLINUX)
ENDIF()

## Test for 64 bits
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET(MACHINE_IS_64 TRUE)
ELSE()
  SET(MACHINE_IS_64 FALSE)
ENDIF()

## Force CMAKE_BUILD_TYPE to Release if not set
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE $ENV{CMAKE_BUILD_TYPE})
ENDIF(NOT CMAKE_BUILD_TYPE)
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT CMAKE_BUILD_TYPE)

## Define the log level according to the build type
IF(CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET(PYLOGLEVEL DEBUG)
ELSE()
  SET(PYLOGLEVEL WARNING)
ENDIF()

## Make all warnings errors on non-windows platforms
#  CURRENTLY DISABLED
IF(NOT (WIN32 OR APPLE))
  SET(ADD_WERROR ON)
  SET(NAMES ACCEPT_SALOME_WARNINGS ACCEPT_${MODULE}_WARNINGS I_AM_A_TROLL_I_DO_NOT_FIX_${MODULE}_WARNINGS)
  FOREACH(name ${NAMES})
    SET(VAL $ENV{${name}})
    IF("${VAL}" STREQUAL "0")
      SET(ADD_WERROR ON)
    ENDIF()
    IF("${VAL}" STREQUAL "1")
      SET(ADD_WERROR OFF)
    ENDIF()
  ENDFOREACH()
  
  IF(ADD_WERROR)
#    SET(CMAKE_C_FLAGS "-Werror")
#    SET(CMAKE_CXX_FLAGS "-Werror")
  ENDIF()
ENDIF()

## TODO: remove this ?
#IF(WIN32)
#    MARK_AS_ADVANCED(CLEAR CMAKE_CONFIGURATION_TYPES)
#    SET(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "compilation types" FORCE)
#ELSE()
#    IF(CMAKE_BUILD_TYPE STREQUAL Debug)
#      SET(CMAKE_C_FLAGS_DEBUG "-g")
#      SET(CMAKE_CXX_FLAGS_DEBUG "-g")
#    ENDIF(CMAKE_BUILD_TYPE STREQUAL Debug)
#    IF(CMAKE_BUILD_TYPE STREQUAL Release)
#      SET(CMAKE_C_FLAGS_RELEASE "-O1 -DNDEBUG")
#      SET(CMAKE_CXX_FLAGS_RELEASE "-O1 -DNDEBUG")
#    ENDIF(CMAKE_BUILD_TYPE STREQUAL Release)
#ENDIF()

IF(WIN32)
  ## Windows specific:  
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)  # To disable windows warnings for strcpy, fopen, ...
  ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)  # To disable windows warnings std::copy, std::transform, ...
  ADD_DEFINITIONS(-DWNT -DWIN32)
  ADD_DEFINITIONS(-D_WIN32_WINNT=0x0500)      # Windows 2000 or later API is required
  ADD_DEFINITIONS(-DPPRO_NT)                  # For medfile
  #ADD_DEFINITIONS(-D_USE_MATH_DEFINES)        # for MEDMEM
    
  SET(PLATFORM_LIBS Ws2_32.lib)
  LIST(APPEND PLATFORM_LIBS Userenv.lib)      # At least for GEOM suit

  # Disable iterator debugging on WINDOWS in Debug Mode
  IF(NOT CMAKE_BUILD_TYPE STREQUAL "RELEASE" AND NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    ADD_DEFINITIONS(-D_SECURE_SCL=0 -D_SECURE_SCL_THROWS=0)
    ADD_DEFINITIONS(-D_HAS_ITERATOR_DEBUGGING=0) # To avoid runtime error during checking iterators
  ENDIF(NOT CMAKE_BUILD_TYPE STREQUAL "RELEASE" AND NOT CMAKE_BUILD_TYPE STREQUAL "Release")
  
  IF(MACHINE_IS_64)
    SET(SIZE_OF_LONG 4)                          # set sizeof(long) to 4 byte
  ELSE()
    SET(SIZE_OF_LONG ${CMAKE_SIZEOF_VOID_P})     # set sizeof(long) the same as size of pointers
  ENDIF()
ELSE()
  ## Linux specific:
  SET(PLATFORM_LIBS dl)    # Dynamic loading (dlopen, dlsym)
  IF(MACHINE_IS_64) 
    ADD_DEFINITIONS(-DPCLINUX64)
  ENDIF(MACHINE_IS_64)
ENDIF()

## define _DEBUG_ macro
IF(NOT CMAKE_BUILD_TYPE STREQUAL "RELEASE" AND NOT CMAKE_BUILD_TYPE STREQUAL "Release")
  ADD_DEFINITIONS(-D_DEBUG_)
ENDIF()

## Apple specific:
IF(APPLE)
  SET(CMAKE_C_COMPILER gcc)
  SET(CMAKE_CXX_COMPILER g++)
  # because default is clang(llvm) with mountain lion at least
ENDIF()

# Compiler flags for coverage testing
IF(NOT WIN32) 
  OPTION(SALOME_BUILD_FOR_GCOV "Add the compilation flags for GCov/LCov" OFF)
  MARK_AS_ADVANCED(SALOME_BUILD_FOR_GCOV)
  IF(SALOME_BUILD_FOR_GCOV)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS    "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  ENDIF()
ENDIF()
