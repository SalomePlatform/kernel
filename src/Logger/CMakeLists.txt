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
  ${OMNIORB_INCLUDE_DIR}
  ${PTHREAD_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${PROJECT_BINARY_DIR}/idl
)
ADD_DEFINITIONS(${OMNIORB_DEFINITIONS})

ADD_LIBRARY(SalomeLoggerServer SALOME_Trace.cxx SALOME_Logger_Server.cxx)
TARGET_LINK_LIBRARIES(SalomeLoggerServer SalomeIDLKernel ${OMNIORB_LIBRARIES} ${PLATFORM_LIBS})
INSTALL(TARGETS SalomeLoggerServer EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

SET(SALOME_Logger_Server_SOURCES
  SALOME_Logger_Server_main.cxx
  SALOME_Logger_Server.hxx
)

SET(SALOME_Logger_Server_LIBS
  SalomeLoggerServer
  SalomeIDLKernel
  ${OMNIORB_LIBRARIES}
)

ADD_EXECUTABLE(SALOME_Logger_Server ${SALOME_Logger_Server_SOURCES})
TARGET_LINK_LIBRARIES(SALOME_Logger_Server ${SALOME_Logger_Server_LIBS})
INSTALL(TARGETS SALOME_Logger_Server EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_BINS})

# Executable scripts to be installed
SALOME_INSTALL_SCRIPTS(SALOME_Trace.py ${SALOME_INSTALL_SCRIPT_PYTHON})

INSTALL(FILES SALOME_Trace.hxx DESTINATION ${SALOME_INSTALL_HEADERS})
