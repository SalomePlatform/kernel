# - Find PThread
# This module finds an installed PThread using the variable PTHREAD_ROOT_DIR 
# as a lookup path.
# It sets the following variables:
#  PTHREAD_FOUND       - set to true if PThread is found
#  PTHREAD_INCLUDE_DIR - the directory where the include files are located
#  PTHREAD_LIBRARIES   - the path to PThread library
#
# The file "pthread.h" is looked for PTHREAD_INCLUDE_DIR.
# Libraries are searched with following names: 
#    pthread
# On Win32:
#    pthreadVC2
# or in Debug mode:
#    pthreadVC2d
#


#############################################################
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
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

IF(NOT PThread_FIND_QUIETLY)
    MESSAGE(STATUS "Looking for PThread...")
ENDIF ()

IF(WIN32)
    SET(PTHREADS_INCLUDE_TO_FIND pthread.h)
    FIND_PATH(PTHREAD_INCLUDE_DIR ${PTHREADS_INCLUDE_TO_FIND})
    # Default build type is assumed to be Release:
    IF(NOT CMAKE_BUILD_TYPE STREQUAL Debug)
        FIND_LIBRARY(PTHREAD_LIBRARIES pthreadVC2)
    ELSE()
        FIND_LIBRARY(PTHREAD_LIBRARIES pthreadVC2d)
    ENDIF()
ELSE(WIN32)
    FIND_PATH(PTHREAD_INCLUDE_DIR pthread.h)
    FIND_LIBRARY(PTHREAD_LIBRARIES NAMES pthread)
ENDIF(WIN32)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PThread REQUIRED_VARS PTHREAD_INCLUDE_DIR PTHREAD_LIBRARIES)

IF(PTHREAD_FOUND)
  IF(NOT PThread_FIND_QUIETLY)
    MESSAGE(STATUS "PThread library: ${PTHREAD_LIBRARIES}")
  ENDIF()
ENDIF()
