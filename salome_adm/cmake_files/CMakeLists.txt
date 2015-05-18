# Copyright (C) 2012-2015  CEA/DEN, EDF R&D, OPEN CASCADE
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

ADD_SUBDIRECTORY(deprecated)

# ===============================================================
# Files to be installed
# ===============================================================

# These files are data, module or lib files
SET(DATA
  FindCAS.cmake
  FindCppUnit.cmake
  FindGraphviz.cmake
  FindSalomeKERNEL.cmake
  FindOmniORB.cmake
  FindOmniORBPy.cmake
  FindPThread.cmake
  FindSalomeBoost.cmake
  FindSalomeCAS.cmake
  FindSalomeCppUnit.cmake
  FindSalomeDoxygen.cmake
  FindSalomeGraphviz.cmake
  FindSalomeHDF5.cmake
  FindSalomeLibBatch.cmake
  FindSalomeLibXml2.cmake
  FindSalomeMEDFile.cmake
  FindSalomeMPI.cmake
  FindSalomeOmniORB.cmake
  FindSalomeOmniORBPy.cmake
  FindSalomePThread.cmake
  FindSalomePython.cmake
  FindSalomeSphinx.cmake
  FindSalomeSWIG.cmake
  FindSphinx.cmake
  FindSalomeVTK.cmake
  FindMEDFile.cmake
  UseOmniORB.cmake
  SalomeMacros.cmake
  SalomeSetupPlatform.cmake
)
INSTALL(FILES ${DATA} DESTINATION ${SALOME_INSTALL_CMAKE})

# These files are executable scripts
SALOME_INSTALL_SCRIPTS(prepare_generating_doc.py ${SALOME_INSTALL_SCRIPT_SCRIPTS})
