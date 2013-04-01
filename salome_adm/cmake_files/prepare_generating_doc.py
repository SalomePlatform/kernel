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
# Usage: prepare_generating_doc.py <output_file> <input_file> <myClass>
# ...
# 1. myClassDC is replaced by myClass
# 2. all methods of myClassDC become global methods of Python package
# ...
# myClass is passed as command line argument
# ...

import os, sys, re

# check command line
if len( sys.argv ) < 4:
    sys.exit("Usage: %s <output_file> <input_file> <myClass>" % os.path.basename(sys.argv[0]))

# open input file
try:
    infile = open(sys.argv[2], 'rb')
except:
    sys.exit("File %s is not found" % sys.argv[2])
    pass

# open output file
try:
    outfile = open(sys.argv[1], 'wb')
except:
    sys.exit("File %s cannot be opened for write" % sys.argv[1])
    pass

# parse input file

isCom   = False
isShift = False

for line in infile.readlines():
    dc_class    = sys.argv[3]    
    dc_class_dc = dc_class+'DC'

    if isShift and ( re.match('^class\s+', line) or re.match('^def\s+', line) ):
        # stop shifting lines as soon as myClassDC definition is over
        isShift = False
        pass
    if re.match('class\s+%s' % dc_class_dc, line):
        # start shifting lines
        isShift = True
        # omit this line (to remove myClassDC from the package)
        continue
    
    # process documentation
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

    # replacements
    if isShift:
        line = re.sub(r'^\s+#', '#', line) 
        line = re.sub(r'^\s+def', 'def', line)
        pass
    line = re.sub(dc_class_dc, dc_class, line)

    # write resulting line
    outfile.write(line)

    pass # end of for

# close input and output files
infile.close()
outfile.close()
