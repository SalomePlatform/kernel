# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

####################################### library
#  SALOMELocalTrace : log on local machine
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
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
salomeinclude_HEADERS= UtilsTest.hxx

# Scripts to be installed
dist_salomescript_PYTHON = TestUtils.py


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
	-I$(srcdir)/$(RPATH)/SALOMELocalTrace -I$(srcdir)/$(RPATH)/SALOMELocalTrace/Test \
	-I$(srcdir)/$(RPATH)/SALOMETraceCollector -I$(srcdir)/$(RPATH)/SALOMETraceCollector/Test \
	-I$(srcdir)/$(RPATH)/Utils -I$(srcdir)/$(RPATH)/Utils/Test

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	@CPPUNIT_LIBS@ \
	$(RPATH)/Utils/libOpUtil.la \
	$(RPATH)/SALOMELocalTrace/Test/libSALOMELocalTraceTest.la $(RPATH)/SALOMELocalTrace/libSALOMELocalTrace.la \
	$(RPATH)/SALOMETraceCollector/Test/libSALOMETraceCollectorTest.la \
	$(RPATH)/Basics/libSALOMEBasics.la
#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libUtilsTest.la 
libUtilsTest_la_SOURCES = UtilsTest.cxx
libUtilsTest_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libUtilsTest_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libUtilsTest_la_LIBADD    = $(COMMON_LIBS)

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = TestUtils
TestUtils_SOURCES  = TestUtils.cxx
TestUtils_CPPFLAGS = $(COMMON_CPPFLAGS)
TestUtils_LDADD    = libUtilsTest.la $(COMMON_LIBS) $(CORBA_LIBS)
