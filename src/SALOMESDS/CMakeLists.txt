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
# Author : Anthony GEAY (EDF R&D)

INCLUDE_DIRECTORIES(
  ${OMNIORB_INCLUDE_DIR}
  ${PTHREAD_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/idl
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../Container
  ${CMAKE_CURRENT_SOURCE_DIR}/../ResourcesManager
)

ADD_DEFINITIONS(${OMNIORB_DEFINITIONS} ${PYTHON_DEFINITIONS})

SET(COMMON_LIBS SalomeNS SalomeContainer SalomeIDLKernel ${OMNIORB_LIBRARIES} ${PYTHON_LIBRARIES})

SET(SalomeSDS_SOURCES
  SALOMESDS_Exception.cxx
  SALOMESDS_RefCountServ.cxx
  SALOMESDS_BasicDataServer.cxx
  SALOMESDS_DataScopeServer.cxx
  SALOMESDS_DataServerManager.cxx
  SALOMESDS_PickelizedPyObjServer.cxx
  SALOMESDS_PickelizedPyObjRdOnlyServer.cxx
  SALOMESDS_PickelizedPyObjRdExtServer.cxx
  SALOMESDS_PickelizedPyObjRdExtInitServer.cxx
  SALOMESDS_PickelizedPyObjRdWrServer.cxx
  SALOMESDS_Transaction.cxx
  SALOMESDS_KeyWaiter.cxx
  )

ADD_LIBRARY(SalomeSDS ${SalomeSDS_SOURCES})
TARGET_LINK_LIBRARIES(SalomeSDS ${COMMON_LIBS} ${PTHREAD_LIBRARIES})
INSTALL(TARGETS SalomeSDS EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

ADD_EXECUTABLE(SALOME_DataScopeServer SALOME_DataScopeServer.cxx)
TARGET_LINK_LIBRARIES(SALOME_DataScopeServer SalomeSDS)
INSTALL(TARGETS SALOME_DataScopeServer DESTINATION ${SALOME_INSTALL_BINS})
INSTALL(FILES SalomeSDSClt.py SALOMEWrappedStdType.py DESTINATION ${SALOME_INSTALL_BINS})
FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
# tests part
SET(LOCAL_TEST_DIR ${KERNEL_TEST_DIR}/SALOMESDS)
INSTALL(FILES TestSalomeSDS.py TestSalomeSDSHelper0.py DESTINATION ${LOCAL_TEST_DIR})
INSTALL(FILES CTestTestfileInstall.cmake DESTINATION ${LOCAL_TEST_DIR} RENAME CTestTestfile.cmake)
