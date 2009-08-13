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
#  File   : Makefile.am
#  Author : Paul RASCLE
#  Module : KERNEL
#  $Header$
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Files to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS= SALOMEDSTest.hxx

# Scripts to be installed
dist_salomescript_PYTHON = TestSALOMEDS.py

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This directory defines the subdirectory src in the top source directory.
RPATH=../..

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	@CPPUNIT_INCLUDES@ \
	-I$(srcdir)/$(RPATH)/Basics               -I$(srcdir)/$(RPATH)/Basics/Test \
	-I$(srcdir)/$(RPATH)/SALOMELocalTrace     -I$(srcdir)/$(RPATH)/SALOMELocalTrace/Test \
	-I$(srcdir)/$(RPATH)/SALOMETraceCollector -I$(srcdir)/$(RPATH)/SALOMETraceCollector/Test \
	-I$(srcdir)/$(RPATH)/NamingService        -I$(srcdir)/$(RPATH)/NamingService/Test \
	-I$(srcdir)/$(RPATH)/Utils                -I$(srcdir)/$(RPATH)/Utils/Test \
	-I$(srcdir)/$(RPATH)/ResourcesManager \
	-I$(srcdir)/$(RPATH)/LifeCycleCORBA \
	-I$(srcdir)/$(RPATH)/SALOMEDS \
	-I$(srcdir)/$(RPATH)/SALOMEDSClient \
	-I$(srcdir)/$(RPATH)/DF \
	-I$(srcdir)/$(RPATH)/SALOMEDSImpl  -I$(srcdir)/$(RPATH)/SALOMEDSImpl/Test \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@ @BOOST_CPPFLAGS@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	@CPPUNIT_LIBS@ \
	$(RPATH)/Basics/libSALOMEBasics.la \
	$(RPATH)/ResourcesManager/libSalomeResourcesManager.la \
	$(RPATH)/Container/libSalomeContainer.la \
	$(RPATH)/NamingService/libSalomeNS.la \
	$(RPATH)/Registry/libRegistry.la \
	$(RPATH)/Notification/libSalomeNotification.la \
	$(RPATH)/Utils/Test/libUtilsTest.la \
	$(RPATH)/Utils/libOpUtil.la \
	$(RPATH)/SALOMELocalTrace/Test/libSALOMELocalTraceTest.la \
	$(RPATH)/SALOMELocalTrace/libSALOMELocalTrace.la \
	$(RPATH)/SALOMETraceCollector/Test/libSALOMETraceCollectorTest.la \
	$(RPATH)/SALOMEDSImpl/Test/libSALOMEDSImplTest.la \
	$(RPATH)/DF/libDF.la \
	$(RPATH)/SALOMEDSImpl/libSalomeDSImpl.la \
	$(RPATH)/SALOMEDSClient/libSalomeDSClient.la \
	$(RPATH)/SALOMEDS/libSalomeDS.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSALOMEDSTest.la 
dist_libSALOMEDSTest_la_SOURCES = SALOMEDSTest.cxx
libSALOMEDSTest_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libSALOMEDSTest_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libSALOMEDSTest_la_LIBADD    = $(COMMON_LIBS)

EXTRA_DIST = \
	SALOMEDSTest_AttributeComment.cxx \
	SALOMEDSTest_AttributeDrawable.cxx \
	SALOMEDSTest_AttributeExpandable.cxx \
	SALOMEDSTest_AttributeExternalFileDef.cxx \
	SALOMEDSTest_AttributeFileType.cxx \
	SALOMEDSTest_AttributeFlags.cxx \
	SALOMEDSTest_AttributeGraphic.cxx \
	SALOMEDSTest_AttributeInteger.cxx \
	SALOMEDSTest_AttributeIOR.cxx \
	SALOMEDSTest_AttributeLocalID.cxx \
	SALOMEDSTest_AttributeName.cxx \
	SALOMEDSTest_AttributeOpened.cxx \
	SALOMEDSTest_AttributeParameter.cxx \
	SALOMEDSTest_AttributePersistentRef.cxx \
	SALOMEDSTest_AttributePixMap.cxx \
	SALOMEDSTest_AttributePythonObject.cxx \
	SALOMEDSTest_AttributeReal.cxx \
	SALOMEDSTest_AttributeSelectable.cxx \
	SALOMEDSTest_AttributeSequenceOfInteger.cxx \
	SALOMEDSTest_AttributeSequenceOfReal.cxx \
	SALOMEDSTest_AttributeStudyProperties.cxx \
	SALOMEDSTest_AttributeTableOfInteger.cxx \
	SALOMEDSTest_AttributeTableOfReal.cxx \
	SALOMEDSTest_AttributeTableOfString.cxx \
	SALOMEDSTest_AttributeTarget.cxx \
	SALOMEDSTest_AttributeTextColor.cxx \
	SALOMEDSTest_AttributeTextHighlightColor.cxx\
	SALOMEDSTest_AttributeTreeNode.cxx \
	SALOMEDSTest_AttributeUserID.cxx \
	SALOMEDSTest_ChildIterator.cxx \
	SALOMEDSTest_SComponent.cxx \
	SALOMEDSTest_SComponentIterator.cxx \
	SALOMEDSTest_SObject.cxx \
	SALOMEDSTest_StudyBuilder.cxx \
	SALOMEDSTest_Study.cxx \
	SALOMEDSTest_StudyManager.cxx \
	SALOMEDSTest_UseCase.cxx

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = TestSALOMEDS
dist_TestSALOMEDS_SOURCES  = TestSALOMEDS.cxx
TestSALOMEDS_CPPFLAGS = $(COMMON_CPPFLAGS)
TestSALOMEDS_LDADD    = libSALOMEDSTest.la \
			$(RPATH)/Basics/libSALOMEBasics.la \
                        $(CORBA_LIBS)
