    #! /usr/bin/env python3
# Copyright (C) 2021-2023  CEA/DEN, EDF R&D
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
import subprocess
import sys

MODULES = []


def main(args):
    ''' Load modules then launch salome
    '''
    if MODULES:
        env_modules = MODULES[:]
        env_modules_option = "--with-env-modules="
        env_modules_l = [x for x in args if x.startswith(env_modules_option)]
        if env_modules_l:
            env_modules += env_modules_l[-1][len(env_modules_option):].split(',')
            args = [x for x in args if not x.startswith(env_modules_option)]
        env_modules_option += "%s" % ','.join(env_modules)
        args.append(env_modules_option)

    appliPath = os.path.dirname(os.path.realpath(__file__))
    os.environ["ROOT_SALOME_INSTALL"]=appliPath
    proc = subprocess.Popen([os.path.join(appliPath, '.salome_run')] + args, close_fds=True)
    out, err = proc.communicate()
    sys.exit(proc.returncode)
