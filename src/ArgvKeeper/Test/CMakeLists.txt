# Copyright (C) 2012-2024  CEA, EDF, OPEN CASCADE
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
  ${CPPUNIT_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/src/ArgvKeeper
)

SET(LOCAL_TEST_DIR ${KERNEL_TEST_DIR}/ArgvKeeper)

ADD_DEFINITIONS(${CPPUNIT_DEFINITIONS})

ADD_EXECUTABLE(TestArgvKeeper ArgvKeeperTest.cxx TestArgvKeeper.cxx)
TARGET_LINK_LIBRARIES(TestArgvKeeper ArgvKeeper ${CPPUNIT_LIBRARIES})

INSTALL(TARGETS TestArgvKeeper DESTINATION ${LOCAL_TEST_DIR})
INSTALL(FILES test_ArgvKeeper.py DESTINATION ${LOCAL_TEST_DIR})
INSTALL(FILES CTestTestfileInstall.cmake
        DESTINATION ${LOCAL_TEST_DIR}
        RENAME CTestTestfile.cmake)
