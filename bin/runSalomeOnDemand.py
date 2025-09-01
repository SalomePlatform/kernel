#!/usr/bin/env python3
# -*- coding:utf-8 -*-
# Copyright (C) 2022-2025  CEA, EDF, OPEN CASCADE
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
# See https://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

#  File   : runSalomeOnDemand.py
#  Author : Konstantin Leontev, Open Cascade
#
## @package runSalomeOnDemand
# \brief Module that provides services to launch SALOME with custom set of modules
#

"""Run SALOME app in the context of adding modules as extensions.
"""

import sys
from runSalomeOnDemand_impl import set_ext_env, logger

if __name__ == "__main__":
    if len(sys.argv) == 3:
        arg_1, arg_2 = sys.argv[1:]
        set_ext_env(arg_1, arg_2)
    else:
        logger.error('You must provide all the arguments!')
        logger.info(set_ext_env.__doc__)
