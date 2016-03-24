# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

#
#  TODO: this file should be removed when switching to full CMake
#

SET(KERNEL_CXXFLAGS -I${KERNEL_ROOT_DIR}/include/salome) # to be removed
SET(KERNEL_INCLUDE_DIRS ${KERNEL_ROOT_DIR}/include/salome)

FIND_LIBRARY(KERNEL_CalciumC CalciumC ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_DF DF ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_Launcher Launcher ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_LifeCycleCORBATest LifeCycleCORBATest ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_NamingServiceTest NamingServiceTest ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_OpUtil OpUtil ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_Registry Registry ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_ResourcesManager ResourcesManager ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SALOMEBasics SALOMEBasics ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeCatalog SalomeCatalog ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeCommunication SalomeCommunication ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeContainer SalomeContainer ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeDatastream SalomeDatastream ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeDSCContainer SalomeDSCContainer ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeDSClient SalomeDSClient ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeDSCSupervBasic SalomeDSCSupervBasic ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeDSCSuperv SalomeDSCSuperv ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeDSImpl SalomeDSImpl ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SALOMEDSImplTest SALOMEDSImplTest ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeDS SalomeDS ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SALOMEDSTest SALOMEDSTest ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeGenericObj SalomeGenericObj ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeHDFPersist SalomeHDFPersist ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeIDLKernel SalomeIDLKernel ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeLauncher SalomeLauncher ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeLifeCycleCORBA SalomeLifeCycleCORBA ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SALOMELocalTrace SALOMELocalTrace ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SALOMELocalTraceTest SALOMELocalTraceTest ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeLoggerServer SalomeLoggerServer ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeMPIContainer SalomeMPIContainer ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeNotification SalomeNotification ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeNS SalomeNS ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeResourcesManager SalomeResourcesManager ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeTestComponentEngine SalomeTestComponentEngine ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeTestMPIComponentEngine SalomeTestMPIComponentEngine ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SALOMETraceCollectorTest SALOMETraceCollectorTest ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_TOOLSDS TOOLSDS ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_UtilsTest UtilsTest ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_with_loggerTraceCollector with_loggerTraceCollector ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(KERNEL_SalomeKernelHelpers SalomeKernelHelpers ${KERNEL_ROOT_DIR}/lib/salome)
