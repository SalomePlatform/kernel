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
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8 FATAL_ERROR)
PROJECT(SalomeKERNEL C CXX)

# Ensure a proper linker behavior:
CMAKE_POLICY(SET CMP0003 NEW)

# Versioning
# ===========
# Project name, upper case
STRING(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UC)

SET(${PROJECT_NAME_UC}_MAJOR_VERSION 2)
SET(${PROJECT_NAME_UC}_MINOR_VERSION 0)
SET(${PROJECT_NAME_UC}_PATCH_VERSION 0)
SET(${PROJECT_NAME_UC}_VERSION
  ${${PROJECT_NAME_UC}_MAJOR_VERSION}.${${PROJECT_NAME_UC}_MINOR_VERSION}.${${PROJECT_NAME_UC}_PATCH_VERSION})
# Temporary backward compatibility:
SET(VERSION ${${PROJECT_NAME_UC}_VERSION})
SET(VERSION_DEV "0")

# Our own set of macros:
LIST(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/salome_adm/cmake_files")
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/SalomeMacros.cmake)

# Platform setup
# ==============
INCLUDE(SalomeSetupPlatform)
# Always build libraries as shared objects:
SET(BUILD_SHARED_LIBS TRUE)
# Temporary step instead of Find Calcium.cmake
SET(CALCIUM_IDL_INT_F77 long)
SET(CALCIUM_CORBA_INT_F77 CORBA::Long)
SET(LONG_OR_INT int)
# Global definitions
ADD_DEFINITIONS(-DSIZEOF_FORTRAN_INTEGER=4 -DSIZEOF_INT=4)
## OmniORB already defines SIZEOF_LONG 
# ADD_DEFINITIONS(-DSIZEOF_LONG=${SIZE_OF_LONG})

# User options
# ============
OPTION(SALOME_USE_MPI "Use MPI containers" OFF)
OPTION(SALOME_BUILD_DOC "Generate SALOME KERNEL documentation" ON)
OPTION(SALOME_BUILD_TESTS "Build SALOME tests" ON)

# Advanced:
OPTION(SALOME_LIGHT_ONLY "Build SALOME Light only (no CORBA)" OFF)
OPTION(SALOME_USE_LIBBATCH "Use LibBatch in KERNEL" ON)
OPTION(SALOME_PACO_PARALLEL "Build with PACO in parallel" OFF)
OPTION(SALOME_LAUNCHER_ONLY "Build only the Launcher part" OFF)
MARK_AS_ADVANCED(SALOME_LIGHT_ONLY SALOME_USE_LIBBATCH SALOME_PACO_PARALLEL SALOME_LAUNCHER_ONLY)

# Directories
# ===========
SET(SALOME_INSTALL_BINS bin/salome CACHE PATH "Install path: SALOME binaries")
SET(SALOME_INSTALL_LIBS lib/salome CACHE PATH "Install path: SALOME libs")
SET(SALOME_INSTALL_IDLS idl/salome CACHE PATH "Install path: SALOME IDL files")
SET(SALOME_INSTALL_HEADERS include/salome CACHE PATH "Install path: SALOME headers")
SET(SALOME_INSTALL_SCRIPT_SCRIPTS ${SALOME_INSTALL_BINS} CACHE PATH 
   "Install path: SALOME scripts")
SET(SALOME_INSTALL_SCRIPT_DATA ${SALOME_INSTALL_BINS} CACHE PATH 
   "Install path: SALOME script data")
SET(SALOME_INSTALL_SCRIPT_PYTHON ${SALOME_INSTALL_BINS} CACHE PATH 
   "Install path: SALOME Python scripts")
SET(SALOME_INSTALL_APPLISKEL_SCRIPTS ${SALOME_INSTALL_BINS}/appliskel CACHE PATH 
   "Install path: SALOME application skeleton - scripts")
SET(SALOME_INSTALL_APPLISKEL_PYTHON ${SALOME_INSTALL_BINS}/appliskel CACHE PATH 
   "Install path: SALOME application skeleton - Python")
SET(SALOME_INSTALL_CMAKE salome_adm/cmake_files CACHE PATH "Install path: SALOME CMake files")

