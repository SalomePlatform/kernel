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
# $Header$
#
SUBDIR_BASE = \
  Basics \
  SALOMELocalTrace \
  HDFPersist\
  KERNEL_PY

SUBDIR_CORBA = \
  DF \
  Logger \
  SALOMETraceCollector \
  Utils \
  NamingService \
  GenericObj \
  Registry \
  ModuleCatalog \
  ResourcesManager \
  Notification \
  NOTIFICATION_SWIG \
  Container \
  TestContainer \
  Launcher \
  LifeCycleCORBA \
  LifeCycleCORBA_SWIG \
  SALOMEDSClient \
  TOOLSDS \
  SALOMEDSImpl \
  SALOMEDS \
  ModuleGenerator \
  Communication \
  Communication_SWIG \
  DSC

SUBDIR_CPPUNIT_BASE= \
  Basics/Test \
  SALOMELocalTrace/Test

SUBDIR_CPPUNIT_CORBA = \
  Logger/Test \
  SALOMETraceCollector/Test \
  Utils/Test \
  NamingService/Test \
  LifeCycleCORBA/Test \
  LifeCycleCORBA_SWIG/Test \
  SALOMEDSImpl/Test \
  SALOMEDS/Test

SUBDIR_CPPUNIT_GENERAL = \
  UnitTests

SUBDIR_MPI = \
  MPIContainer \
  TestMPIContainer

SUBDIR_PARALLEL= ParallelContainer
		 

DIST_SUBDIRS = \
  $(SUBDIR_BASE) \
  $(SUBDIR_CORBA) \
  $(SUBDIR_MPI) \
  $(SUBDIR_PARALLEL) \
  $(SUBDIR_CPPUNIT_BASE) \
  $(SUBDIR_CPPUNIT_CORBA) \
  $(SUBDIR_CPPUNIT_GENERAL)

#
# The following packages are built under conditions.
# A condition is defined using the macro AM_CONDITIONAL (see the
# check_cppunit for example with CPPUNIT_IS_OK.
#

if WITHONLYLAUNCHER
  SUBDIRS = Basics ResourcesManager Launcher
else

SUBDIRS = $(SUBDIR_BASE)

if CORBA_GEN
  SUBDIRS += $(SUBDIR_CORBA)
endif

if MPI_IS_OK
  SUBDIRS += $(SUBDIR_MPI)
endif

if CPPUNIT_IS_OK
if CORBA_GEN
  SUBDIRS += \
	$(SUBDIR_CPPUNIT_BASE)   \
	$(SUBDIR_CPPUNIT_CORBA)  \
	$(SUBDIR_CPPUNIT_GENERAL)
else
  SUBDIRS += \
	$(SUBDIR_CPPUNIT_BASE)   \
	$(SUBDIR_CPPUNIT_GENERAL)
endif
endif
if WITH_PACO_PARALLEL
  SUBDIRS += $(SUBDIR_PARALLEL)
endif

endif
EXTRA_DIST = win32pm
