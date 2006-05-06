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
#  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
#
#
#
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : KERNEL
# $Header$

SUBDIR_BASE = \
  Basics \
  SALOMELocalTrace \
  CASCatch \
  HDFPersist

SUBDIR_CORBA = \
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
  LifeCycleCORBA \
  LifeCycleCORBA_SWIG \
  SALOMEDSClient \
  TOOLSDS \
  SALOMEDSImpl \
  SALOMEDS \
  KERNEL_PY \
  ModuleGenerator \
  Communication \
  Communication_SWIG

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

SUBDIR_BATCH= \
  Batch \
  Batch_SWIG

DIST_SUBDIRS = \
  $(SUBDIR_BASE) \
  $(SUBDIR_CORBA) \
  $(SUBDIR_MPI) \
  $(SUBDIR_BATCH) \
  $(SUBDIR_CPPUNIT_BASE) \
  $(SUBDIR_CPPUNIT_CORBA) \
  $(SUBDIR_CPPUNIT_GENERAL)

#
# The following packages are built under conditions.
# A condition is defined using the macro AM_CONDITIONAL (see the
# check_cppunit for example with CPPUNIT_IS_OK.
#

SUBDIRS = $(SUBDIR_BASE)

if CORBA_GEN
  SUBDIRS += $(SUBDIR_CORBA)
endif

if MPI_IS_OK
  SUBDIRS += $(SUBDIR_MPI)
endif

if WITH_BATCH
  SUBDIRS += $(SUBDIR_BATCH)
endif

if CPPUNIT_IS_OK
  SUBDIRS += $(SUBDIR_CPPUNIT_BASE)
  SUBDIRS += $(SUBDIR_CPPUNIT_CORBA)
  SUBDIRS += $(SUBDIR_CPPUNIT_GENERAL)
endif
