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
SET(KERNEL_CXXFLAGS -I${KERNEL_ROOT_DIR}/include/salome)

FIND_LIBRARY(DF DF ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(Launcher Launcher ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(OpUtil OpUtil ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(Registry Registry ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(ResourcesManager ResourcesManager ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SALOMEBasics SALOMEBasics ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeBatch SalomeBatch ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeCatalog SalomeCatalog ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeCommunication SalomeCommunication ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeContainer SalomeContainer ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeDSCContainer SalomeDSCContainer ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeDSClient SalomeDSClient ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeDSImpl SalomeDSImpl ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeDS SalomeDS ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeGenericObj SalomeGenericObj ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeHDFPersist SalomeHDFPersist ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeIDLKernel SalomeIDLKernel ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeLauncher SalomeLauncher ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeLifeCycleCORBA SalomeLifeCycleCORBA ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SALOMELocalTrace SALOMELocalTrace ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeLoggerServer SalomeLoggerServer ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeNotification SalomeNotification ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeNS SalomeNS ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(SalomeResourcesManager SalomeResourcesManager ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(TOOLSDS TOOLSDS ${KERNEL_ROOT_DIR}/lib/salome)
FIND_LIBRARY(with_loggerTraceCollector with_loggerTraceCollector ${KERNEL_ROOT_DIR}/lib/salome)
