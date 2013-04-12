# Copyright (C) 2012-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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
PROJECT(SALOME_KERNEL)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8 FATAL_ERROR)

IF(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

# directories for packages detection
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/salome_adm/cmake_files)

find_package(PLATFORM)

IF(APPLE)
  SET(CMAKE_C_COMPILER gcc)
  SET(CMAKE_CXX_COMPILER g++)
  # because default is clang(llvm) with mountain lion at least
ENDIF(APPLE)


# Temporary step instead Find Calcium.cmake
SET(CALCIUM_IDL_INT_F77 long)
SET(CALCIUM_CORBA_INT_F77 CORBA::Long)
SET(LONG_OR_INT int)

# Temporary definitions
SET(DOXYGEN_IS_OK 0)
IF(WINDOWS)
 SET(CPPUNIT_IS_OK 0)
ENDIF(WINDOWS)
SET(WITH_LOCAL 1)
SET(WITH_BATCH 1)

ENABLE_TESTING()
SET(VERSION "7.2.0")
SET(VERSION_DEV "0")

SET(BUILD_SHARED_LIBS true)

#Defining all options first
option(SALOME_USE_MPI "Use MPI containers" OFF)
option(SALOME_USE_CORBA "Use distribute application with components" ON)
option(SALOME_USE_DOC "Generate documentation" ON)
option(SALOME_USE_TEST "Generate test" OFF)
option(SALOME_USE_LIBBATCH "Generate Launcher" ON)

# Threads
SET(CMAKE_THREAD_PREFER_PTHREAD)
find_package(Threads REQUIRED)

# required prerequisites

find_package(SalomePYTHON)
find_package(SalomeSWIG)
find_package(SalomeLIBXML2)
find_package(SalomeHDF5)
find_package(SalomeBOOST)

# optional prerequisites

if(SALOME_USE_MPI OR HDF5_IS_PARALLEL)
  find_package(SalomeMPI)
endif(SALOME_USE_MPI OR HDF5_IS_PARALLEL)

if(SALOME_USE_CORBA)
  find_package(OMNIORB)
endif(SALOME_USE_CORBA)

if(SALOME_USE_TEST)
  find_package(SalomeCPPUNIT)
endif(SALOME_USE_TEST)

if(SALOME_USE_DOC)
  find_package(SalomeDOXYGEN)
  find_package(SalomeSPHINX)
endif(SALOME_USE_DOC)

if(SALOME_USE_LIBBATCH)
  find_package(SalomeLIBBATCH)
endif(SALOME_USE_LIBBATCH)

INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/SalomeMacros.cmake)

SET(CALCIUM_IDL_INT_F77 long)
SET(CALCIUM_CORBA_INT_F77 CORBA::Long)
SET(LONG_OR_INT int)


SET(KERNEL_salomebin_BINS bin/salome)

SET(KERNEL_salomelib_LIBS lib/salome)

SET(KERNEL_salomeidl_IDLS idl/salome)

SET(KERNEL_salomeinclude_HEADERS include/salome)

SET(KERNEL_salomeres_DATA share/salome/resources/kernel)

SET(KERNEL_salomeres_SCRIPTS share/salome/resources/kernel)

SET(KERNEL_salomescript_SCRIPTS bin/salome)

SET(KERNEL_salomescript_DATA bin/salome)

SET(KERNEL_salomescript_PYTHON bin/salome)

SET(KERNEL_appliskel_SCRIPTS bin/salome/appliskel)

SET(KERNEL_appliskel_PYTHON bin/salome/appliskel)

SET(KERNEL_pythondir lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages)

SET(KERNEL_salomepythondir ${KERNEL_pythondir}/salome)

SET(KERNEL_sharedpkgpython_PYTHON ${KERNEL_salomepythondir}/shared_modules)

SET(KERNEL_salomeadmcmake salome_adm/cmake_files)

IF(SALOME_USE_CORBA)
  ADD_SUBDIRECTORY(idl)
ENDIF(SALOME_USE_CORBA)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(resources)

IF(NOT WITHONLYLAUNCHER)
  ADD_SUBDIRECTORY(bin)
  ADD_SUBDIRECTORY(salome_adm)
ENDIF(NOT WITHONLYLAUNCHER)

SET(input ${CMAKE_CURRENT_SOURCE_DIR}/KERNEL_version.h.in)
SET(output ${CMAKE_CURRENT_BINARY_DIR}/KERNEL_version.h)
MESSAGE(STATUS "Creation of ${output}")
CONFIGURE_FILE(${input} ${output} @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/KERNEL_version.h DESTINATION ${KERNEL_salomeinclude_HEADERS})

IF(SALOME_USE_DOC)
  ADD_SUBDIRECTORY(doc)
ENDIF(SALOME_USE_DOC)
