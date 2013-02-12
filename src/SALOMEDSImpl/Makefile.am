# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : KERNEL
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Files to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS= \
	SALOMEDSImpl_Attributes.hxx \
	SALOMEDSImpl_SObject.hxx \
	SALOMEDSImpl_SComponent.hxx \
	SALOMEDSImpl_Driver.hxx \
	SALOMEDSImpl_Callback.hxx \
	SALOMEDSImpl_ChildIterator.hxx \
	SALOMEDSImpl_GenericAttribute.hxx \
	SALOMEDSImpl_AttributeIOR.hxx \
	SALOMEDSImpl_AttributeExternalFileDef.hxx \
	SALOMEDSImpl_AttributeFileType.hxx \
	SALOMEDSImpl_AttributePersistentRef.hxx \
	SALOMEDSImpl_AttributeSequenceOfReal.hxx \
	SALOMEDSImpl_AttributeSequenceOfInteger.hxx \
	SALOMEDSImpl_AttributeDrawable.hxx \
	SALOMEDSImpl_AttributeSelectable.hxx \
	SALOMEDSImpl_AttributeExpandable.hxx  \
	SALOMEDSImpl_AttributeOpened.hxx \
	SALOMEDSImpl_AttributeFlags.hxx \
	SALOMEDSImpl_AttributeGraphic.hxx \
	SALOMEDSImpl_AttributeTextColor.hxx \
	SALOMEDSImpl_AttributeTextHighlightColor.hxx \
	SALOMEDSImpl_AttributePixMap.hxx \
	SALOMEDSImpl_AttributeLocalID.hxx \
	SALOMEDSImpl_AttributeTarget.hxx \
	SALOMEDSImpl_AttributeTable.hxx \
	SALOMEDSImpl_AttributeTableOfInteger.hxx \
	SALOMEDSImpl_AttributeTableOfReal.hxx \
	SALOMEDSImpl_AttributeTableOfString.hxx \
	SALOMEDSImpl_AttributeStudyProperties.hxx \
	SALOMEDSImpl_AttributePythonObject.hxx \
	SALOMEDSImpl_AttributeReal.hxx \
	SALOMEDSImpl_AttributeInteger.hxx \
	SALOMEDSImpl_AttributeUserID.hxx \
	SALOMEDSImpl_AttributeTreeNode.hxx \
	SALOMEDSImpl_AttributeName.hxx \
	SALOMEDSImpl_AttributeComment.hxx \
	SALOMEDSImpl_AttributeReference.hxx \
	SALOMEDSImpl_AttributeParameter.hxx \
	SALOMEDSImpl_AttributeString.hxx \
	SALOMEDSImpl_UseCaseBuilder.hxx \
	SALOMEDSImpl_UseCaseIterator.hxx \
	SALOMEDSImpl_SComponentIterator.hxx \
	SALOMEDSImpl_StudyBuilder.hxx \
	SALOMEDSImpl_Study.hxx \
	SALOMEDSImpl_StudyManager.hxx \
	SALOMEDSImpl_ChildNodeIterator.hxx \
	SALOMEDSImpl_Defines.hxx \
	SALOMEDSImpl_IParameters.hxx \
	SALOMEDSImpl_TMPFile.hxx \
	SALOMEDSImpl_GenericVariable.hxx \
	SALOMEDSImpl_ScalarVariable.hxx

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../HDFPersist \
	-I$(srcdir)/../DF \
	-I$(top_builddir) \
	@HDF5_INCLUDES@ \
	@BOOST_CPPFLAGS@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	../Basics/libSALOMEBasics.la \
	../HDFPersist/libSalomeHDFPersist.la \
	../DF/libDF.la \
	@HDF5_LIBS@ 


