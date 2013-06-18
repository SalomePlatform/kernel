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
# Author: A.Geay, V. Sandler, A. Bruneton
#

#----------------------------------------------------------------------------
# LIST_CONTAINS is a macro useful for determining whether a list has a 
# particular entry
#----------------------------------------------------------------------------
MACRO(LIST_CONTAINS var value)
  SET(${var})
  FOREACH(value2 ${ARGN})
    IF(${value} STREQUAL "${value2}")
      SET(${var} TRUE)
    ENDIF (${value} STREQUAL "${value2}")
  ENDFOREACH (value2)
ENDMACRO(LIST_CONTAINS)

#----------------------------------------------------------------------------
# The PARSE_ARGUMENTS macro will take the arguments of another macro and
# define several variables.
#
# USAGE:  PARSE_ARGUMENTS(prefix arg_names options arg1 arg2...)
#
# ARGUMENTS:
#
# prefix: IN: a prefix to put on all variables it creates.
#
# arg_names: IN: a list of names.
# For each item in arg_names, PARSE_ARGUMENTS will create a 
# variable with that name, prefixed with prefix_. Each variable will be filled
# with the arguments that occur after the given arg_name is encountered
# up to the next arg_name or the end of the arguments. All options are
# removed from these lists. PARSE_ARGUMENTS also creates a
# prefix_DEFAULT_ARGS variable containing the list of all arguments up
# to the first arg_name encountered.
#
# options: IN: a list of options.
# For each item in options, PARSE_ARGUMENTS will create a
# variable with that name, prefixed with prefix_. So, for example, if prefix is
# MY_MACRO and options is OPTION1;OPTION2, then PARSE_ARGUMENTS will
# create the variables MY_MACRO_OPTION1 and MY_MACRO_OPTION2. These
# variables will be set to true if the option exists in the command line
# or false otherwise.
# arg_names and options lists should be quoted.
#
# The rest of PARSE_ARGUMENTS are arguments from another macro to be parsed.
#----------------------------------------------------------------------------
MACRO(PARSE_ARGUMENTS prefix arg_names option_names)
  SET(DEFAULT_ARGS)
  FOREACH(arg_name ${arg_names})
    SET(${prefix}_${arg_name})
  ENDFOREACH(arg_name)
  FOREACH(option ${option_names})
    SET(${prefix}_${option} FALSE)
  ENDFOREACH(option)
  SET(current_arg_name DEFAULT_ARGS)
  SET(current_arg_list)
  FOREACH(arg ${ARGN})
    LIST_CONTAINS(is_arg_name ${arg} ${arg_names})
    IF (is_arg_name)
      SET(${prefix}_${current_arg_name} ${current_arg_list})
      SET(current_arg_name ${arg})
      SET(current_arg_list)
    ELSE (is_arg_name)
      LIST_CONTAINS(is_option ${arg} ${option_names})
      IF (is_option)
      SET(${prefix}_${arg} TRUE)
      ELSE (is_option)
      SET(current_arg_list ${current_arg_list} ${arg})
      ENDIF (is_option)
    ENDIF (is_arg_name)
  ENDFOREACH(arg)
  SET(${prefix}_${current_arg_name} ${current_arg_list})
ENDMACRO(PARSE_ARGUMENTS)

#----------------------------------------------------------------------------
# SALOME_INSTALL_SCRIPTS is a macro useful for installing scripts.
#
# USAGE: SALOME_INSTALL_SCRIPTS(file_list path [WORKING_DIRECTORY dir] [DEF_PERMS])
#
# ARGUMENTS:
# file_list: IN : list of files to be installed. This list should be quoted.
# path: IN : full pathname for installing.
# 
# By default files to be installed as executable scripts.
# If DEF_PERMS option is provided, than permissions for installed files are
# only OWNER_WRITE, OWNER_READ, GROUP_READ, and WORLD_READ. 
#----------------------------------------------------------------------------
MACRO(SALOME_INSTALL_SCRIPTS file_list path)
  PARSE_ARGUMENTS(SALOME_INSTALL_SCRIPTS "WORKING_DIRECTORY" "DEF_PERMS" ${ARGN})
  SET(PERMS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
  IF(NOT SALOME_INSTALL_SCRIPTS_DEF_PERMS)
    SET(PERMS ${PERMS} OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)
  ENDIF(NOT SALOME_INSTALL_SCRIPTS_DEF_PERMS)
  FOREACH(file ${file_list})
    SET(PREFIX "")
    IF(SALOME_INSTALL_SCRIPTS_WORKING_DIRECTORY)
      SET(PREFIX "${SALOME_INSTALL_SCRIPTS_WORKING_DIRECTORY}/")
    ENDIF(SALOME_INSTALL_SCRIPTS_WORKING_DIRECTORY)
    INSTALL(FILES ${PREFIX}${file} DESTINATION ${path} PERMISSIONS ${PERMS})
    GET_FILENAME_COMPONENT(ext ${file} EXT)
    IF(ext STREQUAL .py)
      INSTALL(CODE "MESSAGE(STATUS \"py compiling ${CMAKE_INSTALL_PREFIX}/${path}/${file}\")")
      INSTALL(CODE "SET(CMD \"import py_compile ; py_compile.compile('${CMAKE_INSTALL_PREFIX}/${path}/${file}')\")")
      INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c \"\${CMD}\")")
      INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -O -c \"\${CMD}\")")
    ENDIF(ext STREQUAL .py)
  ENDFOREACH(file ${file_list})
