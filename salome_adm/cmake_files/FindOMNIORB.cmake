# Find OmniORB4 cmake module and Pyhon backends
#
# sets the following variables:
# OMNIORB_FOUND        - TRUE if OmniORB4 installation has been found
# OMNIORB_INCLUDE_DIR  - OmniORB4 headers path
# OMNIORB_LIBRARIES    - OmniORB4 libraries 
# OMNIORB_VERSION      - OmniORB4 version
# OMNIORB_IDL_COMPILER - OmniORB4 idl compiler command (omniidl)
# OMNIORB_NAMESERVER   - OmniORB4 CORBA naming service (omniNames)
#
# optional variables:
# OMNIORB_DIR          - OmniORB4 local installation path
# OMNIORBPY_DIR        - OmniORBpy local installation path
#
# This module could use OMNIORB_DIR environment variable if set
# WARNING: The precedence order is the following:
#   1. OMNIORB_DIR cmake variable
#   2. OMNIORB_DIR environment variable
#   3. default cmake search paths
#
# This module could use OMNIORBPY_DIR environment variable if set
# WARNING: The precedence order is the following:
#   1. OMNIORBPY_DIR cmake variable
#   2. OMNIORBPY_DIR environment variable
#   3. default cmake search paths
#
# NOTE: this goes against cmake default behavior for Find* macros, 
# more on this issue: 
# http://www.mail-archive.com/kde-buildsystem@kde.org/msg00589.html
#

# minimum OmniORB4 supported version
# 4.0.6 has known bug float/double marshalling using CORBA::Any
set(OMNIORB_MINIMUM_VERSION "4.1.2")
set(OMNIORB_DIR $ENV{OMNIORB_DIR} CACHE PATH "Path to OmniORB directory")

##############################################################################
# find headers
##############################################################################
find_path(OMNIORB_INCLUDE_DIR omniORB4/CORBA.h
  PATHS "${OMNIORB_DIR}/include" "$ENV{OMNIORB_DIR}/include"
  NO_DEFAULT_PATH)

find_path(OMNIORB_INCLUDE_DIR omniORB4/CORBA.h)


##############################################################################
# find libraries
##############################################################################
if (WIN32)
  find_library(OMNIORB_LIBRARY_omniORB4
	NAMES
	${CMAKE_STATIC_LIBRARY_PREFIX}omniORB4${CMAKE_STATIC_LIBRARY_SUFFIX}
	PATHS ${OMNIORB_DIR}/lib/x86_win32 $ENV{OMNIORB_DIR}/lib/x86_win32
	NO_DEFAULT_PATH
  )
  find_library(OMNIORB_LIBRARY_omniORB4 NAMES
    ${CMAKE_STATIC_LIBRARY_PREFIX}omniORB4${CMAKE_STATIC_LIBRARY_SUFFIX}
  )
else (WIN32)
  find_library(OMNIORB_LIBRARY_omniORB4
    NAMES omniORB4
    PATHS "${OMNIORB_DIR}/lib${LIB_SUFFIX}" "$ENV{OMNIORB_DIR}/lib${LIB_SUFFIX}"
    NO_DEFAULT_PATH)

  find_library(OMNIORB_LIBRARY_omniORB4
    NAMES omniORB4)
endif (WIN32)   
  
if (WIN32)
  FIND_LIBRARY( OMNIORB_LIBRARY_omnithread
	NAMES
	${CMAKE_STATIC_LIBRARY_PREFIX}omnithread${CMAKE_STATIC_LIBRARY_SUFFIX}
	PATHS ${OMNIORB_DIR}/lib/x86_win32 $ENV{OMNIORB_DIR}/lib/x86_win32
	NO_DEFAULT_PATH
  )

  FIND_LIBRARY( OMNIORB_LIBRARY_omnithread
	NAMES
	${CMAKE_STATIC_LIBRARY_PREFIX}omnithread${CMAKE_STATIC_LIBRARY_SUFFIX}
  )
else (WIN32)
  find_library(OMNIORB_LIBRARY_omnithread
    NAMES omnithread
    PATHS "${OMNIORB_DIR}/lib${LIB_SUFFIX}" "$ENV{OMNIORB_DIR}/lib${LIB_SUFFIX}"
    NO_DEFAULT_PATH)

  find_library(OMNIORB_LIBRARY_omnithread
    NAMES omnithread)
endif (WIN32)


if (WIN32)
  FIND_LIBRARY( OMNIORB_LIBRARY_omniDynamic4
    NAMES
      ${CMAKE_STATIC_LIBRARY_PREFIX}omniDynamic4${CMAKE_STATIC_LIBRARY_SUFFIX}
    PATHS ${OMNIORB_DIR}/lib/x86_win32 $ENV{OMNIORB_DIR}/lib/x86_win32
    NO_DEFAULT_PATH
  )

  FIND_LIBRARY( OMNIORB_LIBRARY_omniDynamic4
    NAMES
      ${CMAKE_STATIC_LIBRARY_PREFIX}omniDynamic4${CMAKE_STATIC_LIBRARY_SUFFIX}
  )
