#  SALOME Container : implementation of container and engine for Kernel
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
# Header to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = \
  BatchLight_BatchManager.hxx \
  BatchLight_BatchManager_PBS.hxx \
  BatchLight_BatchManager_SLURM.hxx \
  BatchLight_Job.hxx \
	MpiImpl.hxx \
	SALOME_Launcher.hxx

# Scripts to be installed
dist_salomescript_DATA =

# These files are executable scripts
dist_salomescript_SCRIPTS=

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	@PYTHON_INCLUDES@ \
	@MPI_INCLUDES@ \
	@LIBXML_INCLUDES@ \
	-I$(srcdir)/../Batch \
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
	../Registry/libRegistry.la \
	../Notification/libSalomeNotification.la \
	../Container/libSalomeContainer.la \
	../ResourcesManager/libSalomeResourcesManager.la \
	../NamingService/libSalomeNS.la \
	../Utils/libOpUtil.la \
	../SALOMELocalTrace/libSALOMELocalTrace.la \
	../Basics/libSALOMEBasics.la \
	../Batch/libSalomeBatch.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la \
	@MPI_LIBS@ \
	@CORBA_LIBS@ \
	@LIBXML_LIBS@ \
	@PYTHON_LIBS@

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeLauncher.la
libSalomeLauncher_la_SOURCES=\
	SALOME_Launcher.cxx \
  BatchLight_BatchManager.cxx \
  BatchLight_BatchManager_SLURM.cxx \
  BatchLight_BatchManager_PBS.cxx \
  BatchLight_Job.cxx \
	MpiImpl.cxx

libSalomeLauncher_la_CPPFLAGS =\
	$(COMMON_CPPFLAGS)

libSalomeLauncher_la_LDFLAGS  =\
	-no-undefined -version-info=0:0:0 \
	@LDEXPDYNFLAGS@

libSalomeLauncher_la_LIBADD =\
	$(COMMON_LIBS)


#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = SALOME_LauncherServer

SALOME_LauncherServer_SOURCES =\
	SALOME_LauncherServer.cxx

SALOME_LauncherServer_CPPFLAGS=\
	$(COMMON_CPPFLAGS)

SALOME_LauncherServer_LDADD =\
	libSalomeLauncher.la \
	$(MPI_LIBS) \
	$(CORBA_LIBS) \
	$(LIBXML_LIBS) \
	$(PYTHON_LIBS)


