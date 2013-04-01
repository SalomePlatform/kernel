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

# Swig detection for Salome

set(SWIG_ROOT_DIR $ENV{SWIG_ROOT_DIR} CACHE PATH "Path to Swig directory")
if(EXISTS ${SWIG_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${SWIG_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${SWIG_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${SWIG_ROOT_DIR}/bin)
endif(EXISTS ${SWIG_ROOT_DIR})
find_package(SWIG REQUIRED)
