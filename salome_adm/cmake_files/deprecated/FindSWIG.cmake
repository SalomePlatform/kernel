# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
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

# ------

MESSAGE(STATUS "Check for swig ...")

# ------

IF(SWIG_IS_MANDATORY STREQUAL 0)
  SET(SWIG_IS_MANDATORY 0)
  SET(SWIG_IS_OPTIONAL 1)
ENDIF(SWIG_IS_MANDATORY STREQUAL 0)
IF(SWIG_IS_OPTIONAL STREQUAL 0)
  SET(SWIG_IS_MANDATORY 1)
  SET(SWIG_IS_OPTIONAL 0)
ENDIF(SWIG_IS_OPTIONAL STREQUAL 0)
IF(NOT SWIG_IS_MANDATORY AND NOT SWIG_IS_OPTIONAL)
  SET(SWIG_IS_MANDATORY 1)
  SET(SWIG_IS_OPTIONAL 0)
ENDIF(NOT SWIG_IS_MANDATORY AND NOT SWIG_IS_OPTIONAL)

# ------

SET(SWIG_STATUS 1)
IF(WITHOUT_SWIG OR WITH_SWIG STREQUAL 0)
  SET(SWIG_STATUS 0)
  MESSAGE(STATUS "swig disabled from command line.")
ENDIF(WITHOUT_SWIG OR WITH_SWIG STREQUAL 0)

# ------

IF(SWIG_STATUS)
  IF(WITH_SWIG)
    SET(SWIG_ROOT_USER ${WITH_SWIG})
  ENDIF(WITH_SWIG)
  IF(NOT SWIG_ROOT_USER)
    SET(SWIG_ROOT_USER $ENV{SWIG_ROOT})
  ENDIF(NOT SWIG_ROOT_USER)
  IF(NOT SWIG_ROOT_USER)
    SET(SWIG_ROOT_USER $ENV{SWIGHOME})
  ENDIF(NOT SWIG_ROOT_USER)
ENDIF(SWIG_STATUS)

# ------

IF(SWIG_STATUS)
  SET(SWIG_EXECUTABLE_TO_FIND swig)
  IF(SWIG_ROOT_USER)
    FIND_PROGRAM(SWIG_EXECUTABLE ${SWIG_EXECUTABLE_TO_FIND} PATHS ${SWIG_ROOT_USER}/bin NO_DEFAULT_PATH)
  ELSE(SWIG_ROOT_USER)
    FIND_PROGRAM(SWIG_EXECUTABLE ${SWIG_EXECUTABLE_TO_FIND})
  ENDIF(SWIG_ROOT_USER)
  IF(SWIG_EXECUTABLE)
    MESSAGE(STATUS "${SWIG_EXECUTABLE_TO_FIND} found: ${SWIG_EXECUTABLE}")
  ELSE(SWIG_EXECUTABLE)
    MESSAGE(STATUS "${SWIG_EXECUTABLE_TO_FIND} not found, try to use WITH_SWIG option or SWIG_ROOT (or SWIGHOME) environment variable")
    SET(SWIG_STATUS 0)
  ENDIF(SWIG_EXECUTABLE)
ENDIF(SWIG_STATUS)

# ----

IF(SWIG_STATUS)
  SET(SWIG_FLAGS -c++ -python -shadow)
ELSE(SWIG_STATUS)
  IF(SWIG_IS_MANDATORY)
    MESSAGE(FATAL_ERROR "swig not found ... mandatory ... abort")
  ELSE(SWIG_IS_MANDATORY)
    MESSAGE(STATUS "swig not found ... optional ... disabled")
  ENDIF(SWIG_IS_MANDATORY)
ENDIF(SWIG_STATUS)

# ----

IF(NUMPY_STATUS)
  SET(SWIG_FLAGS ${SWIG_FLAGS} -DWITH_NUMPY=WITH_NUMPY)
ENDIF(NUMPY_STATUS)

# ----
