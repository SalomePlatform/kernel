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

# ===============================================================
# Files to be installed
# ===============================================================

# These files are data, module or lib files
INSTALL(FILES CatalogModulePersonnel.xml_skel DESTINATION ${SALOME_KERNEL_INSTALL_RES_DATA})

SALOME_CONFIGURE_FILE(CatalogResources.xml.in CatalogResources.xml INSTALL ${SALOME_KERNEL_INSTALL_RES_DATA})

INSTALL(FILES channel.cfg SALOMEDS_Resources DESTINATION ${SALOME_KERNEL_INSTALL_RES_DATA})
SALOME_CONFIGURE_FILE(KERNELCatalog.xml.in KERNELCatalog.xml INSTALL ${SALOME_KERNEL_INSTALL_RES_DATA})




