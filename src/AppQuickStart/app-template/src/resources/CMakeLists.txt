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

INCLUDE(UseQtExt)

# --- resources ---

# resource files / to be processed by lrelease
SET(_ts_RESOURCES
  LightApp_images.ts
  LightApp_msg_en.ts
  LightApp_msg_fr.ts
  LightApp_msg_ja.ts
) 

# --- rules ---

QT_INSTALL_TS_RESOURCES("${_ts_RESOURCES}" "${SALOME_[NAME_OF_APPLICATION]_PROFILE_INSTALL_RES_DATA}")
