# Copyright (C) 2012-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
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
INCLUDE(CMakeDependentOption)

PROJECT(SalomeKERNEL C CXX)

# Ensure a proper linker behavior:
CMAKE_POLICY(SET CMP0003 NEW)

# Versioning
# ===========
# Project name, upper case
STRING(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UC)

SET(${PROJECT_NAME_UC}_MAJOR_VERSION 8)
SET(${PROJECT_NAME_UC}_MINOR_VERSION 2)
SET(${PROJECT_NAME_UC}_PATCH_VERSION 0)
SET(${PROJECT_NAME_UC}_VERSION
  ${${PROJECT_NAME_UC}_MAJOR_VERSION}.${${PROJECT_NAME_UC}_MINOR_VERSION}.${${PROJECT_NAME_UC}_PATCH_VERSION})
SET(${PROJECT_NAME_UC}_VERSION_DEV 0)

# Common CMake macros
# ===================
SET(CONFIGURATION_ROOT_DIR $ENV{CONFIGURATION_ROOT_DIR} CACHE PATH "Path to the Salome CMake configuration files")
IF(EXISTS ${CONFIGURATION_ROOT_DIR})
  LIST(APPEND CMAKE_MODULE_PATH "${CONFIGURATION_ROOT_DIR}/cmake")
  INCLUDE(SalomeMacros)
ELSE()
  MESSAGE(FATAL_ERROR "We absolutely need the Salome CMake configuration files, please define CONFIGURATION_ROOT_DIR !")
ENDIF()

# Platform setup
# ==============
INCLUDE(SalomeSetupPlatform)
# Always build libraries as shared objects:
SET(BUILD_SHARED_LIBS TRUE)
# Temporary step instead of Find Calcium.cmake
SET(CALCIUM_IDL_INT_F77 long)
SET(CALCIUM_CORBA_INT_F77 CORBA::Long)
SET(LONG_OR_INT int)

# User options
# ============
OPTION(SALOME_BUILD_DOC "Generate SALOME KERNEL documentation" ON)
OPTION(SALOME_BUILD_TESTS "Build SALOME tests" ON)

# Advanced:
OPTION(SALOME_LIGHT_ONLY "Build SALOME Light only (no CORBA)" OFF)
OPTION(SALOME_USE_LIBBATCH "Use LibBatch in KERNEL" OFF)
CMAKE_DEPENDENT_OPTION(SALOME_PACO_PARALLEL "Build with PACO (implies SALOME_USE_MPI)" OFF
                       "NOT SALOME_LIGHT_ONLY" OFF)
CMAKE_DEPENDENT_OPTION(SALOME_USE_MPI "Use MPI containers" OFF
                       "NOT SALOME_PACO_PARALLEL" ON)
CMAKE_DEPENDENT_OPTION(SALOME_USE_MPI "Use MPI containers" ${SALOME_USE_MPI}
                       "NOT SALOME_LIGHT_ONLY" OFF)
MARK_AS_ADVANCED(SALOME_LIGHT_ONLY SALOME_USE_LIBBATCH SALOME_PACO_PARALLEL)

# Required prerequisites
#   Find "big" prerequisites first - they reference themselves many others
#   -> this can help finding the smaller prerequisites and detect conficts.
# =======================

# Exception: LibBatch is optional but is loaded first, as it can
# give Python, Pthread and SWIG:
IF(SALOME_USE_LIBBATCH)
  FIND_PACKAGE(SalomeLibBatch)
  SALOME_LOG_OPTIONAL_PACKAGE(LibBatch SALOME_USE_LIBBATCH)
ENDIF()

FIND_PACKAGE(SalomePythonInterp REQUIRED)
FIND_PACKAGE(SalomePythonLibs REQUIRED)
FIND_PACKAGE(SalomeNumPySciPy)
FIND_PACKAGE(SalomePThread REQUIRED)
FIND_PACKAGE(SalomeSWIG REQUIRED)
FIND_PACKAGE(SalomeLibXml2 REQUIRED)
FIND_PACKAGE(SalomeHDF5 REQUIRED COMPONENTS C)
FIND_PACKAGE(SalomeBoost REQUIRED)

