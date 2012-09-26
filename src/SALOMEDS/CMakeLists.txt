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
  ${PTHREAD_INCLUDE_DIRS}
  ${OMNIORB_INCLUDE_DIRS}
  ${HDF5_INCLUDE_DIRS}
  ${BOOST_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/salome_adm/unix
  ${CMAKE_CURRENT_SOURCE_DIR}/../HDFPersist
  ${CMAKE_CURRENT_SOURCE_DIR}/../Basics
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMELocalTrace
  ${CMAKE_CURRENT_SOURCE_DIR}/../Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/../DF
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMEDSImpl
  ${CMAKE_CURRENT_SOURCE_DIR}/../NamingService
  ${CMAKE_CURRENT_SOURCE_DIR}/../GenericObj
  ${CMAKE_CURRENT_SOURCE_DIR}/../SALOMEDSClient
  ${CMAKE_CURRENT_SOURCE_DIR}/../LifeCycleCORBA
  ${CMAKE_BINARY_DIR}/idl
)

SET(COMMON_FLAGS "${OMNIORB_DEFINITIONS} ${HDF5_DEFINITIONS} ${BOOST_DEFINITIONS} ${PLATFORM_DEFINITIONS}")
SET(COMMON_LIBS
  TOOLSDS
  SalomeNS
  OpUtil
  SALOMELocalTrace
  SALOMEBasics
  SalomeHDFPersist
  DF
  SalomeDSImpl
  SalomeGenericObj
  SalomeLifeCycleCORBA
  SalomeIDLKernel
  ${HDF5_LIBS}
)

