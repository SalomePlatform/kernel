# Copyright (C) 2015-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
import sys
import select
import subprocess

def __configureTests(args=None, exe=None):
  if args is None:
    args = []
  if exe:
      usage = "Usage: %s [options]"%exe
  else:
      usage = "Usage: %prog [options]"
  epilog  = """\n
Run tests of SALOME components provided with application.\n
Principal options are:
    -h,--help
        Show this help message and exit.

    --print-labels
        Print the list of all labels associated with the test set.
        This option will not run any tests.

    -V,--verbose
        Enable verbose output from tests.
    -VV,--extra-verbose
        Enable more verbose output from tests.
    -Q,--quiet
        Suppress all output.

    -N,--show-only
        Show available tests (without running them).

    -R <regex>, --tests-regex <regex>
        Run tests matching regular expression.
    -E <regex>, --exclude-regex <regex>
        Exclude tests matching regular expression.

    -L <regex>, --label-regex <regex>
        Run tests with labels matching regular expression.
    -LE <regex>, --label-exclude <regex>
        Exclude tests with labels matching regular expression.

For complete description of available options, pleaser refer to ctest documentation.\n
"""
  if not args:
    return []

  if args[0] in ["-h", "--help"]:
    print usage + epilog
    sys.exit(0)

  return args
#

# tests must be in ${ABSOLUTE_APPLI_PATH}/${__testSubDir}/
__testSubDir = "bin/salome/test"

def runTests(args, exe=None):
  args = __configureTests(args, exe)

  appliPath = os.getenv("ABSOLUTE_APPLI_PATH")
  if not appliPath:
      raise SalomeContextException("Unable to find application path. Please check that the variable ABSOLUTE_APPLI_PATH is set.")

  testPath = os.path.join(appliPath, __testSubDir)

  command = ["ctest"] + args
  p = subprocess.Popen(command, cwd=testPath)
  p.communicate()
  sys.exit(p.returncode)
#
