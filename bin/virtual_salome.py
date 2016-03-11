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
        if verbose:
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
        if os.path.lexists(dest):
            print "Do not create symlink %s. It already exists but it's broken" % dest
            return
        if verbose:
            print 'Creating symlink %s' % dest
            pass
        os.symlink(src, dest)
    else:
        if verbose:
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
    __lib__dir__ = "lib"
    return __lib__dir__

# -----------------------------------------------------------------------------

def link_module(options):
    global verbose

    if not options.module_path:
        print "Option module is mandatory"
        return

    module_dir=os.path.abspath(options.module_path)
    if not os.path.exists(module_dir):
        print "Module %s does not exist" % module_dir
        return

    verbose = options.verbose

    home_dir = os.path.expanduser(options.prefix)
    #try to find python version of salome application and put it in versio
    pys=[os.path.split(s)[1] for s in glob.glob(os.path.join(home_dir,get_lib_dir(),"python*.*"))]
    if not pys :
      versio=None
    else:
      versio=pys[0]

    pys=[os.path.split(s)[1] for s in glob.glob(os.path.join(module_dir,get_lib_dir(),"python*.*"))]
    #check if the module has a python version compatible with application version
    if not pys :
      pyversio=versio or py_version
    elif versio is None:
      pyversio=pys[0]
    elif versio in pys:
      pyversio=versio
    else:
      #incompatible python versions
      print "incompatible python versions : application has version %s and module %s has not" % (versio,module_dir)
      return

    module_bin_dir=os.path.join(module_dir,'bin','salome')
    module_test_dir=os.path.join(module_dir,'bin','salome', 'test')
    module_idl_dir=os.path.join(module_dir,'idl','salome')
    module_lib_dir=os.path.join(module_dir,get_lib_dir(),'salome')
    module_pvlib_dir=os.path.join(module_dir,get_lib_dir(),'paraview')
    module_lib_py_dir=os.path.join(module_dir,get_lib_dir(),pyversio,'site-packages','salome')
    module_lib_py_shared_dir=os.path.join(module_dir,get_lib_dir(),pyversio,
                                          'site-packages','salome')
    module_share_dir=os.path.join(module_dir,'share','salome')
    module_doc_gui_dir=os.path.join(module_dir,'doc','salome','gui')
    module_doc_tui_dir=os.path.join(module_dir,'doc','salome','tui')
    module_doc_dir=os.path.join(module_dir,'doc','salome')
    module_sharedoc_dir=os.path.join(module_dir,'share','doc','salome')
    module_sharedoc_gui_dir=os.path.join(module_dir,'share','doc','salome','gui')
    module_sharedoc_tui_dir=os.path.join(module_dir,'share','doc','salome','tui')
    module_sharedoc_examples=os.path.join(module_dir,'share','doc','salome','examples')
    module_sharedoc_dev=os.path.join(module_dir,'share','doc','salome','dev')

    bin_dir=os.path.join(home_dir,'bin','salome')
    test_dir=os.path.join(home_dir,'bin','salome', 'test')
    idl_dir=os.path.join(home_dir,'idl','salome')
    lib_dir=os.path.join(home_dir,'lib','salome')
    pvlib_dir=os.path.join(home_dir,'lib','paraview')
    lib_py_dir=os.path.join(home_dir,'lib',pyversio,'site-packages','salome')
    lib_py_shared_dir=os.path.join(home_dir,'lib',pyversio,
                                   'site-packages','salome')
    share_dir=os.path.join(home_dir,'share','salome')
    doc_gui_dir=os.path.join(home_dir,'doc','salome','gui')
    doc_tui_dir=os.path.join(home_dir,'doc','salome','tui')
    doc_dir=os.path.join(home_dir,'doc','salome')
    sharedoc_dir=os.path.join(home_dir,'share','doc','salome')
    sharedoc_gui_dir=os.path.join(home_dir,'share','doc','salome','gui')
    sharedoc_tui_dir=os.path.join(home_dir,'share','doc','salome','tui')
    sharedoc_examples_dir=os.path.join(home_dir,'share','doc','salome','examples')
    sharedoc_dev_dir=os.path.join(home_dir,'share','doc','salome','dev')

    if options.clear:
        rmtree(bin_dir)
        rmtree(test_dir)
        rmtree(idl_dir)
        rmtree(lib_dir)
        rmtree(lib_py_dir)
        rmtree(share_dir)
        rmtree(doc_dir)
        rmtree(sharedoc_dir)
        pass

    #directory bin/salome : create it and link content
    if os.path.exists(module_bin_dir):
        mkdir(bin_dir)
        mkdir(test_dir)
        for fn in os.listdir(module_bin_dir):
            if fn != "test":
                symlink(os.path.join(module_bin_dir, fn), os.path.join(bin_dir, fn))
            pass
        pass
    else:
        if verbose:
            print module_bin_dir, " doesn't exist"
        pass

    #directory bin/salome/test : create it and link content
    if os.path.exists(module_test_dir):
        # link <appli_path>/bin/salome/test/<module> to <module_path>/bin/salome/test
        print "link %s --> %s"%(os.path.join(test_dir, options.module_name), module_test_dir)
        symlink(module_test_dir, os.path.join(test_dir, options.module_name))
        # register module for testing in CTestTestfile.cmake
        with open(os.path.join(test_dir, "CTestTestfile.cmake"), "ab") as f:
            f.write("SUBDIRS(%s)\n"%options.module_name)
    else:
        if verbose:
            print module_test_dir, " doesn't exist"
        pass

    #directory idl/salome : create it and link content
    if os.path.exists(module_idl_dir):
        mkdir(idl_dir)
        for fn in os.listdir(module_idl_dir):
            symlink(os.path.join(module_idl_dir, fn), os.path.join(idl_dir, fn))
    else:
        if verbose:
            print module_idl_dir, " doesn't exist"

    #directory lib/salome : create it and link content
    if os.path.exists(module_lib_dir):
        mkdir(lib_dir)
        for fn in os.listdir(module_lib_dir):
            symlink(os.path.join(module_lib_dir, fn), os.path.join(lib_dir, fn))
            pass
        pass
    else:
        if verbose:
            print module_lib_dir, " doesn't exist"
        pass

    #directory lib/paraview : create it and link content
    if os.path.exists(module_pvlib_dir):
        mkdir(pvlib_dir)
        for fn in os.listdir(module_pvlib_dir):
            symlink(os.path.join(module_pvlib_dir, fn), os.path.join(pvlib_dir, fn))
            pass
        pass
    else:
        if verbose:
            print module_pvlib_dir, " doesn't exist"
        pass

    #directory lib/pyversio/site-packages/salome : create it and link content
    if not os.path.exists(module_lib_py_dir):
        print "Python directory %s does not exist" % module_lib_py_dir
    else:
        # Specific action for the package salome
        module_lib_pypkg_dir=os.path.join(module_lib_py_dir,"salome")
        lib_pypkg_dir=os.path.join(lib_py_dir,"salome")
        mkdir(lib_pypkg_dir)
        mkdir(lib_py_shared_dir)
        for fn in os.listdir(module_lib_py_dir):
            if fn == "salome": continue
            symlink(os.path.join(module_lib_py_dir, fn), os.path.join(lib_py_dir, fn))
            pass
        if os.path.exists(module_lib_py_shared_dir):
            for fn in os.listdir(module_lib_py_shared_dir):
                symlink(os.path.join(module_lib_py_shared_dir, fn), os.path.join(lib_py_shared_dir, fn))
                pass
            pass
        if os.path.exists(module_lib_pypkg_dir):
            for fn in os.listdir(module_lib_pypkg_dir):
                symlink(os.path.join(module_lib_pypkg_dir, fn), os.path.join(lib_pypkg_dir, fn))
                pass
            pass
        else:
            if verbose:
                print module_lib_py_shared_dir, " doesn't exist"
            pass

    #directory share/doc/salome (KERNEL doc) : create it and link content
    if os.path.exists(module_sharedoc_dir):
        mkdir(sharedoc_dir)
        for fn in os.listdir(module_sharedoc_dir):
            if fn == 'gui':continue
            if fn == 'tui':continue
            if fn == 'examples':continue
            if fn == 'dev':continue
            symlink(os.path.join(module_sharedoc_dir, fn), os.path.join(sharedoc_dir, fn))
            pass
        pass

    #directory share/doc/salome/gui : create it and link content
    if os.path.exists(module_sharedoc_gui_dir):
        mkdir(sharedoc_gui_dir)
        for fn in os.listdir(module_sharedoc_gui_dir):
            symlink(os.path.join(module_sharedoc_gui_dir, fn), os.path.join(sharedoc_gui_dir, fn))
            pass
        pass

    #directory share/doc/salome/tui : create it and link content
    if os.path.exists(module_sharedoc_tui_dir):
        mkdir(sharedoc_tui_dir)
        for fn in os.listdir(module_sharedoc_tui_dir):
            symlink(os.path.join(module_sharedoc_tui_dir, fn), os.path.join(sharedoc_tui_dir, fn))
            pass
        pass

    #directory share/doc/salome/examples : create it and link content
    if os.path.exists(module_sharedoc_examples):
        mkdir(sharedoc_examples_dir)
        for fn in os.listdir(module_sharedoc_examples):
            symlink(os.path.join(module_sharedoc_examples, fn), os.path.join(sharedoc_examples_dir, fn))
            pass
        pass

    #directory share/doc/salome/dev : create it and link content
    if os.path.exists(module_sharedoc_dev):
        mkdir(sharedoc_dev_dir)
        for fn in os.listdir(module_sharedoc_dev):
            symlink(os.path.join(module_sharedoc_dev, fn), os.path.join(sharedoc_dev_dir, fn))
            pass
        pass

    #directory share/salome : create it and link content
    if os.path.exists(module_share_dir):
        mkdir(share_dir)
        for fn in os.listdir(module_share_dir):
            if fn in ["resources","plugins"] :
                # Create the directory and then link the content
                mkdir(os.path.join(share_dir,fn))
                for ffn in os.listdir(os.path.join(module_share_dir,fn)):
                    symlink(os.path.join(module_share_dir, fn, ffn), os.path.join(share_dir,fn, ffn))
            else:
                #other directories (not resources)
                symlink(os.path.join(module_share_dir, fn), os.path.join(share_dir, fn))
    else:
        print "share/salome directory %s does not exist" % module_share_dir
        pass

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

def link_extra_test(options):
    global verbose

    if not options.extra_test_path:
        print "Option extra_test is mandatory"
        return

    extra_test_dir=os.path.abspath(options.extra_test_path)
    if not os.path.exists(extra_test_dir):
        print "Test %s does not exist" % extra_test_dir
        return

    verbose = options.verbose

    home_dir = os.path.expanduser(options.prefix)
    test_dir = os.path.join(home_dir,'bin','salome', 'test')

    if options.clear:
        rmtree(test_dir)
        pass

    #directory bin/salome/test : create it and link content
    if os.path.exists(extra_test_dir):
        # link <appli_path>/bin/salome/test/<extra_test> to <extra_test_path>/bin/salome/test
        print "link %s --> %s"%(os.path.join(test_dir, options.extra_test_name), extra_test_dir)
        symlink(extra_test_dir, os.path.join(test_dir, options.extra_test_name))
        # register extra_test for testing in CTestTestfile.cmake
        with open(os.path.join(test_dir, "CTestTestfile.cmake"), "ab") as f:
            f.write("SUBDIRS(%s)\n"%options.extra_test_name)
    else:
        if verbose:
            print extra_test_dir, " doesn't exist"
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
