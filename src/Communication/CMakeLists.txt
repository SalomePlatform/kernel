# Copyright (C) 2012-2025  CEA, EDF, OPEN CASCADE
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

INCLUDE_DIRECTORIES(
  ${OMNIORB_INCLUDE_DIR}
  ${PTHREAD_INCLUDE_DIR}
  ${MPI_CXX_INCLUDE_PATH}
  ${PYTHON_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${SALOMEBOOTSTRAP_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../GenericObj
  ${PROJECT_BINARY_DIR}/idl
)

SET(COMMON_LIBS
  OpUtil
  ${SALOMEBOOTSTRAP_SALOMELocalTrace}
  SalomeIDLKernel
  SalomeGenericObj
  ${PYTHON_LIBRARIES}
  ${MPI_CXX_LIBRARIES}
)

SET(SalomeCommunication_SOURCES
  SALOME_Comm_i.cxx
  SALOME_Matrix_i.cxx
  SenderFactory.cxx
  MultiCommException.cxx
  SALOMEMultiComm.cxx
  ReceiverFactory.cxx
  MatrixClient.cxx
  SenderByteImpl.cxx
)

ADD_DEFINITIONS(${OMNIORB_DEFINITIONS} ${MPI_DEFINITIONS})

ADD_LIBRARY(SalomeCommunication ${SalomeCommunication_SOURCES})
TARGET_LINK_LIBRARIES(SalomeCommunication ${COMMON_LIBS})
set_target_properties(SalomeCommunication PROPERTIES INSTALL_RPATH "$ORIGIN")
INSTALL(TARGETS SalomeCommunication EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

SET(COMMON_HEADERS_HXX
  MatrixClient.hxx
  MultiCommException.hxx
  Receiver.hxx
  ReceiverFactory.hxx
  Receivers.hxx
  SALOMEMultiComm.hxx
  SALOME_Comm_i.hxx
  SALOME_Communication.hxx
  SALOME_Matrix_i.hxx
  SenderFactory.hxx
  SenderByteImpl.hxx
)

INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
