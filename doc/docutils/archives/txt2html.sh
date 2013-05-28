#!/bin/sh
# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

# ===================================================================
# This shell script is provided for generating the html files
# from the txt files (restructured text) in the source directory.
# Usage: just execute the script where it stands in the source
# directory. The file list has to be updated manually when adding
# a new restructured text file.
# Note that the building process executes a target rstdoc that
# generates the html documentation without need of this script.
# The autoconficuration (check_htmlgen.m4) set the correct generator
# rst2html by replacing the @RST2HTML@ tag.
# ===================================================================
# (CSSI - gboulant - 25/10/05)
# This must be updated manually in this script (for source usage only)
#
RST2HTML=rst2html

FILELIST="index
	  UnitTests
	  SALOME_Application
	  INSTALL
	  kernel_resources
	  KERNEL_Services
	  userguide"

STYLESHEET=rst.css
RSTOPTS="--output-encoding=latin1 --stylesheet=$STYLESHEET"

for file in $FILELIST; do
    bfn=`basename $file .txt`
    echo "Generating ${bfn}.html from ${bfn}.txt ..."
    $RST2HTML $RSTOPTS ${bfn}.txt ${bfn}.html
done

