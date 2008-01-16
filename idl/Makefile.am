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

AM_CXXFLAGS = -fPIC

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
  nstest.idl \
  DSC_Engines.idl \
  SALOME_Ports.idl \
  Calcium_Ports.idl \
  Palm_Ports.idl

MPIIDL_FILES = \
  SALOME_MPIObject.idl \
  SALOME_MPIContainer.idl \
  SALOME_TestMPIComponent.idl  

IDL_FILES = $(BASEIDL_FILES)
if MPI_IS_OK
  IDL_FILES += $(MPIIDL_FILES)
endif

# all the idl files are needed for make dist
EXTRA_DIST= $(BASEIDL_FILES) $(MPIIDL_FILES) \
	    SALOME_Component.xml DSC_Engines.xml SALOME_Ports.xml

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
  nstestSK.cc \
  DSC_EnginesSK.cc \
  SALOME_PortsSK.cc \
  Calcium_PortsSK.cc \
  Palm_PortsSK.cc

DYNIDL_SOURCES = \
  SALOME_PortsDynSK.cc  Calcium_PortsDynSK.cc     SALOME_ContainerManagerDynSK.cc \
  SALOME_CommDynSK.cc   SALOME_RegistryDynSK.cc   SALOME_ModuleCatalogDynSK.cc \
  SALOMEDSDynSK.cc      SALOME_SessionDynSK.cc    SALOME_RessourcesCatalogDynSK.cc \
  DSC_EnginesDynSK.cc   SALOME_ComponentDynSK.cc  SALOME_GenericObjDynSK.cc \
  Palm_PortsDynSK.cc    SALOME_ExceptionDynSK.cc  SALOMEDS_AttributesDynSK.cc \
  LoggerDynSK.cc

MPIIDL_SOURCES = \
  SALOME_MPIObjectSK.cc \
  SALOME_MPIContainerSK.cc \
  SALOME_TestMPIComponentSK.cc  

IDL_SOURCES = $(BASEIDL_SOURCES)
if MPI_IS_OK
  IDL_SOURCES += $(MPIIDL_SOURCES)
endif


STATIDL_SOURCES=$(BASEIDL_SOURCES)
if MPI_IS_OK
  STATIDL_SOURCES += $(MPIIDL_SOURCES)
endif

IDL_SOURCES += $(DYNIDL_SOURCES)

$(DYNIDL_SOURCES) : $(IDL_SOURCES)

# also install all generated headers to use result library from another modules
salomeinclude_DATA= $(STATIDL_SOURCES:%SK.cc=%.hh)

if WITH_PACO_PARALLEL
PAR = SALOME_ExceptionPaCO.h SALOME_ExceptionPaCO.cc \
      SALOME_ComponentPaCO.h SALOME_ComponentPaCO.cc \
      SALOME_PortsPaCO.h SALOME_PortsPaCO.cc \
      DSC_EnginesPaCO.h DSC_EnginesPaCO.cc 

PAR_LIB = libSalomeParallelIDLKernel.la

IDL_PACO      = SALOME_ExceptionPaCO.idl SALOME_ComponentPaCO.idl SALOME_PortsPaCO.idl DSC_EnginesPaCO.idl

GEN_PACO = SALOME_ComponentPaCO_Engines_Container_server.cc \
	   SALOME_ComponentPaCO_Engines_Container_client.cc \
	   SALOME_ComponentPaCO_Engines_Component_server.cc \
	   SALOME_ComponentPaCO_Engines_Component_client.cc \
	   SALOME_ComponentPaCO_Engines_Parallel_Component_server.cc \
	   SALOME_ComponentPaCO_Engines_Parallel_Component_client.cc \
	   SALOME_ComponentPaCO_Engines_fileTransfer_server.cc \
	   SALOME_ComponentPaCO_Engines_fileTransfer_client.cc \
	   SALOME_ComponentPaCO_Engines_Salome_file_server.cc \
	   SALOME_ComponentPaCO_Engines_Salome_file_client.cc \
	   SALOME_ComponentPaCO_Engines_Parallel_Salome_file_server.cc \
	   SALOME_ComponentPaCO_Engines_Parallel_Salome_file_client.cc \
	   SALOME_PortsPaCO_Ports_Port_server.cc \
	   SALOME_PortsPaCO_Ports_Port_client.cc \
	   DSC_EnginesPaCO_Engines_DSC_server.cc \
	   DSC_EnginesPaCO_Engines_DSC_client.cc \
	   DSC_EnginesPaCO_Engines_Superv_Component_server.cc \
	   DSC_EnginesPaCO_Engines_Superv_Component_client.cc \
	   DSC_EnginesPaCO_Engines_Parallel_DSC_server.cc \
	   DSC_EnginesPaCO_Engines_Parallel_DSC_client.cc

