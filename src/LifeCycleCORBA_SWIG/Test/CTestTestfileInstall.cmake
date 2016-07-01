# Copyright (C) 2015-2016  CEA/DEN, EDF R&D
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

SET(TEST_NAMES
  LifeCycleCORBA_SWIGTest
  testcontainermanager
#  TestLifeCycleCORBA_SWIG
  )

IF(NOT WIN32)
  FOREACH(tfile ${TEST_NAMES})
    SET(TEST_NAME ${COMPONENT_NAME}_LC_${tfile})
    ADD_TEST(${TEST_NAME} python ${SALOME_TEST_DRIVER} ${TIMEOUT} ${tfile}.py)
    SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES LABELS "${COMPONENT_NAME}"
      )
  ENDFOREACH()

  # resource manager test needs specific CatalogResouces file in test directory
  SET(TEST_NAME ${COMPONENT_NAME}_LC_testresourcemanager)
  ADD_TEST(${TEST_NAME} python ${SALOME_TEST_DRIVER} ${TIMEOUT} testresourcemanager.py)
  SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES LABELS "${COMPONENT_NAME}"
                                    ENVIRONMENT "USER_CATALOG_RESOURCES_FILE=CatalogResources_resource.xml"
                      )
ENDIF()
