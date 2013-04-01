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

# Python detection for Salome

set(PYTHON_ROOT_DIR $ENV{PYTHON_ROOT_DIR} CACHE PATH "Path to Python directory")
if(EXISTS ${PYTHON_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${PYTHON_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${PYTHON_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${PYTHON_ROOT_DIR}/bin)
endif(EXISTS ${PYTHON_ROOT_DIR})
find_package(PythonInterp REQUIRED)
if(EXISTS ${PYTHON_ROOT_DIR})
  set(PYTHON_INCLUDE_DIR ${PYTHON_ROOT_DIR}/include/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR})
  set(PYTHON_LIBRARY ${PYTHON_ROOT_DIR}/lib/libpython${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}${CMAKE_SHARED_LIBRARY_SUFFIX})
endif(EXISTS ${PYTHON_ROOT_DIR})
find_package(PythonLibs ${PYTHON_VERSION_STRING} EXACT REQUIRED)
MESSAGE("Python includes : " ${PYTHON_INCLUDE_DIR})
MESSAGE("Python library  : " ${PYTHON_LIBRARY})
MESSAGE("Python binary   : " ${PYTHON_EXECUTABLE})
set(PYLOGLEVEL WARNING)
