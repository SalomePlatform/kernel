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

SET(SALOMEBasics_SOURCES
  BasicsGenericDestructor.cxx
  Basics_Utils.cxx
  Basics_DirUtils.cxx
  SALOME_Basics.hxx
  BasicsGenericDestructor.hxx
  Basics_Utils.hxx
  Basics_DirUtils.hxx
)

ADD_LIBRARY(SALOMEBasics ${SALOMEBasics_SOURCES})
INSTALL(TARGETS SALOMEBasics DESTINATION ${KERNEL_salomelib_LIBS})

TARGET_LINK_LIBRARIES(SALOMEBasics ${CMAKE_THREAD_LIBS_INIT} ${PLATFORM_LIBS})

FILE(GLOB SALOMEBasics_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${SALOMEBasics_HEADERS_HXX} DESTINATION ${KERNEL_salomeinclude_HEADERS})
