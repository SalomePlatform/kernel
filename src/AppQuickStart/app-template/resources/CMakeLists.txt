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

# --- resources ---
SET([NAME_OF_APPLICATION]_PROFILE_RESOURCES_FILES
  app_icon.png
  splash.png
  about.png
  icon_about.png
  app_logo.png
)

# --- rules ---
INSTALL(FILES ${[NAME_OF_APPLICATION]_PROFILE_RESOURCES_FILES} DESTINATION ${SALOME_[NAME_OF_APPLICATION]_PROFILE_INSTALL_RES_DATA})

# --- config ---
SALOME_CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/SalomeApp.xml.in ${CMAKE_CURRENT_BINARY_DIR}/SalomeApp.xml INSTALL ${SALOME_[NAME_OF_APPLICATION]_PROFILE_INSTALL_RES_DATA})

# --- sub-directories ---
ADD_SUBDIRECTORY( doc )
