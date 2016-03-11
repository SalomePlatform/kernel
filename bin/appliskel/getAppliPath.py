#!/usr/bin/env python
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

######
# Warning: relpath might be replaced by equivalent os.relpath introduced in
# Python 2.6 (Calibre 7).
# It is still here to ensure compatibility with Calibre 6 (Python 2.5)
def relpath(target, base):
    """ Find relative path from base to target
        if target== "/local/chris/appli" and base== "/local/chris" the result is appli
        if target== /tmp/appli and base /local/chris the result is ../../tmp/appli
    """
    target=target.split(os.path.sep)
    base=base.split(os.path.sep)
    for i in xrange(len(base)):
      if base[i] != target[i]:
        i=i-1
        #not in base
        break
    p=['..']*(len(base)-i-1)+target[i+1:]
    if p == []:
      return '.'
    return os.path.join( *p )
#

def get_appli_path(filePath=None):
    if filePath is None:
        filePath = os.path.realpath(os.path.dirname(__file__))

    homePath = os.path.realpath(os.getenv('HOME'))
    applipath = os.path.relpath(filePath, homePath)
    return applipath
#

if __name__ == "__main__":
    if sys.hexversion < 0x02060000: # Python older than 2.6.0
        applipath = relpath(os.path.realpath(os.path.dirname(__file__)),os.path.realpath(os.getenv('HOME')))
    else:
        applipath = get_appli_path()
    print applipath
#
