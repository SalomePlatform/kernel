# - Find OmniORB4 cmake module and Pyhon backends
#
# Sets the following variables:
# OMNIORB_FOUND        - TRUE if OmniORB4 installation has been found
# OMNIORB_INCLUDE_DIR  - OmniORB4 headers path
# OMNIORB_LIBRARIES    - OmniORB4 libraries 
# OMNIORB_VERSION      - OmniORB4 version
# OMNIORB_IDL_COMPILER - OmniORB4 idl compiler command (omniidl)
# OMNIORB_NAMESERVER   - OmniORB4 CORBA naming service (omniNames)
# OMNIORB_PYTHON_BACKEND
# and many other mainly used in UseOmniORB.cmake  
#
# Detection is made through calls to 
# find_library, find_program, find_path
# and can be guided by extending CMAKE_PREFIX_PATH. Given the CMake precedence
# rule this will take precedence over the standard system paths.
# See for example doc of find_library().  
#

################################################################
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
################################################################

# minimum OmniORB4 supported version
# 4.0.6 has known bug float/double marshalling using CORBA::Any
set(OMNIORB_MINIMUM_VERSION "4.1.2")

##############################################################################
# find headers
##############################################################################
FIND_PATH(OMNIORB_INCLUDE_DIR omniORB4/CORBA.h)

##############################################################################
# find libraries
##############################################################################
IF (WIN32)
  FIND_LIBRARY(OMNIORB_LIBRARY_omniORB4 
    NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}omniORB4${CMAKE_STATIC_LIBRARY_SUFFIX}
    PATHS lib/x86_win32)
  FIND_LIBRARY( OMNIORB_LIBRARY_omnithread
    NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}omnithread${CMAKE_STATIC_LIBRARY_SUFFIX}
    PATHS lib/x86_win32)
  FIND_LIBRARY( OMNIORB_LIBRARY_omniDynamic4
    NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}omniDynamic4${CMAKE_STATIC_LIBRARY_SUFFIX}
    PATHS lib/x86_win32)
ELSE (WIN32)
  FIND_LIBRARY(OMNIORB_LIBRARY_omniORB4 NAMES omniORB4)
  FIND_LIBRARY(OMNIORB_LIBRARY_omnithread NAMES omnithread)
  FIND_LIBRARY(OMNIORB_LIBRARY_omniDynamic4 NAMES omniDynamic4)
ENDIF (WIN32)   
  
# Optional libraries

IF (WIN32)
  FIND_LIBRARY( OMNIORB_LIBRARY_COS4
    NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}COS4${CMAKE_STATIC_LIBRARY_SUFFIX}
    PATHS lib/x86_win32 )
  FIND_LIBRARY( OMNIORB_LIBRARY_COSDynamic4
    NAMES ${CMAKE_STATIC_LIBRARY_PREFIX}COSDynamic4${CMAKE_STATIC_LIBRARY_SUFFIX}
    PATHS lib/x86_win32) 
ELSE (WIN32)
  FIND_LIBRARY(OMNIORB_LIBRARY_COS4 NAMES COS4)
  FIND_LIBRARY(OMNIORB_LIBRARY_COSDynamic4 NAMES COSDynamic4)
ENDIF (WIN32)

##############################################################################
# find command line tools
##############################################################################
IF (WIN32)
  FIND_PROGRAM( OMNIORB_IDL_COMPILER
    NAMES omniidl PATHS bin/x86_win32
    DOC "What is the path where omniidl (the idl compiler) can be found")
  FIND_PROGRAM( OMNIORB_OMNINAMES_COMMAND
    NAMES omniNames PATHS bin/x86_win32 
  DOC "What is the path where omniNames (the ORB server) can be found")
ELSE(WIN32)
  FIND_PROGRAM(OMNIORB_IDL_COMPILER NAMES omniidl)
  FIND_PROGRAM(OMNIORB_NAMESERVER NAMES omniNames)
ENDIF (WIN32)

##############################################################################
# find python back-end
##############################################################################

