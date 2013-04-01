# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

#  SALOME ModuleCatalog : implementation of ModuleCatalog server which parsers xml description of modules
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : SALOME
#  $Header$
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Header an scripts to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = \
		SALOME_ModuleCatalog_impl.hxx \
		SALOME_ModuleCatalog_Acomponent_impl.hxx \
		PathPrefix.hxx \
		SALOME_ModuleCatalog_Parser.hxx \
		SALOME_ModuleCatalog_Parser_IO.hxx \
                SALOME_ModuleCatalog_Handler.hxx \
		SALOME_ModuleCatalog.hxx

# Scripts to be installed
dist_salomescript_PYTHON = TestModuleCatalog.py


#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeCatalog.la

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../NamingService \
	-I$(srcdir)/../Utils \
	-I$(top_builddir)/idl \
	@LIBXML_INCLUDES@ \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	../NamingService/libSalomeNS.la \
	../Utils/libOpUtil.la \
	../SALOMELocalTrace/libSALOMELocalTrace.la \
	../Basics/libSALOMEBasics.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la \
	@LIBXML_LIBS@

libSalomeCatalog_la_SOURCES =\
	SALOME_ModuleCatalog_Handler.cxx  \
	SALOME_ModuleCatalog_Parser_IO.cxx \
	SALOME_ModuleCatalog_impl.cxx \
	SALOME_ModuleCatalog_Acomponent_impl.cxx

libSalomeCatalog_la_CPPFLAGS =\
	$(COMMON_CPPFLAGS)

libSalomeCatalog_la_LDFLAGS = -no-undefined -version-info=0:0:0

libSalomeCatalog_la_LIBADD  =\
	$(COMMON_LIBS)


#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = SALOME_ModuleCatalog_Server SALOME_ModuleCatalog_Client

# SALOME_ModuleCatalog_Server
SALOME_ModuleCatalog_Server_SOURCES  = SALOME_ModuleCatalog_Server.cxx
SALOME_ModuleCatalog_Server_CPPFLAGS =\
	$(COMMON_CPPFLAGS) 

SALOME_ModuleCatalog_Server_LDADD   =\
	libSalomeCatalog.la ${COMMON_LIBS} \
	@CORBA_LIBS@

# SALOME_ModuleCatalog_Client
SALOME_ModuleCatalog_Client_SOURCES  = SALOME_ModuleCatalog_Client.cxx
SALOME_ModuleCatalog_Client_CPPFLAGS =\
	$(COMMON_CPPFLAGS)

SALOME_ModuleCatalog_Client_LDADD   =\
	libSalomeCatalog.la ${COMMON_LIBS} \
	@CORBA_LIBS@
