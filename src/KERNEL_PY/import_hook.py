#  -*- coding: iso-8859-1 -*-
#  Copyright (C) 2007-2010  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

"""
This module replaces the standard import mechanism with one
that filters some imports that can't be done more than once.

This is related to the multi study feature that is implemented
by using the Python multi interpreter feature.
Some modules register objects or classes by calling modules
implemented in C. These operations can't be done multiple times.
So it's very important to control these imports.

Examples:
  - PyQt : import qt calls a C module to register classes
  - OmniORB : import *_idl calls a C module to register CORBA interfaces

Usage:
  - First import the module : import import_hook. This module will
    replace the original importer mechanism

  - Next register the module names or pattern names to filter out::
     import_hook.register_name("a")
     import_hook.register_pattern(pattern)

    where pattern is a function with one parameter, the module name
    to be imported, that returns true or false depending if this module is
    to be filtered or not.

  - Then it's done

IMPORTANT : Every subinterpretor has its own import_hook module. import_hook is not shared among subinterpretors.
The mechanism only works if shared_imported and pattern are shared between all subinterpretors.
This is done by calling init_shared_modules().
  
"""
import sys, imp, __builtin__

# Keep in shared_imported a copy of dictionnary modules
# that need to be imported only once in multi-study context
shared_imported={}

# patterns contains functions that returns 1 or 0 depending if 
# the module name (argument) must be filtered out or not
# These functions are added by calling register_pattern
patterns=[]

original_import=__builtin__.__import__

def register_name(name):
    if shared_imported.has_key(name):return
    shared_imported[name]=None

def register_pattern(pattern):
    patterns.append(pattern)

def is_shared(name):
    """ Indicate if module name is a shared module
        among multiple interpreters (return value=1)
    """
    if shared_imported.has_key(name):return 1
    for pattern in patterns:
        if pattern(name) : return 1
    return 0

def get_shared_imported(name,fromlist):
    """ If the module is registered in shared_imported
        update the sys.modules dict
        Let the real import be done by original_import
    """
    module= shared_imported.get(name)
    if module is None :
       #module name is not shared or not already imported
       #let original_import do the job
       return None

    # module is already imported and shared. Put it in sys.modules and 
    # let original_import finish the job
    sys.modules[name]=module

def get_real_module(mod,name):
    """Return effective module on import
       Standard import returns module A on import A.B
       To get module A.B use get_real_module with name "A.B"
    """
    components = name.split('.')
    for comp in components[1:]:
        mod = getattr(mod, comp)
    return mod

def set_shared_imported(name,module):
    """ Register a shared module
        Name can be a dotted name : package
    """
    shared_imported[name]=module
    #print "Module %s shared registered" % name,module

def import_module(partname, fqname, parent):
    """ Try to import module fqname
        It's parent is module parent and has name partname
    """
    #print "import_module",partname, fqname, parent
    try:
       m = sys.modules[fqname]
    except KeyError:
       pass
    else:
       return m

def ensure_fromlist(m, fromlist, recursive=0):
    """ Return the real modules list to be imported
    """
    #print "ensure_fromlist",m, fromlist, recursive
    l=[]
    for sub in fromlist:
        if sub == "*":
            if not recursive:
                try:
                    all = m.__all__
                except AttributeError:
                    pass
                else:
                    l.extend(ensure_fromlist(m, all, 1))
        else:
          #try to find if sub is an attribute (eventually dotted) of m
          components=sub.split('.')
          has_submod=True
          submod=m
          for comp in components:
            if hasattr(submod,comp):
              submod=getattr(submod, comp)
            else:
              has_submod=False
              break

          if has_submod:
            #the attribute has been found
            if type(submod) == type(sys):
               l.append(("%s.%s" % (m.__name__, sub),submod))
          else:
            subname="%s.%s" % (m.__name__, sub)
            submod = import_module(sub, subname, m)
            if not submod:
               raise ImportError, "No module named " + subname
            l.append((subname,submod))
    return l

def import_hook(name, globals=None, locals=None, fromlist=None, *args, **kwds):
    """ Import replacement for sharing modules among multiple interpreters
        Mostly update sys.modules before doing real import
    """
    #print "import_hook",name,fromlist
    m=get_shared_imported(name,fromlist)

    module= original_import(name, globals, locals, fromlist, *args, **kwds)

    if fromlist:
       #when fromlist is specified, module is the real module
       #fromlist is a list of possibly dotted name
       m=module
       for nam,mod in ensure_fromlist(m, fromlist):
           if is_shared(nam):
              set_shared_imported(nam,mod)
    else: 
       #when fromlist is not specified and name is a dotted name,
       # module is the root package not the real module
       #so we need to retrieve it
       # note: some modules like xml.dom do not play the rule
       # (import xml: no attribute dom, but import xml.dom OK)
       try:
           m=get_real_module(module,name)
       except AttributeError:
           m=None

    if type(m) == type(sys) and is_shared(m.__name__):
       set_shared_imported(m.__name__,m)

    return module

original_reload=__builtin__.reload

def reload_hook(module):
    if is_shared(module.__name__):
       return module
    return original_reload(module)

__builtin__.__import__=import_hook
# Reload is not replaced 
#__builtin__.reload=reload_hook

def init_shared_modules(shared_module):
    global shared_imported, patterns
    shared_imported=shared_module.shared_imported
    patterns=       shared_module.patterns
    for k,v in shared_imported.items():
       if v is not None:sys.modules[k]=v
    shared_imported["salome_shared_modules"]=shared_module
    import salome_shared_modules
    for m in salome_shared_modules.list_modules:
        m.init_shared_modules()
