#  SALOME MPIContainer : implemenation of container based on MPI libraries
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
#  Module : SALOME
#  $Header$



#
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
salomeinclude_HEADERS = TestMPIComponentEngine.hxx

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
# _CS_gbo See wether it can be shared by all packages in a common include!!
COMMON_CPPFLAGS=\
	@MPI_INCLUDES@ \
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../NamingService \
	-I$(srcdir)/../Utils \
	-I$(srcdir)/../Registry \
	-I$(srcdir)/../Notification \
	-I$(srcdir)/../ResourcesManager \
	-I$(srcdir)/../Container \
	-I$(srcdir)/../MPIContainer \
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
	../Container/libSalomeContainer.la \
	../MPIContainer/libSalomeMPIContainer.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la\
	@CORBA_LIBS@

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeTestMPIComponentEngine.la
libSalomeTestMPIComponentEngine_la_SOURCES = TestMPIComponentEngine.cxx
libSalomeTestMPIComponentEngine_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libSalomeTestMPIComponentEngine_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libSalomeTestMPIComponentEngine_la_LIBADD   = $(COMMON_LIBS)



#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = TestMPIContainer
TestMPIContainer_SOURCES = TestMPIContainer.cxx
TestMPIContainer_LDADD   = libSalomeTestMPIComponentEngine.la $(COMMON_LIBS) @MPI_LIBS@
TestMPIContainer_CPPFLAGS = $(COMMON_CPPFLAGS)
