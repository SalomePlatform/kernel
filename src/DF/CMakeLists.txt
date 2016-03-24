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

INCLUDE_DIRECTORIES(
  ${Boost_INCLUDE_DIR}
)
ADD_DEFINITIONS(${BOOST_DEFINITIONS})

SET(DF_SOURCES
  DF_Attribute.cxx
  DF_Label.cxx
  DF_Document.cxx
  DF_Application.cxx
  DF_ChildIterator.cxx
  DF_Container.cxx
)

ADD_LIBRARY(DF ${DF_SOURCES})
INSTALL(TARGETS DF EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

ADD_EXECUTABLE(testDF testDF.cxx)
TARGET_LINK_LIBRARIES(testDF DF)
INSTALL(TARGETS testDF DESTINATION ${SALOME_INSTALL_BINS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
