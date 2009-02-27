#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
MARK_AS_ADVANCED(ISSUE)
FIND_FILE(ISSUE issue /etc)
IF(ISSUE)
  SET(WINDOWS 0)
ELSE()
  SET(WINDOWS 1)
ENDIF(ISSUE)

IF(WINDOWS)
  SET(MACHINE WINDOWS)
  SET(PLATFORM_CPPFLAGS)
  SET(PLATFORM_CPPFLAGS ${PLATFORM_CPPFLAGS} -DWNT)
  SET(PLATFORM_CPPFLAGS ${PLATFORM_CPPFLAGS} -DPPRO_NT)
  SET(PLATFORM_CPPFLAGS ${PLATFORM_CPPFLAGS} /W0)
  #  #  SET(PLATFORM_CPPFLAGS ${PLATFORM_CPPFLAGS} -DWNT -D_CRT_SECURE_NO_WARNINGS)
  #  #  SET(PLATFORM_CPPFLAGS ${PLATFORM_CPPFLAGS} -D_WIN32_WINNT=0x0500)
  #  SET(RM del)
ELSE(WINDOWS)
  SET(MACHINE PCLINUX)
  SET(PLATFORM_CPPFLAGS)
  #  SET(RM /bin/rm)
ENDIF(WINDOWS)

IF(WINDOWS)
  SET(CMAKE_BUILD_TYPE $ENV{CMAKE_BUILD_TYPE})
  IF(CMAKE_BUILD_TYPE STREQUAL Release)
    MARK_AS_ADVANCED(CLEAR CMAKE_CONFIGURATION_TYPES)
    SET(CMAKE_CONFIGURATION_TYPES Release CACHE STRING "compilation types" FORCE)
  ENDIF(CMAKE_BUILD_TYPE STREQUAL Release)
ENDIF(WINDOWS)

IF(WINDOWS)
  SET(CMAKE_INSTALL_PREFIX_ENV $ENV{CMAKE_INSTALL_PREFIX})
  IF(CMAKE_INSTALL_PREFIX_ENV)
    SET(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX_ENV} CACHE PATH "installation prefix" FORCE)
  ENDIF(CMAKE_INSTALL_PREFIX_ENV)
ENDIF(WINDOWS)

## SET(PLATFORM_CPPFLAGS ${PLATFORM_CPPFLAGS} -D_DEBUG_)

#SET(RCP rcp)
#SET(RSH rsh)
#SET(RCP rcp)
