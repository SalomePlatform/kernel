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

#  SALOMELocalTrace : log on local machine
#  File   : Makefile.am
#  Author : Guillaume BOULANT (CSSI)
#  Module : SALOME
#  $Header$
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Files to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS= NamingServiceTest.hxx

# Scripts to be installed
dist_salomescript_PYTHON = TestNamingService.py


#
# ===============================================================
# Local definitions
# ===============================================================
#

# This directory defines the subdirectory src in the top source directory.
RPATH=../..

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	@CPPUNIT_INCLUDES@ -I$(srcdir)/$(RPATH)/NamingService \
	-I$(srcdir)/$(RPATH)/Basics \
	-I$(srcdir)/$(RPATH)/SALOMELocalTrace \
	-I$(srcdir)/$(RPATH)/Utils \
    -I$(srcdir)/$(RPATH)/LifeCycleCORBA \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	@CPPUNIT_LIBS@ \
	../libSalomeNS.la \
	../../LifeCycleCORBA/libSalomeLifeCycleCORBA.la \
	@CORBA_LIBS@

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libNamingServiceTest.la
libNamingServiceTest_la_SOURCES = NamingServiceTest.cxx
libNamingServiceTest_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libNamingServiceTest_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libNamingServiceTest_la_LIBADD   = $(COMMON_LIBS)


#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = TestNamingService
TestNamingService_SOURCES  = TestNamingService.cxx
TestNamingService_CPPFLAGS = \
	$(COMMON_CPPFLAGS) \
	-I$(srcdir)/$(RPATH)/SALOMELocalTrace/Test \
	-I$(srcdir)/$(RPATH)/SALOMETraceCollector/Test \
	-I$(srcdir)/$(RPATH)/Utils/Test \
	-I$(srcdir)/$(RPATH)/Basics/Test


TestNamingService_LDADD    = \
	libNamingServiceTest.la ../libSalomeNS.la \
	$(RPATH)/SALOMELocalTrace/Test/libSALOMELocalTraceTest.la $(RPATH)/SALOMELocalTrace/libSALOMELocalTrace.la \
	$(RPATH)/SALOMETraceCollector/Test/libSALOMETraceCollectorTest.la \
	$(RPATH)/Utils/Test/libUtilsTest.la $(RPATH)/Utils/libOpUtil.la \
	$(RPATH)/Basics/libSALOMEBasics.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la $(COMMON_LIBS)
