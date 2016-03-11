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
  ${PTHREAD_INCLUDE_DIR}
  ${OMNIORB_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
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
  SALOMELocalTrace
  SALOMEBasics
  SalomeIDLKernel
  ${LIBXML2_LIBRARIES}
  ${PTHREAD_LIBRARIES}
)

SET(SalomeCatalog_SOURCES
  SALOME_ModuleCatalog_Handler.cxx
  SALOME_ModuleCatalog_Parser_IO.cxx
  SALOME_ModuleCatalog_impl.cxx
  SALOME_ModuleCatalog_Acomponent_impl.cxx
)
ADD_DEFINITIONS(${LIBXML2_DEFINITIONS} ${OMNIORB_DEFINITIONS})

ADD_LIBRARY(SalomeCatalog ${SalomeCatalog_SOURCES})
TARGET_LINK_LIBRARIES(SalomeCatalog ${COMMON_LIBS})
INSTALL(TARGETS SalomeCatalog EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

ADD_EXECUTABLE(SALOME_ModuleCatalog_Server SALOME_ModuleCatalog_Server.cxx)
TARGET_LINK_LIBRARIES(SALOME_ModuleCatalog_Server SalomeCatalog ${COMMON_LIBS} ${OMNIORB_LIBRARIES})

ADD_EXECUTABLE(SALOME_ModuleCatalog_Client SALOME_ModuleCatalog_Client.cxx)
TARGET_LINK_LIBRARIES(SALOME_ModuleCatalog_Client SalomeCatalog ${COMMON_LIBS} ${OMNIORB_LIBRARIES})

INSTALL(TARGETS SALOME_ModuleCatalog_Server SALOME_ModuleCatalog_Client 
        EXPORT ${PROJECT_NAME}TargetGroup
        DESTINATION ${SALOME_INSTALL_BINS})

# Executable scripts to be installed
SALOME_INSTALL_SCRIPTS(TestModuleCatalog.py ${SALOME_INSTALL_SCRIPT_PYTHON})

SET(COMMON_HEADERS_HXX
  SALOME_ModuleCatalog.hxx
  SALOME_ModuleCatalog_Acomponent_impl.hxx
  SALOME_ModuleCatalog_impl.hxx
  )

INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
