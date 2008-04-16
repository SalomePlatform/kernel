#!/usr/bin/env python

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

import sys, os, string, glob, time, pickle
import orbmodule
from launchConfigureParser import verbose

# this file is extraction of set_env from runSalome.py
# for reusage in automated tests

# salome_subdir variable is used for composing paths like $KERNEL_ROOT_DIR/share/salome/resources, etc.
# before moving to SUIT-based gui, instead of salome_subdir there was args['appname'] used.
# but after - 'appname'  = "SalomeApp", so using it in making the subdirectory is an error.
salome_subdir = "salome"


# -----------------------------------------------------------------------------

def add_path(directory, variable_name):
    """Function helper to add environment variables"""
    if sys.platform == "win32":
      splitsym = ";"
    else:
      splitsym = ":"
    if not os.environ.has_key(variable_name):
        os.environ[variable_name] = ""
        pass
    if os.path.exists(directory):
        newpath=[]
        for _dir in os.environ[variable_name].split(splitsym):
            if os.path.exists(_dir):
                if sys.platform != "win32":
                    if not os.path.samefile(_dir, directory):
                        newpath.append(_dir)
                else:
                    newpath.append(_dir)
            else:
                if os.path.abspath(_dir) != os.path.abspath(directory):
                  newpath.append(_dir)
            pass
        import string
        newpath[:0] = [ directory ]
        newpath = string.join(newpath, splitsym)
        os.environ[variable_name] = newpath
        if variable_name == "PYTHONPATH":
            sys.path[:0] = [directory]

# -----------------------------------------------------------------------------

__lib__dir__ = None
def get_lib_dir():
    global __lib__dir__
    if __lib__dir__: return __lib__dir__
    import platform
    __lib__dir__ = "lib"
    return __lib__dir__

# -----------------------------------------------------------------------------

def get_config():
    """
    Get list of modules, paths.
    
    Read args from launch configure xml file and command line options.
    Check variables <module>_ROOT_DIR and set list of used modules.
    Return args, modules_list, modules_root_dir    
    """
    
    # read args from launch configure xml file and command line options

    #*** Test additional option
    #*** import optparse
    #*** help_str = "Test options addition."
    #*** o_j = optparse.Option("-j", "--join", action="store_true", dest="join", help=help_str)

    import launchConfigureParser
    args = launchConfigureParser.get_env()

    #*** Test additional option
    #*** args = launchConfigureParser.get_env([o_j])
    #*** if args.has_key("join"): print args["join"]

    # Check variables <module>_ROOT_DIR
    # and set list of used modules (without KERNEL)

    modules_list = []
    if args.has_key("modules"):
        modules_list += args["modules"]
    # KERNEL must be last in the list to locate it at the first place in PATH
    if args["gui"] :
        modules_list[:0] = ["GUI"]
    modules_list[:0] = ["KERNEL"]
    modules_list.reverse()

    modules_root_dir = {}

    to_remove_list=[]
    for module in modules_list :
        module_variable=module+"_ROOT_DIR"
        if not os.environ.has_key(module_variable):
            print "*******************************************************"
            print "*"
            print "* Environment variable",module_variable,"must be set"
            print "* Module", module, "will be not available"
            print "*"
            print "********************************************************"
            to_remove_list.append(module)
            continue
            pass
        module_root_dir = os.environ[module_variable]
        modules_root_dir[module]=module_root_dir

    for to_remove in to_remove_list:
        modules_list.remove(to_remove)

    while "KERNEL" in modules_list:
        modules_list.remove("KERNEL")
        pass

    while "GUI" in modules_list:
        modules_list.remove("GUI")
        pass

    if "SUPERV" in modules_list and not 'supervContainer' in args['standalone']:
        args['standalone'].append("supervContainer")
        pass
   
    return args, modules_list, modules_root_dir

# -----------------------------------------------------------------------------

