# Copyright (C) 2012-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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
PROJECT(Salome_<Name_of_Application>_Profile C CXX)

# Ensure a proper linker behavior:
CMAKE_POLICY(SET CMP0003 NEW)

# Versioning
# ===========
# Project name, upper case
STRING(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UC)

SET(${PROJECT_NAME_UC}_MAJOR_VERSION 0)
SET(${PROJECT_NAME_UC}_MINOR_VERSION 0)
SET(${PROJECT_NAME_UC}_PATCH_VERSION 0)
SET(${PROJECT_NAME_UC}_VERSION
  ${${PROJECT_NAME_UC}_MAJOR_VERSION}.${${PROJECT_NAME_UC}_MINOR_VERSION}.${${PROJECT_NAME_UC}_PATCH_VERSION})

# Find KERNEL
# ===========
SET(KERNEL_ROOT_DIR $ENV{KERNEL_ROOT_DIR} CACHE PATH "Path to the Salome KERNEL")
IF(EXISTS ${KERNEL_ROOT_DIR})
  LIST(APPEND CMAKE_MODULE_PATH "${KERNEL_ROOT_DIR}/salome_adm/cmake_files")
  INCLUDE(SalomeMacros)
  FIND_PACKAGE(SalomeKERNEL REQUIRED)
  KERNEL_WITH_CORBA() # check whether KERNEL builded with CORBA
ELSE(EXISTS ${KERNEL_ROOT_DIR})
  MESSAGE(FATAL_ERROR "We absolutely need a Salome KERNEL, please define KERNEL_ROOT_DIR")
ENDIF(EXISTS ${KERNEL_ROOT_DIR})

# User options
# ============
OPTION(SALOME_BUILD_TESTS "Build SALOME tests" ON)
OPTION(SALOME_BUILD_DOC "Generate SALOME <Name_of_Application> documentation" ON)

IF(SALOME_BUILD_TESTS)
  ENABLE_TESTING()
ENDIF()

# KERNEL optionals:
IF(SALOME_BUILD_DOC)
  FIND_PACKAGE(SalomePython)
  FIND_PACKAGE(SalomeSphinx)
  SALOME_LOG_OPTIONAL_PACKAGE(Sphinx SALOME_BUILD_DOC)
ENDIF()

  
# Find GUI
# ===========

SET(GUI_ROOT_DIR $ENV{GUI_ROOT_DIR} CACHE PATH "Path to the Salome GUI")
IF(EXISTS ${GUI_ROOT_DIR})
  LIST(APPEND CMAKE_MODULE_PATH "${GUI_ROOT_DIR}/adm_local/cmake_files")
  FIND_PACKAGE(SalomeGUI REQUIRED)
  FULL_GUI(TRUE) # check whether GUI builded in full mode and with CORBA
  ADD_DEFINITIONS(${GUI_DEFINITIONS})
  INCLUDE_DIRECTORIES(${GUI_INCLUDE_DIRS})
ELSE(EXISTS ${GUI_ROOT_DIR})
  MESSAGE(FATAL_ERROR "We absolutely need a Salome GUI, please define GUI_ROOT_DIR")
ENDIF(EXISTS ${GUI_ROOT_DIR})

# Qt4
FIND_PACKAGE(SalomeQt4 REQUIRED COMPONENTS QtCore QtGui)
INCLUDE(${QT_USE_FILE})

# Directories
# (default values taken from KERNEL)
# ===========
SET(SALOME_INSTALL_SCRIPT_PYTHON "${SALOME_INSTALL_SCRIPT_PYTHON}" CACHE PATH "Install path: SALOME Python scripts")

# Specific to [NAME_OF_APPLICATION]:
SET(SALOME_[NAME_OF_APPLICATION]_PROFILE_INSTALL_RES_DATA "${SALOME_INSTALL_RES}/(name_of_application)_profile" CACHE PATH "Install path: SALOME [NAME_OF_APPLICATION] specific data")
SET(SALOME_[NAME_OF_APPLICATION]_PROFILE_INSTALL_DOC_GUI "${SALOME_INSTALL_DOC}/gui/${PROJECT_NAME_UC}" CACHE PATH "Install path: SALOME [NAME_OF_APPLICATION] gui documentation")
SET(SALOME_[NAME_OF_APPLICATION]_PROFILE_HTML_STATIC_PATH "${SALOME_[NAME_OF_APPLICATION]_PROFILE_INSTALL_DOC_GUI}/_static" CACHE PATH "Install path: SALOME [NAME_OF_APPLICATION] html static data")

MARK_AS_ADVANCED(SALOME_INSTALL_SCRIPT_PYTHON)

INSTALL(CODE "FILE(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/USERS)")

# Sources 
# ========
ADD_SUBDIRECTORY( src )
ADD_SUBDIRECTORY( resources )
ADD_SUBDIRECTORY( doc )

# Additional files
# ================
SET(_salome_context_API_SCRIPTS
   ${KERNEL_ROOT_DIR}/bin/salome/salomeContext.py
   ${KERNEL_ROOT_DIR}/bin/salome/salomeContextUtils.py
   ${KERNEL_ROOT_DIR}/bin/salome/parseConfigFile.py
)
SALOME_INSTALL_SCRIPTS("${_salome_context_API_SCRIPTS}" ${SALOME_INSTALL_SCRIPT_PYTHON})
