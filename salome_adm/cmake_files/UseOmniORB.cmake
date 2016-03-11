# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

INSTALL( CODE "
SET(INSTALL_PYIDL_DIR lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages/salome) # R1 CHECK
IF(WIN32)
  SET(INSTALL_PYIDL_DIR bin/salome) # R1 CHECK
ENDIF(WIN32)

MACRO(OMNIORB_COMPILE_IDL_FORPYTHON_ON_INSTALL MYOMNIORBIDLPYTHON MYIDLPYFLAGS MYIDLFILE MYFULLDIR)
  FILE(MAKE_DIRECTORY \${MYFULLDIR})
  STRING(REPLACE \" \" \";\" MYIDLPYFLAGS2 \${MYIDLPYFLAGS})
  MESSAGE(STATUS \"Compiling \${MYIDLFILE} into \${MYFULLDIR}\")
  EXECUTE_PROCESS(COMMAND \${MYOMNIORBIDLPYTHON} \${MYIDLPYFLAGS2} -C\${MYFULLDIR} \${MYIDLFILE})
ENDMACRO(OMNIORB_COMPILE_IDL_FORPYTHON_ON_INSTALL)
")

#----------------------------------------------------------------------------
# OMNIORB_ADD_MODULE macro: generate CORBA wrappings for a module.
#
# USAGE: OMNIORB_ADD_MODULE(module idlfiles incdirs [linklibs])
#
# ARGUMENTS:
#   module    : module name
#   idlfiles  : list of IDL files to be compiled into module. If just a file name is given, the source
#               tree is first inspected. If not found there, the macro assumes the file will be built
#               in the build tree (CMAKE_CURRENT_BINARY_DIR) thanks to some ADD_CUSTOM_COMMAND() call
#               (used in PARAVIS).
#   incdirs   : additional include dirs for IDL staff
#   linklibs  : additional libraries the module to be linked to (optional)
#
# For example, to build CORBA staff from MyModule.idl for module MyModule
# (that depends only on SALOME KERNEL interfaces), use the following code:
#
#   INCLUDE(UseOmniORB)
#   INCLUDE_DIRECTORIES(${OMNIORB_INCLUDE_DIR} ${KERNEL_INCLUDE_DIRS})
#   OMNIORB_ADD_MODULE(SalomeIDLMyModule MyModule.idl ${KERNEL_ROOT_DIR}/idl/salome ${KERNEL_SalomeIDLKernel})
#   INSTALL(TARGETS SalomeIDLMyModule EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})
# 
# This macro uses the following variables:
# - From FindOmniORB.cmake
#     OMNIORB_IDL            : the path to the omniidl tool
#     OMNIORB_IDLCXXFLAGS    : the options to give to omniidl generator for C++ backend
#     OMNIORB_DEFINITIONS    : additional compile options for C++
# - From FindOmniORBPy.cmake
#     OMNIORB_IDLPYFLAGS     : the options to give to omniidl generator for Python backend
#     OMNIORB_PYTHON_BACKEND : Python backend
# 
# The macro automatically adds a target "omniorb_module_<module>" which can be used to set up 
# dependencies on the generation of the files produced by omniidl (typically the header files).
# 
# TODO:
#   1. Replace hardcoded dirpaths bin/salome, idl/salome, etc by corresponding configuration options.
#   2. Revise/improve OMNIORB_COMPILE_IDL_FORPYTHON_ON_INSTALL macro usage.
#   3. Add proper handling of INCLUDE_DIRECTORIES to minimize this macro usage in target CMakeLists.txt files.
#
#----------------------------------------------------------------------------
MACRO(OMNIORB_ADD_MODULE module idlfiles incdirs)
  # process additional libraries the module to be linked to
  SET(_linklibs ${OMNIORB_LIBRARIES})
  FOREACH(_arg ${ARGN})
    SET(_linklibs ${_linklibs} ${_arg})
  ENDFOREACH()
  
  # module sources
  SET(_sources)
  # module produced files
  SET(_all_outputs)
  # type of the libraries: SHARED for Linux, STATIC for Windows
  SET(_type SHARED)
  IF(WIN32)
    SET(_type STATIC)
  ENDIF()
  IF(NOT WIN32 AND (_type STREQUAL STATIC)) 
    SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
  ENDIF()

  # add additional include dirs to the C++ and Python backend options
  SET(_cxx_flags ${OMNIORB_IDLCXXFLAGS})
  SET(_py_flags "${OMNIORB_IDLPYFLAGS}")
  FOREACH(_f ${incdirs})
    LIST(APPEND _cxx_flags "-I${_f}")
    LIST(APPEND _py_flags  "-I${_f}")
  ENDFOREACH()

  FOREACH(_input ${idlfiles})
    GET_FILENAME_COMPONENT(_base ${_input} NAME_WE)
    GET_FILENAME_COMPONENT(_path ${_input} PATH)
    IF(NOT _path)
      IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_input})
        SET(_input_cmd ${CMAKE_CURRENT_SOURCE_DIR}/${_input})
      ELSE()
        SET(_input_cmd ${CMAKE_CURRENT_BINARY_DIR}/${_input})
      ENDIF()
    ELSE()
      SET(_input_cmd ${_input})
    ENDIF()

    SET(_inc     ${CMAKE_CURRENT_BINARY_DIR}/${_base}.hh)
    SET(_src     ${CMAKE_CURRENT_BINARY_DIR}/${_base}SK.cc)
    SET(_dynsrc  ${CMAKE_CURRENT_BINARY_DIR}/${_base}DynSK.cc)

    LIST(APPEND _sources ${_src})
    LIST(APPEND _sources ${_dynsrc})
    SET(_outputs ${_inc} ${_src} ${_dynsrc})
    LIST(APPEND _all_outputs ${_outputs})

    ADD_CUSTOM_COMMAND(OUTPUT ${_outputs}
      COMMAND ${OMNIORB_IDL_COMPILER} ${_cxx_flags} ${_input_cmd}
      DEPENDS ${_input_cmd})
    
    INSTALL(FILES ${_input_cmd} DESTINATION idl/salome)
    INSTALL(FILES ${_inc}   DESTINATION include/salome)

    IF(OMNIORB_PYTHON_BACKEND)
      STRING(REPLACE ";" " " _tmp "${_py_flags}")
      INSTALL(CODE "OMNIORB_COMPILE_IDL_FORPYTHON_ON_INSTALL( \"${OMNIORB_IDL_COMPILER}\" \"${_tmp}\" \"${_input_cmd}\" \"${CMAKE_INSTALL_PREFIX}/\${INSTALL_PYIDL_DIR}\" )")
    ENDIF()
  ENDFOREACH()

  ADD_LIBRARY(${module} ${_type} ${_sources})
  TARGET_LINK_LIBRARIES(${module} ${_linklibs})
  SET_TARGET_PROPERTIES(${module} PROPERTIES COMPILE_FLAGS "${OMNIORB_DEFINITIONS}")
  ADD_CUSTOM_TARGET(omniorb_module_${module} DEPENDS ${_all_outputs})
ENDMACRO(OMNIORB_ADD_MODULE)
