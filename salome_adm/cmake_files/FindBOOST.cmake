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

MESSAGE(STATUS "Check for boost ...")

# ------

IF(BOOST_IS_MANDATORY STREQUAL 0)
  SET(BOOST_IS_MANDATORY 0)
  SET(BOOST_IS_OPTIONAL 1)
ENDIF(BOOST_IS_MANDATORY STREQUAL 0)
IF(BOOST_IS_OPTIONAL STREQUAL 0)
  SET(BOOST_IS_MANDATORY 1)
  SET(BOOST_IS_OPTIONAL 0)
ENDIF(BOOST_IS_OPTIONAL STREQUAL 0)
IF(NOT BOOST_IS_MANDATORY AND NOT BOOST_IS_OPTIONAL)
  SET(BOOST_IS_MANDATORY 1)
  SET(BOOST_IS_OPTIONAL 0)
ENDIF(NOT BOOST_IS_MANDATORY AND NOT BOOST_IS_OPTIONAL)

# ------

SET(BOOST_STATUS 1)
IF(WITHOUT_BOOST OR WITH_BOOST STREQUAL 0)
  SET(BOOST_STATUS 0)
  MESSAGE(STATUS "boost disabled from command line.")
ENDIF(WITHOUT_BOOST OR WITH_BOOST STREQUAL 0)

# ------

IF(BOOST_STATUS)
  IF(WITH_BOOST)
    SET(BOOST_ROOT_USER ${WITH_BOOST})
  ELSE(WITH_BOOST)
    SET(BOOST_ROOT_USER $ENV{BOOST_ROOT})
  ENDIF(WITH_BOOST)
ENDIF(BOOST_STATUS)

# -----

IF(BOOST_STATUS)
  IF(BOOST_ROOT_USER)
    SET(BOOST_FIND_PATHS_OPTION NO_DEFAULT_PATH)
  ELSE(BOOST_ROOT_USER)
    SET(BOOST_FIND_PATHS_OPTION)
  ENDIF(BOOST_ROOT_USER)
ENDIF(BOOST_STATUS)

# -----

IF(BOOST_STATUS)
  IF(BOOST_ROOT_USER)
    SET(BOOST_INCLUDE_PATHS ${BOOST_ROOT_USER} ${BOOST_ROOT_USER}/include)
  ELSE(BOOST_ROOT_USER)
    SET(BOOST_INCLUDE_PATHS)
  ENDIF(BOOST_ROOT_USER)
  SET(BOOST_INCLUDE_TO_FIND boost/shared_ptr.hpp)
  FIND_PATH(BOOST_INCLUDE_DIR ${BOOST_INCLUDE_TO_FIND} PATHS ${BOOST_INCLUDE_PATHS} ${BOOST_FIND_PATHS_OPTION})
  IF(BOOST_INCLUDE_DIR)
    IF(BOOST_ROOT_USER)
      SET(BOOST_CPPFLAGS -I${BOOST_INCLUDE_DIR})
    ENDIF(BOOST_ROOT_USER)
    MESSAGE(STATUS "${BOOST_INCLUDE_TO_FIND} found in ${BOOST_INCLUDE_DIR}")
  ELSE(BOOST_INCLUDE_DIR)
    SET(BOOST_STATUS 0)
    IF(BOOST_ROOT_USER)
      MESSAGE(STATUS "${BOOST_INCLUDE_TO_FIND} not found in ${BOOST_INCLUDE_PATHS}, check your BOOST installation.")
    ELSE(BOOST_ROOT_USER)
      MESSAGE(STATUS "${BOOST_INCLUDE_TO_FIND} not found on system, try to use WITH_BOOST option or BOOST_ROOT environment variable.")
    ENDIF(BOOST_ROOT_USER)
  ENDIF(BOOST_INCLUDE_DIR)
ENDIF(BOOST_STATUS)

# ----

IF(BOOST_STATUS)
  IF(BOOST_ROOT_USER)
    SET(BOOST_LIB_PATHS ${BOOST_ROOT_USER}/lib)
  ELSE(BOOST_ROOT_USER)
    SET(BOOST_LIB_PATHS)
  ENDIF(BOOST_ROOT_USER)
ENDIF(BOOST_STATUS)

IF(BOOST_STATUS)
  SET(BOOST_LIBS)
  IF(WINDOWS)
    IF(CMAKE_BUILD_TYPE STREQUAL Debug)
      SET(EXTENSION -gd)
    ELSE(CMAKE_BUILD_TYPE STREQUAL Debug)
      SET(EXTENSION)
    ENDIF(CMAKE_BUILD_TYPE STREQUAL Debug)
    FOREACH(name thread date_time signals filesystem system)
      SET(LIB_NAMES)
      FOREACH(version 1_35 1_38)
	SET(LIB_NAMES ${LIB_NAMES} libboost_${name}-vc90-mt${EXTENSION}-${version})
      ENDFOREACH(version 1_35 1_38)
      FIND_LIBRARY(BOOST_LIB_${name} NAMES ${LIB_NAMES} PATHS ${BOOST_LIB_PATHS})
      IF(NOT BOOST_LIB_${name})
	SET(BOOST_STATUS 0)
      ENDIF(NOT BOOST_LIB_${name})
      SET(BOOST_LIBS ${BOOST_LIBS} ${BOOST_LIB_${name}})
    ENDFOREACH(name thread date_time signals filesystem system)
  ELSE(WINDOWS)
    FOREACH(name thread date_time signals filesystem system)
      FIND_LIBRARY(BOOST_LIB_${name} boost_${name} PATHS ${BOOST_LIB_PATHS})
      IF(NOT BOOST_LIB_${name})
	SET(BOOST_STATUS 0)
      ENDIF(NOT BOOST_LIB_${name})
      SET(BOOST_LIBS ${BOOST_LIBS} ${BOOST_LIB_${name}})
    ENDFOREACH(name thread date_time signals filesystem system)
  ENDIF(WINDOWS)
ENDIF(BOOST_STATUS)

# ------

IF(BOOST_STATUS)
  IF(WINDOWS)
    SET(BOOST_CPPFLAGS ${BOOST_CPPFLAGS} -DBOOST_DISABLE_ASSERTS)
  ENDIF(WINDOWS)
ELSE(BOOST_STATUS)
  IF(BOOST_IS_MANDATORY)
    MESSAGE(FATAL_ERROR "boost not found ... mandatory ... abort")
  ELSE(BOOST_IS_MANDATORY)
    MESSAGE(STATUS "boost not found ... optional ... disabled")
  ENDIF(BOOST_IS_MANDATORY)
ENDIF(BOOST_STATUS)

# ------
