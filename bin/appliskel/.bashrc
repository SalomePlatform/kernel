#!/bin/bash
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

# If not running interactively, don't do anything:

[ -z "$PS1" ] && return

# prefix prompt with naming server hostname and port
#export PS1='[NS='${NSHOST}':'${NSPORT}']'${PS1}

NO_COLOR="\[\033[0m\]"
RED="\[\033[0;31m\]"
DARK_RED="\[\033[1;31m\]"
GREEN="\[\033[0;32m\]"
DARK_GREEN="\[\033[1;32m\]"
BROWN="\[\033[0;33m\]"
DARK_BROWN="\[\033[1;33m\]"
BLUE="\[\033[0;34m\]"
DARK_BLUE="\[\033[1;34m\]"
PURPLE="\[\033[0;35m\]"
DARK_PURPLE="\[\033[1;35m\]"
CYAN="\[\033[0;36m\]"
DARK_CYAN="\[\033[1;36m\]"

NS_COLOR=${BROWN}
HOST_COLOR=${BLUE}
PORT_COLOR=${PURPLE}
DEFAULT_COLOR=${NO_COLOR}

export PS1="${DEFAULT_COLOR}[${NS_COLOR}NS=${HOST_COLOR}${NSHOST}${NS_COLOR}:${PORT_COLOR}${NSPORT}${NS_COLOR}${DEFAULT_COLOR}]\$ "
