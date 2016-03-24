# - Find Paco installation
#
# Variables set are:
#    PACO_INCLUDE_DIRS - include directories to add when compiling
#    PACO_LIBRARIES    - Paco libraries to link against
#
# You can set the environment or CMake variable PACO_ROOT_DIR to guide
# the detection mechanism. 
#
#############################################################
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

# ------

IF(NOT Paco_FIND_QUIETLY)
  MESSAGE(STATUS "Check for Paco ...")
ENDIF()

# ------

SET(PACO_ROOT_DIR $ENV{PACO_ROOT_DIR} CACHE PATH "Path to Paco.")
IF(PACO_ROOT_DIR)
  LIST(APPEND CMAKE_PREFIX_PATH "${PACO_ROOT_DIR}")
ENDIF(PACO_ROOT_DIR)

FIND_PATH(PACO_INCLUDE_DIRS PaCO++_InterfaceManager_impl.h)

FIND_LIBRARY(PACO_LIBRARY_PaCO PaCO)
FIND_LIBRARY(PACO_LIBRARY_GaBro GaBro)
FIND_LIBRARY(PACO_LIBRARY_BasicBC BasicBC)
FIND_LIBRARY(PACO_LIBRARY_PaCO_dummy PaCO_dummy)
FIND_LIBRARY(PACO_LIBRARY_PaCO_mpi PaCO_mpi)
FIND_LIBRARY(PACO_LIBRARY_PaCO_omnithread PaCO_omnithread)

FIND_PROGRAM(PACO_IDL PaCOIdlTool.sh)

MARK_AS_ADVANCED(PACO_INCLUDE_DIRS PACO_LIBRARY_PaCO PACO_LIBRARY_GaBro
                 PACO_LIBRARY_BasicBC PACO_LIBRARY_PaCO_dummy
                 PACO_LIBRARY_PaCO_mpi PACO_LIBRARY_PaCO_omnithread PACO_IDL)

SET(PACO_LIBRARIES
    ${PACO_LIBRARY_PaCO}
    ${PACO_LIBRARY_GaBro}
    ${PACO_LIBRARY_BasicBC}
    ${PACO_LIBRARY_PaCO_dummy}
    ${PACO_LIBRARY_PaCO_mpi}
    ${PACO_LIBRARY_PaCO_omnithread})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Paco
    REQUIRED_VARS PACO_INCLUDE_DIRS PACO_LIBRARY_PaCO PACO_LIBRARY_GaBro
                  PACO_LIBRARY_BasicBC PACO_LIBRARY_PaCO_dummy
                  PACO_LIBRARY_PaCO_mpi PACO_LIBRARY_PaCO_omnithread PACO_IDL)
