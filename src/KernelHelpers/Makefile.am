# Copyright (C) 2010-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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
# Author : Guillaume Boulant (EDF) 
#

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

# header files 
salomeinclude_HEADERS =                \
	SALOME_KernelServices.hxx      \
	SALOME_StudyEditor.hxx         \
	SALOMEDS_DriverDefaultImpl.hxx \
	KernelHelpers.hxx

# Libraries targets
lib_LTLIBRARIES = libSalomeKernelHelpers.la

dist_libSalomeKernelHelpers_la_SOURCES =    \
	SALOME_KernelServices.cxx           \
	SALOME_StudyEditor.cxx              \
	SALOMEDS_DriverDefaultImpl.cxx


OMNIORB_CXXFLAGS=@OMNIORB_CXXFLAGS@ @OMNIORB_INCLUDES@
OMNIORB_LIBS=@OMNIORB_LIBS@

libSalomeKernelHelpers_la_CPPFLAGS =    \
	$(OMNIORB_CXXFLAGS)             \
	-I$(srcdir)/../NamingService    \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../Basics           \
	-I$(srcdir)/../Utils            \
	-I$(srcdir)/../LifeCycleCORBA   \
	-I$(srcdir)/../Container        \
	-I$(srcdir)/../Notification     \
	-I$(srcdir)/../GenericObj       \
	-I$(top_builddir)/idl

libSalomeKernelHelpers_la_LDFLAGS  =                 \
	$(OMNIORB_LIBS)                              \
	../NamingService/libSalomeNS.la              \
	../SALOMELocalTrace/libSALOMELocalTrace.la   \
	../Basics/libSALOMEBasics.la                 \
	../Utils/libOpUtil.la                        \
	../LifeCycleCORBA/libSalomeLifeCycleCORBA.la \
	../Container/libSalomeContainer.la           \
	../Notification/libSalomeNotification.la     \
	../GenericObj/libSalomeGenericObj.la         \
	$(top_builddir)/idl/libSalomeIDLKernel.la

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = KernelHelpersUseCases
KernelHelpersUseCases_SOURCES  = KernelHelpersUseCases.cxx
KernelHelpersUseCases_CPPFLAGS =\
	$(libSalomeKernelHelpers_la_CPPFLAGS)

KernelHelpersUseCases_LDADD    = \
	libSalomeKernelHelpers.la \
	$(libSalomeKernelHelpers_la_LDFLAGS)
