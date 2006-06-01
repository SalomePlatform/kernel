#  SALOME Utils : general SALOME's definitions and tools
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
#  Module : SALOME
#  $Header$


include $(top_srcdir)/salome_adm/unix/make_common_starter.am

# header files  
salomeinclude_HEADERS =\
	OpUtil.hxx \
	Utils_Timer.hxx \
	Utils_CorbaException.hxx \
	Utils_CommException.hxx \
	Utils_SALOME_Exception.hxx \
	Utils_ORB_INIT.hxx \
	Utils_Identity.hxx \
	Utils_SINGLETON.hxx \
	Utils_DESTRUCTEUR_GENERIQUE.hxx \
	Utils_ExceptHandlers.hxx \
	Utils_Mutex.hxx

# Scripts to be exported
dist_salomescript_DATA =\
	Utils_Identity.py \
	SALOME_utilities.py

# Libraries targets
lib_LTLIBRARIES = libOpUtil.la
libOpUtil_la_SOURCES =\
	OpUtil.cxx Utils_Timer.cxx duplicate.cxx \
	Utils_CommException.cxx \
	Utils_SALOME_Exception.cxx \
	Utils_Identity.cxx Utils_ORB_INIT.cxx \
	Utils_DESTRUCTEUR_GENERIQUE.cxx \
	Utils_ExceptHandlers.cxx \
	Utils_Mutex.cxx

libOpUtil_la_LDFLAGS  = -no-undefined -version-info=0:0:0

libOpUtil_la_CPPFLAGS = \
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

libOpUtil_la_LIBADD   = \
	../SALOMELocalTrace/libSALOMELocalTrace.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la\
	@CORBA_LIBS@



