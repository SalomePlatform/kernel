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
#  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
#
#
#
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : KERNEL
#  $Header$


include $(top_srcdir)/salome_adm/unix/make_common_starter.am

# This directory defines the subdirectory src in the top source directory.
SRCROOT=$(srcdir)/../..

#
# ===============================================================
# Files to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = SALOMELocalTraceTest.hxx

# Executable scripts to be installed
dist_salomescript_SCRIPTS = TestSALOMELocalTrace.py

#
# ===============================================================
# Libraries targets
# ===============================================================
#

lib_LTLIBRARIES = libSALOMELocalTraceTest.la
libSALOMELocalTraceTest_la_SOURCES  = SALOMELocalTraceTest.cxx
libSALOMELocalTraceTest_la_CPPFLAGS = \
	@CPPUNIT_INCLUDES@ \
	-I$(SRCROOT)/SALOMELocalTrace \
	-I$(SRCROOT)/Basics

libSALOMELocalTraceTest_la_LIBADD = \
	@CPPUNIT_LIBS@ \
	../../SALOMELocalTrace/libSALOMELocalTrace.la

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = TestSALOMELocalTrace
TestSALOMELocalTrace_SOURCES  = TestSALOMELocalTrace.cxx
TestSALOMELocalTrace_CPPFLAGS = \
	@CPPUNIT_INCLUDES@ \
	-I$(SRCROOT)/Basics/Test

TestSALOMELocalTrace_LDADD = \
	@CPPUNIT_LIBS@ \
	libSALOMELocalTraceTest.la \
	../../SALOMELocalTrace/libSALOMELocalTrace.la \
	../../Basics/libSALOMEBasics.la
