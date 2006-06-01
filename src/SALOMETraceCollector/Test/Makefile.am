####################################### library
#  SALOMELocalTrace : log on local machine
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
#  Author : Guillaume Boulant (CSSI)
#  Module : KERNEL
#  $Header$


include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Files to be installed
# ===============================================================
#

# header files  
salomeinclude_HEADERS= SALOMETraceCollectorTest.hxx

# Scripts to be installed
dist_salomescript_SCRIPTS = TestSALOMETraceCollector.py


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
	-I$(srcdir)/$(RPATH)/Basics -I$(srcdir)/$(RPATH)/Basics/Test \
	-I$(srcdir)/$(RPATH)/SALOMELocalTrace -I$(srcdir)/$(RPATH)/SALOMELocalTrace/Test\
	-I$(srcdir)/$(RPATH)/SALOMETraceCollector \
	-I$(top_builddir)/salome_adm/unix

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	@CPPUNIT_LIBS@ \
	$(RPATH)/SALOMELocalTrace/Test/libSALOMELocalTraceTest.la $(RPATH)/SALOMELocalTrace/libSALOMELocalTrace.la \
	$(RPATH)/Basics/libSALOMEBasics.la


#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSALOMETraceCollectorTest.la 
libSALOMETraceCollectorTest_la_SOURCES = SALOMETraceCollectorTest.cxx
libSALOMETraceCollectorTest_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libSALOMETraceCollectorTest_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libSALOMETraceCollectorTest_la_LIBADD    = $(COMMON_LIBS)

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = TestSALOMETraceCollector
TestSALOMETraceCollector_SOURCES  = TestSALOMETraceCollector.cxx
TestSALOMETraceCollector_CPPFLAGS = $(COMMON_CPPFLAGS)
TestSALOMETraceCollector_LDADD    = libSALOMETraceCollectorTest.la $(COMMON_LIBS)
