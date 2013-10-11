# - Find SIMANIO installation
#
# The following variable are set:
#   SIMANIO_FOUND       - TRUE if SIMANIO installation has been found
#   SIMANIO_INCLUDE_DIR - SIMANIO headers path
#   SIMANIO_LIBRARIES   - SIMANIO libraries
#
#  The CMake (or environment) variable SIMANIO_ROOT_DIR can be set to
#  guide the detection and indicate a root directory to look into.
#
############################################################################
# Copyright (C) 2012-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

IF(NOT SIMANIO_FIND_QUIETLY)
    MESSAGE(STATUS "Check for SimanIO ...")
ENDIF()

# ------

SET(SIMANIO_ROOT_DIR $ENV{SIMANIO_ROOT_DIR} CACHE PATH "Path to the SIMANIO.")
IF(SIMANIO_ROOT_DIR)
  LIST(APPEND CMAKE_PREFIX_PATH "${SIMANIO_ROOT_DIR}")
ENDIF()

FIND_PATH(SIMANIO_INCLUDE_DIR SimanIO_Link.hxx)
FIND_LIBRARY(SIMANIO_LIBRARIES NAMES SimanIO)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SIMANIO REQUIRED_VARS SIMANIO_INCLUDE_DIR SIMANIO_LIBRARIES)

IF(SIMANIO_FOUND)
  IF(NOT SIMANIO_FIND_QUIETLY)
    MESSAGE(STATUS "SIMANIO library: ${SIMANIO_LIBRARIES}")
  ENDIF()
ENDIF()
