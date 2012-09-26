# Copyright (C) 2012  CEA/DEN, EDF R&D
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

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

SET_SOURCE_FILES_PROPERTIES(libSALOME_Comm.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libSALOME_Comm.i PROPERTIES SWIG_DEFINITIONS "-shadow")

INCLUDE_DIRECTORIES(
  ${PYTHON_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../Communication
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_BINARY_DIR}/idl
)
SET(_libSALOME_Comm_FLAGS "${OMNIORB_DEFINITIONS} ${PLATFORM_DEFINITIONS}")
SET(_libSALOME_Comm_LIBS
  SalomeCommunication
  ${PYTHON_LIBS}
)

IF(MPI_IS_OK)
  INCLUDE_DIRECTORIES(${MPI_INCLUDE_DIRS})

  SET(_libSALOME_Comm_FLAGS "${_libSALOME_Comm_FLAGS} ${MPI_DEFINITIONS}") 
  SET(_libSALOME_Comm_LIBS
    ${_libSALOME_Comm_LIBS}
    ${MPI_LIBS}
  )
ENDIF(MPI_IS_OK)

SWIG_ADD_MODULE(libSALOME_Comm python libSALOME_Comm.i)
SWIG_LINK_LIBRARIES(libSALOME_Comm ${_libSALOME_Comm_LIBS})
SET_TARGET_PROPERTIES(_libSALOME_Comm PROPERTIES COMPILE_FLAGS "${_libSALOME_Comm_FLAGS}")

IF(${MACHINE} STREQUAL WINDOWS)
  SET_TARGET_PROPERTIES(_libSALOME_Comm PROPERTIES DEBUG_OUTPUT_NAME _libSALOME_Comm_d)
ENDIF(${MACHINE} STREQUAL WINDOWS)

INSTALL(TARGETS _libSALOME_Comm DESTINATION ${KERNEL_salomepythondir})
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/libSALOME_Comm.py DESTINATION ${KERNEL_salomepythondir})