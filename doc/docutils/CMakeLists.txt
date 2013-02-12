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

SET(SPHINXOPTS
)
SET(SOURCEDIR
  ${CMAKE_CURRENT_SOURCE_DIR}
)
SET(PAPEROPT_a4
  -D
  latex_paper_size=a4
)
SET(ALLSPHINXOPTS
  -d
  doctrees
  ${PAPEROPT_a4}
  ${SPHINXOPTS}
  ${SOURCEDIR}
)

SALOME_CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in" "${CMAKE_CURRENT_BINARY_DIR}/conf.py")                

IF(WINDOWS)
  STRING(REPLACE "/" "\\" SCR "@SET PYTHONPATH=${CMAKE_INSTALL_PREFIX}\\lib\\python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}\\site-packages\\salome\;%PYTHONPATH%
  @SET PYTHONPATH=${CMAKE_INSTALL_PREFIX}\\bin\\salome\;%PYTHONPATH%
  @SET PATH=${CMAKE_INSTALL_PREFIX}\\lib\\salome\;%PATH%
  @SET PYTHONPATH=${OMNIORB_ROOT_DIR}\\lib\\x86_win32\;%PYTHONPATH%
  @SET PYTHONPATH=${OMNIORB_ROOT_DIR}\\lib\\python\;%PYTHONPATH%
  @SET PATH=${OMNIORB_ROOT_DIR}\\lib\\x86_win32\;%PATH%
  ")
  SET(EXT "bat")
  SET(CALL_STR "call")
ELSE(WINDOWS)
  SET(SCR  " export PYTHONPATH=${CMAKE_INSTALL_PREFIX}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages/salome:\${PYTHONPATH}
  export PYTHONPATH=${CMAKE_INSTALL_PREFIX}/bin/salome:\${PYTHONPATH}
  export LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib/salome:\${LD_LIBRARY_PATH}
  export PYTHONPATH=${OMNIORB_ROOT_DIR}/lib:\${PYTHONPATH}
  export PYTHONPATH=${OMNIORB_ROOT_DIR}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages:\${PYTHONPATH}
  export PYTHONPATH=${OMNIORBPY_ROOT_DIR}/lib:\${PYTHONPATH}
  export PYTHONPATH=${OMNIORBPY_ROOT_DIR}/lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages:\${PYTHONPATH}
  export LD_LIBRARY_PATH=${OMNIORB_ROOT_DIR}/lib:\${LD_LIBRARY_PATH}
  ")
  SET(EXT "sh")
  SET(CALL_STR ".")
ENDIF(WINDOWS)

FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/env_s.${EXT} "${SCR}")

ADD_CUSTOM_TARGET(html_docs ${CALL_STR} ${CMAKE_CURRENT_BINARY_DIR}/env_s.${EXT} && ${SPHINX_EXECUTABLE} -c ${CMAKE_BINARY_DIR}/doc/docutils -b html ${ALLSPHINXOPTS} docutils
)

INSTALL(CODE "EXECUTE_PROCESS(COMMAND \"${CMAKE_COMMAND}\" --build ${CMAKE_CACHEFILE_DIR} --target html_docs)")
INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docutils DESTINATION share/doc/salome/tui/KERNEL)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/doc/salome/tui/images/head.png DESTINATION share/doc/salome/tui/KERNEL) 