SET(_py_version "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
SET(CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH} ${CMAKE_PREFIX_PATH})
FIND_PATH(OMNIORB_PYTHON_BACKEND
  NAMES python.py
  PATHS "/lib/python${_py_version}/site-packages/omniidl_be" "/usr/lib/omniidl/omniidl_be" 
  DOC "Path to python-backend directory (omniidl_be) including python.py file" )

##############################################################################
# Cook our stuff
##############################################################################

MARK_AS_ADVANCED(OMNIORB_DIR OMNIORB_INCLUDE_DIR)
MARK_AS_ADVANCED(OMNIORB_LIBRARY_omniORB4 OMNIORB_LIBRARY_omnithread OMNIORB_LIBRARY_omniDynamic4)
MARK_AS_ADVANCED(OMNIORB_IDL_COMPILER OMNIORB_NAMESERVER OMNIORB_VERSION)
MARK_AS_ADVANCED(OMNIORB_LIBRARY_COS4 OMNIORB_LIBRARY_COSDynamic4 OMNIORB_PYTHON_BACKEND)

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

# Optionaly, extract the the version number from the acconfig.h file:
# The version check is done at the very bottom of this file.
IF( EXISTS ${OMNIORB_INCLUDE_DIR}/omniORB4/acconfig.h )
    FILE( READ ${OMNIORB_INCLUDE_DIR}/omniORB4/acconfig.h OMNIORB_ACCONFIG_H )
    STRING( REGEX MATCH "#define[\t ]+PACKAGE_VERSION[\t ]+\"([0-9]+.[0-9]+.[0-9]+)\"" OMNIORB_ACCONFIG_H "${OMNIORB_ACCONFIG_H}" )
    STRING( REGEX REPLACE ".*\"([0-9]+.[0-9]+.[0-9]+)\".*" "\\1" OMNIORB_VERSION "${OMNIORB_ACCONFIG_H}" )
ELSE()
    SET( OMNIORB_VERSION "NOT-FOUND" )
ENDIF( EXISTS ${OMNIORB_INCLUDE_DIR}/omniORB4/acconfig.h )
  
SET(OMNIORB_IDLCXXFLAGS -Wba -nf)
SET(OMNIORB_IDLPYFLAGS -bpython)
IF (OMNIORB_PYTHON_BACKEND) 
  SET(OMNIORB_IDLPYFLAGS "-p ${OMNIORB_PYTHON_BACKEND} ${OMNIORB_IDLPYFLAGS}")
ENDIF()
SET(IDLCXXFLAGS ${OMNIORB_IDLCXXFLAGS})
SET(IDLPYFLAGS ${OMNIORB_IDLPYFLAGS})
SET(IDL_CLN_H .hh)
SET(IDL_SRV_H .hh)
SET(OMNIORB_DEFINITIONS "-D__x86__ -DCOMP_CORBA_DOUBLE -DCOMP_CORBA_LONG")
IF(WIN32)
  SET(OMNIORB_DEFINITIONS "${OMNIORB_DEFINITIONS} -D__WIN32__")
ENDIF()
IF(APPLE)
  SET(OMNIORB_DEFINITIONS "${OMNIORB_DEFINITIONS} -D__macos__")#for omnithread.h to be checked...
ENDIF()

# Handle standard arguments:
INCLUDE(FindPackageHandleStandardArgs)
IF(OMNIORB_VERSION)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(OmniORB  
        REQUIRED_VARS OMNIORB_INCLUDE_DIR OMNIORB_LIBRARY_omniORB4 OMNIORB_LIBRARY_omnithread 
                      OMNIORB_LIBRARY_omniDynamic4 OMNIORB_IDL_COMPILER OMNIORB_PYTHON_BACKEND
        VERSION_VAR OMNIORB_VERSION )
ELSE()
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(OmniORB  
        REQUIRED_VARS OMNIORB_INCLUDE_DIR OMNIORB_LIBRARY_omniORB4 OMNIORB_LIBRARY_omnithread 
                      OMNIORB_LIBRARY_omniDynamic4 OMNIORB_IDL_COMPILER OMNIORB_PYTHON_BACKEND)
ENDIF()