SET(SalomeDS_SOURCES
  SALOMEDS.cxx
  SALOMEDS_Driver_i.cxx
  SALOMEDS_StudyManager_i.cxx
  SALOMEDS_UseCaseBuilder_i.cxx
  SALOMEDS_UseCaseIterator_i.cxx
  SALOMEDS_ChildIterator_i.cxx
  SALOMEDS_SComponentIterator_i.cxx
  SALOMEDS_Study_i.cxx
  SALOMEDS_StudyBuilder_i.cxx
  SALOMEDS_SObject_i.cxx
  SALOMEDS_SComponent_i.cxx
  SALOMEDS_GenericAttribute_i.cxx
  SALOMEDS_AttributeComment_i.cxx
  SALOMEDS_AttributeExternalFileDef_i.cxx
  SALOMEDS_AttributeFileType_i.cxx
  SALOMEDS_AttributeIOR_i.cxx
  SALOMEDS_AttributeInteger_i.cxx
  SALOMEDS_AttributeName_i.cxx
  SALOMEDS_AttributePersistentRef_i.cxx
  SALOMEDS_AttributeReal_i.cxx
  SALOMEDS_AttributeSequenceOfReal_i.cxx
  SALOMEDS_AttributeSequenceOfInteger_i.cxx
  SALOMEDS_AttributeDrawable_i.cxx
  SALOMEDS_AttributeSelectable_i.cxx
  SALOMEDS_AttributeOpened_i.cxx
  SALOMEDS_AttributeFlags_i.cxx
  SALOMEDS_AttributeGraphic_i.cxx
  SALOMEDS_AttributeExpandable_i.cxx
  SALOMEDS_AttributeTextColor_i.cxx
  SALOMEDS_AttributeTextHighlightColor_i.cxx
  SALOMEDS_AttributePixMap_i.cxx
  SALOMEDS_AttributeTreeNode_i.cxx
  SALOMEDS_AttributeLocalID_i.cxx
  SALOMEDS_AttributeUserID_i.cxx
  SALOMEDS_AttributeTarget_i.cxx
  SALOMEDS_AttributeTableOfInteger_i.cxx
  SALOMEDS_AttributeTableOfReal_i.cxx
  SALOMEDS_AttributeTableOfString_i.cxx
  SALOMEDS_AttributeStudyProperties_i.cxx
  SALOMEDS_AttributePythonObject_i.cxx
  SALOMEDS_AttributeParameter_i.cxx
  SALOMEDS_AttributeString_i.cxx
  SALOMEDS_SObject.cxx
  SALOMEDS_SComponent.cxx
  SALOMEDS_GenericAttribute.cxx
  SALOMEDS_ChildIterator.cxx
  SALOMEDS_SComponentIterator.cxx
  SALOMEDS_UseCaseIterator.cxx
  SALOMEDS_UseCaseBuilder.cxx
  SALOMEDS_StudyBuilder.cxx
  SALOMEDS_Study.cxx
  SALOMEDS_StudyManager.cxx
  SALOMEDS_AttributeStudyProperties.cxx
  SALOMEDS_AttributeComment.cxx
  SALOMEDS_AttributeDrawable.cxx
  SALOMEDS_AttributeExpandable.cxx
  SALOMEDS_AttributeExternalFileDef.cxx
  SALOMEDS_AttributeFileType.cxx
  SALOMEDS_AttributeFlags.cxx
  SALOMEDS_AttributeGraphic.cxx
  SALOMEDS_AttributeIOR.cxx
  SALOMEDS_AttributeInteger.cxx
  SALOMEDS_AttributeLocalID.cxx
  SALOMEDS_AttributeName.cxx
  SALOMEDS_AttributeOpened.cxx
  SALOMEDS_AttributePythonObject.cxx
  SALOMEDS_AttributeReal.cxx
  SALOMEDS_AttributeSelectable.cxx
  SALOMEDS_AttributeSequenceOfInteger.cxx
  SALOMEDS_AttributePersistentRef.cxx
  SALOMEDS_AttributePixMap.cxx
  SALOMEDS_AttributeSequenceOfReal.cxx
  SALOMEDS_AttributeTableOfInteger.cxx
  SALOMEDS_AttributeTableOfReal.cxx
  SALOMEDS_AttributeTableOfString.cxx
  SALOMEDS_AttributeTarget.cxx
  SALOMEDS_AttributeTextColor.cxx
  SALOMEDS_AttributeTextHighlightColor.cxx
  SALOMEDS_AttributeTreeNode.cxx
  SALOMEDS_AttributeUserID.cxx
  SALOMEDS_TMPFile_i.cxx
  SALOMEDS_AttributeParameter.cxx
  SALOMEDS_AttributeString.cxx
  SALOMEDS_IParameters.cxx
  SALOMEDS_AttLong_i.hxx
  SALOMEDS_AttReal_i.hxx
  SALOMEDS_AttributeComment.hxx
  SALOMEDS_AttributeComment_i.hxx
  SALOMEDS_AttributeDrawable.hxx
  SALOMEDS_AttributeDrawable_i.hxx
  SALOMEDS_AttributeExpandable.hxx
  SALOMEDS_AttributeExpandable_i.hxx
  SALOMEDS_AttributeExternalFileDef.hxx
  SALOMEDS_AttributeExternalFileDef_i.hxx
  SALOMEDS_AttributeFileType.hxx
  SALOMEDS_AttributeFileType_i.hxx
  SALOMEDS_AttributeFlags.hxx
  SALOMEDS_AttributeFlags_i.hxx
  SALOMEDS_AttributeGraphic.hxx
  SALOMEDS_AttributeGraphic_i.hxx
  SALOMEDS_AttributeInteger.hxx
  SALOMEDS_AttributeInteger_i.hxx
  SALOMEDS_AttributeIOR.hxx
  SALOMEDS_AttributeIOR_i.hxx
  SALOMEDS_AttributeLocalID.hxx
  SALOMEDS_AttributeLocalID_i.hxx
  SALOMEDS_AttributeName.hxx
  SALOMEDS_AttributeName_i.hxx
  SALOMEDS_AttributeOpened.hxx
  SALOMEDS_AttributeOpened_i.hxx
  SALOMEDS_AttributePersistentRef.hxx
  SALOMEDS_AttributePersistentRef_i.hxx
  SALOMEDS_AttributePixMap.hxx
  SALOMEDS_AttributePixMap_i.hxx
  SALOMEDS_AttributePythonObject.hxx
  SALOMEDS_AttributePythonObject_i.hxx
  SALOMEDS_AttributeReal.hxx
  SALOMEDS_AttributeReal_i.hxx
  SALOMEDS_AttributeSelectable.hxx
  SALOMEDS_AttributeSelectable_i.hxx
  SALOMEDS_AttributeSequenceOfInteger.hxx
  SALOMEDS_AttributeSequenceOfInteger_i.hxx
  SALOMEDS_AttributeSequenceOfReal.hxx
  SALOMEDS_AttributeSequenceOfReal_i.hxx
  SALOMEDS_Attributes.hxx
  SALOMEDS_AttributeStudyProperties.hxx
  SALOMEDS_AttributeStudyProperties_i.hxx
  SALOMEDS_AttributeTableOfInteger.hxx
  SALOMEDS_AttributeTableOfInteger_i.hxx
  SALOMEDS_AttributeTableOfReal.hxx
  SALOMEDS_AttributeTableOfReal_i.hxx
  SALOMEDS_AttributeTableOfString.hxx
  SALOMEDS_AttributeTableOfString_i.hxx
  SALOMEDS_AttributeTarget.hxx
  SALOMEDS_AttributeTarget_i.hxx
  SALOMEDS_AttributeTextColor.hxx
  SALOMEDS_AttributeTextColor_i.hxx
  SALOMEDS_AttributeTextHighlightColor.hxx
  SALOMEDS_AttributeTextHighlightColor_i.hxx
  SALOMEDS_AttributeTreeNode.hxx
  SALOMEDS_AttributeTreeNode_i.hxx
  SALOMEDS_AttributeUserID.hxx
  SALOMEDS_AttributeUserID_i.hxx
  SALOMEDS_BasicAttributeFactory.hxx
  SALOMEDS_BasicAttribute_i.hxx
  SALOMEDS_Callback_i.hxx
  SALOMEDS_ChildIterator.hxx
  SALOMEDS_ChildIterator_i.hxx
  SALOMEDS_ClientAttributes.hxx
  SALOMEDS_Driver_i.hxx
  SALOMEDS_GenericAttribute.hxx
  SALOMEDS_GenericAttribute_i.hxx
  SALOMEDS.hxx
  SALOMEDS_SComponent.hxx
  SALOMEDS_SComponent_i.hxx
  SALOMEDS_SComponentIterator.hxx
  SALOMEDS_SComponentIterator_i.hxx
  SALOMEDS_SObject.hxx
  SALOMEDS_SObject_i.hxx
  SALOMEDS_StudyBuilder.hxx
  SALOMEDS_StudyBuilder_i.hxx
  SALOMEDS_Study.hxx
  SALOMEDS_Study_i.hxx
  SALOMEDS_StudyManager.hxx
  SALOMEDS_StudyManager_i.hxx
  SALOMEDS_UseCaseBuilder.hxx
  SALOMEDS_UseCaseBuilder_i.hxx
  SALOMEDS_UseCaseIterator.hxx
  SALOMEDS_UseCaseIterator_i.hxx
  SALOMEDS_AttributeParameter.hxx
  SALOMEDS_AttributeParameter_i.hxx
  SALOMEDS_AttributeString.hxx
  SALOMEDS_AttributeString_i.hxx
  SALOMEDS_TMPFile_i.hxx
)

