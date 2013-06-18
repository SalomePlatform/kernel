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
# Author: Adrien Bruneton
#

# HDF5 detection for Salome
#
#  !! Please read the generic detection procedure in SalomeMacros.cmake !!
# 
# --- HDF5 specificities ----
#  MPI root directory used for HDF5 compilation is exposed into MPI_ROOT_DIR_EXP
#

SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(HDF5 HDF5_INCLUDE_DIR 1)
MARK_AS_ADVANCED(FORCE HDF5_INCLUDE_DIR HDF5_LIB)

# 7. Expose MPI configuration to the rest of the world
IF(HDF5_ENABLE_PARALLEL OR HDF5_IS_PARALLEL)
  # Set only one reference boolean variable:
  # (unfortunately what is found in /usr/share/cmake/Modules/FindHDF5.cmake
  #  and in the native HDF5-config.cmake differ!)
  SET(HDF5_IS_PARALLEL TRUE)

  # HDF5 was compiled with MPI support
  # Unfortunately HDF5 doesn't expose its MPI configuration easily ...
  # We sniff the properties of the HDF5 target which should also be there:
  GET_PROPERTY(_lib_lst TARGET hdf5 PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG)
  FOREACH(s ${_lib_lst})
    STRING(FIND "${s}" "mpi." _res)   # should cover WIN(?) and LINUX
    IF(_res GREATER -1)
      GET_FILENAME_COMPONENT(_tmp "${s}" PATH)     # go up to levels
      GET_FILENAME_COMPONENT(MPI_ROOT_DIR_EXP "${_tmp}" PATH)
      BREAK()
    ENDIF()
  ENDFOREACH()
  IF(NOT SalomeHDF5_FIND_QUIETLY)
    MESSAGE(STATUS "HDF5 was compiled with MPI: ${MPI_ROOT_DIR_EXP}")
  ENDIF()  
ENDIF()