ENDMACRO(SALOME_INSTALL_SCRIPTS)

#----------------------------------------------------------------------------
# SALOME_INSTALL_SCRIPTS is a macro useful for installing executable scripts.
# ARGUMENTS:
# PYFILE2COMPINST: IN : list of python files to be installed.
# PYFILELOC: IN : full pathname for installing.
# Permissions of installed files: OWNER_WRITE, OWNER_READ, GROUP_READ, and WORLD_READ
#----------------------------------------------------------------------------
MACRO(INSTALL_AND_COMPILE_PYTHON_FILE PYFILE2COMPINST PYFILELOC)
  INSTALL(CODE "SET(PYTHON_FILE ${f})")
  FOREACH(input ${PYFILE2COMPINST})
    GET_FILENAME_COMPONENT(inputname ${input} NAME)
    INSTALL(FILES ${input} DESTINATION ${CMAKE_INSTALL_PREFIX}/${PYFILELOC})
    INSTALL(CODE "MESSAGE(STATUS \"py compiling ${CMAKE_INSTALL_PREFIX}/${PYFILELOC}/${inputname}\")")
    INSTALL(CODE "SET(CMD \"import py_compile ; py_compile.compile('${CMAKE_INSTALL_PREFIX}/${PYFILELOC}/${inputname}')\")")
    INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c \"\${CMD}\")")
    INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -O -c \"\${CMD}\")")
  ENDFOREACH(input ${PYFILE2COMPINST})
ENDMACRO(INSTALL_AND_COMPILE_PYTHON_FILE PYFILE2COMPINST PYFILELOC)

#----------------------------------------------------------------------------
# SALOME_CONFIGURE_FILE is a macro useful for copying a file to another location 
# and modify its contents.
#
# USAGE: SALOME_CONFIGURE_FILE(in_file out_file [INSTALL dir])
#
# ARGUMENTS:
# in_file: IN : input file with full paths.
# out_file: IN : output file with full paths.
# If INSTALL is specified, then 'out_file' will be installed to the 'dir' directory.
#----------------------------------------------------------------------------
MACRO(SALOME_CONFIGURE_FILE IN_FILE OUT_FILE)
  MESSAGE(STATUS "Creation of ${OUT_FILE}")
  CONFIGURE_FILE(${IN_FILE} ${OUT_FILE} @ONLY)
  PARSE_ARGUMENTS(SALOME_CONFIGURE_FILE "INSTALL" "" ${ARGN})
  IF(SALOME_CONFIGURE_FILE_INSTALL)
    INSTALL(FILES ${OUT_FILE} DESTINATION ${SALOME_CONFIGURE_FILE_INSTALL})
  ENDIF(SALOME_CONFIGURE_FILE_INSTALL)
ENDMACRO(SALOME_CONFIGURE_FILE)


#######################################################################################
# Useful macros for SALOME own package detection system
#

###
# SALOME_CHECK_EQUAL_PATHS(result path1 path2)
#  Check if two paths are identical, resolving links. If the paths do not exist a simple
#  text comparison is performed.
#  result is a boolean.
###
MACRO(SALOME_CHECK_EQUAL_PATHS varRes path1 path2)  
  SET("${varRes}" OFF)
  IF(EXISTS "${path1}")
    GET_FILENAME_COMPONENT(_tmp1 "${path1}" REALPATH)
  ELSE()
    SET(_tmp1 "${path1}")
  ENDIF() 

  IF(EXISTS "${path2}")
    GET_FILENAME_COMPONENT(_tmp2 "${path2}" REALPATH)
  ELSE()
    SET(_tmp2 "${path2}")
  ENDIF() 

  IF("${_tmp1}" STREQUAL "${_tmp2}")
    SET("${varRes}" ON)
  ENDIF()
#  MESSAGE(${${varRes}})
ENDMACRO()