else (WIN32)
  find_library(OMNIORB_LIBRARY_omniDynamic4
    NAMES omniDynamic4
    PATHS "${OMNIORB_DIR}/lib${LIB_SUFFIX}" "$ENV{OMNIORB_DIR}/lib${LIB_SUFFIX}"
    NO_DEFAULT_PATH)

  find_library(OMNIORB_LIBRARY_omniDynamic4
    NAMES omniDynamic4)
endif (WIN32)    

# optional libraries

if (WIN32)
  FIND_LIBRARY( OMNIORB_LIBRARY_COS4
    NAMES
      ${CMAKE_STATIC_LIBRARY_PREFIX}COS4${CMAKE_STATIC_LIBRARY_SUFFIX}
    PATHS ${OMNIORB_DIR}/lib/x86_win32 $ENV{OMNIORB_DIR}/lib/x86_win32
    NO_DEFAULT_PATH 
  )

  FIND_LIBRARY( OMNIORB_LIBRARY_COS4
    NAMES
      ${CMAKE_STATIC_LIBRARY_PREFIX}COS4${CMAKE_STATIC_LIBRARY_SUFFIX}
  )
else (WIN32)
  find_library(OMNIORB_LIBRARY_COS4
    NAMES COS4
    PATHS "${OMNIORB_DIR}/lib${LIB_SUFFIX}" "$ENV{OMNIORB_DIR}/lib${LIB_SUFFIX}"
    NO_DEFAULT_PATH)

  find_library(OMNIORB_LIBRARY_COS4
    NAMES COS4)
endif (WIN32)

if (WIN32)
  FIND_LIBRARY( OMNIORB_LIBRARY_COSDynamic4
    NAMES
      ${CMAKE_STATIC_LIBRARY_PREFIX}COSDynamic4${CMAKE_STATIC_LIBRARY_SUFFIX}
    PATHS ${OMNIORB_DIR}/lib/x86_win32 $ENV{OMNIORB_DIR}/lib/x86_win32
    NO_DEFAULT_PATH
  )

  FIND_LIBRARY( OMNIORB_LIBRARY_COSDynamic4
    NAMES
      ${CMAKE_STATIC_LIBRARY_PREFIX}COSDynamic4${CMAKE_STATIC_LIBRARY_SUFFIX}
  )

else (WIN32)
  find_library(OMNIORB_LIBRARY_COSDynamic4
    NAMES COSDynamic4
    PATHS "${OMNIORB_DIR}/lib${LIB_SUFFIX}" "$ENV{OMNIORB_DIR}/lib${LIB_SUFFIX}"
    NO_DEFAULT_PATH)

  find_library(OMNIORB_LIBRARY_COSDynamic4
    NAMES COSDynamic4)
endif (WIN32)
##############################################################################
# find command line tools
##############################################################################
IF (WIN32)
FIND_PROGRAM( OMNIORB_IDL_COMPILER
  NAMES omniidl
  PATHS ${OMNIORB_DIR}/bin/x86_win32 $ENV{OMNIORB_DIR}/bin/x86_win32
  DOC "What is the path where omniidl (the idl compiler) can be found"
  NO_DEFAULT_PATH
)
FIND_PROGRAM( OMNIORB_IDL_COMPILER
  NAMES omniidl
  DOC "What is the path where omniidl (the idl compiler) can be found"
)

FIND_PROGRAM( OMNIORB_OMNINAMES_COMMAND
  NAMES omniNames
  PATHS ${OMNIORB_DIR}/bin/x86_win32 $ENV{OMNIORB_DIR}/bin/x86_win32
  DOC "What is the path where omniNames (the ORB server) can be found"
  NO_DEFAULT_PATH
)
FIND_PROGRAM( OMNIORB_OMNINAMES_COMMAND
  NAMES omniNames
  DOC "What is the path where omniNames (the ORB server) can be found"
)
ELSE(WIN32)
  find_program(OMNIORB_IDL_COMPILER
    NAMES omniidl
    PATHS "${OMNIORB_DIR}/bin" "$ENV{OMNIORB_DIR}/bin"
    NO_DEFAULT_PATH)
    
  find_program(OMNIORB_IDL_COMPILER
    NAMES omniidl)

  find_program(OMNIORB_NAMESERVER
    NAMES omniNames
    PATHS "${OMNIORB_DIR}/bin" "$ENV{OMNIORB_DIR}/bin"
    NO_DEFAULT_PATH)
    
  find_program(OMNIORB_NAMESERVER
    NAMES omniNames)
ENDIF (WIN32)

##############################################################################
# find python back-end
##############################################################################
FIND_PATH( OMNIORB_PYTHON_BACKEND
  NAMES python.py
  PATHS $ENV{OMNIORB_DIR}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages/omniidl_be $ENV{OMNIORBPY_DIR}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages/omniidl_be
        ${OMNIORB_DIR}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages/omniidl_be ${OMNIORBPY_DIR}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages/omniidl_be
  DOC "Path to python-backend directory (omniidl_be) including python.py file"
  NO_DEFAULT_PATH )

