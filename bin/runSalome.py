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

process_id = {}

# salome_subdir variable is used for composing paths like $KERNEL_ROOT_DIR/share/salome/resources/kernel, etc.
# before moving to SUIT-based gui, instead of salome_subdir there was args['appname'] used.
# but after - 'appname'  = "SalomeApp", so using it in making the subdirectory is an error.
salome_subdir = "salome"

# -----------------------------------------------------------------------------

def add_path(directory, variable_name):
    """Function helper to add environment variables"""
    if not os.environ.has_key(variable_name):
        os.environ[variable_name] = ""
        pass
    if os.path.exists(directory):
        newpath=[]
        for _dir in os.environ[variable_name].split(":"):
            if os.path.exists(_dir):
                if not os.path.samefile(_dir, directory):
                  newpath.append(_dir)
            else:
                if os.path.abspath(_dir) != os.path.abspath(directory):
                  newpath.append(_dir)
            pass
        import string
        newpath[:0] = [ directory ]
        newpath = string.join(newpath,":")
        os.environ[variable_name] = newpath
        if variable_name == "PYTHONPATH":
            sys.path[:0] = [directory]

# -----------------------------------------------------------------------------

__lib__dir__ = None
def get_lib_dir():
    global __lib__dir__
    if __lib__dir__: return __lib__dir__
    import platform
    if platform.architecture()[0] == "64bit":
        __lib__dir__ = "lib64"
    else:
        __lib__dir__ = "lib"
    return get_lib_dir()

# -----------------------------------------------------------------------------

def get_config():
    """
    Get list of modules, paths.
    
    Read args from launch configure xml file and command line options.
    Check variables <module>_ROOT_DIR and set list of used modules.
    Return args, modules_list, modules_root_dir    
    """
    
    # read args from launch configure xml file and command line options
    
    import launchConfigureParser
    args = launchConfigureParser.get_env()
    
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

    if "SUPERV" in modules_list and not 'superv' in args['standalone']:
        args['standalone'].append("superv")
        pass
   
    return args, modules_list, modules_root_dir

# -----------------------------------------------------------------------------

def set_env(args, modules_list, modules_root_dir):
    """Add to the PATH-variables modules specific paths"""
    
    python_version="python%d.%d" % sys.version_info[0:2]
    modules_root_dir_list = []
    if args["gui"] :
        modules_list = modules_list[:] + ["GUI"] 
    modules_list = modules_list[:] + ["KERNEL"] 
    for module in modules_list :
        if modules_root_dir.has_key(module):
            module_root_dir = modules_root_dir[module]
            modules_root_dir_list[:0] = [module_root_dir]
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
            add_path(os.path.join(module_root_dir,get_lib_dir(),
                                  python_version,"site-packages",
                                  salome_subdir),
                     "PYTHONPATH")
            add_path(os.path.join(module_root_dir,get_lib_dir(),salome_subdir),
                     "PYTHONPATH")
            add_path(os.path.join(module_root_dir,get_lib_dir(),
                                  python_version,"site-packages",
                                  salome_subdir,
                                  "shared_modules"),
                     "PYTHONPATH")
            pass
        pass


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
            = modules_root_dir["SMESH"]+"/share/"+args["appname"]+"/resources/smesh"
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
                        = plugin_root+"/share/"+args["appname"]+"/resources/"+plugin.lower()
                    add_path(os.path.join(plugin_root,get_lib_dir(),python_version,
                                          "site-packages",salome_subdir),
                             "PYTHONPATH")
                    add_path(os.path.join(plugin_root,get_lib_dir(),salome_subdir),
                             "PYTHONPATH")
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

    # set CSF_PluginDefaults variable only if it is not customized
    # by the user
    if not os.getenv("CSF_PluginDefaults"):
        os.environ["CSF_PluginDefaults"] \
        = os.path.join(modules_root_dir["KERNEL"],"share",
                       salome_subdir,"resources","kernel")
    os.environ["CSF_SALOMEDS_ResourcesDefaults"] \
    = os.path.join(modules_root_dir["KERNEL"],"share",
                   salome_subdir,"resources","kernel")

    if "GEOM" in modules_list:
        if verbose(): print "GEOM OCAF Resources"
        os.environ["CSF_GEOMDS_ResourcesDefaults"] \
        = os.path.join(modules_root_dir["GEOM"],"share",
                       salome_subdir,"resources","geom")
        if verbose(): print "GEOM Shape Healing Resources"
        os.environ["CSF_ShHealingDefaults"] \
        = os.path.join(modules_root_dir["GEOM"],"share",
                       salome_subdir,"resources","geom")

