#!/usr/bin/env python
# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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

# ...
# Usage: prepare_generating_doc.py <input_file>
# ...
# 1. Remove Python documentation in triple double quotes (like """some_comments""")
# ...

import os, sys, re

# check command line
if len( sys.argv ) < 2:
    sys.exit("Usage: %s <input_file>" % os.path.basename(sys.argv[0]))

# open input file
try:
    infile = open(sys.argv[1], 'rb')
except:
    sys.exit("File %s is not found" % sys.argv[1])
    pass

# open output file with the same name in current directory
try:
    outfile = open(os.path.basename(sys.argv[1]), 'wb')
except:
    sys.exit("File %s cannot be opened for write" % os.path.basename(sys.argv[1]))
    pass

# parse input file

isCom   = False
isShift = False

for line in infile.readlines():
    # 1. remove comments like """some_comments"""
    n = line.find('"""')
    n1 = line[(n+2):].find('"""')
    if (n > -1) and (n1 > -1):
        continue
    if isCom:
        if n > -1:
            isCom = False
            pass
        continue
    else:
        if n > -1:
            isCom = True
            continue
        pass

    # write resulting line
    outfile.write(line)

    pass # end of for

# close input and output files
infile.close()
outfile.close()
