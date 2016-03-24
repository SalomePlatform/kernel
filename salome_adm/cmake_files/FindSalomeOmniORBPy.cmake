# Copyright (C) 2013-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
# Author: Adrien Bruneton
#

# omniORB Python backend detection for SALOME
#
#  !! Please read the generic detection procedure in SalomeMacros.cmake !!
#

IF(SalomeOmniORB_FOUND)
  # If no OMNIORBPY_ROOT_DIR is given, then try with OMNIORB_ROOT_DIR:

  IF(NOT OMNIORBPY_ROOT_DIR_EXP)
    SET(OMNIORBPY_ROOT_DIR "$ENV{OMNIORBPY_ROOT_DIR}" CACHE PATH "Path to the OmniORB Python backend")
    IF(EXISTS "${OMNIORB_ROOT_DIR}" AND (NOT OMNIORBPY_ROOT_DIR))
      MESSAGE(STATUS "Looking for OmniORB Python backend in the OmniORB installation (OMNIORB_ROOT_DIR exists and OMNIORBPY_ROOT_DIR is not defined) ...")  
      SET(OMNIORBPY_ROOT_DIR "${OMNIORB_ROOT_DIR}") 
    ENDIF()
  ENDIF()

  IF(WIN32)
    SET(DIR_COUNT 3) 
  ELSE(WIN32)
    SET(DIR_COUNT 4) 
  ENDIF(WIN32)

  SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(OmniORBPy OMNIORB_PYTHON_BACKEND ${DIR_COUNT})
  #MARK_AS_ADVANCED()
ELSE()
  IF(NOT SalomeOmniORBPy_FIND_QUIETLY)
    MESSAGE(STATUS "OmniORB Python backend needs OmniORB, and OmniORB was not found!")
  ENDIF()
ENDIF()

IF(OMNIORBPY_FOUND)
  SALOME_ACCUMULATE_ENVIRONMENT(LD_LIBRARY_PATH ${OMNIORBPY_LIBRARIES})
  SALOME_ACCUMULATE_ENVIRONMENT(PYTHONPATH ${OMNIORBPY_LIBRARIES}
                                           ${OMNIORBPY_PYTHONPATH})
ENDIF()
