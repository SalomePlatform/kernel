#  Copyright (C) 2007-2010  CEA/DEN, EDF R&D, OPEN CASCADE
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

# ------
#
MESSAGE(STATUS "Check for MPI ...")

# ------

SET(MPI_STATUS 1)

IF(WITH_MPI)
  SET(MPI_ROOT_USER ${WITH_MPI})
ELSE(WITH_MPI)
  SET(MPI_ROOT_USER $ENV{MPI_ROOT})
ENDIF(WITH_MPI)

IF(NOT MPI_ROOT_USER)
  FIND_PROGRAM(MPICC mpicc)
  IF(MPICC)
    MESSAGE(STATUS "mpicc found: ${MPICC}")
    SET(MPI_ROOT ${MPICC})
    GET_FILENAME_COMPONENT(MPI_ROOT ${MPI_ROOT} PATH)
    GET_FILENAME_COMPONENT(MPI_ROOT ${MPI_ROOT} PATH)
    IF(MPICC STREQUAL /usr/bin/mpicc)
    ELSE(MPICC STREQUAL /usr/bin/mpicc)
      SET(MPI_ROOT_USER ${MPI_ROOT})
    ENDIF(MPICC STREQUAL /usr/bin/mpicc)
  ENDIF(MPICC)
ELSE(NOT MPI_ROOT_USER)
  SET(MPI_ROOT ${MPI_ROOT_USER})
ENDIF(NOT MPI_ROOT_USER)

# ------

IF(NOT MPI_ROOT)
  MESSAGE(STATUS "MPI not found, try to use WITH_MPI option or MPI_ROOT environment variable.")
  SET(MPI_STATUS 0)
ENDIF(NOT MPI_ROOT)

# ------

IF(MPI_STATUS)
  MESSAGE(STATUS "Check MPI in ${MPI_ROOT}")

  SET(MPI_INCLUDE_TO_FIND mpi.h)
  
  IF(MPI_ROOT_USER)
    SET(MPI_ROOT ${MPI_ROOT_USER})
    SET(MPI_INCLUDE_PATHS ${MPI_ROOT} ${MPI_ROOT}/include)
    FIND_PATH(MPI_INCLUDES ${MPI_INCLUDE_TO_FIND} PATHS ${MPI_INCLUDE_PATHS} NO_DEFAULT_PATH)
    IF(NOT MPI_INCLUDES)
      MESSAGE(STATUS "MPI include ${MPI_INCLUDE_TO_FIND} not found in ${MPI_INCLUDE_PATHS}, check your MPI installation.")
      SET(MPI_STATUS 0)
    ENDIF(NOT MPI_INCLUDES)
  ELSE(MPI_ROOT_USER)
    FIND_PATH(MPI_INCLUDES ${MPI_INCLUDE_TO_FIND})
    IF(NOT MPI_INCLUDES)
      MESSAGE(STATUS "MPI include ${MPI_INCLUDE_TO_FIND} not found on system, try to use WITH_MPI option or MPI_ROOT environment variable.")
      SET(MPI_STATUS 0)
    ENDIF(NOT MPI_INCLUDES)
    GET_FILENAME_COMPONENT(MPI_ROOT ${MPI_INCLUDES} PATH)
  ENDIF(MPI_ROOT_USER)
  
  MESSAGE(STATUS "MPI include ${MPI_INCLUDE_TO_FIND} found in ${MPI_INCLUDES}")

  SET(MPI_INCLUDES -I${MPI_INCLUDES})
  
  # ------
  
  IF(WINDOWS)
    FIND_LIBRARY(MPI_LIB_THREAD libboost_thread-vc90-mt-gd-1_35 ${MPI_ROOT}/lib)
    FIND_LIBRARY(MPI_LIB_DATE_TIME libboost_date_time-vc90-mt-gd-1_35 ${MPI_ROOT}/lib)
  ELSE(WINDOWS)
    FOREACH(lib mpi_cxx mpi mpich)
      FIND_LIBRARY(MPI_LIB ${lib} ${MPI_ROOT}/lib)
      IF(MPI_LIB)
	IF(lib STREQUAL mpich)
	  SET(MPI_INCLUDES ${MPI_INCLUDES} -DMPICH_IGNORE_CXX_SEEK)
	ENDIF(lib STREQUAL mpich)
      ENDIF(MPI_LIB)
    ENDFOREACH(lib mpi_cxx mpi mpich)
    IF(NOT MPI_LIB)
      MESSAGE(STATUS "MPI lib not found, check your MPI installation.")
      SET(MPI_STATUS 0)
    ENDIF(NOT MPI_LIB)
    MESSAGE(STATUS "MPI lib : ${MPI_LIB}")
  ENDIF(WINDOWS)
  
  SET(MPI_LIBS ${MPI_LIB})
  
  MESSAGE(STATUS "MPI libs: ${MPI_LIBS}")
ENDIF(MPI_STATUS)
  
# ------

IF(MPI_STATUS)
  SET(MPI_IS_OK ON)
ELSE(MPI_STATUS)
  IF(MPI_IS_MANDATORY)
    MESSAGE(FATAL_ERROR "MPI not found but mandatory")
  ELSE(MPI_IS_MANDATORY)
    MESSAGE(STATUS "MPI not found. Build procedure depending of mpi will be disable")
  ENDIF(MPI_IS_MANDATORY)
ENDIF(MPI_STATUS)