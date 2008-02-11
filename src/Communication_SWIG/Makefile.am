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

#
# ===============================================================
# Swig targets
# ===============================================================
# (cf. http://www.geocities.com/foetsch/python/swig_linux.htm)
#
# Step 1: build the wrapping source files with swig
#
# libSALOME_Comm.i -- swig --> swig_wrap.cpp
#                              libSALOME_Comm.py
#
# Step 2: build the dynamic library from cpp built source files and
#         dependant libraries.
#
# swig_wrap.cpp -- gcc --> swig_wrap.o    |-- link --> _libSALOME_Comm.la
#                          +              |
#                          dependant libs |
#
# The file libSALOME_Comm.py will be installed in
# <prefix>/lib/python<version>/site-package/salome.
# The library will be installed in the common place.
#

BUILT_SOURCES = swig_wrap.cpp

SWIG_FLAGS    = @SWIG_FLAGS@ -I$(srcdir) -I$(srcdir)/../Communication
SWIG_SOURCES  = libSALOME_Comm.i


salomepython_PYTHON = libSALOME_Comm.py
salomepyexec_LTLIBRARIES = _libSALOME_Comm.la
_libSALOME_Comm_la_SOURCES  = $(BUILT_SOURCES) $(SWIG_SOURCES)
_libSALOME_Comm_la_CPPFLAGS =\
	@PYTHON_INCLUDES@ \
	-I$(srcdir) -I$(srcdir)/../Communication \
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../Utils \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

_libSALOME_Comm_la_LDFLAGS  = -module
_libSALOME_Comm_la_LIBADD   = \
	../Communication/libSalomeCommunication.la \
	@PYTHON_LIBS@

# _CS_gbo Actually, there's no need to get these flags optional. If
# the check mpi is not ok, the variables will be simply null.
if MPI_IS_OK
_libSALOME_Comm_la_CPPFLAGS+=@MPI_INCLUDES@
_libSALOME_Comm_la_LIBADD+=@MPI_LIBS@
endif


swig_wrap.cpp : $(SWIG_SOURCES)
	$(SWIG) $(SWIG_FLAGS) -o $@ $<

CLEANFILES = swig_wrap.cpp


#
# ===============================================================
# Files to be installed
# ===============================================================
#

# (see LifeCycleCORBA_SWIG for an example)

# It's not needed to specify libSALOME_Comm.py. It is
# automaticaly installed through the swig target salomepython_PYTHON.
