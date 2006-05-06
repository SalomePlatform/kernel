#
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : KERNEL

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Files to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS= \
	SALOMEDS_StudyManager_i.hxx \
	SALOMEDS_Driver_i.hxx \
	SALOMEDS_StudyManager.hxx \
	SALOMEDS_Study_i.hxx \
	SALOMEDS_Study.hxx \
	SALOMEDS_SObject_i.hxx \
	SALOMEDS_SObject.hxx \
	SALOMEDS_SComponent_i.hxx \
	SALOMEDS_SComponent.hxx \
	SALOMEDS_GenericAttribute_i.hxx \
	SALOMEDS_GenericAttribute.hxx \
	SALOMEDS_IParameters.hxx

# Scripts to be installed
dist_salomescript_DATA = SALOME_DriverPy.py

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	@CAS_CPPFLAGS@ @CAS_CXXFLAGS@ \
	@BOOST_CPPFLAGS@ \
	-I$(srcdir)/../HDFPersist \
	@HDF5_INCLUDES@ \
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../Utils \
	-I$(srcdir)/../SALOMEDSImpl \
	-I$(srcdir)/../NamingService \
	-I$(srcdir)/../GenericObj \
	-I$(srcdir)/../SALOMEDSClient \
	-I$(srcdir)/../LifeCycleCORBA \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

# This flag is used to resolve the dependencies of OCC libraries.
LDXMUFLAGS = -L/usr/X11R6/lib@LIB_LOCATION_SUFFIX@ -lXmu

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	../TOOLSDS/libTOOLSDS.la \
	../NamingService/libSalomeNS.la \
	../Utils/libOpUtil.la \
	../SALOMELocalTrace/libSALOMELocalTrace.la \
	../Basics/libSALOMEBasics.la \
	../HDFPersist/libSalomeHDFPersist.la \
	../SALOMEDSImpl/libSalomeDSImpl.la \
	../GenericObj/libSalomeGenericObj.la \
	../LifeCycleCORBA/libSalomeLifeCycleCORBA.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la\
	@CAS_KERNEL@ \
	@HDF5_LIBS@ \
	$(LDXMUFLAGS)