# Optional prerequisites
# ======================

IF(NOT SALOME_LIGHT_ONLY)
  FIND_PACKAGE(SalomeOmniORB REQUIRED)
  FIND_PACKAGE(SalomeOmniORBPy REQUIRED)
ENDIF()
IF(SALOME_USE_MPI)
  FIND_PACKAGE(SalomeMPI REQUIRED)
  IF(HDF5_IS_PARALLEL)
    SALOME_ADD_MPI_TO_HDF5()
  ENDIF()
ENDIF()
IF(SALOME_PACO_PARALLEL)
  FIND_PACKAGE(SalomePaco REQUIRED)
ENDIF()
IF(SALOME_BUILD_TESTS)
  ENABLE_TESTING()
  FIND_PACKAGE(SalomeCppUnit)
  SALOME_LOG_OPTIONAL_PACKAGE(CppUnit SALOME_BUILD_TESTS)
ENDIF()
IF(SALOME_BUILD_DOC)
  FIND_PACKAGE(SalomeDoxygen)
  FIND_PACKAGE(SalomeGraphviz)
  FIND_PACKAGE(SalomeSphinx)
  SALOME_LOG_OPTIONAL_PACKAGE(Doxygen SALOME_BUILD_DOC)
  SALOME_LOG_OPTIONAL_PACKAGE(Graphviz SALOME_BUILD_DOC)
  SALOME_LOG_OPTIONAL_PACKAGE(Sphinx SALOME_BUILD_DOC)
ENDIF()

# Detection summary:
SALOME_PACKAGE_REPORT_AND_CHECK()

# Directories
#
# Directories have to be given after prerequisites (to be able to use
# Python version string for example).
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
SET(SALOME_INSTALL_CMAKE_LOCAL adm_local/cmake_files CACHE PATH
   "Install path: local SALOME CMake files")
SET(SALOME_INSTALL_AMCONFIG salome_adm/unix CACHE PATH
   "Install path: SALOME config files (obsolete, to be removed)")
SET(SALOME_INSTALL_AMCONFIG_LOCAL adm_local/unix CACHE PATH
   "Install path: local SALOME config files (obsolete, to be removed)")

SET(_pydir lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages)
SET(SALOME_INSTALL_PYTHON ${_pydir}/salome CACHE PATH "Install path: SALOME Python stuff")
SET(SALOME_INSTALL_PYTHON_SHARED ${SALOME_INSTALL_PYTHON}/shared_modules CACHE PATH
   "Install path: SALOME Python shared modules")

SET(SALOME_INSTALL_RES share/salome/resources CACHE PATH "Install path: SALOME resources")

SET(SALOME_INSTALL_DOC share/doc/salome CACHE PATH "Install path: SALOME documentation")

# Kernel specific:
SET(SALOME_KERNEL_INSTALL_RES_DATA "${SALOME_INSTALL_RES}/kernel" CACHE PATH "Install path: SALOME KERNEL specific data")
SET(SALOME_KERNEL_INSTALL_RES_SCRIPTS "${SALOME_INSTALL_RES}/kernel" CACHE PATH "Install path: SALOME KERNEL specific scripts")

MARK_AS_ADVANCED(SALOME_INSTALL_BINS SALOME_INSTALL_LIBS SALOME_INSTALL_IDLS SALOME_INSTALL_HEADERS)
MARK_AS_ADVANCED(SALOME_INSTALL_SCRIPT_SCRIPTS SALOME_INSTALL_SCRIPT_DATA SALOME_INSTALL_SCRIPT_PYTHON)
MARK_AS_ADVANCED(SALOME_INSTALL_APPLISKEL_SCRIPTS  SALOME_INSTALL_APPLISKEL_PYTHON SALOME_INSTALL_CMAKE SALOME_INSTALL_CMAKE_LOCAL SALOME_INSTALL_RES)
MARK_AS_ADVANCED(SALOME_INSTALL_PYTHON SALOME_INSTALL_PYTHON_SHARED SALOME_KERNEL_INSTALL_RES_DATA SALOME_KERNEL_INSTALL_RES_SCRIPTS SALOME_INSTALL_DOC)
MARK_AS_ADVANCED(SALOME_INSTALL_AMCONFIG SALOME_INSTALL_AMCONFIG_LOCAL)

