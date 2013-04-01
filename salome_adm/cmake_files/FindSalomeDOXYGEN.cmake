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

# Doxygen and graphviz detection for salome

set(DOXYGEN_ROOT_DIR $ENV{DOXYGEN_ROOT_DIR} CACHE PATH "Path to Doxygen directory")
set(GRAPHVIZ_ROOT_DIR $ENV{GRAPHVIZ_ROOT_DIR} CACHE PATH "Path to Graphviz directory")
if(EXISTS ${DOXYGEN_ROOT_DIR})
  set(CMAKE_PROGRAM_PATH ${DOXYGEN_ROOT_DIR}/bin)
endif(EXISTS ${DOXYGEN_ROOT_DIR})
if(EXISTS ${GRAPHVIZ_ROOT_DIR})
  set(CMAKE_PROGRAM_PATH ${GRAPHVIZ_ROOT_DIR}/bin)
  if(EXISTS ${DOXYGEN_ROOT_DIR})
    set(CMAKE_PROGRAM_PATH ${GRAPHVIZ_ROOT_DIR}/bin ${DOXYGEN_ROOT_DIR}/bin)
  endif(EXISTS ${DOXYGEN_ROOT_DIR})
endif(EXISTS ${GRAPHVIZ_ROOT_DIR})
find_package(Doxygen)
