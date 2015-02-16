# Copyright (C) 2015  CEA/DEN, EDF R&D, OPEN CASCADE
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

"""
Usage: salome_test_helper.py <timeout_delay> <test_file.py> [test file arguments]
"""

import sys
import os
import subprocess
import signal

# Run test
def runTest(command):
  print "Running:", " ".join(command)
  p = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  out, err = p.communicate()
  res = p.returncode

  return res, out, err
#

# Display output and errors
def processResult(res, out, err):
  if out:
    print out
    pass
  if err:
    print err
  print "Status code: ", res
#

# Timeout management
class TimeoutException(Exception):
  """Execption raised when test timeout is reached."""
#
def timeoutHandler(signum, frame):
  raise TimeoutException()
#

if __name__ == "__main__":
  timeout_delay = sys.argv[1]
  args = sys.argv[2:]

  # Add explicit call to python executable if a Python script is passed as
  # first argument
  if not args:
    exit(0)
  _, ext = os.path.splitext(args[0])
  if ext == ".py":
    test_and_args = [sys.executable] + args
  else:
    test_and_args = args

  # Ensure OMNIORB_USER_PATH is set
  from salomeContextUtils import setOmniOrbUserPath
  setOmniOrbUserPath()

  # Set timeout handler
  print "Test timeout explicitely set to: %s seconds"%timeout_delay
  signal.alarm(abs(int(timeout_delay)-10))
  signal.signal(signal.SIGALRM, timeoutHandler)

  # Run test in a new SALOME session
  from salome_test_session import startSession, terminateSession
  res = 1
  try:
    port = startSession()
    res, out, err = runTest(test_and_args)
    processResult(res, out, err)
  except TimeoutException:
    print "FAILED : timeout(%s) is reached"%timeout_delay
  except:
    import traceback
    traceback.print_exc()
    pass

  terminateSession(port)
  exit(res)
#