SET(_pydir lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages)
SET(SALOME_INSTALL_PYTHON ${_pydir}/salome CACHE PATH "Install path: SALOME Python stuff")
SET(SALOME_INSTALL_PYTHON_SHARED ${SALOME_INSTALL_PYTHON}/shared_modules CACHE PATH 
   "Install path: SALOME Python shared modules")

# Kernel specific:
SET(SALOME_KERNEL_INSTALL_RES_DATA share/salome/resources/kernel CACHE PATH "Install path: SALOME KERNEL specific data")
SET(SALOME_KERNEL_INSTALL_RES_SCRIPTS share/salome/resources/kernel CACHE PATH "Install path: SALOME KERNEL specific scripts")

MARK_AS_ADVANCED(SALOME_INSTALL_BINS SALOME_INSTALL_LIBS SALOME_INSTALL_IDLS SALOME_INSTALL_HEADERS)
MARK_AS_ADVANCED(SALOME_INSTALL_SCRIPT_SCRIPTS SALOME_INSTALL_SCRIPT_DATA SALOME_INSTALL_SCRIPT_PYTHON)
MARK_AS_ADVANCED(SALOME_INSTALL_APPLISKEL_SCRIPTS  SALOME_INSTALL_APPLISKEL_PYTHON SALOME_INSTALL_CMAKE)
MARK_AS_ADVANCED(SALOME_INSTALL_PYTHON SALOME_INSTALL_PYTHON_SHARED SALOME_KERNEL_INSTALL_RES_DATA SALOME_KERNEL_INSTALL_RES_SCRIPTS)

# Required prerequisites
#   Find "big" prerequisites first - they reference themselves many others
#   -> this can help finding the smaller prerequisites and detect conficts.
# =======================

FIND_PACKAGE(SalomeLibBatch REQUIRED)
FIND_PACKAGE(SalomePython REQUIRED)
FIND_PACKAGE(SalomePThread REQUIRED)
FIND_PACKAGE(SalomeSWIG REQUIRED)
FIND_PACKAGE(SalomeLibXml2 REQUIRED)

# HDF5 requires a bit more work to ensure MPI is selected:
FIND_PACKAGE(SalomeHDF5 REQUIRED)
ADD_DEFINITIONS(-DH5_USE_16_API)
IF(WINDOWS)
  ADD_DEFINITIONS(-D_HDF5USEDLL_)
ENDIF()
IF(HDF5_IS_PARALLEL AND NOT SALOME_USE_MPI)
   MESSAGE(FATAL_ERROR "HDF5 is compiled with MPI, you have to set SALOME_USE_MPI to ON")
ENDIF()

# Boost
FIND_PACKAGE(SalomeBoost REQUIRED)
IF(WIN32)
  SET(BOOST_DEFINITIONS -DBOOST_DISABLE_ASSERTS)
ENDIF()

# Optional prerequisites
# ======================

IF(NOT SALOME_LIGHT_ONLY)
  FIND_PACKAGE(SalomeOmniORB REQUIRED)
ENDIF() 
IF(SALOME_USE_MPI)
  FIND_PACKAGE(SalomeMPI REQUIRED)
  SET(MPI_INCLUDE_DIRS ${MPI_C_INCLUDE_PATH} ${MPI_CXX_INCLUDE_PATH})
  SET(MPI_LIBRARIES ${MPI_C_LIBRARIES} ${MPI_CXX_LIBRARIES})
ENDIF()
IF(SALOME_BUILD_TESTS)
  ENABLE_TESTING()
  FIND_PACKAGE(SalomeCppUnit REQUIRED)  
ENDIF()
IF(SALOME_BUILD_DOC)
  FIND_PACKAGE(SalomeDoxygen REQUIRED)
  FIND_PACKAGE(SalomeGraphviz)          # Remains optional even here.
  FIND_PACKAGE(SalomeSphinx REQUIRED)
ENDIF()

# Sources 
# ========
IF(NOT SALOME_LIGHT_ONLY)
  ADD_SUBDIRECTORY(idl)
