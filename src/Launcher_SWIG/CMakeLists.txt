# Copyright (C) 2019-2021  CEA/DEN, EDF R&D, OPEN CASCADE
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

INCLUDE(${SWIG_USE_FILE})

ADD_DEFINITIONS(${PYTHON_DEFINITIONS})

SET(Launcher_target_name pylauncher)

IF(SALOME_USE_LIBBATCH)
  ADD_DEFINITIONS(-DWITH_LIBBATCH)
  SET(SWIG_MODULE_${Launcher_target_name}_EXTRA_FLAGS "-DWITH_LIBBATCH")
ENDIF(SALOME_USE_LIBBATCH)

SET_SOURCE_FILES_PROPERTIES(Launcher.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(Launcher.i PROPERTIES SWIG_FLAGS "-py3")


INCLUDE_DIRECTORIES(
  ${PYTHON_INCLUDE_DIRS}
  ${LIBBATCH_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/../ResourcesManager
  ${CMAKE_CURRENT_SOURCE_DIR}/../Launcher
  )

SWIG_ADD_LIBRARY(${Launcher_target_name} LANGUAGE python SOURCES Launcher.i)

SWIG_LINK_LIBRARIES(${Launcher_target_name} ${PYTHON_LIBRARIES} ${PLATFORM_LIBS} Launcher)

SWIG_CHECK_GENERATION(${Launcher_target_name})

INSTALL(TARGETS ${SWIG_MODULE_${Launcher_target_name}_REAL_NAME} DESTINATION ${SALOME_INSTALL_PYTHON})

SET(PYFILES_TO_INSTALL ${PYFILES_TO_INSTALL} ${CMAKE_CURRENT_BINARY_DIR}/pylauncher.py)

SALOME_INSTALL_SCRIPTS("${PYFILES_TO_INSTALL}" ${SALOME_INSTALL_PYTHON} EXTRA_DPYS "${SWIG_MODULE_${Launcher_target_name}_REAL_NAME}")

IF(SALOME_BUILD_TESTS)
  ADD_SUBDIRECTORY(Test)
ENDIF(SALOME_BUILD_TESTS)
