#  SALOME Logger : CORBA server managing trace output
#
#  Copyright (C) 2003  CEA/DEN, EDF R&D
#
#  File   : Makefile.am
#  Author : Guillaume BOULANT, CSSI
#  Module : KERNEL
#  $Header$

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
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

# Libraries targets
lib_LTLIBRARIES = libSalomeLoggerServer.la
libSalomeLoggerServer_la_SOURCES =\
	SALOME_Trace.cxx \
	SALOME_Logger_Server.cxx 

libSalomeLoggerServer_la_CPPFLAGS =\
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@
libSalomeLoggerServer_la_LDFLAGS = -no-undefined -version-info=0:0:0

libSalomeLoggerServer_la_LIBADD  = \
	$(top_builddir)/idl/libSalomeIDLKernel.la\
	@CORBA_LIBS@

# Scripts target
dist_salomescript_DATA = SALOME_Trace.py
