#  SALOME TestContainer : test of container creation and its life cycle
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
#  File   : Makefile.in
#  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
#  Module : SALOME
#  $Header$


include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Files to be installed
# ===============================================================
#
# header files  
# Scripts to be installed
dist_salomescript_DATA = \
	SALOME_TestComponentPy.py \
	TestComponentPy.py \
	TestLogger.py

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../NamingService \
	-I$(srcdir)/../Utils \
	-I$(srcdir)/../Registry \
	-I$(srcdir)/../Notification \
	-I$(srcdir)/../ResourcesManager \
	-I$(srcdir)/../Container \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@


# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	../Container/libSalomeContainer.la \
	../Registry/libRegistry.la \
	../Notification/libSalomeNotification.la \
	../ResourcesManager/libSalomeResourcesManager.la \
	../NamingService/libSalomeNS.la \
	../Utils/libOpUtil.la \
	../SALOMELocalTrace/libSALOMELocalTrace.la \
	../Basics/libSALOMEBasics.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeTestComponentEngine.la
libSalomeTestComponentEngine_la_SOURCES  = \
	SALOME_TestComponent_i.cxx \
	SALOME_TestComponent_i.hxx

libSalomeTestComponentEngine_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libSalomeTestComponentEngine_la_LIBADD   = $(COMMON_LIBS)

#LIB_CLIENT_IDL = SALOME_Component.idl SALOME_TestComponent.idl SALOME_Exception.idl

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = TestContainer TestLogger
TestContainer_SOURCES  = TestContainer.cxx
TestContainer_CPPFLAGS = $(COMMON_CPPFLAGS)
TestContainer_LDADD    = $(COMMON_LIBS) libSalomeTestComponentEngine.la

TestLogger_SOURCES     = TestLogger.cxx
TestLogger_CPPFLAGS    = $(COMMON_CPPFLAGS)
TestLogger_LDADD       = $(COMMON_LIBS) libSalomeTestComponentEngine.la
