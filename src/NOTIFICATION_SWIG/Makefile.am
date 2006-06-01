#  SALOME NOTIFICATION_SWIG : wrapping of Notification sevices in order to be available in Python
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

#
# ===============================================================
# Swig targets
# ===============================================================
# (cf. http://www.geocities.com/foetsch/python/swig_linux.htm)
# see also LifeCycleCORBA_SWIG 
#
BUILT_SOURCES = swig_wrap.cpp

SWIG_FLAGS    = @SWIG_FLAGS@ -I$(srcdir) -I$(srcdir)/../Notification
SWIG_SOURCES  = NOTIFICATION.i

pkgpython_PYTHON = libNOTIFICATION.py
pkgpyexec_LTLIBRARIES = _libNOTIFICATION.la
_libNOTIFICATION_la_SOURCES  = $(BUILT_SOURCES) $(SWIG_SOURCES) \
	NOTIFICATION_Swig.cxx NOTIFICATION_Swig.hxx
_libNOTIFICATION_la_CPPFLAGS =\
	@PYTHON_INCLUDES@ -DHAVE_CONFIG_H \
	-I$(srcdir)/../Notification \
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@


_libNOTIFICATION_la_LDFLAGS  = -module
_libNOTIFICATION_la_LIBADD   = ../Notification/libSalomeNotification.la \
	@PYTHON_LIBS@

swig_wrap.cpp : $(SWIG_SOURCES)
	$(SWIG) $(SWIG_FLAGS) -o $@ $<

CLEANFILES = swig_wrap.cpp
