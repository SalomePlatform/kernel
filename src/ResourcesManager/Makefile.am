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
#  SALOME ResourcesManager
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : SALOME
#  $Header$
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Header to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = \
		SALOME_ResourcesCatalog_Parser.hxx \
		SALOME_ResourcesManager.hxx \
                SALOME_ResourcesCatalog_Handler.hxx \
		SALOME_LoadRateManager.hxx \
		ResourcesManager.hxx \
		ResourcesManager_Defs.hxx

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	-I$(srcdir)/../Batch \
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../NamingService \
	-I$(srcdir)/../Utils \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@LIBXML_INCLUDES@ \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	../NamingService/libSalomeNS.la \
	../Utils/libOpUtil.la \
	../Basics/libSALOMEBasics.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la \
	@LIBXML_LIBS@

#
# ===============================================================
# Libraries targets
# ===============================================================
#
if WITHONLYLAUNCHER
  lib_LTLIBRARIES = libResourcesManager.la
else
  lib_LTLIBRARIES = libResourcesManager.la libSalomeResourcesManager.la
endif
libSalomeResourcesManager_la_SOURCES =\
	SALOME_ResourcesManager.cxx

libSalomeResourcesManager_la_CPPFLAGS =\
	$(COMMON_CPPFLAGS)

libSalomeResourcesManager_la_LDFLAGS = -no-undefined -version-info=0:0:0
libSalomeResourcesManager_la_LIBADD  =\
	$(COMMON_LIBS) libResourcesManager.la

libResourcesManager_la_SOURCES =\
	SALOME_ResourcesCatalog_Parser.cxx \
	SALOME_ResourcesCatalog_Handler.cxx  \
	SALOME_LoadRateManager.cxx \
	ResourcesManager.cxx

libResourcesManager_la_CPPFLAGS =\
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../Utils \
	@LIBXML_INCLUDES@

libResourcesManager_la_LDFLAGS = -no-undefined -version-info=0:0:0
libResourcesManager_la_LIBADD  =\
	../Utils/libOpUtil.la \
	@LIBXML_LIBS@
