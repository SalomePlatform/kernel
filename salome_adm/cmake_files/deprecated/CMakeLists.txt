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

# ===============================================================
# Files to be installed
# ===============================================================

# These files are data, module or lib files
SET(DATA
  am2cmake.py
  FindBOOST.cmake
  FindDOXYGEN.cmake
  FindHDF5.cmake
  FindKERNEL.cmake
  FindLIBXML2.cmake
  FindMPI.cmake
  FindOMNIORB.cmake
  UseOMNIORB.cmake
  FindPLATFORM.cmake
  FindPTHREADS.cmake
  FindPYTHON.cmake
  FindSWIG.cmake
  install_python_from_idl.cmake
  install_and_compile_python_file.cmake
)
INSTALL(FILES ${DATA} DESTINATION ${SALOME_INSTALL_CMAKE}/deprecated)
