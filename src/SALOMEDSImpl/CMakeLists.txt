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
  ${PTHREAD_INCLUDE_DIR}
  ${HDF5_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ${PROJECT_BINARY_DIR}/salome_adm
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../HDFPersist
  ${CMAKE_CURRENT_SOURCE_DIR}/../DF
  ${PROJECT_BINARY_DIR}
)

# This local variable defines the list of FLAGS common to all target in this package.
ADD_DEFINITIONS(${BOOST_DEFINITIONS})
# This local variable defines the list of dependant libraries common to all target in this package.  
SET(COMMON_LIBS
  SALOMEBasics
  SalomeHDFPersist
  DF
  ${HDF5_C_LIBRARIES}
)

SET(SalomeDSImpl_SOURCES
  SALOMEDSImpl_Tool.cxx
  SALOMEDSImpl_Callback.cxx
  SALOMEDSImpl_StudyHandle.cxx
  SALOMEDSImpl_GenericAttribute.cxx
  SALOMEDSImpl_SObject.cxx
  SALOMEDSImpl_SComponent.cxx
  SALOMEDSImpl_ChildIterator.cxx
  SALOMEDSImpl_AttributeIOR.cxx
  SALOMEDSImpl_AttributeExternalFileDef.cxx
  SALOMEDSImpl_AttributeFileType.cxx
  SALOMEDSImpl_AttributePersistentRef.cxx
  SALOMEDSImpl_AttributeSequenceOfReal.cxx
  SALOMEDSImpl_AttributeSequenceOfInteger.cxx
  SALOMEDSImpl_AttributeDrawable.cxx
  SALOMEDSImpl_AttributeSelectable.cxx
  SALOMEDSImpl_AttributeExpandable.cxx
  SALOMEDSImpl_AttributeOpened.cxx
  SALOMEDSImpl_AttributeFlags.cxx
  SALOMEDSImpl_AttributeGraphic.cxx
  SALOMEDSImpl_AttributeTextColor.cxx
  SALOMEDSImpl_AttributeTextHighlightColor.cxx
  SALOMEDSImpl_AttributePixMap.cxx
  SALOMEDSImpl_AttributeLocalID.cxx
  SALOMEDSImpl_AttributeTarget.cxx
  SALOMEDSImpl_AttributeTableOfInteger.cxx
  SALOMEDSImpl_AttributeTableOfReal.cxx
  SALOMEDSImpl_AttributeTableOfString.cxx
  SALOMEDSImpl_AttributeStudyProperties.cxx
  SALOMEDSImpl_AttributePythonObject.cxx
  SALOMEDSImpl_AttributeReal.cxx
  SALOMEDSImpl_AttributeInteger.cxx
  SALOMEDSImpl_AttributeUserID.cxx
  SALOMEDSImpl_AttributeTreeNode.cxx
  SALOMEDSImpl_AttributeName.cxx
  SALOMEDSImpl_AttributeComment.cxx
  SALOMEDSImpl_AttributeReference.cxx
  SALOMEDSImpl_AttributeParameter.cxx
  SALOMEDSImpl_AttributeString.cxx
  SALOMEDSImpl_ChildNodeIterator.cxx
  SALOMEDSImpl_UseCaseBuilder.cxx
  SALOMEDSImpl_UseCaseIterator.cxx
  SALOMEDSImpl_SComponentIterator.cxx
  SALOMEDSImpl_StudyBuilder.cxx
  SALOMEDSImpl_Study.cxx
  SALOMEDSImpl_StudyManager.cxx
  SALOMEDSImpl_IParameters.cxx
  SALOMEDSImpl_TMPFile.cxx
  SALOMEDSImpl_GenericVariable.cxx
  SALOMEDSImpl_ScalarVariable.cxx
) 

ADD_LIBRARY(SalomeDSImpl ${SalomeDSImpl_SOURCES})
TARGET_LINK_LIBRARIES(SalomeDSImpl SALOMELocalTrace ${COMMON_LIBS} ${PLATFORM_LIBS})
INSTALL(TARGETS SalomeDSImpl EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

ADD_EXECUTABLE(testDS testDS.cxx)
TARGET_LINK_LIBRARIES(testDS SalomeDSImpl ${COMMON_LIBS} ${PLATFORM_LIBS})
INSTALL(TARGETS testDS DESTINATION ${SALOME_INSTALL_BINS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
SET(NOINST_HEADERS_HXX
  SALOMEDSImpl_Tool.hxx
  SALOMEDSImpl_StudyHandle.hxx
)
FOREACH(HEADER ${NOINST_HEADERS_HXX})
  LIST(REMOVE_ITEM COMMON_HEADERS_HXX ${CMAKE_CURRENT_SOURCE_DIR}/${HEADER})
ENDFOREACH(HEADER ${NOINST_HEADERS_HXX})
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${SALOME_INSTALL_HEADERS})
