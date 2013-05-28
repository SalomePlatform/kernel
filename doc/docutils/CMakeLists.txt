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
  STRING(REPLACE "/" "\\" SCR "@SET PYTHONPATH=${CMAKE_INSTALL_PREFIX}\\lib\\python2.6\\site-packages\\salome\;%PYTHONPATH%
  @SET PYTHONPATH=${CMAKE_INSTALL_PREFIX}\\bin\\salome\;%PYTHONPATH%
  @SET PATH=${CMAKE_INSTALL_PREFIX}\\lib\\salome\;%PATH%
  @SET PYTHONPATH=${OMNIORB_ROOT_USER}\\lib\\x86_win32\;%PYTHONPATH%
  @SET PYTHONPATH=${OMNIORB_ROOT_USER}\\lib\\python\;%PYTHONPATH%
  @SET PATH=${OMNIORB_ROOT_USER}\\lib\\x86_win32\;%PATH%
  ")
  SET(EXT "bat")
  SET(CALL_STR "call")
ELSE(WINDOWS)
  SET(SCR "export PYTHONPATH=${CMAKE_INSTALL_PREFIX}/lib/python2.6/site-packages/salome:\${PYTHONPATH}
  export PYTHONPATH=${CMAKE_INSTALL_PREFIX}/bin/salome:\${PYTHONPATH}
  export LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib/salome:\${LD_LIBRARY_PATH}
  export PYTHONPATH=${OMNIORB_ROOT_USER}/lib:\${PYTHONPATH}
  export PYTHONPATH=${OMNIORB_ROOT_USER}/lib/python2.6/site-packages:\${PYTHONPATH}
  export LD_LIBRARY_PATH=${OMNIORB_ROOT_USER}/lib:\${LD_LIBRARY_PATH}
  ")
  SET(EXT "sh")
  SET(CALL_STR ".")
ENDIF(WINDOWS)

FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/env_s.${EXT} "${SCR}")

ADD_CUSTOM_TARGET(html_docs ${CALL_STR} ${CMAKE_CURRENT_BINARY_DIR}/env_s.${EXT} && ${SPHINX_EXECUTABLE} -c ${CMAKE_BINARY_DIR}/doc/docutils -b html ${ALLSPHINXOPTS} html
  COMMAND ${PYTHON_EXECUTABLE} -c \"import shutil\;shutil.rmtree('''${CMAKE_INSTALL_PREFIX}/share/doc/salome/tui/KERNEL/docutils''', True)\;shutil.copytree('''${CMAKE_CURRENT_BINARY_DIR}/html''', '''${CMAKE_INSTALL_PREFIX}/share/doc/salome/tui/KERNEL/docutils''')\"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
