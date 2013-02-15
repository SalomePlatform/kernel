# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
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

# This Makefile is responsible of generating the client and server
# implementation of IDL interfaces for both C++ and python usage.
# The building process of the C++ files is in charge of each source
# package and then is not manage here.
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

AM_CXXFLAGS += -fPIC

BASEIDL_FILES = \
  SALOME_Exception.idl \
  SALOME_Comm.idl \
  SALOME_ModuleCatalog.idl \
  SALOME_ResourcesManager.idl \
  SALOME_Launcher.idl \
  SALOMEDS.idl \
  SALOMEDS_Attributes.idl \
  SALOME_Component.idl \
  SALOME_ContainerManager.idl \
  SALOME_TestComponent.idl \
  SALOME_Registry.idl \
  Logger.idl \
  SALOME_GenericObj.idl \
  SALOME_Types.idl \
  SALOME_Session.idl \
  SALOME_TestModuleCatalog.idl \
  nstest.idl \
  DSC_Engines.idl \
  SALOME_Ports.idl \
  SALOME_PyNode.idl \
  Palm_Ports.idl \
  SALOME_PACOExtension.idl \
  SALOME_ParamPorts.idl \
  SALOME_Parametric.idl \
  SALOME_MPIObject.idl

MPIIDL_FILES = \
  SALOME_MPIContainer.idl \
  SALOME_TestMPIComponent.idl  

IDL_FILES = $(BASEIDL_FILES)
if MPI_IS_OK
  IDL_FILES += $(MPIIDL_FILES)
endif

OTHER_IDL_FILES = \
  TestNotif.idl \
  TypeData.idl

# all the idl files are needed for make dist
EXTRA_DIST += $(BASEIDL_FILES) $(MPIIDL_FILES) $(OTHER_IDL_FILES) \
	    SALOME_Component.xml DSC_Engines.xml SALOME_Ports.xml \
	    SALOME_PACOExtension.xml SALOME_ParamPorts.xml \
	    SALOME_GenericObj.xml SALOME_Exception.xml

# This variable defines the files to be installed
salomeidl_DATA        = $(IDL_FILES)
nodist_salomeidl_DATA = Calcium_Ports.idl

# Sources built from idl files
# BUILT_SOURCE = first thing done on make all
# idl dependencies are checked and rebuilt first

BASEIDL_SOURCES =\
  SALOME_ExceptionSK.cc \
  SALOME_CommSK.cc \
  SALOME_ModuleCatalogSK.cc \
  SALOME_ResourcesManagerSK.cc \
  SALOME_LauncherSK.cc \
  SALOMEDSSK.cc \
  SALOMEDS_AttributesSK.cc \
  SALOME_ComponentSK.cc \
  SALOME_ContainerManagerSK.cc \
  SALOME_TestComponentSK.cc \
  SALOME_RegistrySK.cc \
  LoggerSK.cc \
  SALOME_GenericObjSK.cc \
  SALOME_TypesSK.cc \
  SALOME_SessionSK.cc \
  SALOME_TestModuleCatalogSK.cc \
  nstestSK.cc \
  DSC_EnginesSK.cc \
  SALOME_PortsSK.cc \
  SALOME_PyNodeSK.cc \
  Calcium_PortsSK.cc \
  Palm_PortsSK.cc \
  SALOME_PACOExtensionSK.cc \
  SALOME_ParamPortsSK.cc \
  SALOME_ParametricSK.cc \
  SALOME_MPIObjectSK.cc

DYNIDL_SRCS = \
  SALOME_PortsDynSK.cc  Calcium_PortsDynSK.cc     SALOME_ContainerManagerDynSK.cc \
  SALOME_CommDynSK.cc   SALOME_RegistryDynSK.cc   SALOME_ModuleCatalogDynSK.cc \
  SALOMEDSDynSK.cc      SALOME_SessionDynSK.cc    SALOME_ResourcesManagerDynSK.cc SALOME_LauncherDynSK.cc \
  DSC_EnginesDynSK.cc   SALOME_ComponentDynSK.cc  SALOME_GenericObjDynSK.cc SALOME_TypesDynSK.cc \
  Palm_PortsDynSK.cc    SALOME_ExceptionDynSK.cc  SALOMEDS_AttributesDynSK.cc \
  LoggerDynSK.cc        SALOME_PACOExtensionDynSK.cc SALOME_ParamPortsDynSK.cc SALOME_PyNodeDynSK.cc \
  SALOME_ParametricDynSK.cc SALOME_MPIObjectDynSK.cc