def set_env(args, modules_list, modules_root_dir):
    """Add to the PATH-variables modules specific paths"""
    
    python_version="python%d.%d" % sys.version_info[0:2]
    modules_root_dir_list = []
    if os.getenv('SALOME_BATCH') == None:
      os.putenv('SALOME_BATCH','0')
    if args["gui"] :
        modules_list = modules_list[:] + ["GUI"] 
    modules_list = modules_list[:] + ["KERNEL"] 
    for module in modules_list :
        if modules_root_dir.has_key(module):
            module_root_dir = modules_root_dir[module]
            modules_root_dir_list[:0] = [module_root_dir]
            if sys.platform == "win32":
              add_path(os.path.join(module_root_dir,get_lib_dir(),salome_subdir),
                     "PATH")
            else:
              add_path(os.path.join(module_root_dir,get_lib_dir(),salome_subdir),
                     "LD_LIBRARY_PATH")
            add_path(os.path.join(module_root_dir,"bin",salome_subdir),
                     "PATH")
            if os.path.exists(module_root_dir + "/examples") :
                add_path(os.path.join(module_root_dir,"examples"),
                         "PYTHONPATH")
                pass
            add_path(os.path.join(module_root_dir,"bin",salome_subdir),
                     "PYTHONPATH")
            # add lib before site-packages to load script instead of dll if any (win32 platform)
            add_path(os.path.join(module_root_dir,get_lib_dir(),salome_subdir),
                     "PYTHONPATH")
            add_path(os.path.join(module_root_dir,get_lib_dir(),
                                  python_version,"site-packages",
                                  salome_subdir),
                     "PYTHONPATH")
            add_path(os.path.join(module_root_dir,get_lib_dir(),
                                  python_version,"site-packages",
                                  salome_subdir,
                                  "shared_modules"),
                     "PYTHONPATH")
            pass
        pass

    if sys.platform == 'win32':
        os.environ["SALOMEPATH"]=";".join(modules_root_dir_list)
    else:
        os.environ["SALOMEPATH"]=":".join(modules_root_dir_list)
    
    # set trace environment variable
    
    if not os.environ.has_key("SALOME_trace"):
        os.environ["SALOME_trace"]="local"
    if args['file']:
        os.environ["SALOME_trace"]="file:"+args['file'][0]
    if args['logger']:
        os.environ["SALOME_trace"]="with_logger"

    # set environment for SMESH plugins

    if "SMESH" in modules_list:
        os.environ["SMESH_MeshersList"]="StdMeshers"
        if not os.environ.has_key("SALOME_StdMeshersResources"):
            os.environ["SALOME_StdMeshersResources"] \
            = modules_root_dir["SMESH"]+"/share/"+salome_subdir+"/resources/smesh"
            pass
        if args.has_key("SMESH_plugins"):
            for plugin in args["SMESH_plugins"]:
                plugin_root = ""
                if os.environ.has_key(plugin+"_ROOT_DIR"):
                    plugin_root = os.environ[plugin+"_ROOT_DIR"]
                else:
                    # workaround to avoid modifications of existing environment
                    if os.environ.has_key(plugin.upper()+"_ROOT_DIR"):
                        plugin_root = os.environ[plugin.upper()+"_ROOT_DIR"]
                        pass
                    pass
                if plugin_root != "":
                    os.environ["SMESH_MeshersList"] \
                    = os.environ["SMESH_MeshersList"]+":"+plugin
                    if not os.environ.has_key("SALOME_"+plugin+"Resources"):
                        os.environ["SALOME_"+plugin+"Resources"] \
                        = plugin_root+"/share/"+salome_subdir+"/resources/"+plugin.lower()
                    add_path(os.path.join(plugin_root,get_lib_dir(),python_version,
                                          "site-packages",salome_subdir),
                             "PYTHONPATH")
                    add_path(os.path.join(plugin_root,get_lib_dir(),salome_subdir),
                             "PYTHONPATH")


                    if sys.platform == "win32":
                      add_path(os.path.join(plugin_root,get_lib_dir(),salome_subdir),
                          "PATH")
                    else:
                      add_path(os.path.join(plugin_root,get_lib_dir(),salome_subdir),
                               "LD_LIBRARY_PATH")
                    add_path(os.path.join(plugin_root,"bin",salome_subdir),
                             "PYTHONPATH")
                    add_path(os.path.join(plugin_root,"bin",salome_subdir),
                             "PATH")
            pass
        pass

    # set environment for SUPERV module
    os.environ["ENABLE_MACRO_NODE"]="1"
    # set resources variables if not yet set
    # Done now by launchConfigureParser.py
    #if os.getenv("GUI_ROOT_DIR"):
        #if not os.getenv("SalomeAppConfig"): os.environ["SalomeAppConfig"] =  os.getenv("GUI_ROOT_DIR") + "/share/salome/resources/gui"

    os.environ["CSF_SALOMEDS_ResourcesDefaults"] \
    = os.path.join(modules_root_dir["KERNEL"],"share",
                   salome_subdir,"resources","kernel")

    if "GEOM" in modules_list:
        if verbose(): print "GEOM OCAF Resources" 
        
	# set CSF_PluginDefaults variable only if it is not customized
        # by the user

        if not os.getenv("CSF_PluginDefaults"):
    	    os.environ["CSF_PluginDefaults"] \
    	    = os.path.join(modules_root_dir["GEOM"],"share",
                    	   salome_subdir,"resources","geom")
        os.environ["CSF_GEOMDS_ResourcesDefaults"] \
        = os.path.join(modules_root_dir["GEOM"],"share",
                       salome_subdir,"resources","geom")
        if verbose(): print "GEOM Shape Healing Resources"
        os.environ["CSF_ShHealingDefaults"] \
        = os.path.join(modules_root_dir["GEOM"],"share",
                       salome_subdir,"resources","geom")

# -----------------------------------------------------------------------------

def main():
    args, modules_list, modules_root_dir = get_config()
    set_env(args, modules_list, modules_root_dir)
    return args

# -----------------------------------------------------------------------------

if __name__ == "__main__":
   import user
   args = main()
