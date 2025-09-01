#! /usr/bin/env python3
# Copyright (C) 2021-2025  CEA, EDF
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

import json
import logging
import os
import subprocess
import sys
from pathlib import Path

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


def main(args: list):
    ''' Load modules then launch salome
    '''
    appliPath = Path(args.pop(0))
    modules_file = appliPath / "env_modules.json"
    if modules_file.is_file():
        env_modules = json.loads(open(modules_file).read()).get('env_modules', [])
        env_modules_option = "--with-env-modules="
        env_modules_l = [x for x in args if x.startswith(env_modules_option)]
        if env_modules_l:
            env_modules += env_modules_l[-1][len(env_modules_option):].split(',')
            args = [x for x in args if not x.startswith(env_modules_option)]
        env_modules_option += "%s" % ','.join(env_modules)
        args.append(env_modules_option)
    
    salome_appli_options = os.environ.get("SALOME_APPLI_OPTIONS")
    if salome_appli_options:
        command = ""
        options = []
        if args:
            command = args[0]
            options = args[1:] if len(args) > 1 else []
        args = []
        if command:
            args.append(command)
        args = args + salome_appli_options.split() + options
    
    logger.debug(f"args: {args}")

    os.environ["ROOT_SALOME_INSTALL"] = f"{appliPath}"
    salome_run_script = appliPath / '.salome_run'
    proc = subprocess.Popen([f"{salome_run_script}"] + args, close_fds=True)
    out, err = proc.communicate()
    sys.exit(proc.returncode)
