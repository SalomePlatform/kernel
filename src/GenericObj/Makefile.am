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
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : KERNEL
#  $Header$ 
#
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
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@
libSalomeGenericObj_la_LIBADD  = \
	../SALOMELocalTrace/libSALOMELocalTrace.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la