MPIIDL_SOURCES = \
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

if MPI_IS_OK
  DYNIDL_SRCS += SALOME_MPIContainerDynSK.cc SALOME_TestMPIComponentDynSK.cc
endif

$(DYNIDL_SRCS) : $(IDL_SOURCES)

# also install all generated headers to use result library from another modules
salomeinclude_DATA= $(STATIDL_SOURCES:%SK.cc=%.hh)

if WITH_PACO_PARALLEL
PAR = SALOME_ComponentPaCO.hxx SALOME_ComponentPaCO.cxx \
      SALOME_PortsPaCO.hxx SALOME_PortsPaCO.cxx \
      DSC_EnginesPaCO.hxx DSC_EnginesPaCO.cxx \
      SALOME_PACOExtensionPaCO.hxx SALOME_PACOExtensionPaCO.cxx \
      SALOME_ParamPortsPaCO.hxx SALOME_ParamPortsPaCO.cxx \
      SALOME_GenericObjPaCO.hxx SALOME_GenericObjPaCO.cxx

PAR_INCLUDES = SALOME_Exception.hxx SALOME_GenericObj.hxx SALOMEDS.hxx SALOME_PyNode.hxx

PAR_LIB = libSalomeParallelIDLKernel.la

IDL_PACO = SALOME_ComponentPaCO.idl SALOME_PortsPaCO.idl DSC_EnginesPaCO.idl \
	   SALOME_ParamPortsPaCO.idl SALOME_PACOExtensionPaCO.idl \
	   SALOME_GenericObjPaCO.idl

GEN_PACO = SALOME_ComponentPaCO_Engines_Container_server.cxx \
	   SALOME_ComponentPaCO_Engines_Container_client.cxx \
	   SALOME_ComponentPaCO_Engines_EngineComponent_server.cxx \
	   SALOME_ComponentPaCO_Engines_EngineComponent_client.cxx \
	   SALOME_ComponentPaCO_Engines_fileTransfer_server.cxx \
	   SALOME_ComponentPaCO_Engines_fileTransfer_client.cxx \
	   SALOME_ComponentPaCO_Engines_Salome_file_server.cxx \
	   SALOME_ComponentPaCO_Engines_Salome_file_client.cxx \
	   SALOME_PortsPaCO_Ports_Port_server.cxx \
	   SALOME_PortsPaCO_Ports_Port_client.cxx \
	   SALOME_PortsPaCO_Ports_Data_Port_server.cxx \
	   SALOME_PortsPaCO_Ports_Data_Port_client.cxx \
	   DSC_EnginesPaCO_Engines_DSC_server.cxx \
	   DSC_EnginesPaCO_Engines_DSC_client.cxx \
	   DSC_EnginesPaCO_Engines_Superv_Component_server.cxx \
	   DSC_EnginesPaCO_Engines_Superv_Component_client.cxx \
	   SALOME_PACOExtensionPaCO_Engines_Parallel_Component_server.cxx \
	   SALOME_PACOExtensionPaCO_Engines_Parallel_Component_client.cxx \
	   SALOME_PACOExtensionPaCO_Engines_PACO_Container_server.cxx \
	   SALOME_PACOExtensionPaCO_Engines_PACO_Container_client.cxx \
	   SALOME_PACOExtensionPaCO_Engines_Parallel_Salome_file_server.cxx \
	   SALOME_PACOExtensionPaCO_Engines_Parallel_Salome_file_client.cxx \
	   SALOME_PACOExtensionPaCO_Engines_Parallel_DSC_server.cxx \
	   SALOME_PACOExtensionPaCO_Engines_Parallel_DSC_client.cxx \
	   SALOME_ParamPortsPaCO_Ports_Param_Double_Port_server.cxx \
	   SALOME_ParamPortsPaCO_Ports_Param_Double_Port_client.cxx \
	   SALOME_GenericObjPaCO_SALOME_GenericObj_server.cxx \
	   SALOME_GenericObjPaCO_SALOME_GenericObj_client.cxx

