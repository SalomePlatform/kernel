#! /usr/bin/env python

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

import sys
import logging

from getLogger import getLogger


def file_len(fname):
  with open(fname) as f:
    for i, l in enumerate(f):
      pass
    return i + 1
#

if __name__ == "__main__":
  args = sys.argv[1:]
  logger, args = getLogger(args)

  if len(args)==0:
    logger.info("No files given")
  else:
    for filename in args:
      try:
        nb = file_len(filename)
        logger.info("%s is %s lines long"%(filename, nb))
      except IOError:
        logger.info("File '%s' cannot be read"%(filename))
#
