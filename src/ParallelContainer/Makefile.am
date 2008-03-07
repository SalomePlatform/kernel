#  Copyright (C) 2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
#  Author : André RIBES (EDF)
#  Module : KERNEL
#

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Header to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = SALOME_ParallelComponent_i.hxx \
			SALOME_ParallelContainer_i.hxx \
			SALOME_ParallelContainerProxy_i.hxx \
			Parallel_Salome_file_i.hxx

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS= -I$(top_srcdir)/src/Container \
		 -I$(top_srcdir)/src/Notification \
		 -I$(top_srcdir)/src/SALOMELocalTrace \
		 -I$(top_srcdir)/src/Basics \
		 -I$(top_srcdir)/src/NamingService \
		 -I$(top_srcdir)/src/Registry \
		 -I$(top_srcdir)/src/Utils \
		 -I$(srcdir)/../HDFPersist \
		 -I$(top_builddir)/salome_adm/unix \
		 -I$(top_builddir)/idl \
		 -I$(top_srcdir)/src/SALOMETraceCollector \
		 @CORBA_CXXFLAGS@ \
		 @CORBA_INCLUDES@ \
		 @PACO_INCLUDES@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS = $(top_builddir)/src/Container/libSalomeContainer.la \
	      $(top_builddir)/src/HDFPersist/libSalomeHDFPersist.la \
	      $(top_builddir)/idl/libSalomeParallelIDLKernel.la \
	      $(top_builddir)/idl/libSalomeIDLKernel.la \
	      @CORBA_LIBS@ \
	      @PACO_LIBS@

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeParallelContainer.la 

libSalomeParallelContainer_la_SOURCES  = SALOME_ParallelComponent_i.cxx \
					 SALOME_ParallelContainer_i.cxx \
					 SALOME_ParallelContainerProxy_i.cxx \
					 $(top_srcdir)/src/Container/Salome_file_i.cxx \
					 Parallel_Salome_file_i.cxx

libSalomeParallelContainer_la_CXXFLAGS = $(COMMON_CPPFLAGS)

libSalomeParallelContainer_la_LIBADD   = $(COMMON_LIBS)

libSalomeParallelContainer_la_LDFLAGS = -no-undefined -version-info=0:0:0

#
# ===============================================================
# Executables targets
# ===============================================================
#

if MPI_IS_OK
bin_mpi_programs = SALOME_ParallelContainerProxyMpi \
		   SALOME_ParallelContainerNodeMpi 
endif

bin_PROGRAMS = SALOME_ParallelContainerProxyDummy \
	       SALOME_ParallelContainerNodeDummy \
	       $(bin_mpi_programs)

SALOME_ParallelContainerProxyDummy_SOURCES  = SALOME_ParallelContainerProxyDummy.cxx
SALOME_ParallelContainerProxyDummy_CXXFLAGS = $(COMMON_CPPFLAGS)
SALOME_ParallelContainerProxyDummy_LDADD    = libSalomeParallelContainer.la \
					      -L@PACOPATH@/lib -lPaCO_dummy -lPaCO_omnithread \
					      $(COMMON_LIBS)

SALOME_ParallelContainerNodeDummy_SOURCES  = SALOME_ParallelContainerNodeDummy.cxx
SALOME_ParallelContainerNodeDummy_CXXFLAGS = $(COMMON_CPPFLAGS)
SALOME_ParallelContainerNodeDummy_LDADD    = libSalomeParallelContainer.la \
					     -L@PACOPATH@/lib -lPaCO_dummy -lPaCO_omnithread \
					      $(COMMON_LIBS)

SALOME_ParallelContainerProxyMpi_SOURCES  = SALOME_ParallelContainerProxyMpi.cxx
SALOME_ParallelContainerProxyMpi_CXXFLAGS = $(COMMON_CPPFLAGS) @MPI_INCLUDES@
SALOME_ParallelContainerProxyMpi_LDADD    = libSalomeParallelContainer.la \
					    -L@PACOPATH@/lib -lPaCO_dummy -lPaCO_mpi -lPaCO_omnithread \
					    @MPI_LIBS@ \
					    $(COMMON_LIBS)


SALOME_ParallelContainerNodeMpi_SOURCES  = SALOME_ParallelContainerNodeMpi.cxx
SALOME_ParallelContainerNodeMpi_CXXFLAGS = $(COMMON_CPPFLAGS) @MPI_INCLUDES@
SALOME_ParallelContainerNodeMpi_LDADD    = libSalomeParallelContainer.la \
					   -L@PACOPATH@/lib -lPaCO_dummy -lPaCO_mpi -lPaCO_omnithread \
					   @MPI_LIBS@\
					   $(COMMON_LIBS)


