# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
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

# ------
#
MESSAGE(STATUS "Check for MPI ...")

# ------

SET(MPI_STATUS 1)
IF(WITHOUT_MPI OR WITH_MPI STREQUAL 0)
  SET(MPI_STATUS 0)
  MESSAGE(STATUS "mpi disabled from command line.")
ENDIF(WITHOUT_MPI OR WITH_MPI STREQUAL 0)

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

  IF(MPI_INCLUDES)
    MESSAGE(STATUS "MPI include ${MPI_INCLUDE_TO_FIND} found in ${MPI_INCLUDES}")

    SET(MPI_INCLUDE_DIR ${MPI_INCLUDES})
    SET(MPI_INCLUDE_DIRS ${MPI_INCLUDES})
    SET(MPI_INCLUDES -I${MPI_INCLUDES})# to remove
  ENDIF(MPI_INCLUDES)
  
  # ------
  
  IF(WINDOWS)
    FIND_LIBRARY(MPI_LIB_THREAD libboost_thread-vc90-mt-gd-1_35 ${MPI_ROOT}/lib)
    FIND_LIBRARY(MPI_LIB_DATE_TIME libboost_date_time-vc90-mt-gd-1_35 ${MPI_ROOT}/lib)
  ELSE(WINDOWS)
    SET(MPI_LIB_FOUND 0)
    FOREACH(lib mpi_cxx mpi mpich)
      FIND_LIBRARY(MPI_LIB_${lib} ${lib} ${MPI_ROOT}/lib)
      IF(MPI_LIB_${lib})
        SET(MPI_LIB_FOUND 1)
      ENDIF(MPI_LIB_${lib})
    ENDFOREACH(lib mpi_cxx mpi mpich)
    IF(MPI_LIB_FOUND)
      IF(MPI_LIB_mpi_cxx AND MPI_LIB_mpi)
        SET(MPI_LIBS ${MPI_LIB_mpi_cxx} ${MPI_LIB_mpi})
        SET(MPI_INCLUDES ${MPI_INCLUDES} -DOMPI_IGNORE_CXX_SEEK)# to remove
	SET(MPI_DEFINITIONS -DOMPI_IGNORE_CXX_SEEK)
      ELSEIF(MPI_LIB_mpi_cxx)
        SET(MPI_LIBS ${MPI_LIB_mpi_cxx})
        SET(MPI_INCLUDES ${MPI_INCLUDES} -DOMPI_IGNORE_CXX_SEEK)# to remove
	SET(MPI_DEFINITIONS -DOMPI_IGNORE_CXX_SEEK)
      ELSEIF(MPI_LIB_mpi)
        SET(MPI_LIBS ${MPI_LIB_mpi})
        SET(MPI_INCLUDES ${MPI_INCLUDES} -DOMPI_IGNORE_CXX_SEEK)# to remove
	SET(MPI_DEFINITIONS -DOMPI_IGNORE_CXX_SEEK)
      ELSEIF(MPI_LIB_mpich)
        SET(MPI_LIBS ${MPI_LIB_mpich})
        SET(MPI_INCLUDES ${MPI_INCLUDES} -DMPICH_IGNORE_CXX_SEEK)# to remove
	SET(MPI_DEFINITIONS -DMPICH_IGNORE_CXX_SEEK)
      ENDIF()
    ELSE(MPI_LIB_FOUND)
      MESSAGE(STATUS "MPI lib not found, check your MPI installation.")
      SET(MPI_STATUS 0)
    ENDIF(MPI_LIB_FOUND)
  ENDIF(WINDOWS)
  MESSAGE(STATUS "MPI libs: ${MPI_LIBS}")
ENDIF(MPI_STATUS)
  
# ------

IF(MPI_STATUS)
  include(CheckSymbolExists)
  SET(CMAKE_REQUIRED_LIBRARIES ${MPI_LIBS})
  CHECK_SYMBOL_EXISTS(MPI_Publish_name ${MPI_INCLUDE_DIR}/mpi.h MPI2_IS_OK)
  IF(MPI2_IS_OK)
    MESSAGE(STATUS "Your mpi implemtentation is compatible with mpi2 ... adding -DHAVE_MPI2")
    SET(MPI_INCLUDES ${MPI_INCLUDES} -DHAVE_MPI2)# to remove
    SET(MPI_DEFINITIONS "${MPI_DEFINITIONS} -DHAVE_MPI2")
  ENDIF(MPI2_IS_OK)
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
