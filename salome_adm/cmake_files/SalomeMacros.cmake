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