# -----------------------------------------------------------------------------

from killSalome import killAllPorts

def killLocalPort():
    """
    kill servers from a previous SALOME exection, if needed,
    on the CORBA port given in args of runSalome
    """
    
    from killSalomeWithPort import killMyPort
    my_port=str(args['port'])
    try:
        killMyPort(my_port)
    except:
        print "problem in killLocalPort()"
        pass
    pass
    
def givenPortKill(port):
    """
    kill servers from a previous SALOME exection, if needed,
    on the same CORBA port
    """
    
    from killSalomeWithPort import killMyPort
    my_port=port
    try:
        killMyPort(my_port)
    except:
        print "problem in LocalPortKill(), killMyPort("<<port<<")"
        pass
    pass

def kill_salome(args):
    """
    Kill servers from previous SALOME executions, if needed;
    depending on args 'killall' or 'portkill', kill all executions,
    or only execution on the same CORBA port
    """

    if args['killall']:
        killAllPorts()
    elif args['portkill']:
        givenPortKill(str(args['port']))
	
# -----------------------------------------------------------------------------
#
# Definition des classes d'objets pour le lancement des Server CORBA
#

class Server:
    """Generic class for CORBA server launch"""

    def initArgs(self):
        self.PID=None
        self.CMD=[]
        self.ARGS=[]	
        if self.args['xterm']:
            self.ARGS=['xterm', '-iconic', '-sb', '-sl', '500', '-hold']

    def __init__(self,args):
        self.args=args
        self.initArgs()


    def run(self):
        global process_id
        myargs=self.ARGS
        if self.args['xterm']:
            # (Debian) send LD_LIBRARY_PATH to children shells (xterm)
            env_ld_library_path=['env', 'LD_LIBRARY_PATH='
                                 + os.getenv("LD_LIBRARY_PATH")]
            myargs = myargs +['-T']+self.CMD[:1]+['-e'] + env_ld_library_path
        command = myargs + self.CMD
        if verbose(): print "command = ", command
        pid = os.spawnvp(os.P_NOWAIT, command[0], command)
        process_id[pid]=self.CMD
        self.PID = pid


class InterpServer(Server):
    def __init__(self,args):
        self.args=args
        env_ld_library_path=['env', 'LD_LIBRARY_PATH=' + os.getenv("LD_LIBRARY_PATH")]
        self.CMD=['xterm', '-e']+ env_ld_library_path + ['python']
        #self.CMD=['xterm', '-e', 'python']
       
    def run(self):
        global process_id
        command = self.CMD
        #print "command = ", command
        pid = os.spawnvp(os.P_NOWAIT, command[0], command)
        process_id[pid]=self.CMD
        self.PID = pid

# ---

class CatalogServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.SCMD1=['SALOME_ModuleCatalog_Server','-common']
        self.SCMD2=[]
        home_dir=os.getenv('HOME')
        if home_dir is not None:
            self.SCMD2=['-personal',os.path.join(home_dir,'Salome/resources/CatalogModulePersonnel.xml')] 

    def setpath(self,modules_list,modules_root_dir):
        cata_path=[]
        list_modules = modules_list[:]
        list_modules.reverse()
        if self.args["gui"] :
            list_modules = ["KERNEL", "GUI"] + list_modules
        else :
            list_modules = ["KERNEL"] + list_modules
        for module in list_modules:
            if modules_root_dir.has_key(module):
                module_root_dir=modules_root_dir[module]
                module_cata=module+"Catalog.xml"
                #print "   ", module_cata
                if os.path.exists(os.path.join(module_root_dir,
                                               "share",salome_subdir,
                                               "resources",module.lower(),
                                               module_cata)):
                    cata_path.extend(
                        glob.glob(os.path.join(module_root_dir,
                                               "share",salome_subdir,
                                               "resources",module.lower(),
                                               module_cata)))
                else:
                    cata_path.extend(
                        glob.glob(os.path.join(module_root_dir,
                                               "share",salome_subdir,
                                               "resources",
                                               module_cata)))
                pass
            pass
        self.CMD=self.SCMD1 + [string.join(cata_path,':')] + self.SCMD2

# ---

class SalomeDSServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.CMD=['SALOMEDS_Server']

# ---

class RegistryServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.CMD=['SALOME_Registry_Server', '--salome_session','theSession']

# ---

class ContainerCPPServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.CMD=['SALOME_Container','FactoryServer']

# ---

class ContainerPYServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.CMD=['SALOME_ContainerPy.py','FactoryServerPy']

# ---

class ContainerSUPERVServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.CMD=['SALOME_Container','SuperVisionContainer']

# ---

class LoggerServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.CMD=['SALOME_Logger_Server', 'logger.log']

# ---

class SessionServer(Server):
    def __init__(self,args):
        self.args = args.copy()
        # Bug 11512 (Problems with runSalome --xterm on Mandrake and Debian Sarge)
        self.args['xterm']=0
        #
        self.initArgs()
        self.SCMD1=['SALOME_Session_Server']
        self.SCMD2=[]
        if 'registry' in self.args['embedded']:
            self.SCMD1+=['--with','Registry',
                         '(','--salome_session','theSession',')']
        if 'moduleCatalog' in self.args['embedded']:
            self.SCMD1+=['--with','ModuleCatalog','(','-common']
            home_dir=os.getenv('HOME')
            if home_dir is not None:
                self.SCMD2+=['-personal',os.path.join(home_dir,'Salome/resources/CatalogModulePersonnel.xml')] 
            self.SCMD2+=[')']
        if 'study' in self.args['embedded']:
            self.SCMD2+=['--with','SALOMEDS','(',')']
        if 'cppContainer' in self.args['embedded']:
            self.SCMD2+=['--with','Container','(','FactoryServer',')']
        if 'SalomeAppEngine' in self.args['embedded']:
            self.SCMD2+=['--with','SalomeAppEngine','(',')']
            
        if 'cppContainer' in self.args['standalone'] or 'cppContainer' in self.args['embedded']:
            self.SCMD2+=['CPP']
        if 'pyContainer' in self.args['standalone'] or 'pyContainer' in self.args['embedded']:
            self.SCMD2+=['PY']
        if 'supervContainer' in self.args['containers'] or 'supervContainer' in self.args['standalone']:
            self.SCMD2+=['SUPERV']
        if self.args['gui']:
            try:
                session_gui = self.args['session_gui']
            except KeyError:
                session_gui = 1
                pass
            if session_gui:
                self.SCMD2+=['GUI']
                pass
            pass
        if self.args['splash'] and self.args['gui']:
            self.SCMD2+=['SPLASH']
        if self.args['noexcepthandler']:
            self.SCMD2+=['noexcepthandler']
        if self.args.has_key('modules'):
            self.SCMD2+=['--modules (%s)'%":".join(self.args['modules'])]
        if self.args.has_key('test') and len(args['test']) > 0:
            self.SCMD2+=['--test=%s'%(",".join(args['test']))]

    def setpath(self,modules_list,modules_root_dir):
        cata_path=[]
        list_modules = modules_list[:]
        list_modules.reverse()
        if self.args["gui"] :
            list_modules = ["KERNEL", "GUI"] + list_modules
        else :
            list_modules = ["KERNEL"] + list_modules
        for module in list_modules:
            module_root_dir=modules_root_dir[module]
            module_cata=module+"Catalog.xml"
            #print "   ", module_cata
            if os.path.exists(os.path.join(module_root_dir,
                                           "share",salome_subdir,
                                           "resources",module.lower(),
                                           module_cata)):
                cata_path.extend(
                    glob.glob(os.path.join(module_root_dir,"share",
                                           salome_subdir,"resources",
                                           module.lower(),module_cata)))
            else:
                cata_path.extend(
                    glob.glob(os.path.join(module_root_dir,"share",
                                           salome_subdir,"resources",
                                           module_cata)))
            pass
        if (self.args["gui"]) & ('moduleCatalog' in self.args['embedded']):
            self.CMD=self.SCMD1 + [string.join(cata_path,':')] + self.SCMD2
        else:
            self.CMD=self.SCMD1 + self.SCMD2
      
# ---

class ContainerManagerServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.SCMD1=['SALOME_ContainerManagerServer']
        self.SCMD2=[]
        if args["gui"] :
            if 'registry' in self.args['embedded']:
                self.SCMD1+=['--with','Registry',
                             '(','--salome_session','theSession',')']
            if 'moduleCatalog' in self.args['embedded']:
                self.SCMD1+=['--with','ModuleCatalog','(','-common']
                self.SCMD2+=['-personal',
                             '${HOME}/Salome/resources/CatalogModulePersonnel.xml',')']
            if 'study' in self.args['embedded']:
                self.SCMD2+=['--with','SALOMEDS','(',')']
            if 'cppContainer' in self.args['embedded']:
                self.SCMD2+=['--with','Container','(','FactoryServer',')']
        
    def setpath(self,modules_list,modules_root_dir):
        cata_path=[]
        list_modules = modules_list[:]
        list_modules.reverse()
        if self.args["gui"] :
            list_modules = ["GUI"] + list_modules
        for module in ["KERNEL"] + list_modules:
            if modules_root_dir.has_key(module):
                module_root_dir=modules_root_dir[module]
                module_cata=module+"Catalog.xml"
                #print "   ", module_cata
                if os.path.exists(os.path.join(module_root_dir,
                                               "share",salome_subdir,
                                               "resources",module.lower(),
                                               module_cata)):
                    cata_path.extend(
                        glob.glob(os.path.join(module_root_dir,"share",
                                               self.args['appname'],"resources",
                                               module.lower(),module_cata)))
                else:
                    cata_path.extend(
                        glob.glob(os.path.join(module_root_dir,"share",
                                               self.args['appname'],"resources",
                                               module_cata)))
                pass
            pass
        if (self.args["gui"]) & ('moduleCatalog' in self.args['embedded']):
            self.CMD=self.SCMD1 + [string.join(cata_path,':')] + self.SCMD2
        else:
            self.CMD=self.SCMD1 + self.SCMD2

class NotifyServer(Server):
    def __init__(self,args,modules_root_dir):
        self.args=args
        self.initArgs()
        self.modules_root_dir=modules_root_dir
        myLogName = os.environ["LOGNAME"]
        self.CMD=['notifd','-c',
                  self.modules_root_dir["KERNEL"] +'/share/salome/resources/kernel/channel.cfg',
                  '-DFactoryIORFileName=/tmp/'+myLogName+'_rdifact.ior',
                  '-DChannelIORFileName=/tmp/'+myLogName+'_rdichan.ior',
                  '-DReportLogFile=/tmp/'+myLogName+'_notifd.report',
                  '-DDebugLogFile=/tmp/'+myLogName+'_notifd.debug',
                  ]

#
# -----------------------------------------------------------------------------

def startGUI():
    """Salome Session Graphic User Interface activation"""
    import Engines
    import SALOME
    import SALOMEDS
    import SALOME_ModuleCatalog
    reload(Engines)
    reload(SALOME)
    reload(SALOMEDS)
    import SALOME_Session_idl
    session=clt.waitNS("/Kernel/Session",SALOME.Session)
    session.GetInterface()
  
# -----------------------------------------------------------------------------

def startSalome(args, modules_list, modules_root_dir):
    """Launch all SALOME servers requested by args"""
    init_time = os.times()

    if verbose(): print "startSalome ", args
    
    #
    # Initialisation ORB et Naming Service
    #
   
    clt=orbmodule.client()

    # (non obligatoire) Lancement Logger Server
    # et attente de sa disponibilite dans le naming service
    #

    if args['logger']:
        myServer=LoggerServer(args)
        myServer.run()
        clt.waitLogger("Logger")

    # Notify Server launch
    #

    if verbose(): print "Notify Server to launch"

    myServer=NotifyServer(args,modules_root_dir)
    myServer.run()

    # Lancement Session Server (to show splash ASAP)
    #

    if args["gui"]:
        mySessionServ = SessionServer(args)
        mySessionServ.setpath(modules_list,modules_root_dir)
        mySessionServ.run()

    #
    # Lancement Registry Server,
    # attente de la disponibilite du Registry dans le Naming Service
    #

    if ('registry' not in args['embedded']) | (args["gui"] == 0) :
        myServer=RegistryServer(args)
        myServer.run()
        clt.waitNSPID("/Registry",myServer.PID)

    #
    # Lancement Catalog Server,
    # attente de la disponibilite du Catalog Server dans le Naming Service
    #
    

    if ('moduleCatalog' not in args['embedded']) | (args["gui"] == 0):
        cataServer=CatalogServer(args)
        cataServer.setpath(modules_list,modules_root_dir)
        cataServer.run()
        import SALOME_ModuleCatalog
        clt.waitNSPID("/Kernel/ModulCatalog",cataServer.PID,SALOME_ModuleCatalog.ModuleCatalog)

    #
    # Lancement SalomeDS Server,
    # attente de la disponibilite du SalomeDS dans le Naming Service
    #

    #print "ARGS = ",args
    if ('study' not in args['embedded']) | (args["gui"] == 0):
        print "RunStudy"
        myServer=SalomeDSServer(args)
        myServer.run()
        clt.waitNSPID("/myStudyManager",myServer.PID)

    #
    # Lancement ContainerManagerServer
    #
    
    myCmServer = ContainerManagerServer(args)
    myCmServer.setpath(modules_list,modules_root_dir)
    myCmServer.run()


    from Utils_Identity import getShortHostName
    
    if os.getenv("HOSTNAME") == None:
        if os.getenv("HOST") == None:
            os.environ["HOSTNAME"]=getShortHostName()
        else:
            os.environ["HOSTNAME"]=os.getenv("HOST")

    theComputer = getShortHostName()
    
    #
    # Lancement Container C++ local,
    # attente de la disponibilite du Container C++ local dans le Naming Service
    #

    if ('cppContainer' in args['standalone']) | (args["gui"] == 0):
        myServer=ContainerCPPServer(args)
        myServer.run()
        clt.waitNSPID("/Containers/" + theComputer + "/FactoryServer",myServer.PID)

    #
    # Lancement Container Python local,
    # attente de la disponibilite du Container Python local
    # dans le Naming Service
    #

    if 'pyContainer' in args['standalone']:
        myServer=ContainerPYServer(args)
        myServer.run()
        clt.waitNSPID("/Containers/" + theComputer + "/FactoryServerPy",myServer.PID)

    #
    # Lancement Container Supervision local,
    # attente de la disponibilite du Container Supervision local
    # dans le Naming Service
    #

    if 'supervContainer' in args['standalone']:
        myServer=ContainerSUPERVServer(args)
        myServer.run()
        clt.waitNSPID("/Containers/" + theComputer + "/SuperVisionContainer",myServer.PID)

    #
    # Attente de la disponibilite du Session Server dans le Naming Service
    #
    
    if args["gui"]:
