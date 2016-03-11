# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

# KERNEL detection for salome - this is typically called by dependent modules
# (GUI, PARAVIS, etc ...)
#
# The detection is simpler than for other prerequisites.
# Indeed calling SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS() with SalomeKERNEL is tricky:
#  - one would write  SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(KERNEL xyz n)
#  - then the macro would look first for a file named SalomeKERNELConfig.cmake (=the normal situation)
#  - if not found (because KERNEL_ROOT_DIR was badly set for example) the macro would then look
# for a file named FindSalomeKERNEL.cmake
#  => this is the current file, and that would trigger an infinite recursion ... :-)
#  This could be detected with a flag mechanism, but honestly this becomes an overkill.
#
# So we go for a simple lookup, without conflict check:
#

IF(NOT SalomeKERNEL_FIND_QUIETLY)
  MESSAGE(STATUS "Looking for Salome KERNEL ...")
ENDIF()

SET(CMAKE_PREFIX_PATH "${KERNEL_ROOT_DIR}")
SALOME_FIND_PACKAGE(SalomeKERNEL SalomeKERNEL CONFIG)

IF(NOT SalomeKERNEL_FIND_QUIETLY)
  MESSAGE(STATUS "Found Salome KERNEL: ${KERNEL_ROOT_DIR}")
ENDIF()

FOREACH(_res ${SalomeKERNEL_EXTRA_ENV})
  SALOME_ACCUMULATE_ENVIRONMENT(${_res} "${SalomeKERNEL_EXTRA_ENV_${_res}}")
ENDFOREACH()

#----------------------------------------------------------------------------
# KERNEL_WITH_CORBA is a macro useful for determining whether a KERNEL module 
# builded in not light mode(with CORBA)
#----------------------------------------------------------------------------
MACRO(KERNEL_WITH_CORBA)
  IF(SALOME_KERNEL_LIGHT_ONLY)
    MESSAGE(FATAL_ERROR "\nWe absolutely need a Salome KERNEL module with CORBA.\nPlease set option SALOME_LIGHT_ONLY to OFF when building KERNEL module.")
  ENDIF()
ENDMACRO(KERNEL_WITH_CORBA)
