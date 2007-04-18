# Copyright (C) 2005  OPEN CASCADE, CEA, EDF R&D, LEG
#           PRINCIPIA R&D, EADS CCR, Lip6, BV, CEDRAT
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either 
# version 2.1 of the License.
# 
# This library is distributed in the hope that it will be useful 
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
"""Create a virtual Salome installation 

Based on a script created by Ian Bicking.

Typical use::

  python virtual_salome.py -v --prefix="." --module=/local/chris/SALOME2/RELEASES/Install/KERNEL_V3_1_0b1

install module KERNEL in the current directory
"""

import sys, os, optparse, shutil,glob,fnmatch
py_version = 'python%s.%s' % (sys.version_info[0], sys.version_info[1])

verbose=0

# -----------------------------------------------------------------------------

def mkdir(path):
    """Create a directory and all the intermediate directories if path does not exist"""
    if not os.path.exists(path):
        print 'Creating %s' % path
        os.makedirs(path)
    else:
        if verbose:
            print 'Directory %s already exists' % path
            pass
        pass

# -----------------------------------------------------------------------------

def symlink(src, dest):
    """Create a link if it does not exist"""
    if not os.path.exists(dest):
        if verbose:
            print 'Creating symlink %s' % dest
            pass
        os.symlink(src, dest)
    else:
        print 'Symlink %s already exists' % dest
        pass
    pass

# -----------------------------------------------------------------------------

def rmtree(dir):
    """Remove (recursive) a directory if it exists"""
    if os.path.exists(dir):
        print 'Deleting tree %s' % dir
        shutil.rmtree(dir)
    else:
        if verbose:
            print 'Do not need to delete %s; already gone' % dir
            pass
        pass
    pass

# -----------------------------------------------------------------------------

__lib__dir__ = None
def get_lib_dir():
    global __lib__dir__
    if __lib__dir__: return __lib__dir__
    import platform
    if platform.architecture()[0] == "64bit":
        if platform.machine() == "ia64":
            __lib__dir__ = "lib"
        else:
            __lib__dir__ = "lib64"
    else:
        __lib__dir__ = "lib"
    return get_lib_dir()

# -----------------------------------------------------------------------------

