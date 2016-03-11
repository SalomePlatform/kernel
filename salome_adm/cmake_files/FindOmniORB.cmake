# - Find OmniORB4 cmake module and Pyhon backends
#
# Sets the following variables:
# OMNIORB_FOUND        - TRUE if OmniORB4 installation has been found
# OMNIORB_INCLUDE_DIR  - OmniORB4 headers path
# OMNIORB_LIBRARIES    - OmniORB4 libraries 
# OMNIORB_VERSION      - OmniORB4 version
# OMNIORB_IDL_COMPILER - OmniORB4 idl compiler command (omniidl)
# OMNIORB_NAMESERVER   - OmniORB4 CORBA naming service (omniNames)
# OMNIORB_PYTHONPATH   - path to the OmniORB Python modules 
# and many other mainly used in UseOmniORB.cmake  
#
# Detection is made through calls to 
# find_library, find_program, find_path
# and can be guided by extending CMAKE_PREFIX_PATH. Given the CMake precedence
# rule this will take precedence over the standard system paths.
# See for example doc of find_library().  
#

################################################################
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

# minimum OmniORB4 supported version
# 4.0.6 has known bug float/double marshalling using CORBA::Any
SET(OmniORB_MINIMUM_VERSION "4.1.2")

IF(WIN32)
  # Guide the FIND_LIBRARY command towards the correct directory <...>/lib/x86_win32:
  SET(CMAKE_LIBRARY_ARCHITECTURE x86_win32)
ENDIF()

##############################################################################
# find headers
##############################################################################
FIND_PATH(OMNIORB_INCLUDE_DIR omniORB4/CORBA.h)

##############################################################################
# find libraries
##############################################################################

# Win release / debug specific stuff:
IF (WIN32)
  IF(CMAKE_BUILD_TYPE STREQUAL Debug)
    SET(OMNIORB_RD_SUFFIX d)
  ELSE()
    SET(OMNIORB_RD_SUFFIX)
  ENDIF()
ENDIF()

IF (WIN32)
  FIND_LIBRARY(OMNIORB_LIBRARY_omniORB4 
    NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}omniORB4_rt${OMNIORB_RD_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX})
  FIND_LIBRARY( OMNIORB_LIBRARY_omnithread
    NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}omnithread_rt${OMNIORB_RD_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX})
  FIND_LIBRARY( OMNIORB_LIBRARY_omniDynamic4
    NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}omniDynamic4_rt${OMNIORB_RD_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX})
ELSE (WIN32)
  FIND_LIBRARY(OMNIORB_LIBRARY_omniORB4 NAMES omniORB4)
  FIND_LIBRARY(OMNIORB_LIBRARY_omnithread NAMES omnithread)
  FIND_LIBRARY(OMNIORB_LIBRARY_omniDynamic4 NAMES omniDynamic4)
ENDIF (WIN32)   
  
# Optional libraries

IF (WIN32)
  FIND_LIBRARY( OMNIORB_LIBRARY_COS4
    NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}COS4_rt${OMNIORB_RD_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX})
  FIND_LIBRARY( OMNIORB_LIBRARY_COSDynamic4
    NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}COSDynamic4_rt${OMNIORB_RD_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}) 
ELSE (WIN32)
  FIND_LIBRARY(OMNIORB_LIBRARY_COS4 NAMES COS4)
  FIND_LIBRARY(OMNIORB_LIBRARY_COSDynamic4 NAMES COSDynamic4)
ENDIF (WIN32)

##############################################################################
# find command line tools
##############################################################################
IF (WIN32)
  FIND_PROGRAM( OMNIORB_IDL_COMPILER
    NAMES omniidl 
    PATH_SUFFIXES x86_win32
    DOC "What is the path where omniidl (the idl compiler) can be found")
  FIND_PROGRAM( OMNIORB_OMNINAMES_COMMAND
    NAMES omniNames 
    PATH_SUFFIXES x86_win32
  DOC "What is the path where omniNames (the ORB server) can be found")
ELSE(WIN32)
  FIND_PROGRAM(OMNIORB_IDL_COMPILER NAMES omniidl)
  FIND_PROGRAM(OMNIORB_NAMESERVER NAMES omniNames)
ENDIF (WIN32)

##############################################################################
# Cook our stuff
##############################################################################

