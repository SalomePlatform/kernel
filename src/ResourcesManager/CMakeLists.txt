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

INCLUDE_DIRECTORIES(
  ${LIBXML2_INCLUDE_DIR}
  ${PTHREAD_INCLUDE_DIR}
  ${OMNIORB_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${PROJECT_BINARY_DIR}/idl
)


# This local variable defines the list of dependant libraries common to all target in this package.
SET(COMMON_LIBS
  SalomeNS
  OpUtil
  SALOMEBasics
  SalomeIDLKernel
  ${LIBXML2_LIBRARIES}
)

SET(ResourcesManager_SOURCES
    SALOME_ResourcesCatalog_Parser.cxx
    SALOME_ResourcesCatalog_Handler.cxx
    SALOME_LoadRateManager.cxx
    ResourcesManager.cxx
)

ADD_DEFINITIONS(${OMNIORB_DEFINITIONS} ${LIBXML2_DEFINITIONS})

ADD_LIBRARY(ResourcesManager ${ResourcesManager_SOURCES})
TARGET_LINK_LIBRARIES(ResourcesManager SALOMEBasics ${LIBXML2_LIBRARIES})
INSTALL(TARGETS ResourcesManager EXPORT ${PROJECT_NAME}TargetGroup 
    DESTINATION ${SALOME_INSTALL_LIBS})

SET(SalomeResourcesManager_SOURCES
  SALOME_ResourcesManager.cxx
  SALOME_ResourcesManager_Client.cxx
  SALOME_ResourcesManager_Common.cxx
)
ADD_LIBRARY(SalomeResourcesManager ${SalomeResourcesManager_SOURCES})
TARGET_LINK_LIBRARIES(SalomeResourcesManager ResourcesManager ${COMMON_LIBS})
INSTALL(TARGETS SalomeResourcesManager EXPORT ${PROJECT_NAME}TargetGroup
        DESTINATION ${SALOME_INSTALL_LIBS})

SET(COMMON_HEADERS_HXX
  ResourcesManager.hxx
  ResourcesManager_Defs.hxx
  SALOME_LoadRateManager.hxx
  SALOME_ResourcesCatalog_Parser.hxx
  SALOME_ResourcesManager.hxx
  SALOME_ResourcesManager_Client.hxx
  SALOME_ResourcesManager_Common.hxx
  )
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
