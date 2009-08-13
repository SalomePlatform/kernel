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
#  SALOME MPIContainer : implemenation of container based on MPI libraries
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : SALOME
#  $Header$
# ===============================================================
# _CS_gbo Caution !! This package may not work. I didn't test it
# because no mpi soft is installed on my computer. To be checked.
# ===============================================================
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Header to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = \
	MPIObject_i.hxx \
	MPIContainer_i.hxx

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
# _CS_gbo See wether it can be shared by all packages in a common include!!
COMMON_CPPFLAGS=\
	@PYTHON_INCLUDES@ \
	@MPI_INCLUDES@ \
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../NamingService \
	-I$(srcdir)/../Utils \
	-I$(srcdir)/../Registry \
	-I$(srcdir)/../Notification \
	-I$(srcdir)/../ResourcesManager \
	-I$(srcdir)/../Container \
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
	../Container/libSalomeContainer.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la\
	@CORBA_LIBS@

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeMPIContainer.la
libSalomeMPIContainer_la_SOURCES = MPIObject_i.cxx  MPIContainer_i.cxx
libSalomeMPIContainer_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libSalomeMPIContainer_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libSalomeMPIContainer_la_LIBADD   = $(COMMON_LIBS)



#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = SALOME_MPIContainer
SALOME_MPIContainer_SOURCES = SALOME_MPIContainer.cxx
SALOME_MPIContainer_LDADD   = libSalomeMPIContainer.la @PYTHON_LIBS@ @MPI_LIBS@
SALOME_MPIContainer_CPPFLAGS = $(COMMON_CPPFLAGS)


