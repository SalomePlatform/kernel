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
#  SALOME Logger : CORBA server managing trace output
#  File   : Makefile.am
#  Author : Guillaume BOULANT, CSSI
#  Module : KERNEL
#  $Header$
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

# header files  
salomeinclude_HEADERS = \
	SALOME_Trace.hxx

#
# This makefile defines a executable target (SALOME_Logger_Server) and
# a library target (libSalomeLoggerServer.la). The executable depends
# on the library.
#

# Program files
bin_PROGRAMS = SALOME_Logger_Server
SALOME_Logger_Server_SOURCES = \
	SALOME_Logger_Server_main.cxx \
	SALOME_Logger_Server.hxx

SALOME_Logger_Server_LDADD   = libSalomeLoggerServer.la
SALOME_Logger_Server_CPPFLAGS =\
	-I$(top_builddir)/idl \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../Basics \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

# Libraries targets
lib_LTLIBRARIES = libSalomeLoggerServer.la
libSalomeLoggerServer_la_SOURCES =\
	SALOME_Trace.cxx \
	SALOME_Logger_Server.cxx 

libSalomeLoggerServer_la_CPPFLAGS =\
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@
libSalomeLoggerServer_la_LDFLAGS = -no-undefined -version-info=0:0:0

libSalomeLoggerServer_la_LIBADD  = \
	$(top_builddir)/idl/libSalomeIDLKernel.la\
	@CORBA_LIBS@

# Scripts target
dist_salomescript_PYTHON = SALOME_Trace.py
