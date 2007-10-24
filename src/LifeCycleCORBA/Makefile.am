#  SALOME LifeCycleCORBA : implementation of containers and engines life cycle both in Python and C++
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
#  Author : Guillaume Boulant
#  Module : KERNEL
#  $Header$

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Files to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = \
	SALOME_LifeCycleCORBA.hxx \
	SALOME_FileTransferCORBA.hxx

# Scripts to be installed
#dist_salomescript_DATA =\
#	Launchers.py

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../SALOMETraceCollector \
	-I$(srcdir)/../NamingService \
	-I$(srcdir)/../Utils \
	-I$(srcdir)/../Container \
	-I$(srcdir)/../ResourcesManager \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@ @LIBXML_INCLUDES@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	../Container/libSalomeContainer.la \
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
lib_LTLIBRARIES = libSalomeLifeCycleCORBA.la
libSalomeLifeCycleCORBA_la_SOURCES  = \
	SALOME_LifeCycleCORBA.cxx \
	SALOME_FileTransferCORBA.cxx
libSalomeLifeCycleCORBA_la_CPPFLAGS = \
	$(COMMON_CPPFLAGS) \
	@PYTHON_INCLUDES@ \
	-I$(srcdir)/../Batch \
	-I$(srcdir)/../Container \
	-I$(srcdir)/../Notification

libSalomeLifeCycleCORBA_la_LIBADD   = $(COMMON_LIBS)
# _CS_gbo Check whether Python lib or Qt lib is not required here.
# For better understanding, it could be better to specified
# explicitely all libraries even those that are implicitly given
# throught a library mentionned in COMMON_LIBS.

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = Test_LifeCycleCORBA TestContainerManager
Test_LifeCycleCORBA_SOURCES  = Test_LifeCycleCORBA.cxx
Test_LifeCycleCORBA_CPPFLAGS =\
	-I$(srcdir)/../Registry \
	-I$(srcdir)/../Notification \
	$(COMMON_CPPFLAGS)

Test_LifeCycleCORBA_LDADD    = \
	libSalomeLifeCycleCORBA.la \
	../Registry/libRegistry.la \
	../Notification/libSalomeNotification.la \
	../Container/libSalomeContainer.la \
	$(COMMON_LIBS) \
	@CORBA_LIBS@

TestContainerManager_SOURCES  = TestContainerManager.cxx
TestContainerManager_CPPFLAGS =\
	-I$(srcdir)/../Batch \
	-I$(srcdir)/../Registry \
	-I$(srcdir)/../Notification \
	$(COMMON_CPPFLAGS)

TestContainerManager_LDADD    = \
	libSalomeLifeCycleCORBA.la \
	../Registry/libRegistry.la \
	../Notification/libSalomeNotification.la \
	../Container/libSalomeContainer.la \
	$(COMMON_LIBS) \
	@CORBA_LIBS@

