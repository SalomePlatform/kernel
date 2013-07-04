# - Find OmniORB4 Python backend
#
# Sets the following variables:
# OMNIORB_PYTHON_BACKEND - path to the Python backend
#
# Detection is made by looking for  
#    lib/python${_py_version}/site-packages/omniidl_be/python.py
#

################################################################
# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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
################################################################

##############################################################################
# find python back-end
##############################################################################

SET(_py_version "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
SET(CMAKE_FIND_ROOT_PATH ${CMAKE_PREFIX_PATH})
SET(_doc "Path to python-backend directory (omniidl_be) including python.py file")
IF(WIN32)
  FIND_PATH(OMNIORB_PYTHON_BACKEND
     NAMES python.py PATHS "/lib/python/omniidl_be" DOC ${_doc}) 
ELSE()
  FIND_PATH(OMNIORB_PYTHON_BACKEND
     NAMES python.py
     PATHS "/lib/python${_py_version}/site-packages/omniidl_be" "/usr/lib/omniidl/omniidl_be" 
     DOC ${_doc})
ENDIF()

##############################################################################
# Cook our stuff
##############################################################################
MARK_AS_ADVANCED(OMNIORB_PYTHON_BACKEND)
  
#SET(OMNIORB_IDLCXXFLAGS -Wba -nf)
SET(OMNIORB_IDLPYFLAGS -bpython)
IF (OMNIORB_PYTHON_BACKEND) 
  SET(OMNIORB_IDLPYFLAGS "-p ${OMNIORB_PYTHON_BACKEND} ${OMNIORB_IDLPYFLAGS}")
ENDIF()
#SET(IDLCXXFLAGS ${OMNIORB_IDLCXXFLAGS})
SET(IDLPYFLAGS ${OMNIORB_IDLPYFLAGS})
#SET(IDL_CLN_H .hh)
#SET(IDL_SRV_H .hh)
#SET(OMNIORB_DEFINITIONS "-D__x86__ -DCOMP_CORBA_DOUBLE -DCOMP_CORBA_LONG")
#IF(WIN32)
#  SET(OMNIORB_DEFINITIONS "${OMNIORB_DEFINITIONS} -D__WIN32__")
#ENDIF()
#IF(APPLE)
  #SET(OMNIORB_DEFINITIONS "${OMNIORB_DEFINITIONS} -D__macos__")#for omnithread.h to be checked...
#ENDIF()

# Handle standard arguments:
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OmniORBPy REQUIRED_VARS OMNIORB_PYTHON_BACKEND)
