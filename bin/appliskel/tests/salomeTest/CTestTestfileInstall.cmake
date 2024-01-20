# Copyright (C) 2017-2024  CEA, EDF
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

SET(tname salome_test)

IF (WIN32)
  SET(PY_EXEC $ENV{PYTHONBIN})
ELSE()
  SET(PY_EXEC python)
ENDIF()

IF(NOT WIN32)
  SET(TEST_NAME ${COMPONENT_NAME}_SALOME_TEST_${tname}_bash)
  ADD_TEST(${TEST_NAME} bash ${tname}.sh)
  SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES LABELS "${COMPONENT_NAME}" TIMEOUT ${TIMEOUT} WILL_FAIL ON)

  SET(TEST_NAME ${COMPONENT_NAME}_SALOME_TEST_${tname}_shell)
  ADD_TEST(${TEST_NAME} bash ${tname}_in_shell.sh)
  SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES LABELS "${COMPONENT_NAME}" TIMEOUT ${TIMEOUT} WILL_FAIL ON)
ENDIF()

SET(TEST_NAME ${COMPONENT_NAME}_SALOME_TEST_${tname}_py)
ADD_TEST(${TEST_NAME} ${PY_EXEC} ${tname}.py)
SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES LABELS "${COMPONENT_NAME}" TIMEOUT ${TIMEOUT} WILL_FAIL ON)

SET(TEST_NAME ${COMPONENT_NAME}_SALOME_TEST_${tname}_driver)
ADD_TEST(${TEST_NAME} ${PY_EXEC} ${SALOME_TEST_DRIVER} ${TIMEOUT} ${tname}.py)
SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES LABELS "${COMPONENT_NAME}" TIMEOUT ${TIMEOUT} WILL_FAIL ON)
