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
Usage: salome_test_helper.py <timeout_delay> <test command> [test command arguments]
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
  # About res value:
  # A negative value -N indicates that the child was terminated by signal N (Unix only).
  # On Unix, the value 11 generally corresponds to a segmentation fault.
  return res, out, err
#

def processResultSpecialParavis(res, out, err):
  # :TRICKY: Special case of returncode=127
  # When using paraview in SALOME environment, the following error
  # systematically appears when exiting paraview (it's also true when using
  # PARAVIS and exiting SALOME):
  # Inconsistency detected by ld.so: dl-close.c: 738: _dl_close: Assertion `map->l_init_called' failed!
  # For PARAVIS tests purpose, paraview functionalities are accessed in each
  # test; these tests are run in the above subprocess call.
  # The assertion error implies a subprocess return code of 127, and the test
  # status is considered as "failed".
  # The tricky part here is to discard such return codes, waiting for a fix
  # maybe in paraview...
  if res == 127 and err.startswith("Inconsistency detected by ld.so: dl-close.c"):
      print "    ** THE FOLLOWING MESSAGE IS DISCARDED WHEN ANALYZING TEST SUCCESSFULNESS **"
      print err,
      print "    ** end of message **"
      res = 0
  elif err:
      print "    ** Detected error **"
      print "Error code: ", res
      print err,
      print "    ** end of message **"
      pass

  if out:
      print out
  return res
#

# Display output and errors
def processResult(res, out, err):
  if out:
    print out
    pass
  if err:
    print err
  print "Status code: ", res
  return res
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
    print "Invalid arguments for salome_test_helper.py. No command defined."
    exit(1)
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

  # Run test in a new SALOME instance
  from salome_instance import SalomeInstance
  res = 1
  try:
    salome_instance = SalomeInstance.start(shutdown_servers=True)
    port = salome_instance.get_port()
    res, out, err = runTest(test_and_args)
    #res = processResult(res, out, err)
    res = processResultSpecialParavis(res, out, err)
  except TimeoutException:
    print "FAILED : timeout(%s) is reached"%timeout_delay
  except:
    import traceback
    traceback.print_exc()
    pass

  salome_instance.stop()
  print "Exit test with status code:", res
  exit(res)
#
