# Copyright (C) 2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

include(CTest)
ENABLE_TESTING()

# define environment for running tests
SET(PYTHON_VERSION ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR})

SET(THIS_PYTHONPATH ${CMAKE_SOURCE_DIR}/bin/salome:$ENV{PYTHONPATH})
#SET(THIS_LD_LIBRARY_PATH $ENV{CMAKE_SOURCE_DIR}/lib/salome:$ENV{LD_LIBRARY_PATH})

# add tests (use make test to run)
FILE(GLOB tests "${CMAKE_CURRENT_SOURCE_DIR}/Test*.py")
FOREACH(file ${tests})
  GET_FILENAME_COMPONENT(testname ${file} NAME_WE)
  ADD_TEST(${testname} ${PYTHON_EXECUTABLE} "${file}")
#  SET_PROPERTY(TEST ${testname} APPEND PROPERTY ENVIRONMENT PYTHONPATH=${THIS_PYTHONPATH} LD_LIBRARY_PATH=${THIS_LD_LIBRARY_PATH})
  SET_PROPERTY(TEST ${testname} APPEND PROPERTY ENVIRONMENT PYTHONPATH=${THIS_PYTHONPATH})
ENDFOREACH()

# install Python scripts
FILE(GLOB scripts "${CMAKE_CURRENT_SOURCE_DIR}/*.py")
SALOME_INSTALL_SCRIPTS("${scripts}" ${SALOME_INSTALL_SCRIPT_SCRIPTS}/appliskel/tests/concurrentSession)