####
# SALOME_FIND_PACKAGE(englobingPackageName standardPackageName modus)
# Encapsulate the call to the standard FIND_PACKAGE(standardPackageName) passing all the options
# given when calling the command FIND_PACKAGE(customPackageName)
# Modus is either MODULE or CONFIG (cf standard FIND_PACKAGE() documentation).
# This macro is to be called from within the FindCustomPackage.cmake file.
####
MACRO(SALOME_FIND_PACKAGE englobPkg stdPkg mode)
  # Only bother if the package was not already found:
  # Some old packages use the lower case version - standard should be to always use
  # upper case:
  STRING(TOUPPER ${stdPkg} stdPkgUC)
  IF(NOT (${stdPkg}_FOUND OR ${stdPkgUC}_FOUND))
    IF(${englobPkg}_FIND_QUIETLY)
      SET(_tmp_quiet "QUIET")
    ELSE()
      SET(_tmp_quiet)
    ENDIF()  
    IF(${englobPkg}_FIND_REQUIRED)
      SET(_tmp_req "REQUIRED")
    ELSE()
      SET(_tmp_req)
    ENDIF()  
    IF(${englobPkg}_FIND_VERSION_EXACT)
      SET(_tmp_exact "EXACT")
    ELSE()
      SET(_tmp_exact)
    ENDIF()
    IF(${englobPkg}_FIND_COMPONENTS)
      STRING(REPLACE ";" " " _tmp_compo ${${englobPkg}_FIND_COMPONENTS})
    ELSE()
      SET(_tmp_compo)
    ENDIF()

    # Call the root FIND_PACKAGE():
    IF(_tmp_compo)
      FIND_PACKAGE(${stdPkg} ${${englobPkg}_FIND_VERSION} ${_tmp_exact} ${mode} ${_tmp_quiet} ${_tmp_req} COMPONENTS ${_tmp_compo})
    ELSE()
      FIND_PACKAGE(${stdPkg} ${${englobPkg}_FIND_VERSION} ${_tmp_exact} ${mode} ${_tmp_quiet} ${_tmp_req})
    ENDIF()
  ENDIF()
ENDMACRO()


