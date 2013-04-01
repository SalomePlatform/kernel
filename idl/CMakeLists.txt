# Copyright (C) 2012-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/UseOMNIORB.cmake)

INCLUDE_DIRECTORIES(
  ${OMNIORB_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

CONFIGURE_FILE(Calcium_Ports.idl.in Calcium_Ports.idl @ONLY)

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
IF(WINDOWS)
  SET(COMMON_FLAGS "${COMMON_FLAGS} -DNOGDI")
  SET(TYPE STATIC)
ELSE(WINDOWS)
  SET(TYPE SHARED)
ENDIF(WINDOWS)

OMNIORB_ADD_MODULE(SalomeIDLKernel "${SalomeIDLKernel_IDLSOURCES}" "${IDL_INCLUDE_DIRS}" ${TYPE})
ADD_DEFINITIONS(${COMMON_FLAGS})
TARGET_LINK_LIBRARIES(SalomeIDLKernel ${OMNIORB_LIBRARIES})

INSTALL(TARGETS SalomeIDLKernel DESTINATION ${KERNEL_salomelib_LIBS})

IF(WITH_PACO_PARALLEL)
  SET(SalomeParallelIDLKernel_IDLSOURCES
    SALOME_ComponentPaCO.idl
    SALOME_PortsPaCO.idl
    DSC_EnginesPaCO.idl
    SALOME_ParamPortsPaCO.idl
    SALOME_PACOExtensionPaCO.idl
    SALOME_GenericObjPaCO.idl
  )
  OMNIORB_ADD_MODULE(SalomeParallelIDLKernel "${SalomeIDLKernel_IDLSOURCES}" "${IDL_INCLUDE_DIRS}")
  TARGET_LINK_LIBRARIES(SalomeParallelIDLKernel ${OMNIORB_LIBRARIES})

  INSTALL(TARGETS SalomeParallelIDLKernel DESTINATION ${KERNEL_salomelib_LIBS})
ENDIF(WITH_PACO_PARALLEL)
