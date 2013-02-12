# Copyright (C) 2012  CEA/DEN, EDF R&D, OPEN CASCADE
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

INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindPLATFORM.cmake)

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
SET(VERSION "7.0.0")
SET(VERSION_DEV "1")

SET(BUILD_SHARED_LIBS true)

#Defining all options first
option(SALOME_USE_MPI "Use MPI containers" OFF)
option(SALOME_USE_CORBA "Use distribute application with components" ON)
option(SALOME_USE_DOC "Generate documentation" ON)
option(SALOME_USE_TEST "Generate test" OFF)
option(SALOME_USE_LIBBATCH "Generate Launcher" OFF)


# Threads
SET(CMAKE_THREAD_PREFER_PTHREAD)
find_package(Threads REQUIRED)


# Python
set(PYTHON_ROOT_DIR $ENV{PYTHON_ROOT_DIR} CACHE PATH "Path to Python directory")
if(EXISTS ${PYTHON_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${PYTHON_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${PYTHON_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${PYTHON_ROOT_DIR}/bin)
endif(EXISTS ${PYTHON_ROOT_DIR})
find_package(PythonInterp REQUIRED)
# Set PythonLibs_FIND_VERSION To avoid problems when several versions are in the system
SET(PythonLibs_FIND_VERSION ${PYTHON_VERSION_STRING})
find_package(PythonLibs REQUIRED)
set(PYLOGLEVEL WARNING)

# Swig
set(SWIG_ROOT_DIR $ENV{SWIG_ROOT_DIR} CACHE PATH "Path to Swig directory")
if(EXISTS ${SWIG_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${SWIG_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${SWIG_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${SWIG_ROOT_DIR}/bin)
endif(EXISTS ${SWIG_ROOT_DIR})
find_package(SWIG REQUIRED)

# LIBXML2
set(LIBXML2_ROOT_DIR $ENV{LIBXML2_ROOT_DIR} CACHE PATH "Path to LibXml2 directory")
if(EXISTS ${LIBXML2_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${LIBXML2_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${LIBXML2_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${LIBXML2_ROOT_DIR}/bin)
endif(EXISTS ${LIBXML2_ROOT_DIR})
find_package(LibXml2 REQUIRED)
if(${LIBXML2_VERSION_STRING} VERSION_LESS "2.7.8")
  message(FATAL_ERROR "LIbXml2 required version is 2.7.8, version is ${LIBXML2_VERSION_STRING} , please defined LIBXML2_ROOT_DIR as environment or cmake variable")
endif(${LIBXML2_VERSION_STRING} VERSION_LESS "2.7.8")

# HDF5
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

# MPI
if(SALOME_USE_MPI OR HDF5_IS_PARALLEL)
  set(MPI_ROOT_DIR $ENV{MPI_ROOT_DIR} CACHE PATH "Path to Mpi directory")
  if(EXISTS ${MPI_ROOT_DIR})
    set(CMAKE_INCLUDE_PATH ${MPI_ROOT_DIR}/include)
    set(CMAKE_LIBRARY_PATH ${MPI_ROOT_DIR}/lib)
    set(CMAKE_PROGRAM_PATH ${MPI_ROOT_DIR}/bin)
  endif(EXISTS ${MPI_ROOT_DIR})
  find_package(MPI REQUIRED)
  if(HDF5_IS_PARALLEL)
    set(HDF5_INCLUDE_DIRS ${HDF5_INCLUDE_DIRS} ${MPI_C_INCLUDE_PATH} ${MPI_CXX_INCLUDE_PATH} )
    set(HDF5_LIBRARIES ${HDF5_LIBRARIES} ${MPI_C_LIBRARIES} ${MPI_CXX_LIBRARIES})
  endif(HDF5_IS_PARALLEL)
endif(SALOME_USE_MPI OR HDF5_IS_PARALLEL)

# Omniorb
if(SALOME_USE_CORBA)
  INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindOMNIORB.cmake)
endif(SALOME_USE_CORBA)

# Boost
set(Boost_ROOT_DIR $ENV{Boost_ROOT_DIR} CACHE PATH "Path to Boost directory")
if(EXISTS ${Boost_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${Boost_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${Boost_ROOT_DIR}/lib)
endif(EXISTS ${Boost_ROOT_DIR})
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
#find_package( Boost 1.49 REQUIRED  )#COMPONENTS thread date_time signals filesystem system)
find_package( Boost REQUIRED  )#COMPONENTS thread date_time signals filesystem system)
if(WINDOWS)
  set(BOOST_DEFINITIONS -DBOOST_DISABLE_ASSERTS)
endif(WINDOWS)

# CPPUNIT
if(SALOME_USE_TEST)
  set(CPPUNIT_ROOT_DIR $ENV{CPPUNIT_ROOT_DIR} CACHE PATH "Path to Cppunit directory")
  if(EXISTS ${CPPUNIT_ROOT_DIR})
    set(CMAKE_INCLUDE_PATH ${CPPUNIT_ROOT_DIR}/include)
    set(CMAKE_LIBRARY_PATH ${CPPUNIT_ROOT_DIR}/lib)
    set(CMAKE_PROGRAM_PATH ${CPPUNIT_ROOT_DIR}/bin)
  endif(EXISTS ${CPPUNIT_ROOT_DIR})
  INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindCPPUNIT.cmake)
endif(SALOME_USE_TEST)

# Doxygen and graphviz
if(SALOME_USE_DOC)
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
endif(SALOME_USE_DOC)

# Libbatch
if(SALOME_USE_LIBBATCH)
  INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindLIBBATCH.cmake)
endif(SALOME_USE_LIBBATCH)

# Sphinx
if(SALOME_USE_DOC)
  set(SPHINX_ROOT_DIR $ENV{SPHINX_ROOT_DIR} CACHE PATH "Path to Sphinx directory")
  if(EXISTS ${SPHINX_ROOT_DIR})
    set(CMAKE_PROGRAM_PATH ${SPHINX_ROOT_DIR}/bin)
  endif(EXISTS ${SPHINX_ROOT_DIR})
  INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindSPHINX.cmake)
endif(SALOME_USE_DOC)

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
