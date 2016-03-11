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
  ${Boost_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${PROJECT_SOURCE_DIR}/src/DSC/DSC_User
  ${PROJECT_BINARY_DIR}/src/DSC/DSC_User/Datastream/Palm
  ${PROJECT_BINARY_DIR}/src/DSC/DSC_User/Datastream/Calcium
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

ADD_SUBDIRECTORY(Palm)
ADD_SUBDIRECTORY(Calcium)

# This local variable defines the list of dependant libraries common to all target in this package.
SET(COMMON_LIBS
  SalomePalm
  SalomeCalcium
  SalomeDSCSuperv
)

ADD_DEFINITIONS(${BOOST_DEFINITIONS})

ADD_LIBRARY(SalomeDatastream fake.cc)
TARGET_LINK_LIBRARIES(SalomeDatastream ${COMMON_LIBS})
INSTALL(TARGETS SalomeDatastream EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

SET(COMMON_HEADERS_HXX
  AdjacentFunctor.hxx
  ConstTraits.hxx
  CorbaTypeManipulator.hxx
  CouplingPolicy.hxx
  DataIdFilter.hxx
  DisplayPair.hxx
  FindKeyPredicate.hxx
  GenericPort.hxx
  GenericProvidesPort.hxx
  GenericUsesPort.hxx
  IteratorTraits.hxx
)

INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
