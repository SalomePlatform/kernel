# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

# -* Makefile *- 
# Author : Renaud Barate (EDF R&D)
# Module : KERNEL
# $Header$

include $(top_srcdir)/salome_adm/unix/make_common_starter.am
appliskeldir = $(salomescriptdir)/appliskel

#
# ===============================================================
# Files to be installed
# ===============================================================
#

# These files are executable scripts
dist_appliskel_SCRIPTS = \
    envd                \
    searchFreePort.sh   \
    runRemote.sh        \
    runAppli            \
    runConsole          \
    runSession          \
    runSalomeScript     \
    runTests            \
    .bashrc

# These files are python files
dist_appliskel_PYTHON = \
    getAppliPath.py             \
    update_catalogs.py          \
    kill_remote_containers.py

install-data-hook:
	@for f in $(dist_appliskel_PYTHON) ; do \
        chmod -f a+x $(DESTDIR)$(appliskeldir)/$$f ; \
    done