def link_module(options):

    if not options.module:
        print "Option module is mandatory"
        return 
   
    module_dir=os.path.abspath(options.module)
    if not os.path.exists(module_dir):
        print "Module %s does not exist" % module_dir
        return

    home_dir = os.path.expanduser(options.prefix)

    module_bin_dir=os.path.join(module_dir,'bin','salome')
    module_lib_dir=os.path.join(module_dir,get_lib_dir(),'salome')
    module_lib_py_dir=os.path.join(module_dir,get_lib_dir(),py_version,'site-packages','salome')
    module_lib_py_shared_dir=os.path.join(module_dir,get_lib_dir(),py_version,
                                          'site-packages','salome','shared_modules')
    module_share_dir=os.path.join(module_dir,'share','salome','resources')
    module_doc_gui_dir=os.path.join(module_dir,'doc','salome','gui')
    module_doc_tui_dir=os.path.join(module_dir,'doc','salome','tui')
    module_doc_dir=os.path.join(module_dir,'doc','salome')
    module_sharedoc_dir=os.path.join(module_dir,'share','doc','salome')

    if not os.path.exists(module_lib_py_dir):
        print "Python directory %s does not exist" % module_lib_py_dir
        return

    bin_dir=os.path.join(home_dir,'bin','salome')
    lib_dir=os.path.join(home_dir,'lib','salome')
    lib_py_dir=os.path.join(home_dir,'lib',py_version,'site-packages','salome')
    lib_py_shared_dir=os.path.join(home_dir,'lib',py_version,
                                   'site-packages','salome','shared_modules')
    share_dir=os.path.join(home_dir,'share','salome','resources')
    doc_gui_dir=os.path.join(home_dir,'doc','salome','gui')
    doc_tui_dir=os.path.join(home_dir,'doc','salome','tui')
    doc_dir=os.path.join(home_dir,'doc','salome')
    sharedoc_dir=os.path.join(home_dir,'share','doc','salome')

    verbose = options.verbose

    if options.clear:
        rmtree(bin_dir)
        rmtree(lib_dir)
        rmtree(lib_py_dir)
        rmtree(share_dir)
        rmtree(doc_dir)
        rmtree(sharedoc_dir)
        pass
    
    #directory bin/salome : create it and link content
    if os.path.exists(module_bin_dir):
        mkdir(bin_dir)
        for fn in os.listdir(module_bin_dir):
            symlink(os.path.join(module_bin_dir, fn), os.path.join(bin_dir, fn))
            pass
        pass
    else:
        print module_bin_dir, " doesn't exist"
        pass    
    
    #directory lib/salome : create it and link content
    if os.path.exists(module_lib_dir):
        mkdir(lib_dir)
        for fn in os.listdir(module_lib_dir):
            symlink(os.path.join(module_lib_dir, fn), os.path.join(lib_dir, fn))
            pass
        pass
    else:
        print module_lib_dir, " doesn't exist"
        pass    
    
    #directory lib/py_version/site-packages/salome : create it and link content
    mkdir(lib_py_shared_dir)
    for fn in os.listdir(module_lib_py_dir):
        if fn == "shared_modules": continue
        symlink(os.path.join(module_lib_py_dir, fn), os.path.join(lib_py_dir, fn))
        pass    
    if os.path.exists(module_lib_py_shared_dir):
        for fn in os.listdir(module_lib_py_shared_dir):
            symlink(os.path.join(module_lib_py_shared_dir, fn), os.path.join(lib_py_shared_dir, fn))
            pass
        pass
    else:
        print module_lib_py_shared_dir, " doesn't exist"
        pass    

    #directory share/doc/salome (KERNEL doc) : create it and link content
    if os.path.exists(module_sharedoc_dir):
        mkdir(sharedoc_dir)
        for fn in os.listdir(module_sharedoc_dir):
            symlink(os.path.join(module_sharedoc_dir, fn), os.path.join(sharedoc_dir, fn))
            pass
        pass
    pass


    #directory share/salome/resources : create it and link content
    mkdir(share_dir)
    for fn in os.listdir(module_share_dir):
        symlink(os.path.join(module_share_dir, fn), os.path.join(share_dir, fn))

    #html files in doc/salome directory
    if os.path.exists(module_doc_dir):
        mkdir(doc_dir)
        for fn in os.listdir(module_doc_dir):
            if fn == 'gui':continue
            if fn == 'tui':continue
            symlink(os.path.join(module_doc_dir, fn), os.path.join(doc_dir, fn))
            pass
        pass
    
    #directory doc/salome/gui : create it and link content
    if os.path.exists(module_doc_gui_dir):
        mkdir(doc_gui_dir)
        for fn in os.listdir(module_doc_gui_dir):
            symlink(os.path.join(module_doc_gui_dir, fn), os.path.join(doc_gui_dir, fn))
            pass
        pass
    
    #directory doc/salome/tui : create it and link content
    if os.path.exists(module_doc_tui_dir):
        mkdir(doc_tui_dir)
        for fn in os.listdir(module_doc_tui_dir):
            symlink(os.path.join(module_doc_tui_dir, fn), os.path.join(doc_tui_dir, fn))
            pass
        pass

# -----------------------------------------------------------------------------

def main():
    usage="""usage: %prog [options]
Typical use is:
  python virtual_salome.py -v --prefix="." --module=/local/chris/SALOME2/RELEASES/Install/KERNEL_V3_1_0b1
"""
    parser = optparse.OptionParser(usage=usage)

    parser.add_option('-v', '--verbose', action='count', dest='verbose',
                      default=0, help="Increase verbosity")

    parser.add_option('--prefix', dest="prefix", default='.',
                      help="The base directory to install to (default .)")

    parser.add_option('--module', dest="module", 
                      help="The module directory to install in (mandatory)")

    parser.add_option('--clear', dest='clear', action='store_true',
        help="Clear out the install and start from scratch")

    options, args = parser.parse_args()
    link_module(options)
    pass
    
# -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()
    pass
