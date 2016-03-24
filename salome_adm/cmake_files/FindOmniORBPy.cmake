# - Find OmniORB4 Python backend
#
# Sets the following variables:
# OMNIORB_PYTHON_BACKEND - path to the Python backend
# OMNIORBPY_PYTHONPATH   - Python path to the OmniORB modules
#
# Detection is made by looking for  
#    lib/python${_py_version}/site-packages/omniidl_be/python.py
#

################################################################
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

##############################################################################
# find python back-end
##############################################################################

SET(_py_version "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
FIND_PATH(OMNIORB_PYTHON_BACKEND
  NAMES python.py
  PATHS "${OMNIORBPY_ROOT_DIR}" "/usr" 
  PATH_SUFFIXES "/lib/python${_py_version}/site-packages/omniidl_be" "/lib/omniidl/omniidl_be" "/lib/python/omniidl_be"
  DOC "Path to python-backend directory (omniidl_be) including python.py file")

##############################################################################
# Cook our stuff
##############################################################################
MARK_AS_ADVANCED(OMNIORB_PYTHON_BACKEND)
  
SET(OMNIORB_IDLPYFLAGS)
IF (OMNIORB_PYTHON_BACKEND) 
  LIST(APPEND OMNIORB_IDLPYFLAGS -bpython -p${OMNIORB_PYTHON_BACKEND})
ENDIF()
SET(IDLPYFLAGS ${OMNIORB_IDLPYFLAGS})

GET_FILENAME_COMPONENT(OMNIORBPY_PYTHONPATH "${OMNIORB_PYTHON_BACKEND}" PATH)

# Handle standard arguments:
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OmniORBPy REQUIRED_VARS OMNIORB_PYTHON_BACKEND)
