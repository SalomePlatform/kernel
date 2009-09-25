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
#  Author : André RIBES (EDF), Eric Fayolle (EDF)
#  Module : KERNEL
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Header to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = calcium_port_factory.hxx \
			CorbaTypes2CalciumTypes.hxx \
			CalciumTypes2CorbaTypes.hxx \
			CalciumGenericProvidesPort.hxx \
			calcium_uses_port.hxx \
			CalciumGenericUsesPort.hxx \
			calcium_provides_port.hxx \
			calcium_intc_port_uses.hxx \
			calcium_long_port_uses.hxx \
			calcium_real_port_uses.hxx \
			calcium_double_port_uses.hxx \
			calcium_string_port_uses.hxx \
			calcium_logical_port_uses.hxx \
			calcium_complex_port_uses.hxx \
			CalciumCouplingPolicy.hxx \
			CalciumException.hxx \
			CalciumTypes.hxx \
			CalciumCxxInterface.hxx \
			CalciumMacroCInterface.hxx \
			CalciumCInterface.hxx \
			CalciumInterface.hxx \
			Copy2UserSpace.hxx \
			Copy2CorbaSpace.hxx \
			CalciumPortTraits.hxx \
			calcium.h \
			calciumf.h \
			Calcium.hxx \
			calciumP.h \
			calciumE.h \
			version.h \
			calcium.hf

nodist_salomeinclude_HEADERS = calcium_integer_port_uses.hxx CalciumProvidesPort.hxx CalciumFortranInt.h

EXTRA_DIST=fortoc.h calcium_integer_port_uses.hxx.in CalciumProvidesPort.hxx.in CalciumFortranInt.h.in

calcium_integer_port_uses.hxx: calcium_integer_port_uses.hxx.in
CalciumProvidesPort.hxx     : CalciumProvidesPort.hxx.in
CalciumFortranInt.h      : CalciumFortranInt.h.in

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS= -I$(top_srcdir)/src/DSC/DSC_User \
		 -I$(top_srcdir)/src/DSC/DSC_User/Datastream \
		 -I$(top_srcdir)/src/DSC/DSC_User/Datastream/Palm \
		 -I$(top_srcdir)/src/DSC/DSC_User/Datastream/Calcium \
		 -I$(top_srcdir)/src/DSC/DSC_User/Basic \
		 -I$(top_srcdir)/src/DSC/DSC_Basic \
		 -I$(top_srcdir)/src/SALOMELocalTrace \
		 -I$(top_srcdir)/src/Basics \
		 -I$(top_srcdir)/src/Utils \
		 -I$(top_srcdir)/src/Container \
		 -I$(top_srcdir)/src/Notification \
		 -I$(top_builddir)/idl \
		 -I$(top_builddir)/src/DSC/DSC_User/Datastream/Calcium \
		 @CORBA_CXXFLAGS@ @CORBA_INCLUDES@ @BOOST_CPPFLAGS@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS = @CORBA_LIBS@

#
# ===============================================================
# Libraries targets
# ===============================================================
#
noinst_LTLIBRARIES = libSalomeCalcium.la 

libSalomeCalcium_la_SOURCES  = calcium_port_factory.cxx \
			       calcium_uses_port.cxx	\
			       calcium_provides_port.cxx\
			       calcium_repository_types.cxx \
			       CalciumCouplingPolicy.cxx \
			       CorbaTypes2CalciumTypes.cxx \
			       CalciumCxxInterface.cxx \
			       CalciumTypes2CorbaTypes.cxx \
                               CalciumCInterface.cxx \
			       CalciumProvidesPort.cxx \
			       Calcium.cxx \
			       calcium_destructors_port_uses.cxx

libSalomeCalcium_la_CXXFLAGS = $(COMMON_CPPFLAGS)

libSalomeCalcium_la_LDFLAGS  = -no-undefined -version-info=0:0:0

AM_CFLAGS         = -fexceptions
lib_LTLIBRARIES = libCalciumC.la
libCalciumC_la_SOURCES = Calcium.c Calcium.cxx calciumf.c
libCalciumC_la_CXXFLAGS = $(COMMON_CPPFLAGS)

#
# ===============================================================
# Executables targets
# ===============================================================
#
noinst_PROGRAMS              = test_DataIdContainer
## testInterpolation
## testInterpolation_SOURCES = testInterpolation.cxx
## testInterpolation_CXXFLAGS = -I/usr/include

test_DataIdContainer_SOURCES = test_DataIdContainer.cxx 

test_DataIdContainer_CXXFLAGS = $(COMMON_CPPFLAGS)

test_DataIdContainer_LDADD   = libSalomeCalcium.la $(top_builddir)/src/Utils/libOpUtil.la

test_DataIdContainer_LDFLAGS = $(COMMON_LIBS)

CLEANFILES = calcium_integer_port_uses.hxx CalciumProvidesPort.hxx CalciumFortranInt.h
