# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

#  File   : Makefile.am
#  Author : André RIBES (EDF), Eric Fayolle (EDF)
#  Module : KERNEL

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

# ===============================================================
# Local definitions
# ===============================================================
#

BUILT_SOURCES = calcium_wrap.cpp calcium.py

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS= -I$(top_srcdir)/src/DSC/DSC_User \
		 -I$(top_srcdir)/src/DSC/DSC_User/Datastream \
		 -I$(top_srcdir)/src/DSC/DSC_User/Datastream/Palm \
		 -I$(top_srcdir)/src/DSC/DSC_User/Datastream/Calcium \
                 -I$(top_builddir)/src/DSC/DSC_User/Datastream/Calcium \
		 -I$(top_srcdir)/src/DSC/DSC_User/Basic \
		 -I$(top_srcdir)/src/DSC/DSC_Basic \
		 -I$(top_srcdir)/src/SALOMELocalTrace \
		 -I$(top_srcdir)/src/Basics \
		 -I$(top_srcdir)/src/Utils \
		 -I$(top_srcdir)/src/Container \
		 -I$(top_srcdir)/src/GenericObj \
		 -I$(top_srcdir)/src/Notification \
		 -I$(top_builddir)/idl \
		 @CORBA_CXXFLAGS@ @CORBA_INCLUDES@ @BOOST_CPPFLAGS@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS = @CORBA_LIBS@

#
# ===============================================================
# Libraries targets
# ===============================================================
#

AM_CFLAGS         = -fexceptions
salomepython_PYTHON = calcium.py dsccalcium.py
salomepyexec_LTLIBRARIES = _calcium.la
MY_SWIG_FLAGS= $(SWIG_FLAGS) -noexcept -I$(top_srcdir)/src/DSC/DSC_User/Datastream/Calcium
SWIG_SRC=calcium.i
_calcium_la_SOURCES = calcium_wrap.cpp
_calcium_la_LDFLAGS = -module
_calcium_la_LIBADD = ../DSC_User/Datastream/Calcium/libCalciumC.la \
										 ../DSC_User/Datastream/libSalomeDatastream.la \
										 ../DSC_User/Basic/libSalomeDSCSupervBasic.la \
										 ../DSC_User/libSalomeDSCSuperv.la \
										 ../DSC_Basic/libSalomeDSCContainer.la \
										 ../../Container/libSalomeContainer.la 

_calcium_la_CPPFLAGS = $(PYTHON_INCLUDES) $(COMMON_CPPFLAGS)

EXTRA_DIST+=calcium_wrap.cpp calcium.i

calcium_wrap.cpp calcium.py:calcium.i
	$(SWIG) $(MY_SWIG_FLAGS) -o calcium_wrap.cpp $<

CLEANFILES = calcium_wrap.cpp

