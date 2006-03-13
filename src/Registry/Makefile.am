#  SALOME Registry : Registry server implementation
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

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

# header files  
salomeinclude_HEADERS =\
	RegistryConnexion.hxx \
	RegistryService.hxx

# Libraries targets
lib_LTLIBRARIES = libRegistry.la
libRegistry_la_SOURCES =\
	RegistryConnexion.cxx \
	RegistryService.cxx 

libRegistry_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libRegistry_la_CPPFLAGS =\
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../NamingService \
	-I$(srcdir)/../Utils \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

libRegistry_la_LIBADD   =\
	../NamingService/libSalomeNS.la \
	../Utils/libOpUtil.la \
	../SALOMELocalTrace/libSALOMELocalTrace.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la


# Executables targets
bin_PROGRAMS = SALOME_Registry_Server
SALOME_Registry_Server_SOURCES  = SALOME_Registry_Server.cxx
SALOME_Registry_Server_LDADD    =\
	libRegistry.la \
	../Basics/libSALOMEBasics.la \
	@CORBA_LIBS@
SALOME_Registry_Server_CPPFLAGS =\
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../NamingService \
	-I$(srcdir)/../Utils \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@
