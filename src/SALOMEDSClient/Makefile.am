#  SALOMEDSClient: client interface of SALOMEDS
# 
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : KERNEL

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

salomeinclude_HEADERS=\
	        SALOMEDSClient_definitions.hxx \
                SALOMEDSClient_AttributeComment.hxx \
	        SALOMEDSClient_AttributeDrawable.hxx \
                SALOMEDSClient_AttributeExpandable.hxx \
                SALOMEDSClient_AttributeExternalFileDef.hxx \
                SALOMEDSClient_AttributeFileType.hxx \
                SALOMEDSClient_AttributeFlags.hxx \
                SALOMEDSClient_AttributeGraphic.hxx \
                SALOMEDSClient_AttributeIOR.hxx \
                SALOMEDSClient_AttributeInteger.hxx \
                SALOMEDSClient_AttributeLocalID.hxx \
                SALOMEDSClient_AttributeName.hxx \
                SALOMEDSClient_AttributeOpened.hxx \
                SALOMEDSClient_AttributePersistentRef.hxx \
                SALOMEDSClient_AttributePixMap.hxx \
                SALOMEDSClient_AttributePythonObject.hxx \
                SALOMEDSClient_AttributeReal.hxx \
                SALOMEDSClient_AttributeSelectable.hxx \
                SALOMEDSClient_AttributeSequenceOfInteger.hxx \
                SALOMEDSClient_AttributeSequenceOfReal.hxx \
                SALOMEDSClient_AttributeStudyProperties.hxx \
                SALOMEDSClient_AttributeTableOfInteger.hxx \
                SALOMEDSClient_AttributeTableOfReal.hxx \
                SALOMEDSClient_AttributeTableOfString.hxx \
                SALOMEDSClient_AttributeTarget.hxx \
                SALOMEDSClient_AttributeTextColor.hxx \
                SALOMEDSClient_AttributeTextHighlightColor.hxx \
                SALOMEDSClient_AttributeTreeNode.hxx \
                SALOMEDSClient_AttributeUserID.hxx \
		SALOMEDSClient_AttributeParameter.hxx \
                SALOMEDSClient_ChildIterator.hxx \
                SALOMEDSClient_GenericAttribute.hxx \
                SALOMEDSClient_SComponent.hxx \
                SALOMEDSClient_SComponentIterator.hxx \
                SALOMEDSClient_SObject.hxx \
                SALOMEDSClient_Study.hxx \
                SALOMEDSClient_StudyBuilder.hxx \
                SALOMEDSClient_StudyManager.hxx \
                SALOMEDSClient_UseCaseBuilder.hxx \
                SALOMEDSClient_UseCaseIterator.hxx \
		SALOMEDSClient_ClientFactory.hxx \
		SALOMEDSClient.hxx

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS= \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@



# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeDSClient.la
libSalomeDSClient_la_SOURCES = 	\
	SALOMEDSClient_ClientFactory.cxx


libSalomeDSClient_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libSalomeDSClient_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libSalomeDSClient_la_LIBADD   = $(COMMON_LIBS)
