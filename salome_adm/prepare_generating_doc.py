#!/usr/bin/env python3
# Copyright (C) 2007-2024  CEA, EDF, OPEN CASCADE
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

#################################################################################
#
# File:   prepare_generating_doc.py
# Author: Anastasiya NIKOLAEVA, Open CASCADE S.A.S
#
#################################################################################
#
# Pre-process Python module for documentation generation by doxygen.
# The script removes the staff that should not appear in the resulting
# documentation.
#
# Current the script does:
# 1. remove Python documentation in triple double quotes (like """some_comments""")
#
# Usage: prepare_generating_doc.py [-o <output_file>] <input_file>
#
# If <output_file> is not specified, it is generated in the current directory.
#
###############################################################################

import os, sys

def main(input_file, output_file = None):

    # open input file
    try:
        infile = open(input_file, 'rb')
    except Exception:
        sys.exit("File %s is not found" % input_file)
        pass

    if not output_file: output_file = os.path.basename(input_file)

    # open output file
    try:
        outfile = open(output_file, 'wb')
    except Exception:
        sys.exit("File %s cannot be opened for write" % output_file)
        pass

    # parse input file

    isCom   = False
    isShift = False

    for line in infile.readlines():
        # 1. remove comments like """some_comments"""
        n = line.find(b'"""')
        n1 = line[(n+2):].find(b'"""')
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

    pass

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    h  = "Output file (if not specified, generated in the current directory)"
    parser.add_argument("-o", "--output", dest="output",
                        action="store", default=None, metavar="file",
                        help=h)
    parser.add_argument('input_file')
    args = parser.parse_args()
    main( args.input_file, args.output )
    pass