# Accumulate environment variables for KERNEL module
SALOME_ACCUMULATE_ENVIRONMENT(PYTHONPATH NOCHECK ${CMAKE_INSTALL_PREFIX}/${SALOME_INSTALL_BINS}
                                                 ${CMAKE_INSTALL_PREFIX}/${SALOME_INSTALL_PYTHON}
                                                 ${CMAKE_INSTALL_PREFIX}/${SALOME_INSTALL_PYTHON_SHARED})
SALOME_ACCUMULATE_ENVIRONMENT(LD_LIBRARY_PATH NOCHECK ${CMAKE_INSTALL_PREFIX}/${SALOME_INSTALL_LIBS})

# For salome test
SET(KERNEL_TEST_DIR ${SALOME_INSTALL_SCRIPT_SCRIPTS}/test)
SET(KERNEL_TEST_LIB ${SALOME_INSTALL_SCRIPT_SCRIPTS}/test/lib)
CONFIGURE_FILE(CTestTestfileInstall.cmake.in CTestTestfileInstall.cmake @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/CTestTestfileInstall.cmake
        DESTINATION ${KERNEL_TEST_DIR}
        RENAME CTestTestfile.cmake)

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
SALOME_XVERSION(${PROJECT_NAME})
SALOME_CONFIGURE_FILE(KERNEL_version.h.in KERNEL_version.h INSTALL ${SALOME_INSTALL_HEADERS})

# Configuration export
# ====================
INCLUDE(CMakePackageConfigHelpers)

# List of targets in this project we want to make visible to the rest of the world.
# They all have to be INSTALL'd with the option "EXPORT ${PROJECT_NAME}TargetGroup"

SET(_${PROJECT_NAME}_exposed_targets
  SALOMEBasics SALOMELocalTrace SalomeHDFPersist OpUtil)

# CORBA specific targets:
IF(NOT SALOME_LIGHT_ONLY)
  LIST(APPEND _${PROJECT_NAME}_exposed_targets
    DF SalomeLoggerServer with_loggerTraceCollector SalomeNS SalomeGenericObj
    Registry SalomeCatalog ResourcesManager SalomeResourcesManager
    SalomeNotification SalomeContainer SalomeSDS Launcher SalomeLauncher
    SalomeLifeCycleCORBA SalomeDSClient TOOLSDS SalomeKernelHelpers
    SalomeDSImpl SalomeDS SalomeCommunication SalomeDSCContainer)
  # Temporary solution for package src\DSC on windows
  IF(NOT WIN32)
    LIST(APPEND _${PROJECT_NAME}_exposed_targets
      CalciumC SalomeCalcium SalomeDatastream SalomePalm SalomeDSCSupervBasic
      SalomeDSCSuperv)
    IF(SALOME_PACO_PARALLEL)
      LIST(APPEND _${PROJECT_NAME}_exposed_targets SalomeParallelDSCContainer)
    ENDIF()
  ENDIF()
ENDIF()

# MPI specific targets:
IF(SALOME_USE_MPI)
  LIST(APPEND _${PROJECT_NAME}_exposed_targets
    SalomeMPIContainer SalomeTestMPIComponentEngine)
ENDIF()

# Test targets:
IF(SALOME_BUILD_TESTS)
  LIST(APPEND _${PROJECT_NAME}_exposed_targets SALOMELocalTraceTest UnitTests)
  IF(NOT SALOME_LIGHT_ONLY)
    LIST(APPEND _${PROJECT_NAME}_exposed_targets
      SALOMETraceCollectorTest UtilsTest NamingServiceTest SalomeTestComponentEngine
      LifeCycleCORBATest SalomeKernelHelpersTest SALOMEDSImplTest SALOMEDSTest)
  ENDIF()
