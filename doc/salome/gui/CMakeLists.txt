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

SALOME_CONFIGURE_FILE(doxyfile.in doxyfile)
SALOME_CONFIGURE_FILE(static/header.html.in static/header.html)

SET(gen_doc "${CMAKE_SOURCE_DIR}/salome_adm/prepare_generating_doc.py")
IF(WIN32)
  STRING(REPLACE "/" "\\" gen_doc ${gen_doc})
ENDIF(WIN32)
SET(kernel_py ${CMAKE_SOURCE_DIR}/src/KERNEL_PY)

ADD_CUSTOM_TARGET(usr_docs ${CMAKE_COMMAND} -E make_directory tmp
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/kernel.py ${kernel_py}/kernel/__init__.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/deprecation.py ${kernel_py}/kernel/deprecation.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/termcolor.py ${kernel_py}/kernel/termcolor.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/logger.py ${kernel_py}/kernel/logger.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/enumerate.py ${kernel_py}/kernel/enumerate.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/uiexception.py ${kernel_py}/kernel/uiexception.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/datamodeler.py ${kernel_py}/kernel/datamodeler.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/diclookup.py ${kernel_py}/kernel/diclookup.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/services.py ${kernel_py}/kernel/services.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/studyedit.py ${kernel_py}/kernel/studyedit.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/unittester.py ${kernel_py}/kernel/unittester.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/pyunittester.py ${kernel_py}/kernel/pyunittester.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/parametric.py ${kernel_py}/kernel/parametric/__init__.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/study_exchange_vars.py ${kernel_py}/kernel/parametric/study_exchange_vars.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/compo_utils.py ${kernel_py}/kernel/parametric/compo_utils.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/pyscript_utils.py ${kernel_py}/kernel/parametric/pyscript_utils.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/salome.py ${kernel_py}/__init__.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/salome_iapp.py ${kernel_py}/salome_iapp.py
  COMMAND ${PYTHON_EXECUTABLE} ${gen_doc} -o tmp/SALOME_DriverPy.py ${CMAKE_SOURCE_DIR}/src/SALOMEDS/SALOME_DriverPy.py
  COMMAND ${DOXYGEN_EXECUTABLE} doxyfile
  COMMAND  ${CMAKE_COMMAND} -E remove_directory tmp
  VERBATIM 
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}             
)

INSTALL(CODE "EXECUTE_PROCESS(COMMAND \"${CMAKE_COMMAND}\" --build ${PROJECT_BINARY_DIR} --target usr_docs)")
INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/KERNEL DESTINATION ${SALOME_INSTALL_DOC}/gui)
INSTALL(FILES images/head.png DESTINATION ${SALOME_INSTALL_DOC}/gui/KERNEL) 

SET(MAKE_CLEAN_FILES KERNEL log.txt)
SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${MAKE_CLEAN_FILES}")
