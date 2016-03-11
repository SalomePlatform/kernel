# -*- coding: iso-8859-1 -*-
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

__author__="gboulant"
__date__ ="$21 mai 2010 18:00:23$"


def findFiles(rootpath, excludes=None):
    """
    This looks after files recursively from the specified rootpath,
    but without visiting directories whose basename is in the list
    @param excludes.
    """
    if not os.path.exists(rootpath):
        raise RuntimeError("the path %s does not exists"%rootpath)

    if excludes is None:
        excludes = []
    exclude_options=""
    for excludepath in excludes:
        exclude_options+="-e %s "%excludepath

    listfiles=[]        
    stream=os.popen("find %s -type f | grep -v -e '\.svn' %s 2>/dev/null"%(rootpath,exclude_options))
    for line in stream.readlines():
        listfiles.append(line.split('\n')[0])
        
    return listfiles

import sys
from stat import ST_MODE, S_ISDIR, S_ISREG
def walktree(rootpath, callback, **kwargs):
    '''
    This recursively descends the directory tree rooted at rootpath,
    calling the callback function for each regular file
    '''
    for f in os.listdir(rootpath):
        pathname = os.path.join(rootpath, f)
        try:
            mode = os.stat(pathname)[ST_MODE]
        except OSError, e:
            # It probably means that the file is a broken inode
            mode = -1
        if S_ISDIR(mode):
            # It's a directory, recurse into it
            walktree(pathname, callback, **kwargs)
        elif S_ISREG(mode):
            # It's a file, call the callback function
            callback(pathname, **kwargs)
        else:
            # Unknown file type, print a message
            print 'Skipping %s' % pathname


#
# =============================================================
# Use cases and unit test functions
# =============================================================
#
import os
try:
    TESTDOCDIR=os.path.join(os.environ["KERNEL_ROOT_DIR"],"share")
except KeyError:
    TESTDOCDIR="/tmp"
    
def TEST_findFiles():
    print "########## find 1"
    rootpath=TESTDOCDIR
    listfiles=findFiles(rootpath)
    for filename in listfiles:
        print filename

    print "########## find 2"
    excludes=[os.path.join(TESTDOCDIR,"doc")]
    listfiles=findFiles(rootpath,excludes)
    for filename in listfiles:
        print filename

    return True

# This is the test callback function
def visitfile_withargs(file, rootid):
    print 'visiting file %s (rootid=%s)'%(file,str(rootid))

def visitfile_withoutargs(file):
    print 'visiting file %s'%(file)

def TEST_walktree():
    #walktree(TESTDOCDIR, visitfile_withargs, rootid=2)
    walktree(TESTDOCDIR, visitfile_withoutargs)
    return True

if __name__ == "__main__":
    import unittester
    unittester.run("syshelper", "TEST_findFiles")
    unittester.run("syshelper", "TEST_walktree")
