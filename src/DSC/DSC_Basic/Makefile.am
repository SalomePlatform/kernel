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

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Header to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = ConnectionManager_i.hxx \
			DSC_Callbacks.hxx \
			DSC_i.hxx \
			DSC_interface.hxx \
			PortProperties_i.hxx \
			DSC_Basic.hxx
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
		 -I$(top_srcdir)/src/Utils \
		 -I$(top_builddir)/salome_adm/unix \
		 -I$(top_builddir)/idl \
		 @CORBA_CXXFLAGS@ @CORBA_INCLUDES@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS = $(top_builddir)/src/Container/libSalomeContainer.la \
	      @CORBA_LIBS@

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeDSCContainer.la 

libSalomeDSCContainer_la_SOURCES  = DSC_i.cxx \
				    DSC_interface.cxx \
				    PortProperties_i.cxx

libSalomeDSCContainer_la_CXXFLAGS = $(COMMON_CPPFLAGS)

libSalomeDSCContainer_la_LIBADD   = $(COMMON_LIBS)

libSalomeDSCContainer_la_LDFLAGS = -no-undefined -version-info=0:0:0

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = SALOME_ConnectionManagerServer

SALOME_ConnectionManagerServer_SOURCES  = SALOME_ConnectionManagerServer.cxx \
					  ConnectionManager_i.cxx 

SALOME_ConnectionManagerServer_CXXFLAGS = $(COMMON_CPPFLAGS)

SALOME_ConnectionManagerServer_LDADD    = $(top_builddir)/idl/libSalomeIDLKernel.la \
					  $(top_builddir)/src/NamingService/libSalomeNS.la \
					  $(top_builddir)/src/SALOMELocalTrace/libSALOMELocalTrace.la
