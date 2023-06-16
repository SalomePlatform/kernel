#! /usr/bin/env python3
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2023  CEA/DEN, EDF R&D, OPEN CASCADE
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

## @file killSalome.py
#  @brief Forcibly stop all running %SALOME sessions.

"""
Forcibly stop all running SALOME sessions.

To stop all SALOME sessions, do the following:

* in shell:

    $ killSalome.py

* in Python script:

    from killSalome import killAllPorts
    killAllPorts()
"""

# pragma pylint: disable=invalid-name

import os
import os.path as osp
import re
import sys
from contextlib import suppress
from glob import glob

from killSalomeWithPort import getPiDict, killMyPort, killUnkilledProcesses

def killAllPorts():
    """
    Stop all running SALOME sessions owned by the current user.
    """
    # detect ports used by SALOME session and kill processes
    for hidden in (True, False):
        pidict_t = getPiDict(port='#####', hidden=hidden)
        dir_pidict_t = osp.dirname(pidict_t)
        fn_pidict_t = osp.basename(pidict_t).replace('#####', r'(\d*)')
        fn_rex = re.compile('^{}'.format(fn_pidict_t))
        with suppress(IOError):
            for fname in os.listdir(dir_pidict_t):
                m_res = fn_rex.match(fname)
                if m_res:
                    killMyPort(m_res.group(1))

    # kill possibly unkilled processes
    killUnkilledProcesses()

    # other clean-up actions
    # - delete uri files needed by ompi-server
    for path in glob(osp.join(osp.expanduser('~'), '.urifile_*')):
        with suppress(IOError):
            os.remove(path)

def main():
    """
    Main function
    """
    from argparse import ArgumentParser
    parser = ArgumentParser(description='Forcibly stop all running SALOME sessions')
    parser.parse_args()

    try:
        from salomeContextUtils import setOmniOrbUserPath
        setOmniOrbUserPath()
    except Exception as exc: # pragma pylint: disable=broad-except
        print(exc)
        sys.exit(1)

    killAllPorts()

if __name__ == '__main__':
    main()
