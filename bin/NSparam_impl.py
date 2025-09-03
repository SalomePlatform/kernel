#! /usr/bin/env python3
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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

import os

def getNSparams(info=""):
    """
    check environment for omniORB configuration file.
    parse the file to find the line defining naming service  host and port,
    set environment variables NSPORT and NSHOST,
    get host and port,
    if   info==host print host
    elif info==port print host
    else    print 2 strings on stdout on one line: host port
    """
    from salome.kernel.salome_utils import getORBcfgInfo
    _, my_host, my_port = getORBcfgInfo()
    if info=='host':
        os.environ['NSHOST'] = my_host
        return my_host
        pass
    elif info=='port':
        os.environ['NSPORT'] = my_port
        return my_port
        pass
    else:
        return my_host, my_port
    pass
#
