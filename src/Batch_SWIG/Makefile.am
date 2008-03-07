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
# libBatch_Swig.i -- swig --> swig_wrap.cpp
#                             libBatch_Swig.py
#
# Step 2: build the dynamic library from cpp built source files and
#         dependant libraries.
#
# swig_wrap.cpp -- gcc --> swig_wrap.o    |-- link --> _libBatch_Swig.la
#                          +              |
#                          dependant libs |
#
# The file libBatch_Swig.py will be installed in
# <prefix>/lib/python<version>/site-package/salome.
# The library will be installed in the common place.
#

BUILT_SOURCES = swig_wrap.cpp

SWIG_FLAGS    = @SWIG_FLAGS@ -I$(srcdir) -I$(srcdir)/../Batch
SWIG_SOURCES  = libBatch_Swig.i libBatch_Swig_exception.i libBatch_Swig_typemap.i

salomepython_PYTHON           = libBatch_Swig.py
salomepyexec_LTLIBRARIES      = _libBatch_Swig.la
_libBatch_Swig_la_SOURCES  = $(BUILT_SOURCES) $(SWIG_SOURCES)
_libBatch_Swig_la_CPPFLAGS = @PYTHON_INCLUDES@ -I$(srcdir) -I$(srcdir)/../Batch
_libBatch_Swig_la_LDFLAGS  = -module
_libBatch_Swig_la_LIBADD   = ../Batch/libSalomeBatch.la @PYTHON_LIBS@

if WITH_OPENPBS
_libBatch_Swig_la_CPPFLAGS += @OPENPBS_INCLUDES@
_libBatch_Swig_la_LIBADD   += @OPENPBS_LIBDIR@ @OPENPBS_LIBS@
endif

swig_wrap.cpp : $(SWIG_SOURCES)
	$(SWIG) $(SWIG_FLAGS) -o $@ $<

CLEANFILES = swig_wrap.cpp


#
# ===============================================================
# Files to be installed
# ===============================================================
#

# Scripts to be installed.
dist_salomescript_SCRIPTS = Batch_test.py

# You don't need to specify libBatch_Swig.py. It is automatically
# installed by means of the swig target salomepython_PYTHON.