##----------------        
        import Engines
        import SALOME
        import SALOMEDS
        import SALOME_ModuleCatalog
        reload(Engines)
        reload(SALOME)
        reload(SALOMEDS)
        import SALOME_Session_idl
        session=clt.waitNSPID("/Kernel/Session",mySessionServ.PID,SALOME.Session)

    end_time = os.times()
    if verbose(): print
    print "Start SALOME, elapsed time : %5.1f seconds"% (end_time[4]
                                                         - init_time[4])

    # ASV start GUI without Loader
    #if args['gui']:
    #    session.GetInterface()

    #
    # additionnal external python interpreters
    #
    nbaddi=0
    
    try:
        if 'interp' in args:
            if args['interp']:
                nbaddi = int(args['interp'][0])
    except:
        import traceback
        traceback.print_exc()
        print "-------------------------------------------------------------"
        print "-- to get an external python interpreter:runSalome --interp=1"
        print "-------------------------------------------------------------"
        
    print "additional external python interpreters: ", nbaddi
    if nbaddi:
        for i in range(nbaddi):
            print "i=",i
            anInterp=InterpServer(args)
            anInterp.run()
    
    return clt

# -----------------------------------------------------------------------------

def useSalome(args, modules_list, modules_root_dir):
    """
    Launch all SALOME servers requested by args,
    save list of process, give info to user,
    show registered objects in Naming Service.
    """
    
    clt=None
    try:
        clt = startSalome(args, modules_list, modules_root_dir)
    except:
        import traceback
        traceback.print_exc()
        print
        print
        print "--- erreur au lancement Salome ---"
        
    #print process_id

    from killSalomeWithPort import getPiDict
    filedict = getPiDict(args['port'])

    process_ids = []
    try:
        fpid=open(filedict, 'r')
        process_ids=pickle.load(fpid)
        fpid.close()
    except:
        pass
    
    fpid=open(filedict, 'w')
    process_ids.append(process_id)
    pickle.dump(process_ids,fpid)
    fpid.close()
    
    if verbose(): print """
    Saving of the dictionary of Salome processes in %s
    To kill SALOME processes from a console (kill all sessions from all ports):
      python killSalome.py 
    To kill SALOME from the present interpreter, if it is not closed :
      killLocalPort()      --> kill this session
                               (use CORBA port from args of runSalome)
      givenPortKill(port)  --> kill a specific session with given CORBA port 
      killAllPorts()       --> kill all sessions
    
    runSalome, with --killall option, starts with killing
    the processes resulting from the previous execution.
    """%filedict
    
    #
    #  Impression arborescence Naming Service
    #
    
    if clt != None:
        if verbose():
            print
            print " --- registered objects tree in Naming Service ---"
            clt.showNS()
            pass
        
        # run python scripts, passed via -t option
        toimport = args['pyscript']
        i = 0
        while i < len( toimport ) :
            if toimport[ i ] == 'killall':
                killAllPorts()
                import sys
                sys.exit(0)
            else:
                print 'importing',toimport[ i ]
                doimport = 'import ' + toimport[ i ]
                exec doimport
            i = i + 1

    return clt

