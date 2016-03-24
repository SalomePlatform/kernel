# - Find LibBatch installation
#
# The following variable are set:
#   LIBBATCH_FOUND        - TRUE if LibBatch installation has been found
#   LIBBATCH_INCLUDE_DIRS - LibBatch headers path
#   LIBBATCH_LIBRARIES    - LibBatch libraries
#
#  The CMake (or environment) variable LIBBATCH_ROOT_DIR can be set to
#  guide the detection and indicate a root directory to look into.
#
############################################################################
# Copyright (C) 2012-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

# ------

IF(NOT LIBBATCH_FIND_QUIETLY)
    MESSAGE(STATUS "Check for LibBatch ...")
ENDIF()

# ------

SET(LIBBATCH_ROOT_DIR $ENV{LIBBATCH_ROOT_DIR} CACHE PATH "Path to the LibBatch.")
IF(LIBBATCH_ROOT_DIR)
  LIST(APPEND CMAKE_PREFIX_PATH "${LIBBATCH_ROOT_DIR}")
ENDIF()

FIND_PATH(LIBBATCH_INCLUDE_DIRS libbatch/BatchManager.hxx)
FIND_LIBRARY(LIBBATCH_LIBRARIES NAMES batch)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibBatch REQUIRED_VARS LIBBATCH_INCLUDE_DIRS LIBBATCH_LIBRARIES)

IF(LIBBATCH_FOUND)
  IF(NOT LIBBATCH_FIND_QUIETLY)
    MESSAGE(STATUS "LibBatch library: ${LIBBATCH_LIBRARIES}")
  ENDIF()
ENDIF()