INCLUDES_PACO = SALOME_ComponentPaCO_Engines_Container_server.h \
		SALOME_ComponentPaCO_Engines_Container_client.h \
                SALOME_ComponentPaCO_Engines_Component_server.h \
		SALOME_ComponentPaCO_Engines_Component_client.h \
                SALOME_ComponentPaCO_Engines_Parallel_Component_server.h \
		SALOME_ComponentPaCO_Engines_Parallel_Component_client.h \
		SALOME_ComponentPaCO_Engines_fileTransfer_server.h \
		SALOME_ComponentPaCO_Engines_fileTransfer_client.h \
		SALOME_ComponentPaCO_Engines_Salome_file_server.h \
		SALOME_ComponentPaCO_Engines_Salome_file_client.h \
		SALOME_ComponentPaCO_Engines_Parallel_Salome_file_server.h \
		SALOME_ComponentPaCO_Engines_Parallel_Salome_file_client.h \
		SALOME_ExceptionPaCO.h \
		SALOME_ComponentPaCO.h \
		SALOME_Exception.h \
		SALOME_Component.h \
		SALOME_PortsPaCO_Ports_Port_server.h \
		SALOME_PortsPaCO_Ports_Port_client.h \
		SALOME_PortsPaCO.h \
		SALOME_Ports.h \
		DSC_EnginesPaCO_Engines_DSC_server.h \
		DSC_EnginesPaCO_Engines_DSC_client.h \
                DSC_EnginesPaCO_Engines_Superv_Component_server.h \
		DSC_EnginesPaCO_Engines_Superv_Component_client.h \
		DSC_EnginesPaCO_Engines_Parallel_DSC_server.h \
		DSC_EnginesPaCO_Engines_Parallel_DSC_client.h \
		DSC_EnginesPaCO.h \
		DSC_Engines.h

XML           = SALOME_Exception.xml SALOME_Component.xml DSC_Engines.xml SALOME_Ports.xml
endif

idldir = $(prefix)/idl/salome
nodist_idl_DATA = ${XML} ${IDL_PACO} 
nodist_salomeinclude_HEADERS = ${INCLUDES_PACO} 

BUILT_SOURCES = $(IDL_SOURCES) $(PAR)

SALOME_ExceptionPaCO.h SALOME_ExceptionPaCO.cc : SALOME_Exception.idl SALOME_Exception.xml
SALOME_ComponentPaCO.h SALOME_ComponentPaCO.cc : SALOME_Component.idl SALOME_Component.xml
SALOME_PortsPaCO.h SALOME_PortsPaCO.cc : SALOME_Ports.idl SALOME_Ports.xml
DSC_EnginesPaCO.h DSC_EnginesPaCO.cc : DSC_Engines.idl DSC_Engines.xml

lib_LTLIBRARIES = libSalomeIDLKernel.la $(PAR_LIB)

nodist_libSalomeIDLKernel_la_SOURCES = $(IDL_SOURCES)
libSalomeIDLKernel_la_CPPFLAGS =-I$(top_builddir)/salome_adm/unix \
				-I$(top_builddir)/idl \
				@CORBA_CXXFLAGS@ @CORBA_INCLUDES@
libSalomeIDLKernel_la_LDFLAGS = -no-undefined -version-info=0:0:0
libSalomeIDLKernel_la_LIBADD  = @CORBA_LIBS@

nodist_libSalomeParallelIDLKernel_la_SOURCES = $(PAR) $(GEN_PACO)
libSalomeParallelIDLKernel_la_CPPFLAGS =-I$(top_builddir)/idl \
					@CORBA_CXXFLAGS@ @CORBA_INCLUDES@ \
					@PACO_INCLUDES@
libSalomeParallelIDLKernel_la_LDFLAGS = -no-undefined -version-info=0:0:0
libSalomeParallelIDLKernel_la_LIBADD  = libSalomeIDLKernel.la @PACO_LIBS@

# These variables defines the building process of CORBA files
OMNIORB_IDL         = @OMNIORB_IDL@
OMNIORB_IDLCXXFLAGS = @OMNIORB_IDLCXXFLAGS@
OMNIORB_IDLPYFLAGS  = @OMNIORB_IDLPYFLAGS@ -I$(top_builddir)/salome/idl
IDLCXXFLAGS = -bcxx @IDLCXXFLAGS@ -Wba -I$(top_builddir)/salome/idl
IDLPYFLAGS  = @IDLPYFLAGS@

# potential problem on parallel make on the following - multiple outputs
SUFFIXES = .idl .hh SK.cc
.idlSK.cc:
	$(OMNIORB_IDL) $(IDLCXXFLAGS) $(OMNIORB_IDLCXXFLAGS) $<
.idl.hh:
	$(OMNIORB_IDL) $(IDLCXXFLAGS) $(OMNIORB_IDLCXXFLAGS) $<

%PaCO.h %PaCO.cc : %.idl %.xml
	@PACO_IDL@ -I $(srcdir):@PACOPATH@/idl $^ omniORB4 0

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

CLEANFILES = *PaCO* *.h

