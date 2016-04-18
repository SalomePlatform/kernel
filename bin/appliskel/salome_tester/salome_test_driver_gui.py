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

"""
Usage: salome_test_driver_gui.py <timeout_delay> <test command> [test command arguments]
"""

import sys
import os
import subprocess
import signal

# Timeout management
class TimeoutException(Exception):
  """Exception raised when test timeout is reached."""
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
    print "Invalid arguments for salome_test_driver_gui.py. No command defined."
    exit(1)
  _, ext = os.path.splitext(args[0])
  test_and_args = args

  # Ensure OMNIORB_USER_PATH is set
  from salomeContextUtils import setOmniOrbUserPath
  setOmniOrbUserPath()

  # Set timeout handler
  print "Test timeout explicitely set to: %s seconds"%timeout_delay
  timeout_sec = abs(int(timeout_delay)-10)
  if sys.platform == 'win32':
    from threading import Timer
    timer = Timer(timeout_sec, timeoutHandler)
    timer.start()
  else:
    signal.alarm(timeout_sec)
    signal.signal(signal.SIGALRM, timeoutHandler)

  # Run test in a new SALOME instance
  from salome_instance import SalomeInstance
  try:
    salome_instance = SalomeInstance.start(with_gui=True, args=test_and_args)
  except TimeoutException:
    print "FAILED : timeout(%s) is reached"%timeout_delay
  except:
    import traceback
    traceback.print_exc()
    pass

  salome_instance.stop()
  if sys.platform == 'win32':
    timer.cancel()
#  print "Exit test with status code:", res
#  exit(res)
#
