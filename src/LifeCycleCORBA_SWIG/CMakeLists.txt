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

INCLUDE(${SWIG_USE_FILE})

SET_SOURCE_FILES_PROPERTIES(libSALOME_LifeCycleCORBA.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(libSALOME_LifeCycleCORBA.i PROPERTIES SWIG_DEFINITIONS "-shadow")
ADD_DEFINITIONS(${OMNIORB_DEFINITIONS})

INCLUDE_DIRECTORIES(
  ${PYTHON_INCLUDE_DIRS}
  ${PTHREAD_INCLUDE_DIR}
  ${OMNIORB_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../LifeCycleCORBA
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${PROJECT_BINARY_DIR}/idl
  )

SWIG_ADD_MODULE(libSALOME_LifeCycleCORBA python libSALOME_LifeCycleCORBA.i)
SWIG_LINK_LIBRARIES(libSALOME_LifeCycleCORBA ${PYTHON_LIBRARIES} SalomeLifeCycleCORBA)
TARGET_LINK_LIBRARIES(_libSALOME_LifeCycleCORBA ${PLATFORM_LIBS})

IF(${MACHINE} STREQUAL WINDOWS)
  SET_TARGET_PROPERTIES(_libSALOME_LifeCycleCORBA PROPERTIES DEBUG_OUTPUT_NAME _libSALOME_LifeCycleCORBA_d)
ENDIF(${MACHINE} STREQUAL WINDOWS)

INSTALL(TARGETS _libSALOME_LifeCycleCORBA DESTINATION ${SALOME_INSTALL_PYTHON})
SET(SCRIPTS
  LifeCycleCORBA.py 
  TestLifeCycleCORBA.py 
)
SALOME_INSTALL_SCRIPTS("${SCRIPTS}" ${SALOME_INSTALL_SCRIPT_PYTHON})
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/libSALOME_LifeCycleCORBA.py DESTINATION ${SALOME_INSTALL_PYTHON})
