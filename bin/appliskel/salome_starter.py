#! /usr/bin/env python

# Copyright (C) 2014-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

# This file is imported by launchers to help with application folder detection.
# Import is possible because Python automatically add to sys.path the folder of
# a running script (here a launcher).

def __detectAppliPath(fromPath, launcherFile):
  detection_criterion = "USERS" # the application folder is found if it contains a USERS subfolder

  users_folder = os.path.join(fromPath, detection_criterion)
  if os.path.isdir(users_folder):
    return fromPath

  pattern = os.path.sep + os.path.join("bin", "salome", "appliskel")
  if fromPath.endswith(pattern):
    currentPath = __detectAppliPath(fromPath[:-len(pattern)], launcherFile)
    if not currentPath is None:
      return currentPath

  if sys.platform.startswith("linux"):
    link_target = os.readlink(os.path.join(fromPath,launcherFile)) # LINUX ONLY
    currentPath = os.path.dirname(os.path.abspath(link_target))
    return __detectAppliPath(currentPath, launcherFile)

  return None
#

# Preliminary work to initialize path to SALOME Python modules
def initialize(launcherPath, launcherFile):
  homePath = os.path.realpath(os.path.expanduser('~'))
  appliPath = __detectAppliPath(launcherPath, launcherFile)

  if appliPath is None:
    print "ERROR: Unable to find application folder"
    sys.exit(0)

  appliPath = os.path.relpath(appliPath, homePath)
  absoluteAppliPath = os.path.join(homePath, appliPath)
  os.environ['APPLI'] = appliPath
  os.environ['ABSOLUTE_APPLI_PATH'] = absoluteAppliPath

  sys.path[:0] = [os.path.realpath(os.path.join(absoluteAppliPath, "bin", "salome", "test"))]
  sys.path[:0] = [os.path.realpath(os.path.join(absoluteAppliPath, "bin", "salome"))]

  # define folder to store omniorb config (initially in virtual application folder)
  try:
    from salomeContextUtils import setOmniOrbUserPath
    setOmniOrbUserPath()
  except Exception, e:
    print e
    sys.exit(1)
# End of preliminary work
