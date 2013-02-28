# Copyright (C) 2012  CEA/DEN, EDF R&D, OPEN CASCADE
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

ADD_SUBDIRECTORY(deprecated)

# ===============================================================
# Files to be installed
# ===============================================================

# These files are data, module or lib files
SET(DATA
  FindCPPUNIT.cmake
  FindKERNEL.cmake
  FindOMNIORB.cmake
  UseOMNIORB.cmake
  FindPLATFORM.cmake
  FindSPHINX.cmake
  FindSalomeBOOST.cmake
  FindSalomeCPPUNIT.cmake
  FindSalomeDOXYGEN.cmake
  FindSalomeHDF5.cmake
  FindSalomeLIBBATCH.cmake
  FindSalomeLIBXML2.cmake
  FindSalomeMPI.cmake
  FindSalomePYTHON.cmake
  FindSalomeSPHINX.cmake
  FindSalomeSWIG.cmake
  InstallAndCompilePythonFile.cmake
  SalomeMacros.cmake
)
INSTALL(FILES ${DATA} DESTINATION ${KERNEL_salomeadmcmake})

# These files are executable scripts
SALOME_INSTALL_SCRIPTS(prepare_generating_doc.py ${KERNEL_salomescript_SCRIPTS})
