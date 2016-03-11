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

# --- rules ---

IF(NOT WIN32)
# ** Now in CTestTestfileInstall.cmake **
# ** In this file only remain unit tests (no SALOME session is needed) **
#  ADD_TEST(NAME SalomeLauncher
#           COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../../UnitTests/prepare_test.py
#           ${CMAKE_CURRENT_SOURCE_DIR}/test_launcher.py
#           -d KERNEL_ROOT_DIR=${CMAKE_INSTALL_PREFIX}
#          )
  INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/test_launcher.py
          DESTINATION ${KERNEL_TEST_DIR}/Launcher)

  INSTALL(FILES CTestTestfileInstall.cmake
          DESTINATION ${KERNEL_TEST_DIR}/Launcher
          RENAME CTestTestfile.cmake)
ENDIF()
