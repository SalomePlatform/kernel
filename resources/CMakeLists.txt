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
INSTALL(FILES CatalogModulePersonnel.xml_skel DESTINATION ${SALOME_KERNEL_INSTALL_RES_DATA})

CONFIGURE_FILE(CatalogResources.xml.in CatalogResources.xml @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/CatalogResources.xml DESTINATION ${SALOME_KERNEL_INSTALL_RES_DATA})

IF(NOT SALOME_LAUNCHER_ONLY)
  INSTALL(FILES channel.cfg SALOMEDS_Resources DESTINATION ${SALOME_KERNEL_INSTALL_RES_DATA})

  CONFIGURE_FILE(KERNELCatalog.xml.in KERNELCatalog.xml @ONLY)
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/KERNELCatalog.xml DESTINATION ${SALOME_KERNEL_INSTALL_RES_DATA})

ENDIF()




