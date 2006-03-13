#  Copyright (C) 2003  CEA/DEN, EDF R&D
#
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : KERNEL
#  $Header$ 

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

# header files  
salomeinclude_HEADERS =\
	SALOME_GenericObj_i.hh

# Libraries targets
lib_LTLIBRARIES = libSalomeGenericObj.la
libSalomeGenericObj_la_SOURCES =\
	SALOME_GenericObj_i.cc

libSalomeGenericObj_la_LDFLAGS = -no-undefined -version-info=0:0:0
libSalomeGenericObj_la_CPPFLAGS = \
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@
libSalomeGenericObj_la_LIBADD  = \
	$(top_builddir)/idl/libSalomeIDLKernel.la
