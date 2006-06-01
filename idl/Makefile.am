# Copyright (C) 2005  OPEN CASCADE, CEA, EDF R&D, LEG
#           PRINCIPIA R&D, EADS CCR, Lip6, BV, CEDRAT
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either 
# version 2.1 of the License.
# 
# This library is distributed in the hope that it will be useful 
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

#
# This Makefile is responsible of generating the client and server
# implementation of IDL interfaces for both C++ and python usage.
# The building process of the C++ files is in charge of each source
# package and then is not manage here.
#

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

BASEIDL_FILES = \
  SALOME_Exception.idl \
  SALOME_Comm.idl \
  SALOME_ModuleCatalog.idl \
  SALOME_RessourcesCatalog.idl \
  SALOMEDS.idl \
  SALOMEDS_Attributes.idl \
  SALOME_Component.idl \
  SALOME_ContainerManager.idl \
  SALOME_TestComponent.idl \
  SALOME_Registry.idl \
  Logger.idl \
  SALOME_GenericObj.idl \
  SALOME_Session.idl \
  SALOME_TestModuleCatalog.idl \
  nstest.idl

MPIIDL_FILES = \
  SALOME_MPIObject.idl \
  SALOME_MPIContainer.idl \
  SALOME_TestMPIComponent.idl  

IDL_FILES = $(BASEIDL_FILES)
if MPI_IS_OK
  IDL_FILES += $(MPIIDL_FILES)
endif

# all the idl files are needed for make dist
EXTRA_DIST= $(BASEIDL_FILES) $(MPIIDL_FILES)

# This variable defines the files to be installed
salomeidl_DATA = $(IDL_FILES)

# Sources built from idl files
# BUILT_SOURCE = first thing done on make all
# idl dependencies are checked and rebuilt first

BASEIDL_SOURCES =\
  SALOME_ExceptionSK.cc \
  SALOME_CommSK.cc \
  SALOME_ModuleCatalogSK.cc \
  SALOME_RessourcesCatalogSK.cc \
  SALOMEDSSK.cc \
  SALOMEDS_AttributesSK.cc \
  SALOME_ComponentSK.cc \
  SALOME_ContainerManagerSK.cc \
  SALOME_TestComponentSK.cc \
  SALOME_RegistrySK.cc \
  LoggerSK.cc \
  SALOME_GenericObjSK.cc \
  SALOME_SessionSK.cc \
  SALOME_TestModuleCatalogSK.cc \
  nstestSK.cc

MPIIDL_SOURCES = \
  SALOME_MPIObjectSK.cc \
  SALOME_MPIContainerSK.cc \
  SALOME_TestMPIComponentSK.cc  

IDL_SOURCES = $(BASEIDL_SOURCES)
if MPI_IS_OK
  IDL_SOURCES += $(MPIIDL_SOURCES)
endif

BUILT_SOURCES = $(IDL_SOURCES)

lib_LTLIBRARIES = libSalomeIDLKernel.la
nodist_libSalomeIDLKernel_la_SOURCES = $(BUILT_SOURCES)
libSalomeIDLKernel_la_CPPFLAGS =\
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@
libSalomeIDLKernel_la_LDFLAGS = -no-undefined -version-info=0:0:0
libSalomeIDLKernel_la_LIBADD  = @CORBA_LIBS@

# These variables defines the building process of CORBA files
OMNIORB_IDL         = @OMNIORB_IDL@
OMNIORB_IDLCXXFLAGS = @OMNIORB_IDLCXXFLAGS@
OMNIORB_IDLPYFLAGS  = @OMNIORB_IDLPYFLAGS@ -I$(top_builddir)/salome/idl
IDLCXXFLAGS = -bcxx @IDLCXXFLAGS@ -I$(top_builddir)/salome/idl
IDLPYFLAGS  = @IDLPYFLAGS@

# potential problem on parallel make on the following - multiple outputs
SUFFIXES = .idl .hh SK.cc
.idlSK.cc:
	$(OMNIORB_IDL) $(IDLCXXFLAGS) $(OMNIORB_IDLCXXFLAGS) $<
.idl.hh:
	$(OMNIORB_IDL) $(IDLCXXFLAGS) $(OMNIORB_IDLCXXFLAGS) $<


install-exec-local: $(IDL_FILES:%=$(top_srcdir)/idl/%)
	$(INSTALL) -d  $(pkgpythondir)
	ls $^ | while read file; do \
	  $(OMNIORB_IDL) $(IDLPYFLAGS) -C$(pkgpythondir) $$file ; \
	done

# uninstall-local removes too much, but it works in distcheck
uninstall-local:
	rm -rf $(pkgpythondir)/*

mostlyclean-local:
	-rm -f *.hh *.cc .depidl

# we use cpp to generate dependencies between idl files.
# option x c tells the preprocessor to consider idl as a c file.
# if an idl is modified, all idl dependencies are rebuilt

.depidl: $(IDL_FILES)
	@echo "" > $@
	@for dep in $^ dummy; do \
	  if [ $$dep != "dummy" ]; then \
	    echo Building dependencies for $$dep; \
	    $(CPP) $(C_DEPEND_FLAG) -x c -I$(srcdir) $$dep 2>/dev/null | \
	    sed 's/\.o/\SK.cc/' >>$@; \
	  fi; \
	done ;

-include .depidl

