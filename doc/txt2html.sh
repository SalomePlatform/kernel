#/bin/sh
# ===================================================================
# This shell script is provided for generating the html files
# from the txt files (restructured text) in the source directory.
#
# Usage: just execute the script where it stands in the source
# directory. The file list has to be updated manually when adding
# a new restructured text file.
#
# Note that the building process executes a target rstdoc that
# generates the html documentation without need of this script.
# The autoconficuration (check_htmlgen.m4) set the correct generator
# rst2html by replacing the @RST2HTML@ tag.
# ===================================================================
# (CSSI - gboulant - 25/10/05)
#

# This must be updated manually in this script (for source usage only)
RST2HTML=rst2html

FILELIST="index
	  UnitTests
	  SALOME_Application
	  INSTALL
	  kernel_resources
	  userguide"

STYLESHEET=rst.css
RSTOPTS="--output-encoding=latin1 --stylesheet=$STYLESHEET"

for file in $FILELIST; do
    bfn=`basename $file .txt`
    echo "Generating ${bfn}.html from ${bfn}.txt ..."
    $RST2HTML $RSTOPTS ${bfn}.txt ${bfn}.html
done