INCLUDES_PACO = SALOME_ComponentPaCO_Engines_Container_server.hxx \
		SALOME_ComponentPaCO_Engines_Container_client.hxx \
		SALOME_ComponentPaCO_Engines_EngineComponent_server.hxx \
		SALOME_ComponentPaCO_Engines_EngineComponent_client.hxx \
		SALOME_ComponentPaCO_Engines_fileTransfer_server.hxx \
		SALOME_ComponentPaCO_Engines_fileTransfer_client.hxx \
		SALOME_ComponentPaCO_Engines_Salome_file_server.hxx \
		SALOME_ComponentPaCO_Engines_Salome_file_client.hxx \
		SALOME_ComponentPaCO.hxx \
		SALOME_Component.hxx \
		SALOME_PortsPaCO_Ports_Port_server.hxx \
		SALOME_PortsPaCO_Ports_Port_client.hxx \
		SALOME_PortsPaCO_Ports_Data_Port_server.hxx \
		SALOME_PortsPaCO_Ports_Data_Port_client.hxx \
		SALOME_PortsPaCO.hxx \
		SALOME_Ports.hxx \
		DSC_EnginesPaCO_Engines_DSC_server.hxx \
		DSC_EnginesPaCO_Engines_DSC_client.hxx \
		DSC_EnginesPaCO_Engines_Superv_Component_server.hxx \
		DSC_EnginesPaCO_Engines_Superv_Component_client.hxx \
		DSC_EnginesPaCO.hxx \
		DSC_Engines.hxx \
		SALOME_PACOExtensionPaCO_Engines_Parallel_Component_server.hxx \
		SALOME_PACOExtensionPaCO_Engines_Parallel_Component_client.hxx \
		SALOME_PACOExtensionPaCO_Engines_PACO_Container_server.hxx \
		SALOME_PACOExtensionPaCO_Engines_PACO_Container_client.hxx \
		SALOME_PACOExtensionPaCO_Engines_Parallel_Salome_file_server.hxx \
		SALOME_PACOExtensionPaCO_Engines_Parallel_Salome_file_client.hxx \
		SALOME_PACOExtensionPaCO_Engines_Parallel_DSC_server.hxx \
		SALOME_PACOExtensionPaCO_Engines_Parallel_DSC_client.hxx \
		SALOME_PACOExtensionPaCO.hxx \
		SALOME_PACOExtension.hxx \
		SALOME_ParamPortsPaCO_Ports_Param_Double_Port_server.hxx \
		SALOME_ParamPortsPaCO_Ports_Param_Double_Port_client.hxx \
		SALOME_ParamPortsPaCO.hxx \
		SALOME_ParamPorts.hxx \
		SALOME_GenericObjPaCO_SALOME_GenericObj_server.hxx \
		SALOME_GenericObjPaCO_SALOME_GenericObj_client.hxx \
		SALOME_GenericObjPaCO.hxx \
		SALOME_GenericObj.hxx

XML           = SALOME_Component.xml DSC_Engines.xml SALOME_Ports.xml \
		SALOME_PACOExtension.xml SALOME_ParamPorts.xml \
		SALOME_GenericObj.xml
endif

idldir = $(prefix)/idl/salome
nodist_idl_DATA = ${XML} ${IDL_PACO} Calcium_Ports.idl
nodist_salomeinclude_HEADERS = ${INCLUDES_PACO} 

BUILT_SOURCES = $(IDL_SOURCES) $(PAR_INCLUDES) $(PAR)

