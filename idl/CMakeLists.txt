# Copyright (C) 2012-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
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

INCLUDE(${CONFIGURATION_ROOT_DIR}/cmake/UseOmniORB.cmake)

INCLUDE_DIRECTORIES(
  ${OMNIORB_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

SALOME_CONFIGURE_FILE(Calcium_Ports.idl.in Calcium_Ports.idl)

SET(SalomeIDLKernel_IDLSOURCES
  SALOME_Exception.idl
  SALOME_Comm.idl
  SALOME_ModuleCatalog.idl
  SALOME_ResourcesManager.idl
  SALOME_Launcher.idl
  SALOMEDS.idl
  SALOMEDS_Attributes.idl
  SALOME_Component.idl
  SALOME_ContainerManager.idl
  SALOME_SDS.idl
  SALOME_Registry.idl
  Logger.idl
  SALOME_GenericObj.idl
  SALOME_Types.idl
  SALOME_Session.idl
  nstest.idl
  DSC_Engines.idl
  SALOME_Ports.idl
  SALOME_PyNode.idl
  Palm_Ports.idl
  SALOME_PACOExtension.idl
  SALOME_ParamPorts.idl
  SALOME_Parametric.idl
  SALOME_MPIObject.idl
  SALOME_TestComponent.idl
  SALOME_TestModuleCatalog.idl
  ${CMAKE_CURRENT_BINARY_DIR}/Calcium_Ports.idl
)

IF(SALOME_USE_MPI)
  SET(SalomeIDLKernel_IDLSOURCES ${SalomeIDLKernel_IDLSOURCES} SALOME_MPIContainer.idl SALOME_TestMPIComponent.idl)
ENDIF(SALOME_USE_MPI)

SET(IDL_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

SET(COMMON_FLAGS ${OMNIORB_DEFINITIONS})
SET(TYPE)
IF(WIN32)
  SET(COMMON_FLAGS "${COMMON_FLAGS} -DNOGDI")
  SET(TYPE STATIC)
ELSE(WIN32)
  SET(TYPE SHARED)
ENDIF(WIN32)

OMNIORB_ADD_MODULE(SalomeIDLKernel "${SalomeIDLKernel_IDLSOURCES}" "${IDL_INCLUDE_DIRS}")
ADD_DEFINITIONS(${COMMON_FLAGS})
TARGET_LINK_LIBRARIES(SalomeIDLKernel ${OMNIORB_LIBRARIES})

INSTALL(TARGETS SalomeIDLKernel EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

IF(SALOME_PACO_PARALLEL)

  SET(SalomeParallelIDLKernel_PACO_XML_FILES
    SALOME_Component.xml
    SALOME_Ports.xml
    DSC_Engines.xml
    SALOME_PACOExtension.xml
    SALOME_ParamPorts.xml
    SALOME_GenericObj.xml
  )

  SET(SalomeParallelIDLKernel_SALOME_Component_GEN_PACO_SOURCES
    SALOME_ComponentPaCO_Engines_Container_server.cxx
	SALOME_ComponentPaCO_Engines_Container_client.cxx
	SALOME_ComponentPaCO_Engines_EngineComponent_server.cxx
	SALOME_ComponentPaCO_Engines_EngineComponent_client.cxx
	SALOME_ComponentPaCO_Engines_fileTransfer_server.cxx
	SALOME_ComponentPaCO_Engines_fileTransfer_client.cxx
	SALOME_ComponentPaCO_Engines_Salome_file_server.cxx
	SALOME_ComponentPaCO_Engines_Salome_file_client.cxx
  )

  SET(SalomeParallelIDLKernel_SALOME_Ports_GEN_PACO_SOURCES
	SALOME_PortsPaCO_Ports_Port_server.cxx
	SALOME_PortsPaCO_Ports_Port_client.cxx
	SALOME_PortsPaCO_Ports_Data_Port_server.cxx
	SALOME_PortsPaCO_Ports_Data_Port_client.cxx
  )

  SET(SalomeParallelIDLKernel_DSC_Engines_GEN_PACO_SOURCES
	DSC_EnginesPaCO_Engines_DSC_server.cxx
	DSC_EnginesPaCO_Engines_DSC_client.cxx
	DSC_EnginesPaCO_Engines_Superv_Component_server.cxx
	DSC_EnginesPaCO_Engines_Superv_Component_client.cxx
  )

  SET(SalomeParallelIDLKernel_SALOME_PACOExtension_GEN_PACO_SOURCES
	SALOME_PACOExtensionPaCO_Engines_Parallel_Component_server.cxx
	SALOME_PACOExtensionPaCO_Engines_Parallel_Component_client.cxx
	SALOME_PACOExtensionPaCO_Engines_PACO_Container_server.cxx
	SALOME_PACOExtensionPaCO_Engines_PACO_Container_client.cxx
	SALOME_PACOExtensionPaCO_Engines_Parallel_Salome_file_server.cxx
	SALOME_PACOExtensionPaCO_Engines_Parallel_Salome_file_client.cxx
	SALOME_PACOExtensionPaCO_Engines_Parallel_DSC_server.cxx
	SALOME_PACOExtensionPaCO_Engines_Parallel_DSC_client.cxx
  )

  SET(SalomeParallelIDLKernel_SALOME_ParamPorts_GEN_PACO_SOURCES
	SALOME_ParamPortsPaCO_Ports_Param_Double_Port_server.cxx
	SALOME_ParamPortsPaCO_Ports_Param_Double_Port_client.cxx
  )

  SET(SalomeParallelIDLKernel_SALOME_GenericObj_GEN_PACO_SOURCES
	SALOME_GenericObjPaCO_SALOME_GenericObj_server.cxx
	SALOME_GenericObjPaCO_SALOME_GenericObj_client.cxx
  )

  SET(SalomeParallelIDLKernel_PAR_SOURCES)
  SET(SalomeParallelIDLKernel_GEN_PACO_SOURCES)
  FOREACH(input ${SalomeParallelIDLKernel_PACO_XML_FILES})
    GET_FILENAME_COMPONENT(base ${input} NAME_WE)
    SET(input ${CMAKE_CURRENT_SOURCE_DIR}/${input})
    SET(baseidl ${CMAKE_CURRENT_SOURCE_DIR}/${base}.idl)
    SET(basehdr ${CMAKE_CURRENT_BINARY_DIR}/${base}.hxx)
    SET(genidl ${CMAKE_CURRENT_BINARY_DIR}/${base}PaCO.idl)
    SET(parsrc ${CMAKE_CURRENT_BINARY_DIR}/${base}PaCO.cxx)
    SET(parhdr ${CMAKE_CURRENT_BINARY_DIR}/${base}PaCO.hxx)
    SET(genpacosrc)
    SET(genpacohdr)
    FOREACH(genpacosrcname ${SalomeParallelIDLKernel_${base}_GEN_PACO_SOURCES})
      GET_FILENAME_COMPONENT(genpacobase ${genpacosrcname} NAME_WE)
      LIST(APPEND genpacosrc ${CMAKE_CURRENT_BINARY_DIR}/${genpacosrcname})
      LIST(APPEND genpacohdr ${CMAKE_CURRENT_BINARY_DIR}/${genpacobase}.hxx)
    ENDFOREACH(genpacosrcname ${SalomeParallelIDLKernel_${base}_GEN_PACO_SOURCES})
    LIST(APPEND SalomeParallelIDLKernel_PAR_SOURCES ${parsrc})
    LIST(APPEND SalomeParallelIDLKernel_GEN_PACO_SOURCES ${genpacosrc})
    ADD_CUSTOM_COMMAND(OUTPUT ${genidl} ${parsrc} ${parhdr} ${genpacosrc} ${genpacohdr}
      COMMAND ${OMNIORB_IDL_COMPILER} -p${PACO_ROOT_DIR}/lib/python -bpaco -Wb${input},${CMAKE_CURRENT_SOURCE_DIR}:${PACO_ROOT_DIR}/idl ${baseidl}
      DEPENDS ${input} ${baseidl})
    INSTALL(FILES ${input} ${genidl} DESTINATION ${SALOME_INSTALL_IDLS})
    INSTALL(FILES ${basehdr} ${parhdr} ${genpacohdr} DESTINATION ${SALOME_INSTALL_HEADERS})
    IF(OMNIORB_PYTHON_BACKEND)
      SET(IDLPYFLAGS "-bpython -p${OMNIORB_PYTHON_BACKEND} -I${CMAKE_CURRENT_SOURCE_DIR} -I${PACO_ROOT_DIR}/idl")
      INSTALL(CODE "OMNIORB_COMPILE_IDL_FORPYTHON_ON_INSTALL( \"${OMNIORB_IDL_COMPILER}\" \"${IDLPYFLAGS}\" \"${genidl}\" \"${CMAKE_INSTALL_PREFIX}/\${INSTALL_PYIDL_DIR}\" )")
    ENDIF(OMNIORB_PYTHON_BACKEND)
  ENDFOREACH(input ${SalomeParallelIDLKernel_PACO_XML_FILES})

  SET(SalomeParallelIDLKernel_PAR_INCLUDES
    SALOME_Exception.hxx
    SALOME_GenericObj.hxx
    SALOMEDS.hxx
    SALOME_PyNode.hxx
  )

  FOREACH(output ${SalomeParallelIDLKernel_PAR_INCLUDES})
    GET_FILENAME_COMPONENT(base ${output} NAME_WE)
    SET(baseidl ${CMAKE_CURRENT_SOURCE_DIR}/${base}.idl)
    ADD_CUSTOM_COMMAND(OUTPUT ${output}
      COMMAND ${OMNIORB_IDL_COMPILER} -bcxx -Wbh=.hxx -Wbs=.cxx ${baseidl}
      DEPENDS ${baseidl})
  ENDFOREACH(output ${SalomeParallelIDLKernel_PAR_INCLUDES})
  ADD_CUSTOM_TARGET(BUILD_PAR_INCLUDES DEPENDS ${SalomeParallelIDLKernel_PAR_INCLUDES})

  INCLUDE_DIRECTORIES(${PACO_INCLUDE_DIRS})
  ADD_LIBRARY(SalomeParallelIDLKernel
    ${SalomeParallelIDLKernel_PAR_SOURCES}
    ${SalomeParallelIDLKernel_GEN_PACO_SOURCES}
  )
  ADD_DEPENDENCIES(SalomeParallelIDLKernel BUILD_PAR_INCLUDES)

  INSTALL(TARGETS SalomeParallelIDLKernel EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})
ENDIF()