IF(WINDOWS)
  SET(COMMON_FLAGS "${COMMON_FLAGS} -DNOGDI")
ENDIF(WINDOWS)

ADD_LIBRARY(SalomeDS SHARED ${SalomeDS_SOURCES})
SET_TARGET_PROPERTIES(SalomeDS PROPERTIES COMPILE_FLAGS "${COMMON_FLAGS}")
TARGET_LINK_LIBRARIES(SalomeDS ${COMMON_LIBS})
INSTALL(TARGETS SalomeDS DESTINATION ${KERNEL_salomelib_LIBS})

ADD_EXECUTABLE(SALOMEDS_Server SALOMEDS_Server.cxx)
SET_TARGET_PROPERTIES(SALOMEDS_Server PROPERTIES COMPILE_FLAGS "${COMMON_FLAGS}")
TARGET_LINK_LIBRARIES(SALOMEDS_Server SalomeDS SALOMEBasics ${COMMON_LIBS} ${OMNIORB_LIBS})

ADD_EXECUTABLE(SALOMEDS_Client SALOMEDS_Client.cxx)
SET_TARGET_PROPERTIES(SALOMEDS_Client PROPERTIES COMPILE_FLAGS "${COMMON_FLAGS}")
TARGET_LINK_LIBRARIES(SALOMEDS_Client SalomeDS SALOMEBasics ${COMMON_LIBS} ${OMNIORB_LIBS})

INSTALL(TARGETS SALOMEDS_Server SALOMEDS_Client DESTINATION ${KERNEL_salomebin_BINS})

# Executable scripts to be installed
SALOME_INSTALL_SCRIPTS(SALOME_DriverPy.py ${KERNEL_salomescript_PYTHON})

SET(COMMON_HEADERS_HXX 
  SALOMEDS_StudyManager_i.hxx
  SALOMEDS_Driver_i.hxx
  SALOMEDS_StudyManager.hxx
  SALOMEDS_Study_i.hxx
  SALOMEDS_Study.hxx
  SALOMEDS_SObject_i.hxx
  SALOMEDS_SObject.hxx
  SALOMEDS_SComponent_i.hxx
  SALOMEDS_SComponent.hxx
  SALOMEDS_GenericAttribute_i.hxx
  SALOMEDS_GenericAttribute.hxx
  SALOMEDS_IParameters.hxx
  SALOMEDS_Defines.hxx
)
INSTALL(FILES ${COMMON_HEADERS_HXX} DESTINATION ${KERNEL_salomeinclude_HEADERS})