SALOME_GenericObjPaCO.idl SALOME_GenericObjPaCO.hxx SALOME_GenericObjPaCO.cxx : SALOME_GenericObj.idl SALOME_GenericObj.xml
SALOME_ComponentPaCO.idl SALOME_ComponentPaCO.hxx SALOME_ComponentPaCO.cxx : SALOME_Component.idl SALOME_Component.xml SALOME_GenericObjPaCO.idl
SALOME_PortsPaCO.idl SALOME_PortsPaCO.hxx SALOME_PortsPaCO.cxx : SALOME_Ports.idl SALOME_Ports.xml
DSC_EnginesPaCO.idl DSC_EnginesPaCO.hxx DSC_EnginesPaCO.cxx : DSC_Engines.idl DSC_Engines.xml
SALOME_PACOExtensionPaCO.idl SALOME_PACOExtensionPaCO.hxx SALOME_PACOExtensionPaCO.cxx : SALOME_PACOExtension.idl SALOME_PACOExtension.xml
SALOME_ParamPortsPaCO.idl SALOME_ParamPortsPaCO.hxx SALOME_ParamPortsPaCO.cxx : SALOME_ParamPorts.idl SALOME_ParamPorts.xml

lib_LTLIBRARIES = libSalomeIDLKernel.la $(PAR_LIB)

nodist_libSalomeIDLKernel_la_SOURCES = $(IDL_SOURCES) $(DYNIDL_SRCS)
libSalomeIDLKernel_la_CPPFLAGS =-I$(top_builddir)/idl \
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
IDLCXXFLAGS = -bcxx @IDLCXXFLAGS@ -I$(srcdir) -I$(top_builddir)/salome/idl
IDLPYFLAGS  = @IDLPYFLAGS@ -I$(srcdir)

# potential problem on parallel make on the following - multiple outputs
SUFFIXES = .idl .hh SK.cc
.idlSK.cc:
	$(IDL) $(IDLCXXFLAGS) $<
SK.cc.hh:

#Dummy rule to replace the following one (for parallel make)
#.idl.hh:
#	$(IDL) $(IDLCXXFLAGS) $<

if WITH_PACO_PARALLEL

%PaCO.idl %PaCO.hxx %PaCO.cxx : %.idl %.xml
	$(IDL) -p@PACOPATH@/lib/python -bpaco -Wb$(top_srcdir)/idl/$*.xml,$(srcdir):@PACOPATH@/idl $(top_srcdir)/idl/$*.idl

.idl.hxx:
	$(IDL) $(PACOIDLCXXFLAGS) -bcxx -Wbh=.hxx -Wbs=.cxx $<
endif

install-exec-local: $(IDL_FILES:%=$(top_srcdir)/idl/%) Calcium_Ports.idl
	$(INSTALL) -d  $(DESTDIR)$(salomepythondir)
	ls $^ | while read file; do \
	  $(IDL) $(IDLPYFLAGS) -C$(DESTDIR)$(salomepythondir) $$file ; \
	done
if WITH_PACO_PARALLEL
	ls $(top_builddir)/idl/*.idl | while read file; do \
	  $(IDL) $(IDLPYFLAGS) -I$(top_srcdir)/idl -I@PACOPATH@/idl -C$(DESTDIR)$(salomepythondir) $$file ; \
	done
endif

# uninstall-local removes too much, but it works in distcheck
uninstall-local:
	for filen in `find $(DESTDIR)$(salomepythondir) -maxdepth 1` ; do \
	  case $${filen} in                                               \
	  $(DESTDIR)$(salomepythondir) )    ;;                            \
	  $(DESTDIR)$(salomepypkgdir) )     ;;                            \
	  $(DESTDIR)$(sharedpkgpythondir) ) ;;                            \
	  * ) rm -rf $${filen} ;;                                         \
	  esac ;                                                          \
	done ;

mostlyclean-local:
	-rm -f *.hh *.cc .depidl Calcium_Ports.idl

# we use cpp to generate dependencies between idl files.
# option x c tells the preprocessor to consider idl as a c file.
# if an idl is modified, all idl dependencies are rebuilt

.depidl: $(IDL_FILES) Calcium_Ports.idl
	@echo "" > $@
	@for dep in $^ dummy; do \
	  if [ $$dep != "dummy" ]; then \
	    echo Building dependencies for $$dep; \
	    $(CPP) $(C_DEPEND_FLAG) -x c -I$(srcdir) $$dep 2>/dev/null | \
	    sed 's/\.o/\SK.cc/' >>$@; \
	  fi; \
	done ;

-include .depidl

CLEANFILES = *PaCO* *.hxx *.cxx Calcium_Ports.idl


