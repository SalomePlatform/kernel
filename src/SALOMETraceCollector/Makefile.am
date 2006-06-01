#  SALOMETraceCollector
#
#  Copyright (C) 2003  CEA/DEN, EDF R&D
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
#  File   : Makefile.am
#  Author : Guillaume BOULANT, CSSI
#  Module : KERNEL
#  $Header$

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

# header files  
salomeinclude_HEADERS = \
	SALOMETraceCollector.hxx \
	TraceCollector_WaitForServerReadiness.hxx

# Libraries targets
lib_LTLIBRARIES = libwith_loggerTraceCollector.la

libwith_loggerTraceCollector_la_SOURCES  =\
	SALOMETraceCollector.cxx \
	TraceCollector_WaitForServerReadiness.cxx 

libwith_loggerTraceCollector_la_CPPFLAGS =\
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

libwith_loggerTraceCollector_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libwith_loggerTraceCollector_la_LIBADD   = \
	../SALOMELocalTrace/libSALOMELocalTrace.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la\
	@CORBA_LIBS@
