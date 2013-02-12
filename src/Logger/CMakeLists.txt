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
  ${OMNIORB_INCLUDE_DIR}
  ${CMAKE_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_BINARY_DIR}/idl
)
ADD_DEFINITIONS(${OMNIORB_DEFINITIONS})

ADD_LIBRARY(SalomeLoggerServer SALOME_Trace.cxx SALOME_Logger_Server.cxx)
TARGET_LINK_LIBRARIES(SalomeLoggerServer SalomeIDLKernel ${OMNIORB_LIBRARIES} ${PLATFORM_LIBS})
INSTALL(TARGETS SalomeLoggerServer DESTINATION ${KERNEL_salomelib_LIBS})

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
INSTALL(TARGETS SALOME_Logger_Server DESTINATION ${KERNEL_salomebin_BINS})

# Executable scripts to be installed
SALOME_INSTALL_SCRIPTS(SALOME_Trace.py ${KERNEL_salomescript_PYTHON})

INSTALL(FILES SALOME_Trace.hxx DESTINATION ${KERNEL_salomeinclude_HEADERS})
