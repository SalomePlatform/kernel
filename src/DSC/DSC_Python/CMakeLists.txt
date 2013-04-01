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

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

SET_SOURCE_FILES_PROPERTIES(calcium.i PROPERTIES CPLUSPLUS ON)
SET(CMAKE_SWIG_FLAGS "-noexcept")
ADD_DEFINITIONS(${OMNIORB_DEFINITIONS} ${BOOST_DEFINITIONS})

INCLUDE_DIRECTORIES(
  ${OMNIORB_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/salome_adm
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_User
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_User/Datastream
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_User/Datastream/Palm
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_User/Datastream/Calcium
  ${CMAKE_BINARY_DIR}/src/DSC/DSC_User/Datastream/Calcium
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_User/Basic
  ${CMAKE_SOURCE_DIR}/src/DSC/DSC_Basic
  ${CMAKE_SOURCE_DIR}/src/SALOMELocalTrace
  ${CMAKE_SOURCE_DIR}/src/Basics
  ${CMAKE_SOURCE_DIR}/src/Utils
  ${CMAKE_SOURCE_DIR}/src/Container
  ${CMAKE_SOURCE_DIR}/src/GenericObj
  ${CMAKE_SOURCE_DIR}/src/Notification
  ${CMAKE_BINARY_DIR}/idl
  )

SET(_calcium_LIBS
  CalciumC
  SalomeDatastream
  SalomeDSCSupervBasic
  SalomeDSCSuperv
  SalomeDSCContainer
  SalomeContainer
  ${OMNIORB_LIBRARIES}
)

SWIG_ADD_MODULE(calcium python calcium.i)
SWIG_LINK_LIBRARIES(calcium ${_calcium_LIBS})
INSTALL(TARGETS _calcium DESTINATION ${KERNEL_salomepythondir})

SET(SCRIPTS
  dsccalcium.py
)
SALOME_INSTALL_SCRIPTS( ${SCRIPTS} ${KERNEL_salomepythondir})
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/calcium.py DESTINATION ${KERNEL_salomepythondir})
