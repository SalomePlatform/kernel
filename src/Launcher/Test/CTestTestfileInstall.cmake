# Copyright (C) 2015-2024  CEA, EDF
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

IF(NOT WIN32)
  SET(TEST_NAME ${COMPONENT_NAME}_Launcher)
  ADD_TEST(${TEST_NAME} ${PYTHON_TEST_DRIVER} 2000 test_launcher.py)
  SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES LABELS "${COMPONENT_NAME}")
    
    SET(TEST_NAME ${COMPONENT_NAME}_AttachedLauncher)
    ADD_TEST(${TEST_NAME} ${PYTHON_TEST_DRIVER} 2000 TestSSLAttached.py)
    SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES LABELS "${COMPONENT_NAME}")

  SET(TEST_NAME ${COMPONENT_NAME}_StressLauncher)
  ADD_TEST(${TEST_NAME} ${PYTHON_TEST_DRIVER} 2000 ./test_stress.sh)
  SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES LABELS "${COMPONENT_NAME}")

  SET(TEST_NAME ${COMPONENT_NAME}_PerfLogManager1)
  ADD_TEST(${TEST_NAME} ${PYTHON_TEST_DRIVER} 2000 testPerfLogManager1.py)
  SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES LABELS "${COMPONENT_NAME}")
  
  SET(TEST_NAME ${COMPONENT_NAME}_testCrashProofContainer)
  ADD_TEST(${TEST_NAME} ${PYTHON_TEST_DRIVER} 2000 testCrashProofContainer.py)
  SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES LABELS "${COMPONENT_NAME}")
  # /!\ DO NOT SET TIMEOUT PROPERTY IF USING ${SALOME_TEST_DRIVER}
  #     BUT PASS TIMEOUT VALUE TO THE DRIVER

ENDIF()