FIND_PATH( OMNIORB_PYTHON_BACKEND
  NAMES python.py
  PATHS /usr/lib/omniidl/omniidl_be
  DOC "Path to python-backend directory (omniidl_be) including python.py file" )
##############################################################################
# cook our stuff
##############################################################################
set(OMNIORB_FOUND "FALSE")

if(OMNIORB_INCLUDE_DIR AND
    OMNIORB_LIBRARY_omniORB4 AND
    OMNIORB_LIBRARY_omnithread AND
    OMNIORB_LIBRARY_omniDynamic4 AND
    OMNIORB_IDL_COMPILER)
  set(OMNIORB_FOUND "TRUE")
  mark_as_advanced(OMNIORB_DIR)
  mark_as_advanced(OMNIORB_INCLUDE_DIR)
  mark_as_advanced(OMNIORB_LIBRARY_omniORB4)
  mark_as_advanced(OMNIORB_LIBRARY_omnithread)
  mark_as_advanced(OMNIORB_LIBRARY_omniDynamic4)
  mark_as_advanced(OMNIORB_IDL_COMPILER)
  mark_as_advanced(OMNIORB_NAMESERVER)
  mark_as_advanced(OMNIORB_VERSION)
  mark_as_advanced(OMNIORB_LIBRARY_COS4)
  mark_as_advanced(OMNIORB_LIBRARY_COSDynamic4)
  mark_as_advanced(OMNIORB_PYTHON_BACKEND)

  set(OMNIORB_LIBRARIES
    ${OMNIORB_LIBRARY_omniORB4}
    ${OMNIORB_LIBRARY_omnithread}
    ${OMNIORB_LIBRARY_omniDynamic4})

  if(OMNIORB_LIBRARY_COS4)
    set(OMNIORB_LIBRARIES ${OMNIORB_LIBRARIES} ${OMNIORB_LIBRARY_COS4})
  endif()
  if(OMNIORB_LIBRARY_COSDynamic4)
    set(OMNIORB_LIBRARIES ${OMNIORB_LIBRARIES} ${OMNIORB_LIBRARY_COSDynamic4})
  endif()

  # Optionaly, extract the the version number from the acconfig.h file:
  if( EXISTS ${OMNIORB_INCLUDE_DIR}/omniORB4/acconfig.h )
    file( READ ${OMNIORB_INCLUDE_DIR}/omniORB4/acconfig.h OMNIORB_ACCONFIG_H )
    string( REGEX MATCH "#define[\t ]+PACKAGE_VERSION[\t ]+\"([0-9]+.[0-9]+.[0-9]+)\"" OMNIORB_ACCONFIG_H "${OMNIORB_ACCONFIG_H}" )
    string( REGEX REPLACE ".*\"([0-9]+.[0-9]+.[0-9]+)\".*" "\\1" OMNIORB_VERSION "${OMNIORB_ACCONFIG_H}" )

    if( ${OMNIORB_VERSION} VERSION_LESS ${OMNIORB_MINIMUM_VERSION} )
      message( "WARNING: your version of omniORB is older than the minimum required one (${OMNIORB_MINIMUM_VERSION}), using DIET with this version may result in undetermined behaviors." )
    endif()


  else( EXISTS ${OMNIORB_INCLUDE_DIR}/omniORB4/acconfig.h )
    set( OMNIORB_VERSION "NOT-FOUND" )
  endif( EXISTS ${OMNIORB_INCLUDE_DIR}/omniORB4/acconfig.h )
  set( OMNIORB_VERSION ${OMNIORB_VERSION}
      CACHE STRING "OmniORB version number." )
  
  SET(OMNIORB_IDLCXXFLAGS -Wba -nf)
  SET(OMNIORB_IDLPYFLAGS -bpython)
  IF (OMNIORB_PYTHON_BACKEND) 
    SET(OMNIORB_IDLPYFLAGS "-p ${OMNIORB_PYTHON_BACKEND} ${OMNIORB_IDLPYFLAGS}")
  ENDIF(OMNIORB_PYTHON_BACKEND)
  SET(IDLCXXFLAGS ${OMNIORB_IDLCXXFLAGS})
  SET(IDLPYFLAGS ${OMNIORB_IDLPYFLAGS})
  SET(IDL_CLN_H .hh)
  SET(IDL_SRV_H .hh)
  SET(OMNIORB_DEFINITIONS "-D__x86__ -DCOMP_CORBA_DOUBLE -DCOMP_CORBA_LONG")
  IF(WINDOWS)
    SET(OMNIORB_DEFINITIONS "${OMNIORB_DEFINITIONS} -D__WIN32__")
  ENDIF(WINDOWS)
  IF(APPLE)
    SET(OMNIORB_DEFINITIONS "${OMNIORB_DEFINITIONS} -D__macos__")#for omnithread.h to be checked...
  ENDIF(APPLE)


else()
  message(FATAL_ERROR "OmniORB is required, please define OMNIORB_DIR as environment or cmake variable")
endif()
