#  Copyright (C) 2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
			CalciumProvidesPort.hxx \
			calcium_integer_port_uses.hxx \
			calcium_real_port_uses.hxx \
			calcium_double_port_uses.hxx \
			calcium_string_port_uses.hxx \
			calcium_logical_port_uses.hxx \
			calcium_complex_port_uses.hxx \
			CalciumCouplingPolicy.hxx \
			CalciumException.hxx \
			CalciumTypes.hxx \
			CalciumInterface.hxx \
			Copy2UserSpace.hxx \
			Copy2CorbaSpace.hxx \
			CalciumPortTraits.hxx \
			calcium.h \
			calciumP.h \
			version.h \
			calcium.hf
#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS= -I$(top_srcdir)/src/DSC/DSC_User \
		 -I$(top_srcdir)/src/DSC/DSC_User/Datastream \
		 -I$(top_srcdir)/src/DSC/DSC_Basic \
		 -I$(top_srcdir)/src/SALOMELocalTrace \
		 -I$(top_srcdir)/src/Basics \
		 -I$(top_srcdir)/src/Utils \
		 -I$(top_builddir)/salome_adm/unix \
		 -I$(top_builddir)/idl \
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
			       CalciumTypes2CorbaTypes.cxx \
			       CalciumProvidesPort.cxx \
			       calcium_destructors_port_uses.cxx

libSalomeCalcium_la_CXXFLAGS = $(COMMON_CPPFLAGS)

libSalomeCalcium_la_LDFLAGS  = -no-undefined -version-info=0:0:0

AM_CFLAGS         = -fexceptions
lib_LTLIBRARIES = libCalciumC.la
libCalciumC_la_SOURCES = Calcium.c 

#
# ===============================================================
# Executables targets
# ===============================================================
#
noinst_PROGRAMS              = test_DataIdContainer
## testInterpolation

testInterpolation_SOURCES = testInterpolation.cxx
testInterpolation_CXXFLAGS = -I/usr/include

test_DataIdContainer_SOURCES = test_DataIdContainer.cxx 

test_DataIdContainer_CXXFLAGS = $(COMMON_CPPFLAGS)

test_DataIdContainer_LDADD   = libSalomeCalcium.la $(top_builddir)/src/Utils/libOpUtil.la

test_DataIdContainer_LDFLAGS = $(COMMON_LIBS)