# -----------------------------------------------------------------------------

def registerEnv(args, modules_list, modules_root_dir):
    """
    Register args, modules_list, modules_root_dir in a file
    for further use, when SALOME is launched embedded in an other application.
    """
    fileEnv = '/tmp/' + os.getenv('USER') + "_" + str(args['port']) \
            + '_' + args['appname'].upper() + '_env'
    fenv=open(fileEnv,'w')
    pickle.dump((args, modules_list, modules_root_dir),fenv)
    fenv.close()
    os.environ["SALOME_LAUNCH_CONFIG"] = fileEnv

# -----------------------------------------------------------------------------

def searchFreePort(args, save_config=1):
    print "Searching for a free port for naming service:",
    NSPORT=2810
    limit=NSPORT
    limit=limit+10
    while 1:
        import os
        status = os.system("netstat -ltn | grep -E :%s > /dev/null 2>&1"%(NSPORT))
        if status:
            print "%s - OK"%(NSPORT)
            #
            from os import getpid
            tmp_file = '/tmp/hostname_%s'%(getpid())
            from os import system
            system('hostname > %s'%(tmp_file))
            f = open(tmp_file)
            myhost = f.read()
            myhost = myhost[:-1]
            f.close()
            system('rm -f %s'%(tmp_file))
            #
            home = os.environ['HOME']
            appli=os.environ.get("APPLI")
            if appli is not None:
                home='%s/%s'%(home,appli)
                pass
            #
            os.environ['OMNIORB_CONFIG'] = '%s/.omniORB_%s_%s.cfg'%(home, myhost, NSPORT)
            initref = "NameService=corbaname::%s:%s"%(myhost, NSPORT)
            os.environ['NSPORT'] = "%s"%(NSPORT)
            os.environ['NSHOST'] = "%s"%(myhost)
            f = open(os.environ['OMNIORB_CONFIG'], "w")
            import CORBA
            if CORBA.ORB_ID == "omniORB4":
                f.write("InitRef = %s\n"%(initref))
            else:
                f.write("ORBInitRef %s\n"%(initref))
                pass
            f.close()
            args['port'] = os.environ['NSPORT']
            #
            if save_config:
                from os import system
                system('ln -sf %s %s/.omniORB_last.cfg'%(os.environ['OMNIORB_CONFIG'], home))
                pass
            #
            break
        print "%s"%(NSPORT),
        if NSPORT == limit:
            msg  = "\n"
            msg += "Can't find a free port to launch omniNames\n"
            msg += "Try to kill the running servers and then launch SALOME again.\n"
            raise msg
        NSPORT=NSPORT+1
        pass
    return
    
# -----------------------------------------------------------------------------

def no_main():
    """Salome Launch, when embedded in other application"""
    fileEnv = os.environ["SALOME_LAUNCH_CONFIG"]
    fenv=open(fileEnv,'r')
    args, modules_list, modules_root_dir = pickle.load(fenv)
    fenv.close()
    kill_salome(args)
    searchFreePort(args, 0)
    clt = useSalome(args, modules_list, modules_root_dir)
    return clt

# -----------------------------------------------------------------------------

def main():
    """Salome launch as a main application"""
    import sys
    if len(sys.argv) == 2:
        if sys.argv[1] == "-nothing":
            searchFreePort({})
            import os
            print "port:%s"%(os.environ['NSPORT'])
            import sys
            sys.exit(0)
            pass
        pass
    save_config = 1
    import sys
    if len(sys.argv) > 1:
        if sys.argv[1] == "--nosave-config":
            save_config = 0
            sys.argv[1:2] = []
            pass
        pass
    #
    args, modules_list, modules_root_dir = get_config()
    kill_salome(args)
    searchFreePort(args, save_config)
    set_env(args, modules_list, modules_root_dir)
    clt = useSalome(args, modules_list, modules_root_dir)
    return clt,args

# -----------------------------------------------------------------------------

if __name__ == "__main__":
   import user
   clt,args = main()
