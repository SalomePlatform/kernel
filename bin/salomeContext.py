#! /usr/bin/env python3
# Copyright (C) 2013-2026  CEA, EDF, OPEN CASCADE
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


import sys
import logging
import configparser
if sys.version_info[:2] >= (3,12):
  from configparser import ConfigParser as SafeConfigParser
else:
  from configparser import SafeConfigParser
from salome.kernel.parseConfigFile import parseConfigFile

import tempfile
import pickle
import platform
import subprocess
from typing import Any, List, Optional, Tuple, Union

from salome.kernel.salomeContext_impl import SalomeContext, SalomeContextException, usage

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(usage=usage(do_print=False))
    parser.add_argument(
        "context",
        # type=bytes,
        help="The pickled context class object")
    parser.add_argument(
        "args",
        # type=bytes,
        help="The pickled args")
    args = parser.parse_args()

    context : SalomeContext = pickle.loads(args.context.encode("latin1"))
    cargs: str= pickle.loads(args.args.encode("latin1"))
    status = context._startSalome(cargs)
    sys.exit(status)
