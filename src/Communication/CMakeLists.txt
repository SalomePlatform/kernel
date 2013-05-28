# Copyright (C) 2012  CEA/DEN, EDF R&D, OPEN CASCADE
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
  ${PTHREAD_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIRS}
  ${MPI_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_BINARY_DIR}/idl
)

SET(COMMON_LIBS
  OpUtil
  SALOMELocalTrace
  SalomeIDLKernel
)

SET(SalomeCommunication_SOURCES
  SALOME_Comm_i.cxx
  SALOME_Matrix_i.cxx
  SenderFactory.cxx
  MultiCommException.cxx
  SALOMEMultiComm.cxx
  ReceiverFactory.cxx
  MatrixClient.cxx
  MultiCommException.hxx
  SALOME_Comm_i.hxx
  SALOME_Matrix_i.hxx
  SenderFactory.hxx
  ReceiverFactory.hxx
  MatrixClient.hxx
  SALOMEMultiComm.hxx
  Receivers.hxx
  Receiver.hxx
)

ADD_LIBRARY(SalomeCommunication SHARED ${SalomeCommunication_SOURCES})
SET_TARGET_PROPERTIES(SalomeCommunication PROPERTIES COMPILE_FLAGS "${OMNIORB_DEFINITIONS} ${MPI_DEFINITIONS} ${PLATFORM_DEFINITIONS}")
TARGET_LINK_LIBRARIES(SalomeCommunication ${COMMON_LIBS} ${PYTHON_LIBS} ${MPI_LIBS})
INSTALL(TARGETS SalomeCommunication DESTINATION ${KERNEL_salomelib_LIBS})

SET(COMMON_HEADERS_HXX
  MatrixClient.hxx
  MultiCommException.hxx
  Receiver.hxx
  ReceiverFactory.hxx
  Receivers.hxx
  Receivers.cxx
  SALOMEMultiComm.hxx
  SALOME_Comm_i.hxx
  SALOME_Communication.hxx
  SALOME_Matrix_i.hxx
  SenderFactory.hxx
)

INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${KERNEL_salomeinclude_HEADERS})
