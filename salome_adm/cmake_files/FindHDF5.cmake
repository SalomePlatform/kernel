#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
FIND_PROGRAM(HDF5_DUMP h5dump)

SET(HDF5_ROOT ${HDF5_DUMP})
GET_FILENAME_COMPONENT(HDF5_ROOT ${HDF5_ROOT} PATH)
GET_FILENAME_COMPONENT(HDF5_ROOT ${HDF5_ROOT} PATH)

FIND_PATH(HDF5_INCLUDES H5public.h ${HDF5_ROOT}/include)
SET(HDF5_INCLUDES -I${HDF5_INCLUDES})

IF(WINDOWS)
  IF(CMAKE_BUILD_TYPE STREQUAL Release)
    FIND_LIBRARY(HDF5_LIBS hdf5 ${HDF5_ROOT}/lib)
  ELSE(CMAKE_BUILD_TYPE STREQUAL Release)
    FIND_LIBRARY(HDF5_LIBS hdf5d ${HDF5_ROOT}/lib)
  ENDIF(CMAKE_BUILD_TYPE STREQUAL Release)
ELSE(WINDOWS)
  FIND_LIBRARY(HDF5_LIBS hdf5 ${HDF5_ROOT}/lib)
ENDIF(WINDOWS)

#SET(ZLIB_ROOT $ENV{ZLIB_ROOT})
#STRING(LENGTH "${ZLIB_ROOT}" NB)
#IF(${NB} GREATER 0)
#  FIND_LIBRARY(ZLIB_LIBS zlib ${ZLIB_ROOT}/lib)
#  SET(HDF5_LIBS ${HDF5_LIBS} ${ZLIB_LIBS})
#ENDIF(${NB} GREATER 0)

#SET(SZIP_ROOT $ENV{SZIP_ROOT})
#STRING(LENGTH "${SZIP_ROOT}" NB)
#IF(${NB} GREATER 0)
#  FIND_LIBRARY(SZIP_LIBS szlib ${SZIP_ROOT}/lib)
#  SET(HDF5_LIBS ${HDF5_LIBS} ${SZIP_LIBS})
#ENDIF(${NB} GREATER 0)
