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

INCLUDE(${SWIG_USE_FILE})

SET_SOURCE_FILES_PROPERTIES(NOTIFICATION.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(NOTIFICATION.i PROPERTIES SWIG_DEFINITIONS "-shadow")
SET_SOURCE_FILES_PROPERTIES(NOTIFICATIONPYTHON_wrap.cxx PROPERTIES COMPILE_FLAGS "${OMNIORB_DEFINITIONS} -DHAVE_CONFIG_H")

INCLUDE_DIRECTORIES(
  ${PYTHON_INCLUDE_DIRS}
  ${PTHREAD_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../Notification
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  )

SWIG_ADD_MODULE(libNOTIFICATION python NOTIFICATION.i NOTIFICATION_Swig.cxx)
SWIG_LINK_LIBRARIES(libNOTIFICATION ${PYTHON_LIBRARIES} SalomeNotification)
ADD_DEFINITIONS(${OMNIORB_DEFINITIONS} ${PLATFORM_DEFINITIONS})

IF(${MACHINE} STREQUAL WINDOWS)
  SET_TARGET_PROPERTIES(_libNOTIFICATION PROPERTIES DEBUG_OUTPUT_NAME _libNOTIFICATION_d)
ENDIF(${MACHINE} STREQUAL WINDOWS)

INSTALL(TARGETS _libNOTIFICATION DESTINATION ${KERNEL_salomepythondir})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMON_HEADERS_HXX} DESTINATION ${MED_salomeinclude_HEADERS})
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/libNOTIFICATION.py DESTINATION ${KERNEL_salomepythondir})