MARK_AS_ADVANCED(OMNIORB_DIR OMNIORB_INCLUDE_DIR)
MARK_AS_ADVANCED(OMNIORB_LIBRARY_omniORB4 OMNIORB_LIBRARY_omnithread OMNIORB_LIBRARY_omniDynamic4)
MARK_AS_ADVANCED(OMNIORB_IDL_COMPILER OMNIORB_NAMESERVER OMNIORB_VERSION)
MARK_AS_ADVANCED(OMNIORB_LIBRARY_COS4 OMNIORB_LIBRARY_COSDynamic4)

SET(OMNIORB_LIBRARIES
    ${OMNIORB_LIBRARY_omniORB4}
    ${OMNIORB_LIBRARY_omnithread}
    ${OMNIORB_LIBRARY_omniDynamic4})

IF(OMNIORB_LIBRARY_COS4)
    LIST(APPEND OMNIORB_LIBRARIES ${OMNIORB_LIBRARY_COS4})
ENDIF()
IF(OMNIORB_LIBRARY_COSDynamic4)
    LIST(APPEND OMNIORB_LIBRARIES ${OMNIORB_LIBRARY_COSDynamic4})
ENDIF()

# Set path to the OmniORB Python modules
GET_FILENAME_COMPONENT(_tmp_ROOT_DIR "${OMNIORB_LIBRARIES}" PATH) 
SET(OMNIORB_PYTHONPATH "${_tmp_ROOT_DIR}/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages")

# Optionaly, extract the the version number from the acconfig.h file:
# The version check is done at the very bottom of this file.
IF( EXISTS ${OMNIORB_INCLUDE_DIR}/omniORB4/acconfig.h )
    FILE( READ ${OMNIORB_INCLUDE_DIR}/omniORB4/acconfig.h OMNIORB_ACCONFIG_H )
    STRING( REGEX MATCH "#define[\t ]+PACKAGE_VERSION[\t ]+\"([0-9]+.[0-9]+.[0-9]+)\"" OMNIORB_ACCONFIG_H "${OMNIORB_ACCONFIG_H}" )
    STRING( REGEX REPLACE ".*\"([0-9]+.[0-9]+.[0-9]+)\".*" "\\1" OMNIORB_VERSION "${OMNIORB_ACCONFIG_H}" )
ELSE()
    SET( OMNIORB_VERSION "NOT-FOUND" )
ENDIF( EXISTS ${OMNIORB_INCLUDE_DIR}/omniORB4/acconfig.h )
  
SET(OMNIORB_IDLCXXFLAGS -bcxx -Wba -nf)
SET(IDLCXXFLAGS ${OMNIORB_IDLCXXFLAGS})
SET(IDL_CLN_H .hh)
SET(IDL_SRV_H .hh)
SET(OMNIORB_DEFINITIONS "-D__x86__ -DCOMP_CORBA_DOUBLE -DCOMP_CORBA_LONG")
IF(WIN32)
  ## OmniORB isn`t defines SIZEOF_LONG and SIZEOF_INT on WIN32 platform
  ## Note SIZE_OF_LONG calculates in the SalomeSetupPlatform.cmake 
  SET(OMNIORB_DEFINITIONS "${OMNIORB_DEFINITIONS} -D__WIN32__ -DSIZEOF_INT=4 -DSIZEOF_LONG=${SIZE_OF_LONG}")
ENDIF()
IF(APPLE)
  SET(OMNIORB_DEFINITIONS "${OMNIORB_DEFINITIONS} -D__macos__")#for omnithread.h to be checked...
ENDIF()

# Handle standard arguments:
INCLUDE(FindPackageHandleStandardArgs)
IF(OMNIORB_VERSION)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(OmniORB  
        REQUIRED_VARS OMNIORB_INCLUDE_DIR OMNIORB_LIBRARY_omniORB4 OMNIORB_LIBRARY_omnithread 
                      OMNIORB_LIBRARY_omniDynamic4 OMNIORB_IDL_COMPILER 
        VERSION_VAR OMNIORB_VERSION )
ELSE()
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(OmniORB  
        REQUIRED_VARS OMNIORB_INCLUDE_DIR OMNIORB_LIBRARY_omniORB4 OMNIORB_LIBRARY_omnithread 
                      OMNIORB_LIBRARY_omniDynamic4 OMNIORB_IDL_COMPILER)
ENDIF()
