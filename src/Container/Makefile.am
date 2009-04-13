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
#  SALOME Container : implementation of container and engine for Kernel
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : KERNEL
#  $Header$
#
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
	SALOME_Container.hxx \
	Salome_file_i.hxx

# Scripts to be installed
dist_salomescript_PYTHON =\
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
	@HDF5_INCLUDES@ \
	-I$(srcdir)/../Batch \
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../NamingService \
	-I$(srcdir)/../Utils \
	-I$(srcdir)/../Registry \
	-I$(srcdir)/../Notification \
	-I$(srcdir)/../ResourcesManager \
	-I$(srcdir)/../HDFPersist \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@ @LIBXML_INCLUDES@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	../Registry/libRegistry.la \
	../Notification/libSalomeNotification.la \
	../ResourcesManager/libSalomeResourcesManager.la \
	../ResourcesManager/libResourcesManager.la \
	../NamingService/libSalomeNS.la \
	../Utils/libOpUtil.la \
	../SALOMELocalTrace/libSALOMELocalTrace.la \
	../Basics/libSALOMEBasics.la \
	../HDFPersist/libSalomeHDFPersist.la \
	../Batch/libSalomeBatch.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la \
	@MPI_LIBS@ \
	@CORBA_LIBS@ \
	$(PYTHON_LIBS)

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
	SALOME_ContainerManager.cxx \
	Salome_file_i.cxx

libSalomeContainer_la_CPPFLAGS =\
	$(COMMON_CPPFLAGS)

libSalomeContainer_la_LDFLAGS  =\
	-no-undefined -version-info=0:0:0 \
	@LDEXPDYNFLAGS@

libSalomeContainer_la_LIBADD =\
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
bin_PROGRAMS = SALOME_Container
noinst_PROGRAMS = TestSalome_file

SALOME_Container_SOURCES =\
	SALOME_Container.cxx \
	SALOME_Container_SignalsHandler.cxx

SALOME_Container_CPPFLAGS =\
	$(COMMON_CPPFLAGS)

SALOME_Container_LDADD = \
	libSalomeContainer.la \
	../Basics/libSALOMEBasics.la \
	$(HDF5_LIBS) \
	$(MPI_LIBS) \
	$(CORBA_LIBS) \
	$(PYTHON_LIBS)

SALOME_Container_LDFLAGS  =\
-Xlinker -export-dynamic

TestSalome_file_SOURCES =\
	TestSalome_file.cxx

TestSalome_file_CPPFLAGS =\
	$(COMMON_CPPFLAGS)

TestSalome_file_LDADD =\
	libSalomeContainer.la \
	../Basics/libSALOMEBasics.la \
	$(HDF5_LIBS) \
	$(MPI_LIBS) \
	$(CORBA_LIBS) \
	$(PYTHON_LIBS)

install-data-hook:
	@for f in $(dist_salomescript_PYTHON) ; do \
          chmod -f a+x $(DESTDIR)$(salomescriptdir)/$$f ; \
        done
