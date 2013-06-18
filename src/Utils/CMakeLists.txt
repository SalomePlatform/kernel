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

INCLUDE_DIRECTORIES(
  ${OMNIORB_INCLUDE_DIR}
  ${PTHREAD_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_BINARY_DIR}/idl
)
SET(OpUtil_SOURCES
  Utils_Timer.cxx
  duplicate.cxx
  Utils_CommException.cxx
  Utils_SALOME_Exception.cxx
  Utils_Identity.cxx
  Utils_ORB_INIT.cxx
  Utils_DESTRUCTEUR_GENERIQUE.cxx
  Utils_ExceptHandlers.cxx
  Utils_Mutex.cxx
)

ADD_DEFINITIONS(${OMNIORB_DEFINITIONS} ${PTHREAD_DEFINITIONS})
ADD_LIBRARY(OpUtil ${OpUtil_SOURCES})
TARGET_LINK_LIBRARIES(OpUtil SALOMELocalTrace SalomeIDLKernel ${OMNIORB_LIBRARIES} ${PTHREAD_LIBRARIES})
INSTALL(TARGETS OpUtil DESTINATION ${SALOME_INSTALL_LIBS})

SET(COMMON_SCRIPTS
  Utils_Identity.py 
  SALOME_utilities.py
)
SALOME_INSTALL_SCRIPTS("${COMMON_SCRIPTS}" ${SALOME_INSTALL_SCRIPT_PYTHON})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
