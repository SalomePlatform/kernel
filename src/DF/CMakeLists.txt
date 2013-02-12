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

INCLUDE_DIRECTORIES(
  ${Boost_INCLUDE_DIRS}
)
ADD_DEFINITIONS(${BOOST_DEFINITIONS})

SET(DF_SOURCES
  DF_Attribute.cxx
  DF_Label.cxx
  DF_Document.cxx
  DF_Application.cxx
  DF_ChildIterator.cxx
  DF_Container.cxx
  DF_Attribute.hxx
  DF_Label.hxx
  DF_Application.hxx
  DF_Document.hxx
  DF_ChildIterator.hxx
  DF_Container.hxx
  DF_definitions.hxx
)

ADD_LIBRARY(DF ${DF_SOURCES})
INSTALL(TARGETS DF DESTINATION ${KERNEL_salomelib_LIBS})

ADD_EXECUTABLE(testDF testDF.cxx)
TARGET_LINK_LIBRARIES(testDF DF)
INSTALL(TARGETS testDF DESTINATION ${KERNEL_salomebin_BINS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${KERNEL_salomeinclude_HEADERS})
