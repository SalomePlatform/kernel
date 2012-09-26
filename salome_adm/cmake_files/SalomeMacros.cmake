# Copyright (C) 2007-2012  CEA/DEN, EDF R&D
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

# File defining miscellaneous macros

#----------------------------------------------------------------------------
# SALOME_INSTALL_SCRIPTS is a macro useful for installing executable scripts.
# ARGUMENTS:
# file_list: IN : list of files to be installed as executable scripts.
# path: IN : full pathname for installing.
#----------------------------------------------------------------------------
MACRO(SALOME_INSTALL_SCRIPTS file_list path)
  SET(PERMS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
  FOREACH(file ${file_list})
    INSTALL(FILES ${file} DESTINATION ${path} PERMISSIONS ${PERMS})
    GET_FILENAME_COMPONENT(ext ${file} EXT)
    IF(ext STREQUAL .py)
      INSTALL(CODE "MESSAGE(STATUS \"py compiling ${CMAKE_INSTALL_PREFIX}/${path}/${file}\")")
      INSTALL(CODE "SET(CMD \"import py_compile ; py_compile.compile('${CMAKE_INSTALL_PREFIX}/${path}/${file}')\")")
      INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c \"\${CMD}\")")
      INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -O -c \"\${CMD}\")")
    ENDIF(ext STREQUAL .py)
  ENDFOREACH(file ${file_list})
ENDMACRO(SALOME_INSTALL_SCRIPTS)