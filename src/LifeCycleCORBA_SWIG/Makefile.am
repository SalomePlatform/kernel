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

#
# ===============================================================
# Swig targets
# ===============================================================
# (cf. http://www.geocities.com/foetsch/python/swig_linux.htm)
#
# Step 1: build the wrapping source files with swig
#
# libSALOME_LifeCycleCORBA.i -- swig --> swig_wrap.cpp
#                                        libSALOME_LifeCycleCORBA.py
#
# Step 2: build the dynamic library from cpp built source files and
#         dependant libraries.
#
# swig_wrap.cpp -- gcc --> swig_wrap.o    |-- link --> _libSALOME_LifeCycleCORBA.la
#                          +              |
#                          dependant libs |
#
# The file libSALOME_LifeCycleCORBA.py will be installed in
# <prefix>/lib/python<version>/site-package/salome.
# The library will be installed in the same place.
#

BUILT_SOURCES = swig_wrap.cpp

SWIG_FLAGS    = @SWIG_FLAGS@ -I$(srcdir) -I$(srcdir)/../LifeCycleCORBA -I$(srcdir)/../Utils
SWIG_SOURCES  = libSALOME_LifeCycleCORBA.i

salomepython_PYTHON = libSALOME_LifeCycleCORBA.py
salomepyexec_LTLIBRARIES = _libSALOME_LifeCycleCORBA.la
_libSALOME_LifeCycleCORBA_la_SOURCES  = $(BUILT_SOURCES) $(SWIG_SOURCES)
_libSALOME_LifeCycleCORBA_la_CPPFLAGS =\
	@PYTHON_INCLUDES@ \
	-I$(srcdir) \
	-I$(srcdir)/../NamingService \
	-I$(srcdir)/../LifeCycleCORBA \
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../Utils \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

_libSALOME_LifeCycleCORBA_la_LDFLAGS  = -module
_libSALOME_LifeCycleCORBA_la_LIBADD   = \
	../LifeCycleCORBA/libSalomeLifeCycleCORBA.la \
	@PYTHON_LIBS@

swig_wrap.cpp : $(SWIG_SOURCES)
	$(SWIG) $(SWIG_FLAGS) -o $@ $<

CLEANFILES = swig_wrap.cpp


#
# ===============================================================
# Files to be installed
# ===============================================================
#

# Scripts to be installed.
dist_salomescript_DATA =\
	LifeCycleCORBA.py \
	TestLifeCycleCORBA.py

# It's not needed to specify libSALOME_LifeCycleCORBA.py. It is
# automaticaly installed through the swig target salomepython_PYTHON.