#LDFLAGS+=  -lSalomeGenericObj -lSalomeLifeCycleCORBA

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeDS.la
libSalomeDS_la_SOURCES = 	\
	SALOMEDS.cxx \
	SALOMEDS_Driver_i.cxx \
	SALOMEDS_StudyManager_i.cxx \
	SALOMEDS_UseCaseBuilder_i.cxx \
	SALOMEDS_UseCaseIterator_i.cxx \
	SALOMEDS_ChildIterator_i.cxx \
	SALOMEDS_SComponentIterator_i.cxx \
	SALOMEDS_Study_i.cxx \
	SALOMEDS_StudyBuilder_i.cxx \
	SALOMEDS_SObject_i.cxx \
	SALOMEDS_SComponent_i.cxx \
	SALOMEDS_GenericAttribute_i.cxx \
	SALOMEDS_AttributeComment_i.cxx \
	SALOMEDS_AttributeExternalFileDef_i.cxx \
	SALOMEDS_AttributeFileType_i.cxx \
	SALOMEDS_AttributeIOR_i.cxx \
	SALOMEDS_AttributeInteger_i.cxx \
	SALOMEDS_AttributeName_i.cxx \
	SALOMEDS_AttributePersistentRef_i.cxx \
	SALOMEDS_AttributeReal_i.cxx \
	SALOMEDS_AttributeSequenceOfReal_i.cxx \
	SALOMEDS_AttributeSequenceOfInteger_i.cxx \
	SALOMEDS_AttributeDrawable_i.cxx \
	SALOMEDS_AttributeSelectable_i.cxx \
	SALOMEDS_AttributeOpened_i.cxx \
	SALOMEDS_AttributeFlags_i.cxx \
	SALOMEDS_AttributeGraphic_i.cxx \
	SALOMEDS_AttributeExpandable_i.cxx \
	SALOMEDS_AttributeTextColor_i.cxx  \
	SALOMEDS_AttributeTextHighlightColor_i.cxx  \
	SALOMEDS_AttributePixMap_i.cxx  \
	SALOMEDS_AttributeTreeNode_i.cxx \
	SALOMEDS_AttributeLocalID_i.cxx \
	SALOMEDS_AttributeUserID_i.cxx \
	SALOMEDS_AttributeTarget_i.cxx \
	SALOMEDS_AttributeTableOfInteger_i.cxx \
	SALOMEDS_AttributeTableOfReal_i.cxx \
	SALOMEDS_AttributeTableOfString_i.cxx \
	SALOMEDS_AttributeStudyProperties_i.cxx \
	SALOMEDS_AttributePythonObject_i.cxx \
	SALOMEDS_AttributeParameter_i.cxx \
	SALOMEDS_SObject.cxx \
	SALOMEDS_SComponent.cxx \
	SALOMEDS_GenericAttribute.cxx \
	SALOMEDS_ChildIterator.cxx \
	SALOMEDS_SComponentIterator.cxx \
	SALOMEDS_UseCaseIterator.cxx \
	SALOMEDS_UseCaseBuilder.cxx \
	SALOMEDS_StudyBuilder.cxx \
	SALOMEDS_Study.cxx \
	SALOMEDS_StudyManager.cxx \
	SALOMEDS_AttributeStudyProperties.cxx \
	SALOMEDS_AttributeComment.cxx \
	SALOMEDS_AttributeDrawable.cxx \
	SALOMEDS_AttributeExpandable.cxx \
	SALOMEDS_AttributeExternalFileDef.cxx \
	SALOMEDS_AttributeFileType.cxx \
	SALOMEDS_AttributeFlags.cxx \
	SALOMEDS_AttributeGraphic.cxx \
	SALOMEDS_AttributeIOR.cxx \
	SALOMEDS_AttributeInteger.cxx \
	SALOMEDS_AttributeLocalID.cxx \
	SALOMEDS_AttributeName.cxx \
	SALOMEDS_AttributeOpened.cxx \
	SALOMEDS_AttributePythonObject.cxx \
	SALOMEDS_AttributeReal.cxx \
	SALOMEDS_AttributeSelectable.cxx \
	SALOMEDS_AttributeSequenceOfInteger.cxx \
	SALOMEDS_AttributePersistentRef.cxx \
	SALOMEDS_AttributePixMap.cxx \
	SALOMEDS_AttributeSequenceOfReal.cxx \
	SALOMEDS_AttributeTableOfInteger.cxx \
	SALOMEDS_AttributeTableOfReal.cxx \
	SALOMEDS_AttributeTableOfString.cxx \
	SALOMEDS_AttributeTarget.cxx \
	SALOMEDS_AttributeTextColor.cxx \
	SALOMEDS_AttributeTextHighlightColor.cxx \
	SALOMEDS_AttributeTreeNode.cxx \
	SALOMEDS_AttributeUserID.cxx \
	SALOMEDS_TMPFile_i.cxx \
	SALOMEDS_AttributeParameter.cxx \
	SALOMEDS_IParameters.cxx \
	\
	Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString.hxx \
	Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel.hxx \
	Handle_SALOMEDS_DrawableAttribute.hxx \
	Handle_SALOMEDS_ExpandableAttribute.hxx \
	Handle_SALOMEDS_ExternalFileDef.hxx \
	Handle_SALOMEDS_FileType.hxx \
	Handle_SALOMEDS_IORAttribute.hxx \
	Handle_SALOMEDS_LocalIDAttribute.hxx \
	Handle_SALOMEDS_OCAFApplication.hxx \
	Handle_SALOMEDS_OpenedAttribute.hxx \
	Handle_SALOMEDS_PersRefAttribute.hxx \
	Handle_SALOMEDS_PixMapAttribute.hxx \
	Handle_SALOMEDS_PythonObjectAttribute.hxx \
	Handle_SALOMEDS_SelectableAttribute.hxx \
	Handle_SALOMEDS_SequenceOfIntegerAttribute.hxx \
	Handle_SALOMEDS_SequenceOfRealAttribute.hxx \
	Handle_SALOMEDS_StudyPropertiesAttribute.hxx \
	Handle_SALOMEDS_TableOfIntegerAttribute.hxx \
	Handle_SALOMEDS_TableOfRealAttribute.hxx \
	Handle_SALOMEDS_TableOfStringAttribute.hxx \
	Handle_SALOMEDS_TargetAttribute.hxx \
	Handle_SALOMEDS_TextColorAttribute.hxx \
	Handle_SALOMEDS_TextHighlightColorAttribute.hxx \
	SALOMEDS_AttLong_i.hxx \
	SALOMEDS_AttReal_i.hxx \
	SALOMEDS_AttributeComment.hxx \
	SALOMEDS_AttributeComment_i.hxx \
	SALOMEDS_AttributeDrawable.hxx \
	SALOMEDS_AttributeDrawable_i.hxx \
	SALOMEDS_AttributeExpandable.hxx \
	SALOMEDS_AttributeExpandable_i.hxx \
	SALOMEDS_AttributeExternalFileDef.hxx \
	SALOMEDS_AttributeExternalFileDef_i.hxx \
	SALOMEDS_AttributeFileType.hxx \
	SALOMEDS_AttributeFileType_i.hxx \
	SALOMEDS_AttributeFlags.hxx \
	SALOMEDS_AttributeFlags_i.hxx \
	SALOMEDS_AttributeGraphic.hxx \
	SALOMEDS_AttributeGraphic_i.hxx \
	SALOMEDS_AttributeInteger.hxx \
	SALOMEDS_AttributeInteger_i.hxx \
	SALOMEDS_AttributeIOR.hxx \
	SALOMEDS_AttributeIOR_i.hxx \
	SALOMEDS_AttributeLocalID.hxx \
	SALOMEDS_AttributeLocalID_i.hxx \
	SALOMEDS_AttributeName.hxx \
	SALOMEDS_AttributeName_i.hxx \
	SALOMEDS_AttributeOpened.hxx \
	SALOMEDS_AttributeOpened_i.hxx \
	SALOMEDS_AttributePersistentRef.hxx \
	SALOMEDS_AttributePersistentRef_i.hxx \
	SALOMEDS_AttributePixMap.hxx \
	SALOMEDS_AttributePixMap_i.hxx \
	SALOMEDS_AttributePythonObject.hxx \
	SALOMEDS_AttributePythonObject_i.hxx \
	SALOMEDS_AttributeReal.hxx \
	SALOMEDS_AttributeReal_i.hxx \
	SALOMEDS_AttributeSelectable.hxx \
	SALOMEDS_AttributeSelectable_i.hxx \
	SALOMEDS_AttributeSequenceOfInteger.hxx \
	SALOMEDS_AttributeSequenceOfInteger_i.hxx \
	SALOMEDS_AttributeSequenceOfReal.hxx \
	SALOMEDS_AttributeSequenceOfReal_i.hxx \
	SALOMEDS_Attributes.hxx \
	SALOMEDS_AttributeStudyProperties.hxx \
	SALOMEDS_AttributeStudyProperties_i.hxx \
	SALOMEDS_AttributeTableOfInteger.hxx \
	SALOMEDS_AttributeTableOfInteger_i.hxx \
	SALOMEDS_AttributeTableOfReal.hxx \
	SALOMEDS_AttributeTableOfReal_i.hxx \
	SALOMEDS_AttributeTableOfString.hxx \
	SALOMEDS_AttributeTableOfString_i.hxx \
	SALOMEDS_AttributeTarget.hxx \
	SALOMEDS_AttributeTarget_i.hxx \
	SALOMEDS_AttributeTextColor.hxx \
	SALOMEDS_AttributeTextColor_i.hxx \
	SALOMEDS_AttributeTextHighlightColor.hxx \
	SALOMEDS_AttributeTextHighlightColor_i.hxx \
	SALOMEDS_AttributeTreeNode.hxx \
	SALOMEDS_AttributeTreeNode_i.hxx \
	SALOMEDS_AttributeUserID.hxx \
	SALOMEDS_AttributeUserID_i.hxx \
	SALOMEDS_BasicAttributeFactory.hxx \
	SALOMEDS_BasicAttribute_i.hxx \
	SALOMEDS_Callback_i.hxx \
	SALOMEDS_ChildIterator.hxx \
	SALOMEDS_ChildIterator_i.hxx \
	SALOMEDS_ClientAttributes.hxx \
	SALOMEDS_DataMapIteratorOfDataMapOfIntegerString.hxx \
	SALOMEDS_DataMapIteratorOfDataMapStringLabel.hxx \
	SALOMEDS_DataMapNodeOfDataMapOfIntegerString.hxx \
	SALOMEDS_DataMapNodeOfDataMapStringLabel.hxx \
	SALOMEDS_DataMapOfIntegerString.hxx \
	SALOMEDS_DataMapStringLabel.hxx \
	SALOMEDS_DrawableAttribute.hxx \
	SALOMEDS_Driver_i.hxx \
	SALOMEDS_ExpandableAttribute.hxx \
	SALOMEDS_ExternalFileDef.hxx \
	SALOMEDS_FileType.hxx \
	SALOMEDS_FlagsAttribute.hxx \
	SALOMEDS_GenericAttribute.hxx \
	SALOMEDS_GenericAttribute_i.hxx \
	SALOMEDS_GraphicAttribute.hxx \
	SALOMEDS.hxx \
	SALOMEDS_IORAttribute.hxx \
	SALOMEDS_LocalIDAttribute.hxx \
	SALOMEDS_OCAFApplication.hxx \
	SALOMEDS_OpenedAttribute.hxx \
	SALOMEDS_PersRefAttribute.hxx \
	SALOMEDS_PixMapAttribute.hxx \
	SALOMEDS_PythonObjectAttribute.hxx \
	SALOMEDS_SAttribute_i.hxx \
	SALOMEDS_SComponent.hxx \
	SALOMEDS_SComponent_i.hxx \
	SALOMEDS_SComponentIterator.hxx \
	SALOMEDS_SComponentIterator_i.hxx \
	SALOMEDS_SelectableAttribute.hxx \
	SALOMEDS_SequenceOfIntegerAttribute.hxx \
	SALOMEDS_SequenceOfRealAttribute.hxx \
	SALOMEDS_SObject.hxx \
	SALOMEDS_SObject_i.hxx \
	SALOMEDS_StudyBuilder.hxx \
	SALOMEDS_StudyBuilder_i.hxx \
	SALOMEDS_Study.hxx \
	SALOMEDS_Study_i.hxx \
	SALOMEDS_StudyManager.hxx \
	SALOMEDS_StudyManager_i.hxx \
	SALOMEDS_StudyPropertiesAttribute.hxx \
	SALOMEDS_TableOfIntegerAttribute.hxx \
	SALOMEDS_TableOfRealAttribute.hxx \
	SALOMEDS_TableOfStringAttribute.hxx \
	SALOMEDS_TargetAttribute.hxx \
	SALOMEDS_TextColorAttribute.hxx \
	SALOMEDS_TextHighlightColorAttribute.hxx \
	SALOMEDS_UseCaseBuilder.hxx \
	SALOMEDS_UseCaseBuilder_i.hxx \
	SALOMEDS_UseCaseIterator.hxx \
	SALOMEDS_UseCaseIterator_i.hxx \
	SALOMEDS_AttributeParameter.hxx \
	SALOMEDS_AttributeParameter_i.hxx \
	SALOMEDS_TMPFile_i.hxx

libSalomeDS_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libSalomeDS_la_LDFLAGS  = -Wl,-E -no-undefined -version-info=0:0:0 @LDEXPDYNFLAGS@
libSalomeDS_la_LIBADD    = $(COMMON_LIBS)

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = SALOMEDS_Server SALOMEDS_Client

SALOMEDS_Server_SOURCES  = SALOMEDS_Server.cxx
SALOMEDS_Server_CPPFLAGS = $(COMMON_CPPFLAGS)
SALOMEDS_Server_LDADD    = \
	libSalomeDS.la $(COMMON_LIBS) \
	../ResourcesManager/libSalomeResourcesManager.la \
	@CAS_OCAF@ \
	@CORBA_LIBS@

SALOMEDS_Client_SOURCES  = SALOMEDS_Client.cxx
SALOMEDS_Client_CPPFLAGS = $(COMMON_CPPFLAGS)
SALOMEDS_Client_LDADD    = \
	libSalomeDS.la $(COMMON_LIBS) \
	../ResourcesManager/libSalomeResourcesManager.la \
	-lTKLCAF -lTKMath \
	@CORBA_LIBS@

