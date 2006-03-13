#  SALOME DataTypeCatalog : catalog of data types
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

#
# ===============================================================
# Header to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = \
		SALOME_DataTypeCatalog_impl.hxx \
		SALOME_DataTypeCatalog_Parser.hxx \
                SALOME_DataTypeCatalog_Handler.hxx 

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../NamingService \
	-I$(srcdir)/../Utils \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
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

lib_LTLIBRARIES = libSalomeDataTypeCatalog.la
libSalomeDataTypeCatalog_la_SOURCES=\
	SALOME_DataTypeCatalog_Handler.cxx  \
	SALOME_DataTypeCatalog_impl.cxx

libSalomeDataTypeCatalog_la_CPPFLAGS =\
	$(COMMON_CPPFLAGS) \
	@QT_MT_INCLUDES@

libSalomeDataTypeCatalog_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libSalomeDataTypeCatalog_la_LIBADD   =\
	$(COMMON_LIBS) \
	@QT_MT_LIBS@

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = SALOME_DataTypeCatalog_Server SALOME_DataTypeCatalog_Client

#SALOME_DataTypeCatalog_Server
SALOME_DataTypeCatalog_Server_SOURCES  = SALOME_DataTypeCatalog_Server.cxx
SALOME_DataTypeCatalog_Server_CPPFLAGS =\
	$(COMMON_CPPFLAGS) \
	@QT_MT_INCLUDES@

SALOME_DataTypeCatalog_Server_LDADD    =\
	libSalomeDataTypeCatalog.la \
	$(COMMON_LIBS) \
	@CORBA_LIBS@

#SALOME_DataTypeCatalog_Client
SALOME_DataTypeCatalog_Client_SOURCES  = SALOME_DataTypeCatalog_Client.cxx
SALOME_DataTypeCatalog_Client_CPPFLAGS =\
	$(COMMON_CPPFLAGS) \
	@QT_MT_INCLUDES@

SALOME_DataTypeCatalog_Client_LDADD    =\
	libSalomeDataTypeCatalog.la \
	$(COMMON_LIBS) \
	@CORBA_LIBS@
