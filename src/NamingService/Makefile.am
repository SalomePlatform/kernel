#  SALOME NamingService : wrapping NamingService services
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
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
#
#
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : KERNEL
#  $Header$

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

# header files  
salomeinclude_HEADERS =\
	SALOME_NamingService.hxx \
	ServiceUnreachable.hxx \
	NamingService_WaitForServerReadiness.hxx \
	SALOME_NamingService_defs.hxx

# Scripts to be exported
dist_salomescript_DATA =\
	SALOME_NamingServicePy.py


# Libraries targets
lib_LTLIBRARIES        = libSalomeNS.la
libSalomeNS_la_SOURCES =\
	SALOME_NamingService.cxx \
	ServiceUnreachable.cxx \
	NamingService_WaitForServerReadiness.cxx

libSalomeNS_la_LDFLAGS  = -no-undefined -version-info=0:0:0

libSalomeNS_la_CPPFLAGS = \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../Utils \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

libSalomeNS_la_LIBADD = ../Utils/libOpUtil.la
