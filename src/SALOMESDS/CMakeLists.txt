# Copyright (C) 2012-2023  CEA, EDF, OPEN CASCADE
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

include(${SWIG_USE_FILE})

INCLUDE_DIRECTORIES(
  ${OMNIORB_INCLUDE_DIR}
  ${PTHREAD_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/idl
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../ArgvKeeper
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../Launcher
  ${CMAKE_CURRENT_SOURCE_DIR}/../Container
  ${CMAKE_CURRENT_SOURCE_DIR}/../ResourcesManager
  ${CMAKE_CURRENT_SOURCE_DIR}/../KernelHelpers
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_DEFINITIONS(${OMNIORB_DEFINITIONS} ${PYTHON_DEFINITIONS})

SET(COMMON_LIBS ArgvKeeper SalomeNS SalomeContainer SalomeIDLKernel ${OMNIORB_LIBRARIES} ${PYTHON_LIBRARIES})

SET(SalomeSDS_SOURCES
  SALOMESDS_Exception.cxx
  SALOMESDS_RefCountServ.cxx
  SALOMESDS_BasicDataServer.cxx
  SALOMESDS_DataScopeServer.cxx
  SALOMESDS_RequestSwitcher.cxx
  SALOMESDS_DataServerManager.cxx
  SALOMESDS_PickelizedPyObjServer.cxx
  SALOMESDS_PickelizedPyObjRdOnlyServer.cxx
  SALOMESDS_PickelizedPyObjRdExtServer.cxx
  SALOMESDS_PickelizedPyObjRdExtInitServer.cxx
  SALOMESDS_PickelizedPyObjRdWrServer.cxx
  SALOMESDS_Transaction.cxx
  SALOMESDS_KeyWaiter.cxx
  SALOMESDS_Sha1Keeper.cxx
  )

ADD_LIBRARY(SalomeSDS ${SalomeSDS_SOURCES})
TARGET_LINK_LIBRARIES(SalomeSDS ${COMMON_LIBS} ${PTHREAD_LIBRARIES})
INSTALL(TARGETS SalomeSDS EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

ADD_EXECUTABLE(SALOME_DataScopeServer SALOME_DataScopeServer.cxx)
TARGET_LINK_LIBRARIES(SALOME_DataScopeServer SalomeSDS)
ADD_EXECUTABLE(SALOME_DataScopeServer_SSL SALOME_DataScopeServer_SSL.cxx)
TARGET_LINK_LIBRARIES(SALOME_DataScopeServer_SSL SalomeSDS)
INSTALL(TARGETS SALOME_DataScopeServer SALOME_DataScopeServer_SSL DESTINATION ${SALOME_INSTALL_BINS})
INSTALL(FILES SalomeSDSClt.py SALOMEWrappedStdType.py DESTINATION ${SALOME_INSTALL_BINS})
FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})

#

SET(KernelSDS_HEADERS KernelSDS.hxx KernelSDS.i)
SET(KernelSDS_SOURCES KernelSDS.cxx ${KernelSDS_HEADERS})
SET_SOURCE_FILES_PROPERTIES(KernelSDS.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(KernelSDS.i PROPERTIES SWIG_FLAGS "-py3")
SET_SOURCE_FILES_PROPERTIES(KernelSDS_wrap.cpp PROPERTIES COMPILE_FLAGS "-DHAVE_CONFIG_H")
SET(_swig_SCRIPTS ${CMAKE_CURRENT_BINARY_DIR}/KernelSDS.py )
IF(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
  SWIG_ADD_MODULE(KernelSDS python ${KernelSDS_SOURCES})
ELSE()
  SWIG_ADD_LIBRARY(KernelSDS LANGUAGE python SOURCES ${KernelSDS_SOURCES})
ENDIF()
IF(${MACHINE} STREQUAL WINDOWS)
  SET_TARGET_PROPERTIES(_KernelSDS PROPERTIES DEBUG_OUTPUT_NAME _KernelSDS_d)
ENDIF(${MACHINE} STREQUAL WINDOWS)
SWIG_LINK_LIBRARIES(KernelSDS ${PYTHON_LIBRARIES} ${PLATFORM_LIBS} SalomeSDS)
install(TARGETS _KernelSDS DESTINATION ${SALOME_INSTALL_LIBS})
install(FILES ${KernelSDS_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
SALOME_INSTALL_SCRIPTS("${_swig_SCRIPTS}"  ${SALOME_INSTALL_BINS} EXTRA_DPYS "${SWIG_MODULE_KernelSDS_REAL_NAME}")

# tests part
SET(LOCAL_TEST_DIR ${KERNEL_TEST_DIR}/SALOMESDS)
INSTALL(FILES TestSalomeSDS.py TestSalomeSDSHelper0.py DESTINATION ${LOCAL_TEST_DIR})
INSTALL(FILES CTestTestfileInstall.cmake DESTINATION ${LOCAL_TEST_DIR} RENAME CTestTestfile.cmake)