####################################################################"
# SALOME_FIND_PACKAGE_DETECT_CONFLICTS(pkg referenceVariable upCount)
#    pkg              : name of the system package to be detected
#    referenceVariable: variable containing a path that can be browsed up to 
# retrieve the package root directory (xxx_ROOT_DIR)
#    upCount          : number of times we have to go up from the path <referenceVariable>
# to obtain the package root directory.  
#   
# For example:  SALOME_FIND_PACKAGE_DETECT_CONFLICTS(SWIG SWIG_EXECUTABLE 2) 
#
# Generic detection (and conflict check) procedure for package XYZ:
# 1. Load a potential env variable XYZ_ROOT_DIR as a default choice for the cache entry XYZ_ROOT_DIR
#    If empty, load a potential XYZ_ROOT_DIR_EXP as default value (path exposed by another package depending
# directly on XYZ)
# 2. Invoke FIND_PACKAGE() in this order:
#    * in CONFIG mode first (if possible): priority is given to a potential 
#    "XYZ-config.cmake" file
#    * then switch to the standard MODULE mode, appending on CMAKE_PREFIX_PATH 
# the above XYZ_ROOT_DIR variable
# 3. Extract the path actually found into a temp variable _XYZ_TMP_DIR
# 4. Warn if XYZ_ROOT_DIR is set and doesn't match what was found (e.g. when CMake found the system installation
#    instead of what is pointed to by XYZ_ROOT_DIR - happens when a typo in the content of XYZ_ROOT_DIR).
# 5. Conflict detection:
#    * check the temp variable against a potentially existing XYZ_ROOT_DIR_EXP
# 6. Finally expose what was *actually* found in XYZ_ROOT_DIR.  
# 7. Specific stuff: for example exposing a prerequisite of XYZ to the rest of the world for future 
# conflict detection. This is added after the call to the macro by the callee.
#
MACRO(SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS pkg referenceVariable upCount)
  STRING(TOUPPER ${pkg} pkg_UC)

  # 1. Load environment or any previously detected root dir for the package
  IF(DEFINED ENV{${pkg_UC}_ROOT_DIR})
    FILE(TO_CMAKE_PATH "$ENV{${pkg_UC}_ROOT_DIR}" _${pkg_UC}_ROOT_DIR_ENV)
    SET(_dflt_value "${_${pkg_UC}_ROOT_DIR_ENV}")
  ELSE()
    # will be blank if no package was previously loaded
    SET(_dflt_value "${${pkg_UC}_ROOT_DIR_EXP}")
  ENDIF()

  #   Make cache entry 
  SET(${pkg_UC}_ROOT_DIR "${_dflt_value}" CACHE PATH "Path to ${pkg_UC} directory")

  # 2. Find package - config mode first (i.e. looking for XYZ-config.cmake)
  IF(EXISTS "${${pkg_UC}_ROOT_DIR}")
    # Hope to find direclty a CMake config file there
    SET(_CONF_DIR "${${pkg_UC}_ROOT_DIR}/share/cmake") 

    # Try find_package in config mode with a hard-coded guess. This
    # has the priority.
    STRING(TOLOWER ${pkg} _pkg_lc)
    FIND_PACKAGE(${pkg} NO_MODULE QUIET PATHS "${_CONF_DIR}" "${_CONF_DIR}/${pkg}" 
              "${_CONF_DIR}/${_pkg_lc}")
    MARK_AS_ADVANCED(${pkg}_DIR)
    
    IF (NOT (${pkg_UC}_FOUND OR ${pkg}_FOUND))  
      LIST(APPEND CMAKE_PREFIX_PATH "${${pkg_UC}_ROOT_DIR}")
    ELSE()
      MESSAGE(STATUS "Found ${pkg} in CONFIG mode!")
    ENDIF()
  ENDIF()

  # Otherwise try the standard way (module mode, with the standard CMake Find*** macro):
  SALOME_FIND_PACKAGE("Salome${pkg}" ${pkg} MODULE)
  #MESSAGE("dbg ${pkg_UC} / ${PTHREAD_FOUND} / ${${pkg_UC}_FOUND}")
  # Set the "FOUND" variable for the SALOME wrapper:
  IF(${pkg_UC}_FOUND OR ${pkg}_FOUND)
    SET(SALOME${pkg_UC}_FOUND TRUE)
  ELSE()
    SET(SALOME${pkg_UC}_FOUND FALSE)
  ENDIF()
  
  IF (${pkg_UC}_FOUND OR ${pkg}_FOUND)
    # 3. Set the root dir which was finally retained by going up "upDir" times
    # from the given reference path. The variable "referenceVariable" may be a list
    # - we take its first element. 
    #   Note the double de-reference of "referenceVariable":
    LIST(LENGTH "${${referenceVariable}}" _tmp_len)
    IF(_tmp_len)
       LIST(GET "${${referenceVariable}}" 0 _tmp_ROOT_DIR)
    ELSE()
       SET(_tmp_ROOT_DIR "${${referenceVariable}}")
    ENDIF()
    IF(${upCount})
      MATH(EXPR _rge "${upCount}-1") 
      FOREACH(_unused RANGE ${_rge})        
        GET_FILENAME_COMPONENT(_tmp_ROOT_DIR "${_tmp_ROOT_DIR}" PATH)
      ENDFOREACH()
    ENDIF()

    # 4. Warn if CMake found something not located under ENV(XYZ_ROOT_DIR)
    IF(DEFINED ENV{${pkg_UC}_ROOT_DIR})
      SALOME_CHECK_EQUAL_PATHS(_res "${_tmp_ROOT_DIR}" "${_${pkg_UC}_ROOT_DIR_ENV}")
      IF(NOT _res)
        MESSAGE(WARNING "${pkg} was found, but not at the path given by the "
            "environment ${pkg_UC}_ROOT_DIR! Is the variable correctly set? "
            "The two paths are: ${_tmp_ROOT_DIR} and: ${_${pkg_UC}_ROOT_DIR_ENV}")
        
      ELSE()
        MESSAGE(STATUS "${pkg} found directory matches what was specified in the ${pkg_UC}_ROOT_DIR variable, all good!")    
      ENDIF()
    ELSE()
        MESSAGE(STATUS "Environment variable ${pkg_UC}_ROOT_DIR is not defined. The system installation was found.")
    ENDIF()

    # 5. Conflict detection
    # 5.1  From another prerequisite using the package
    IF(${pkg_UC}_ROOT_DIR_EXP)
        SALOME_CHECK_EQUAL_PATHS(_res "${_tmp_ROOT_DIR}" "${${pkg_UC}_ROOT_DIR_EXP}") 
        IF(NOT _res)
           MESSAGE(WARNING "Warning: ${pkg}: detected version conflicts with a previously found ${pkg}!"
                            "The two paths are " ${_tmp_ROOT_DIR} " vs " ${${pkg_UC}_ROOT_DIR_EXP})
        ELSE()
            MESSAGE(STATUS "${pkg} directory matches what was previously exposed by another prereq, all good!")
        ENDIF()        
    ENDIF()
    
    # 6. Save the found installation
    #
    SET(${pkg_UC}_ROOT_DIR "${_tmp_ROOT_DIR}")
     
  ELSE()
    MESSAGE(STATUS "${pkg} was not found.")  
  ENDIF()
ENDMACRO(SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS)

