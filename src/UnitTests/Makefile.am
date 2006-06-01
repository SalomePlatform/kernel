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
# Files to be installed
# ===============================================================
#

# Executable scripts to be installed
dist_salomescript_SCRIPTS = UnitTests.py

#
# ===============================================================
# Executables targets
# ===============================================================
#

bin_PROGRAMS = UnitTests
UnitTests_SOURCES  = UnitTests.cxx
UnitTests_CPPFLAGS =\
	@CPPUNIT_INCLUDES@ \
	-I$(srcdir)/../Basics -I$(srcdir)/../Basics/Test \
	-I$(srcdir)/../SALOMELocalTrace -I$(srcdir)/../SALOMELocalTrace/Test \
	-I$(top_builddir)/salome_adm/unix

if CORBA_GEN
  UnitTests_CPPFLAGS +=\
	-DWITH_CORBA \
	-I$(srcdir)/../SALOMETraceCollector -I$(srcdir)/../SALOMETraceCollector/Test \
	-I$(srcdir)/../NamingService -I$(srcdir)/../NamingService/Test \
	-I$(srcdir)/../Utils -I$(srcdir)/../Utils/Test \
	-I$(srcdir)/../LifeCycleCORBA -I$(srcdir)/../LifeCycleCORBA/Test \
	-I$(srcdir)/../SALOMDESImpl -I$(srcdir)/../SALOMEDSImpl/Test \
	-I$(srcdir)/../SALOMDES -I$(srcdir)/../SALOMEDS/Test \
	-I$(top_builddir)/idl \
	-I$(srcdir)/../Registry \
	-I$(srcdir)/../Notification \
	-I$(srcdir)/../ResourcesManager \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@
endif

UnitTests_LDADD    =\
	@CPPUNIT_LIBS@ \
	../Basics/libSALOMEBasics.la \
	../SALOMELocalTrace/libSALOMELocalTrace.la ../SALOMELocalTrace/Test/libSALOMELocalTraceTest.la

if CORBA_GEN
  UnitTests_LDADD    +=\
	../Registry/libRegistry.la \
	../Notification/libSalomeNotification.la \
	../ResourcesManager/libSalomeResourcesManager.la \
	../NamingService/libSalomeNS.la ../NamingService/Test/libNamingServiceTest.la \
	../Container/libSalomeContainer.la \
	../SALOMETraceCollector/Test/libSALOMETraceCollectorTest.la \
	../Utils/libOpUtil.la ../Utils/Test/libUtilsTest.la \
	../LifeCycleCORBA/libSalomeLifeCycleCORBA.la ../LifeCycleCORBA/Test/libLifeCycleCORBATest.la \
	../SALOMEDSImpl/libSalomeDSImpl.la ../SALOMEDSImpl/Test/libSALOMEDSImplTest.la \
	../SALOMEDS/libSalomeDS.la ../SALOMEDS/Test/libSALOMEDSTest.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la\
	@CORBA_LIBS@
endif
