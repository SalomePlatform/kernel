#!/usr/bin/env python

import sys, os, string, glob, time, pickle
import orbmodule

process_id = {}

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

def get_config():
    """
    Get list of modules, paths.
    
    Read args from launch configure xml file and command line options.
    Check variables <module>_ROOT_DIR and set list of used modules.
    Return args, modules_list, modules_root_dir    
    """
    
    # read args from launch configure xml file and command line options
    
    import launchConfigureParser
    args = launchConfigureParser.args
    
    # Check variables <module>_ROOT_DIR
    # and set list of used modules (without KERNEL)

    modules_list = []
    if args.has_key("modules"):
        modules_list += args["modules"]
    # KERNEL must be last in the list to locate it at the first place in PATH 
    modules_list[:0] = ["KERNEL"]
    modules_list.reverse()

    modules_root_dir = {}

    to_remove_list=[]
    for module in modules_list :
        module_variable=module.upper()+"_ROOT_DIR"
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

    if "SUPERV" in modules_list and not 'superv' in args['standalone']:
        args['standalone'].append("superv")
        pass
   
    return args, modules_list, modules_root_dir

# -----------------------------------------------------------------------------

def set_env(args, modules_list, modules_root_dir):
    """Add to the PATH-variables modules specific paths"""
    
    python_version="python%d.%d" % sys.version_info[0:2]
    modules_root_dir_list = []
    modules_list = modules_list[:] + ["KERNEL"] 
    for module in modules_list :
        module_root_dir = modules_root_dir[module]
        modules_root_dir_list[:0] = [module_root_dir]
        add_path(os.path.join(module_root_dir,"lib",args['appname']),
                 "LD_LIBRARY_PATH")
        add_path(os.path.join(module_root_dir,"bin",args['appname']),
                 "PATH")
        if os.path.exists(module_root_dir + "/examples") :
            add_path(os.path.join(module_root_dir,"examples"),
                     "PYTHONPATH")
        add_path(os.path.join(module_root_dir,"bin",args['appname']),
                 "PYTHONPATH")
        add_path(os.path.join(module_root_dir,"lib",
                              python_version,"site-packages",args['appname']),
                 "PYTHONPATH")
        add_path(os.path.join(module_root_dir,"lib",args['appname']),
                 "PYTHONPATH")
        add_path(os.path.join(module_root_dir,"lib",
                              python_version,"site-packages",args['appname'],
                              "shared_modules"),
                 "PYTHONPATH")

    os.environ["SALOMEPATH"]=":".join(modules_root_dir_list)
    
    # special path for logger lib if needeed
    
    os.environ["SALOME_trace"]="local"
    if args['file']:
        os.environ["SALOME_trace"]=args['file'][0]
    if args['logger']:
        os.environ["SALOME_trace"]="with_logger"
        locdir=os.environ['PWD']
        libtracedir=os.path.join(locdir,"libSalomeTrace")
        libtrace = os.path.join(modules_root_dir["KERNEL"],"lib",
                                args['appname'],
                                "libSALOMELoggerClient.so.0.0.0")
        libtraceln = os.path.join(libtracedir,"libSALOMELocalTrace.so")
        aCommand = 'rm -rf ' + libtracedir + "; "
        aCommand += 'mkdir ' + libtracedir + "; "
        aCommand += 'ln -s ' + libtrace + " " + libtraceln + "; "
        aCommand += 'ln -s ' + libtrace + " " + libtraceln + ".0; "
        aCommand += 'ln -s ' + libtrace + " " + libtraceln + ".0.0.0; "
        os.system(aCommand)
        add_path(libtracedir, "LD_LIBRARY_PATH")

    # set environment for SMESH plugins

    if "SMESH" in modules_list:
        os.environ["SMESH_MeshersList"]="StdMeshers"
        if not os.environ.has_key("SALOME_StdMeshersResources"):
            os.environ["SALOME_StdMeshersResources"] \
            = modules_root_dir["SMESH"]+"/share/"+args["appname"]+"/resources"
            pass
        if args.has_key("SMESH_plugins"):
            for plugin in args["SMESH_plugins"]:
                if os.environ.has_key(plugin.upper()+"_ROOT_DIR"):
                    os.environ["SMESH_MeshersList"] \
                    = os.environ["SMESH_MeshersList"]+":"+plugin
                    plugin_root = os.environ[plugin.upper()+"_ROOT_DIR"]
                    if not os.environ.has_key("SALOME_"+plugin+"Resources"):
                        os.environ["SALOME_"+plugin+"Resources"] \
                        = plugin_root+"/share/"+args["appname"]+"/resources"
                    add_path(os.path.join(plugin_root,"lib",python_version,
                                          "site-packages",args['appname']),
                             "PYTHONPATH")
                    add_path(os.path.join(plugin_root,"lib",args['appname']),
                             "PYTHONPATH")
                    add_path(os.path.join(plugin_root,"lib",args['appname']),
                             "LD_LIBRARY_PATH")
                    add_path(os.path.join(plugin_root,"bin",args['appname']),
                             "PYTHONPATH")
                    add_path(os.path.join(plugin_root,"bin",args['appname']),
                             "PATH")
            pass
        pass

    # set environment for SUPERV module
    os.environ["ENABLE_MACRO_NODE"]="1"
   

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
# Définition des classes d'objets pour le lancement des Server CORBA
#