ENDIF()

# PACO specific targets:
IF(SALOME_PACO_PARALLEL)
  LIST(APPEND _${PROJECT_NAME}_exposed_targets SalomeParallelContainer)
ENDIF()

IF(NOT SALOME_LIGHT_ONLY)
  LIST(APPEND _${PROJECT_NAME}_exposed_targets SalomeIDLKernel)
  IF(SALOME_PACO_PARALLEL)
    LIST(APPEND _${PROJECT_NAME}_exposed_targets SalomeParallelIDLKernel)
  ENDIF()
ENDIF()

# Add all targets to the build-tree export set
EXPORT(TARGETS ${_${PROJECT_NAME}_exposed_targets}
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
SET(OMNIORBPY_ROOT_DIR "${OMNIORBPY_ROOT_DIR}")
SET(LIBBATCH_ROOT_DIR "${LIBBATCH_ROOT_DIR}")

SET(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include")

# Build variables that will be expanded when configuring Salome<MODULE>Config.cmake:
SALOME_CONFIGURE_PREPARE(CppUnit Graphviz Doxygen Sphinx MPI omniORB LibBatch
                         PThread Boost HDF5 libXml2 Python)

CONFIGURE_PACKAGE_CONFIG_FILE(${PROJECT_NAME}Config.cmake.in
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION "${SALOME_INSTALL_CMAKE}"
    PATH_VARS CONF_INCLUDE_DIRS SALOME_INSTALL_CMAKE CMAKE_INSTALL_PREFIX
       CPPUNIT_ROOT_DIR GRAPHVIZ_ROOT_DIR DOXYGEN_ROOT_DIR
       SPHINX_ROOT_DIR MPI_ROOT_DIR OMNIORB_ROOT_DIR OMNIORBPY_ROOT_DIR LIBBATCH_ROOT_DIR
       PTHREAD_ROOT_DIR BOOST_ROOT_DIR HDF5_ROOT_DIR LIBXML2_ROOT_DIR
       PYTHON_ROOT_DIR SWIG_ROOT_DIR)

#   - in the install tree (VSR 16/08/2013: TEMPORARILY COMMENT THIS - TO REMOVE?):
#       Get the relative path of the include directory so
#       we can register it in the generated configuration files:
#SET(CONF_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/${INSTALL_INCLUDE_DIR}")
#CONFIGURE_PACKAGE_CONFIG_FILE(salome_adm/cmake_files/${PROJECT_NAME}Config.cmake.in
#    ${PROJECT_BINARY_DIR}/to_install/${PROJECT_NAME}Config.cmake
#    INSTALL_DESTINATION "${SALOME_INSTALL_CMAKE}"
#    PATH_VARS CONF_INCLUDE_DIRS SALOME_INSTALL_CMAKE CMAKE_INSTALL_PREFIX
#       CPPUNIT_ROOT_DIR GRAPHVIZ_ROOT_DIR DOXYGEN_ROOT_DIR
#       SPHINX_ROOT_DIR MPI_ROOT_DIR OMNIORB_ROOT_DIR OMNIORBPY_ROOT_DIR LIBBATCH_ROOT_DIR
#       PTHREAD_ROOT_DIR BOOST_ROOT_DIR HDF5_ROOT_DIR LIBXML2_ROOT_DIR
#       PYTHON_ROOT_DIR SWIG_ROOT_DIR)

WRITE_BASIC_PACKAGE_VERSION_FILE(${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${${PROJECT_NAME_UC}_VERSION}
    COMPATIBILITY AnyNewerVersion)

# Install the CMake configuration files:
INSTALL(FILES
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "${SALOME_INSTALL_CMAKE}")

# Install the export set for use with the install-tree
INSTALL(EXPORT ${PROJECT_NAME}TargetGroup DESTINATION "${SALOME_INSTALL_CMAKE}"
        FILE ${PROJECT_NAME}Targets.cmake)