ENDIF()
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(resources)
ADD_SUBDIRECTORY(bin)
ADD_SUBDIRECTORY(salome_adm)
IF(SALOME_BUILD_DOC)
  ADD_SUBDIRECTORY(doc)
ENDIF()

# Header configuration
# ====================
SET(input ${CMAKE_CURRENT_SOURCE_DIR}/KERNEL_version.h.in)
SET(output ${CMAKE_CURRENT_BINARY_DIR}/KERNEL_version.h)
MESSAGE(STATUS "Creation of ${output}")
CONFIGURE_FILE(${input} ${output} @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/KERNEL_version.h DESTINATION ${SALOME_INSTALL_HEADERS})

# Configuration export
# ====================
INCLUDE(CMakePackageConfigHelpers)

# Add all targets to the build-tree export set
EXPORT(TARGETS SALOMEBasics
  FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake)

# Create the configuration files:
#   - in the build tree:

#      Ensure the variables are always defined for the configure:
SET(CPPUNIT_ROOT_DIR "${CPPUNIT_ROOT_DIR}")
SET(GRAPHVIZ_ROOT_DIR "${GRAPHVIZ_ROOT_DIR}")
SET(DOXYGEN_ROOT_DIR "${DOXYGEN_ROOT_DIR}")
SET(SPHINX_ROOT_DIR "${SPHINX_ROOT_DIR}")
SET(MPI_ROOT_DIR "${MPI_ROOT_DIR}")
SET(OMNIORB_ROOT_DIR "${OMNIORB_ROOT_DIR}")
SET(LIBBATCH_ROOT_DIR "${LIBBATCH_ROOT_DIR}")
 
SET(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include")
CONFIGURE_PACKAGE_CONFIG_FILE(salome_adm/cmake_files/${PROJECT_NAME}Config.cmake.in 
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION "${SALOME_INSTALL_CMAKE}"
    PATH_VARS CONF_INCLUDE_DIRS SALOME_INSTALL_CMAKE CMAKE_INSTALL_PREFIX
       CPPUNIT_ROOT_DIR GRAPHVIZ_ROOT_DIR DOXYGEN_ROOT_DIR
       SPHINX_ROOT_DIR MPI_ROOT_DIR OMNIORB_ROOT_DIR LIBBATCH_ROOT_DIR
       PTHREAD_ROOT_DIR BOOST_ROOT_DIR HDF5_ROOT_DIR LIBXML2_ROOT_DIR
       PYTHON_ROOT_DIR SWIG_ROOT_DIR)

#   - in the install tree:
#       Get the relative path of the include directory so 
#       we can register it in the generated configuration files:
SET(CONF_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/${INSTALL_INCLUDE_DIR}")
CONFIGURE_PACKAGE_CONFIG_FILE(salome_adm/cmake_files/${PROJECT_NAME}Config.cmake.in 
    ${PROJECT_BINARY_DIR}/to_install/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION "${SALOME_INSTALL_CMAKE}"
    PATH_VARS CONF_INCLUDE_DIRS SALOME_INSTALL_CMAKE CMAKE_INSTALL_PREFIX 
       CPPUNIT_ROOT_DIR GRAPHVIZ_ROOT_DIR DOXYGEN_ROOT_DIR
       SPHINX_ROOT_DIR MPI_ROOT_DIR OMNIORB_ROOT_DIR LIBBATCH_ROOT_DIR
       PTHREAD_ROOT_DIR BOOST_ROOT_DIR HDF5_ROOT_DIR LIBXML2_ROOT_DIR
       PYTHON_ROOT_DIR SWIG_ROOT_DIR)

WRITE_BASIC_PACKAGE_VERSION_FILE(${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${${PROJECT_NAME_UC}_VERSION}
    COMPATIBILITY AnyNewerVersion)
  
# Install the CMake configuration files:
INSTALL(FILES
  "${PROJECT_BINARY_DIR}/to_install/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "${SALOME_INSTALL_CMAKE}")

# Install the export set for use with the install-tree
INSTALL(EXPORT salomeKernelTargets DESTINATION "${SALOME_INSTALL_CMAKE}")
