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
	SALOME_Component_i.hxx \
	SALOME_Container_i.hxx \
	SALOME_FileTransfer_i.hxx \
	SALOME_FileRef_i.hxx \
	SALOME_ContainerManager.hxx \
	Container_init_python.hxx \
	SALOME_Container.hxx

# Scripts to be installed
dist_salomescript_DATA =\
	SALOME_ComponentPy.py \
	SALOME_Container.py

# These files are executable scripts
dist_salomescript_SCRIPTS=\
	SALOME_ContainerPy.py

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	@PYTHON_INCLUDES@ \
	@MPI_INCLUDES@ \
	@CAS_CPPFLAGS@ @CAS_CXXFLAGS@ \
	@QT_MT_INCLUDES@ \
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../NamingService \
	-I$(srcdir)/../Utils \
	-I$(srcdir)/../Registry \
	-I$(srcdir)/../Notification \
	-I$(srcdir)/../ResourcesManager \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@ 

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	../Registry/libRegistry.la \
	../Notification/libSalomeNotification.la \
	../ResourcesManager/libSalomeResourcesManager.la \
	../NamingService/libSalomeNS.la \
	../Utils/libOpUtil.la \
	../SALOMELocalTrace/libSALOMELocalTrace.la \
	../Basics/libSALOMEBasics.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la\
	@MPI_LIBS@ \
	@CORBA_LIBS@

#	@PYTHON_LIBS@

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeContainer.la
libSalomeContainer_la_SOURCES=\
	Component_i.cxx \
	Container_i.cxx \
	SALOME_FileTransfer_i.cxx \
	SALOME_FileRef_i.cxx \
	Container_init_python.cxx \
	SALOME_ContainerManager.cxx

libSalomeContainer_la_CPPFLAGS =\
	$(COMMON_CPPFLAGS)

libSalomeContainer_la_LDFLAGS  =\
	-no-undefined -version-info=0:0:0 \
	@LDEXPDYNFLAGS@

libSalomeContainer_la_LIBADD =\
	$(PYTHON_LIBS) \
	$(COMMON_LIBS)

if WITH_PACO_PARALLEL
libSalomeContainer_la_CPPFLAGS += -DWITH_PACO_PARALLEL @PACO_INCLUDES@
libSalomeContainer_la_LIBADD += @PACO_LIBS@
endif

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = SALOME_Container SALOME_ContainerManagerServer
SALOME_Container_SOURCES =\
	SALOME_Container.cxx \
	SALOME_Container_SignalsHandler.cxx

SALOME_Container_CPPFLAGS =\
	$(COMMON_CPPFLAGS)

SALOME_Container_LDADD =\
	libSalomeContainer.la \
	$(COMMON_LIBS) \
	../Basics/libSALOMEBasics.la

SALOME_ContainerManagerServer_SOURCES =\
	SALOME_ContainerManagerServer.cxx 

SALOME_ContainerManagerServer_CPPFLAGS=\
	$(COMMON_CPPFLAGS)

SALOME_ContainerManagerServer_LDADD =\
	libSalomeContainer.la \
	$(COMMON_LIBS) \
	../Basics/libSALOMEBasics.la