#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeDSImpl.la
libSalomeDSImpl_la_SOURCES =\
	SALOMEDSImpl_Tool.cxx \
	SALOMEDSImpl_Callback.cxx \
	SALOMEDSImpl_StudyHandle.cxx \
	SALOMEDSImpl_GenericAttribute.cxx \
	SALOMEDSImpl_SObject.cxx \
	SALOMEDSImpl_SComponent.cxx \
	SALOMEDSImpl_ChildIterator.cxx \
	SALOMEDSImpl_AttributeIOR.cxx \
	SALOMEDSImpl_AttributeExternalFileDef.cxx \
	SALOMEDSImpl_AttributeFileType.cxx \
	SALOMEDSImpl_AttributePersistentRef.cxx \
	SALOMEDSImpl_AttributeSequenceOfReal.cxx \
	SALOMEDSImpl_AttributeSequenceOfInteger.cxx \
	SALOMEDSImpl_AttributeDrawable.cxx \
	SALOMEDSImpl_AttributeSelectable.cxx \
	SALOMEDSImpl_AttributeExpandable.cxx  \
	SALOMEDSImpl_AttributeOpened.cxx \
	SALOMEDSImpl_AttributeFlags.cxx \
	SALOMEDSImpl_AttributeGraphic.cxx \
	SALOMEDSImpl_AttributeTextColor.cxx \
	SALOMEDSImpl_AttributeTextHighlightColor.cxx \
	SALOMEDSImpl_AttributePixMap.cxx \
	SALOMEDSImpl_AttributeLocalID.cxx \
	SALOMEDSImpl_AttributeTarget.cxx \
	SALOMEDSImpl_AttributeTableOfInteger.cxx \
	SALOMEDSImpl_AttributeTableOfReal.cxx \
	SALOMEDSImpl_AttributeTableOfString.cxx \
	SALOMEDSImpl_AttributeStudyProperties.cxx \
	SALOMEDSImpl_AttributePythonObject.cxx \
	SALOMEDSImpl_AttributeReal.cxx \
	SALOMEDSImpl_AttributeInteger.cxx \
	SALOMEDSImpl_AttributeUserID.cxx \
	SALOMEDSImpl_AttributeTreeNode.cxx \
	SALOMEDSImpl_AttributeName.cxx \
	SALOMEDSImpl_AttributeComment.cxx \
	SALOMEDSImpl_AttributeReference.cxx \
	SALOMEDSImpl_AttributeParameter.cxx \
	SALOMEDSImpl_AttributeString.cxx \
	SALOMEDSImpl_ChildNodeIterator.cxx \
	SALOMEDSImpl_UseCaseBuilder.cxx \
	SALOMEDSImpl_UseCaseIterator.cxx \
	SALOMEDSImpl_SComponentIterator.cxx \
	SALOMEDSImpl_StudyBuilder.cxx \
	SALOMEDSImpl_Study.cxx \
	SALOMEDSImpl_StudyManager.cxx \
	SALOMEDSImpl_IParameters.cxx \
	SALOMEDSImpl_TMPFile.cxx \
	SALOMEDSImpl_GenericVariable.cxx \
	SALOMEDSImpl_ScalarVariable.cxx \
	\
	SALOMEDSImpl_AttributeComment.hxx \
	SALOMEDSImpl_AttributeDrawable.hxx \
	SALOMEDSImpl_AttributeExpandable.hxx \
	SALOMEDSImpl_AttributeExternalFileDef.hxx \
	SALOMEDSImpl_AttributeFileType.hxx \
	SALOMEDSImpl_AttributeFlags.hxx \
	SALOMEDSImpl_AttributeGraphic.hxx \
	SALOMEDSImpl_AttributeInteger.hxx \
	SALOMEDSImpl_AttributeIOR.hxx \
	SALOMEDSImpl_AttributeLocalID.hxx \
	SALOMEDSImpl_AttributeName.hxx \
	SALOMEDSImpl_AttributeOpened.hxx \
	SALOMEDSImpl_AttributePersistentRef.hxx \
	SALOMEDSImpl_AttributePixMap.hxx \
	SALOMEDSImpl_AttributePythonObject.hxx \
	SALOMEDSImpl_AttributeReal.hxx \
	SALOMEDSImpl_AttributeReference.hxx \
	SALOMEDSImpl_AttributeSelectable.hxx \
	SALOMEDSImpl_AttributeSequenceOfInteger.hxx \
	SALOMEDSImpl_AttributeSequenceOfReal.hxx \
	SALOMEDSImpl_Attributes.hxx \
	SALOMEDSImpl_AttributeStudyProperties.hxx \
	SALOMEDSImpl_AttributeTable.hxx \
	SALOMEDSImpl_AttributeTableOfInteger.hxx \
	SALOMEDSImpl_AttributeTableOfReal.hxx \
	SALOMEDSImpl_AttributeTableOfString.hxx \
	SALOMEDSImpl_AttributeTarget.hxx \
	SALOMEDSImpl_AttributeTextColor.hxx \
	SALOMEDSImpl_AttributeTextHighlightColor.hxx \
	SALOMEDSImpl_AttributeTreeNode.hxx \
	SALOMEDSImpl_AttributeUserID.hxx \
	SALOMEDSImpl_AttributeParameter.hxx \
	SALOMEDSImpl_AttributeString.hxx \
	SALOMEDSImpl_Callback.hxx \
	SALOMEDSImpl_ChildIterator.hxx \
	SALOMEDSImpl_ChildNodeIterator.hxx \
	SALOMEDSImpl_Defines.hxx \
	SALOMEDSImpl_Driver.hxx \
	SALOMEDSImpl_GenericAttribute.hxx \
	SALOMEDSImpl_SComponent.hxx \
	SALOMEDSImpl_SComponentIterator.hxx \
	SALOMEDSImpl_SObject.hxx \
	SALOMEDSImpl_StudyBuilder.hxx \
	SALOMEDSImpl_StudyHandle.hxx \
	SALOMEDSImpl_Study.hxx \
	SALOMEDSImpl_StudyManager.hxx \
	SALOMEDSImpl_Tool.hxx \
	SALOMEDSImpl_UseCaseBuilder.hxx \
	SALOMEDSImpl_UseCaseIterator.hxx \
	SALOMEDSImpl_GenericVariable.hxx \
	SALOMEDSImpl_ScalarVariable.hxx

libSalomeDSImpl_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libSalomeDSImpl_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libSalomeDSImpl_la_LIBADD   = $(COMMON_LIBS)

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS    = testDS
testDS_SOURCES  = testDS.cxx
testDS_CPPFLAGS = $(COMMON_CPPFLAGS)
testDS_LDADD    = libSalomeDSImpl.la ../DF/libDF.la $(COMMON_LIBS)
