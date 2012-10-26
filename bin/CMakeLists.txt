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

ADD_SUBDIRECTORY(appliskel)

CONFIGURE_FILE(VERSION.in VERSION @ONLY)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/VERSION DESTINATION ${KERNEL_salomebin_BINS})

# ===============================================================
# Files to be installed
# ===============================================================

# These files are data, module or lib files
INSTALL(FILES config_appli.xml salome.launch DESTINATION ${KERNEL_salomescript_DATA})

# These files are executable scripts
SET(SCRIPTS
  appli_clean.sh
  appli_install.sh
  createAppli.sh
  runIDLparser
  runNS.sh
  runSalome
  runSalome.csh
  runSalome.ksh
  runSalome.bat
  waitNS.sh
  addToKillList.py
  appli_gen.py
  envSalome.py
  killSalome.py
  killSalomeWithPort.py
  launchConfigureParser.py
  launchSalome.py
  nameserver.py
  NSparam.py
  orbmodule.py
  ORBConfigFile.py
  runNS.py
  runSalome.py
  salomeConsole.py
  salome_session.py
  salome_utils.py
  searchFreePort.py
  server.py
  setenv.py
  showNS.py
  shutdownSalome.py
  virtual_salome.py
  waitContainers.py
  waitNS.py
  )
SALOME_INSTALL_SCRIPTS("${SCRIPTS}" ${KERNEL_salomescript_SCRIPTS})
