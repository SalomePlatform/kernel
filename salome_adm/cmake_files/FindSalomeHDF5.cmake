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

# HDF5 detection for Salome

set(HDF5_ROOT_DIR $ENV{HDF5_ROOT_DIR} CACHE PATH "Path to Hdf5 directory")
if(EXISTS ${HDF5_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${HDF5_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${HDF5_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${HDF5_ROOT_DIR}/bin)
endif(EXISTS ${HDF5_ROOT_DIR})
find_package(HDF5 COMPONENTS C REQUIRED)
if (HDF5_FOUND)
  set(HDF5_DEFINITIONS "-DH5_USE_16_API ${HDF5_DEFINITIONS}" )
  if(WINDOWS)
    set(HDF5_DEFINITIONS "-D_HDF5USEDLL_ ${HDF5_DEFINITIONS}" )
  endif(WINDOWS)
endif(HDF5_FOUND)
