# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

# Author: Guillaume Boulant (EDF/R&D)

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

lib_LTLIBRARIES = libSalomeKernelHelpersTest.la

salomeinclude_HEADERS = KernelHelpersUnitTests.hxx

dist_libSalomeKernelHelpersTest_la_SOURCES = KernelHelpersUnitTests.cxx

OMNIORB_CXXFLAGS=@OMNIORB_CXXFLAGS@ @OMNIORB_INCLUDES@
OMNIORB_LIBS=@OMNIORB_LIBS@

libSalomeKernelHelpersTest_la_CXXFLAGS =      \
	@CPPUNIT_INCLUDES@                    \
	$(OMNIORB_CXXFLAGS)                   \
	-I$(top_srcdir)/src/Basics/Test       \
	-I$(srcdir)/..                        \
	-I$(top_srcdir)/src/NamingService     \
	-I$(top_srcdir)/src/SALOMELocalTrace  \
	-I$(top_srcdir)/src/Basics            \
	-I$(top_srcdir)/src/Utils             \
	-I$(top_srcdir)/src/LifeCycleCORBA    \
	-I$(top_srcdir)/src/Container         \
	-I$(top_srcdir)/src/Notification      \
	-I$(top_srcdir)/src/GenericObj        \
	-I$(top_builddir)/idl


libSalomeKernelHelpersTest_la_LIBADD   =                            \
	@CPPUNIT_LIBS@                                              \
	../libSalomeKernelHelpers.la                            \
	$(OMNIORB_LIBS)                                             \
	$(top_builddir)/src/NamingService/libSalomeNS.la            \
	$(top_builddir)/src/SALOMELocalTrace/libSALOMELocalTrace.la \
	$(top_builddir)/src/Basics/libSALOMEBasics.la     \
	$(top_builddir)/src/Utils/libOpUtil.la            \
	$(top_builddir)/src/LifeCycleCORBA/libSalomeLifeCycleCORBA.la \
	$(top_builddir)/src/Container/libSalomeContainer.la \
	$(top_builddir)/src/Notification/libSalomeNotification.la \
	$(top_builddir)/src/GenericObj/libSalomeGenericObj.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la


bin_PROGRAMS = TestKernelHelpers

TestKernelHelpers_SOURCES = TestKernelHelpers.cxx

TestKernelHelpers_CXXFLAGS = $(libSalomeKernelHelpersTest_la_CXXFLAGS)
TestKernelHelpers_LDADD    = $(libSalomeKernelHelpersTest_la_LIBADD) libSalomeKernelHelpersTest.la

dist_salomescript_PYTHON = TestKernelHelpers.py
UNIT_TEST_PROG = TestKernelHelpers
