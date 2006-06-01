#
#  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
#
#
#  File   : Makefile.am
#  Author : Paul RASCLE
#  Module : KERNEL
#  $Header$

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Files to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS= SALOMEDSTest.hxx

# Scripts to be installed
dist_salomescript_SCRIPTS = TestSALOMEDS.py

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
	-I$(srcdir)/$(RPATH)/SALOMEDSClient \
	-I$(srcdir)/$(RPATH)/SALOMEDSImpl         -I$(srcdir)/$(RPATH)/SALOMEDSImpl/Test \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CAS_CPPFLAGS@ @CAS_CXXFLAGS@ \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

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
libSALOMEDSTest_la_SOURCES = SALOMEDSTest.cxx
libSALOMEDSTest_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libSALOMEDSTest_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libSALOMEDSTest_la_LIBADD    = $(COMMON_LIBS)

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = TestSALOMEDS
TestSALOMEDS_SOURCES  = TestSALOMEDS.cxx
TestSALOMEDS_CPPFLAGS = $(COMMON_CPPFLAGS)
TestSALOMEDS_LDADD    = \
	libSALOMEDSTest.la ../libSalomeDS.la \
	$(RPATH)/NamingService/Test/libNamingServiceTest.la \
	$(COMMON_LIBS)

