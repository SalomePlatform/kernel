# Copyright (C) 2013-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
import logging

def getLogger(args=None):
  if args is None:
    args = []
  outfileOptionPrefix = "outfile="
  outfileArgs = [ str(x) for x in args if str(x).startswith(outfileOptionPrefix) ]
  allFiles = [ x.replace(outfileOptionPrefix, '') for x in outfileArgs ]
  args = [ x for x in args if not str(x).startswith(outfileOptionPrefix) ]

  logger = logging.getLogger(__name__)
  if len(allFiles) == 0:
    logger.addHandler(logging.StreamHandler())
  else:
    for currentFile in allFiles:
      elements = currentFile.split(',')
      for elt in elements:
        elt = os.path.realpath(os.path.expanduser(elt))
        hdlr = logging.FileHandler(elt)
        logger.addHandler(hdlr)
  #
  logger.level = logging.DEBUG
  return logger, args
#