class Server:
    """Generic class for CORBA server launch"""

    def initArgs(self):
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
        # print "command = ", command
        pid = os.spawnvp(os.P_NOWAIT, command[0], command)
        process_id[pid]=self.CMD

# ---

class CatalogServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.SCMD1=['SALOME_ModuleCatalog_Server','-common']
        self.SCMD2=['-personal',
                    '${HOME}/Salome/resources/CatalogModulePersonnel.xml'] 

    def setpath(self,modules_list,modules_root_dir):
        cata_path=[]
        list_modules = modules_list[:]
        list_modules.reverse()
        for module in ["KERNEL"] + list_modules:
            module_root_dir=modules_root_dir[module]
            module_cata=module+"Catalog.xml"
            print "   ", module_cata
            cata_path.extend(
                glob.glob(os.path.join(module_root_dir,
                                       "share",self.args['appname'],
                                       "resources",module_cata)))
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

class SessionLoader(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.CMD=['SALOME_Session_Loader']
        if "cppContainer" in self.args['standalone'] \
        or "cppContainer" in self.args['embedded']:
            self.CMD=self.CMD+['CPP']
        if "pyContainer" in self.args['standalone'] \
        or "pyContainer" in self.args['embedded']:
            self.CMD=self.CMD+['PY']
        if "supervContainer" in self.args['containers'] \
        or "supervContainer" in self.args['standalone']:
            self.CMD=self.CMD+['SUPERV']
        if self.args['gui']:
            self.CMD=self.CMD+['GUI']
        print self.CMD

# ---

class SessionServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.SCMD1=['SALOME_Session_Server']
        self.SCMD2=[]
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
        for module in ["KERNEL"] + list_modules:
            module_root_dir=modules_root_dir[module]
            module_cata=module+"Catalog.xml"
            print "   ", module_cata
            cata_path.extend(
                glob.glob(os.path.join(module_root_dir,"share",
                                       self.args['appname'],"resources",
                                       module_cata)))
        if 'moduleCatalog' in self.args['embedded']:
            self.CMD=self.SCMD1 + [string.join(cata_path,':')] + self.SCMD2
        else:
            self.CMD=self.SCMD1 + self.SCMD2

##      # arguments SALOME_Session_Server pour ddd
##      comm_ddd=""
##      for mot in self.CMD:
##          if mot == "(":
##              comm_ddd+='"(" '
##          elif mot == ")":
##              comm_ddd+='")" '
##          else:
##              comm_ddd+=mot+" "
##      print comm_ddd
      
# ---

class NotifyServer(Server):
    def __init__(self,args,modules_root_dir):
        self.args=args
        self.initArgs()
        self.modules_root_dir=modules_root_dir
        myLogName = os.environ["LOGNAME"]
        self.CMD=['notifd','-c',
                  self.modules_root_dir["KERNEL"]
                  +'/share/salome/resources/channel.cfg',
                  '-DFactoryIORFileName=/tmp/'+myLogName+'_rdifact.ior',
                  '-DChannelIORFileName=/tmp/'+myLogName+'_rdichan.ior']

#
# -----------------------------------------------------------------------------

def startGUI():
    """Salome Session Graphic User Interface activation"""
    import SALOME
    session=clt.waitNS("/Kernel/Session",SALOME.Session)
    session.GetInterface()
  
# -----------------------------------------------------------------------------

def startSalome(args, modules_list, modules_root_dir):
    """Launch all SALOME servers requested by args"""
    init_time = os.times()

    #
    # Lancement Session Loader
    #

    print "startSalome ", args
    
    if args['gui']:
        myServer=SessionLoader(args)
        myServer.run()

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

    myServer=NotifyServer(args,modules_root_dir)
    myServer.run()

    #
    # Lancement Registry Server,
    # attente de la disponibilité du Registry dans le Naming Service
    #

    if 'registry' not in args['embedded']:
        myServer=RegistryServer(args)
        myServer.run()
        clt.waitNS("/Registry")

    #
    # Lancement Catalog Server,
    # attente de la disponibilité du Catalog Server dans le Naming Service
    #

    if 'moduleCatalog' not in args['embedded']:
        cataServer=CatalogServer(args)
        cataServer.setpath(modules_list,modules_root_dir)
        cataServer.run()
        import SALOME_ModuleCatalog
        clt.waitNS("/Kernel/ModulCatalog",SALOME_ModuleCatalog.ModuleCatalog)

    #
    # Lancement SalomeDS Server,
    # attente de la disponibilité du SalomeDS dans le Naming Service
    #

    os.environ["CSF_PluginDefaults"] \
    = os.path.join(modules_root_dir["KERNEL"],"share",
                   args['appname'],"resources")
    os.environ["CSF_SALOMEDS_ResourcesDefaults"] \
    = os.path.join(modules_root_dir["KERNEL"],"share",
                   args['appname'],"resources")

    if "GEOM" in modules_list:
        print "GEOM OCAF Resources"
        os.environ["CSF_GEOMDS_ResourcesDefaults"] \
        = os.path.join(modules_root_dir["GEOM"],"share",
                       args['appname'],"resources")
	print "GEOM Shape Healing Resources"
        os.environ["CSF_ShHealingDefaults"] \
        = os.path.join(modules_root_dir["GEOM"],"share",
                       args['appname'],"resources")

    if 'study' not in args['embedded']:
        myServer=SalomeDSServer(args)
        myServer.run()
        clt.waitNS("/myStudyManager")

    #
    # Lancement Session Server
    #

    mySessionServ = SessionServer(args)
    mySessionServ.setpath(modules_list,modules_root_dir)
    mySessionServ.run()

    #macomm2=['ddd']
    #pid = os.spawnvp(os.P_NOWAIT, macomm2[0], macomm2)
    #
    # Attente de la disponibilité du Session Server dans le Naming Service
    #

    import SALOME
    session=clt.waitNS("/Kernel/Session",SALOME.Session)

    from Utils_Identity import getShortHostName
    
    if os.getenv("HOSTNAME") == None:
        if os.getenv("HOST") == None:
            os.environ["HOSTNAME"]=getShortHostName()
        else:
            os.environ["HOSTNAME"]=os.getenv("HOST")

    theComputer = getShortHostName()
  
    #
    # Lancement Container C++ local,
    # attente de la disponibilité du Container C++ local dans le Naming Service
    #

    if 'cppContainer' in args['standalone']:
        myServer=ContainerCPPServer(args)
        myServer.run()
        clt.waitNS("/Containers/" + theComputer + "/FactoryServer")

    #
    # Lancement Container Python local,
    # attente de la disponibilité du Container Python local
    # dans le Naming Service
    #

    if 'pyContainer' in args['standalone']:
        myServer=ContainerPYServer(args)
        myServer.run()
        clt.waitNS("/Containers/" + theComputer + "/FactoryServerPy")

    #
    # Lancement Container Supervision local,
    # attente de la disponibilité du Container Supervision local
    # dans le Naming Service
    #

    if 'supervContainer' in args['standalone']:
        myServer=ContainerSUPERVServer(args)
        myServer.run()
        clt.waitNS("/Containers/" + theComputer + "/SuperVisionContainer")

    end_time = os.times()
    print
    print "Start SALOME, elapsed time : %5.1f seconds"% (end_time[4]
                                                         - init_time[4])

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
        print
        print
        print "--- erreur au lancement Salome ---"
        
    #print process_id
    
    filedict = '/tmp/' + os.getenv('USER') + "_" + str(args['port']) \
             + '_' + args['appname'].upper() + '_pidict'
   
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
    
    print """
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
        print
        print " --- registered objects tree in Naming Service ---"
        clt.showNS()

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

def no_main():
    """Salome Launch, when embedded in other application"""
    fileEnv = os.environ["SALOME_LAUNCH_CONFIG"]
    fenv=open(fileEnv,'r')
    args, modules_list, modules_root_dir = pickle.load(fenv)
    fenv.close()
    kill_salome(args)
    clt = useSalome(args, modules_list, modules_root_dir)
    return clt

# -----------------------------------------------------------------------------

def main():
    """Salome launch as a main application"""
    args, modules_list, modules_root_dir = get_config()
    kill_salome(args)
    set_env(args, modules_list, modules_root_dir)
    clt = useSalome(args, modules_list, modules_root_dir)
    return clt,args

# -----------------------------------------------------------------------------

if __name__ == "__main__":
   clt,args = main()
