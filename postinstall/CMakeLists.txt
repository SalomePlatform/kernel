# Copyright (C) 2025  CEA, EDF
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

# The only goal of this file is to put execution of ConfigureRPathPostEx.cmake script
# at the end of the installation process. This is necessary to ensure that
# all the targets are already installed and all the paths are set correctly.
# NOTE: Now this script fails because it can't get property from targets.

# The only goal of this file is to run RPATH configuration on post install step.

include(${CONFIGURATION_ROOT_DIR}/cmake/ConfigureRPathPostEx.cmake)

# Pass the parent of SALOME_INSTALL_LIBS to process both lib/salome and lib/pythonx.x/site-packages
configure_rpath_post_install("${SALOME_INSTALL_BINS};${SALOME_INSTALL_LIBS}/..")

#patch_omniidl_gen_py_package( ${CMAKE_INSTALL_PREFIX} kernel )
