#!/usr/bin/env python

import sys, os, string, glob, time, pickle

### read launch configure xml file and command line options
import launchConfigureParser
args = launchConfigureParser.args

### kill servers if it is need

from killSalome import killAllPorts

def killLocalPort():
    from killSalomeWithPort import killMyPort
    my_port=str(args['port'])
    try:
        killMyPort(my_port)
    except:
        print "problem in killLocalPort()"
        pass
    pass
    
if args['killall']:
    killAllPorts()
elif args['portkill']:
    killLocalPort()
	
# -----------------------------------------------------------------------------
#
# Fonctions helper pour ajouter des variables d'environnement
#

def add_path(directory, variable_name):
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


init_time = os.times()
# -----------------------------------------------------------------------------
#
# Check variables <module>_ROOT_DIR and set list of used modules (without KERNEL)
# Add to the PATH-variables modules' specific paths
#
modules_list = []
if args.has_key("modules"):
    modules_list += args["modules"]
modules_list[:0] = ["KERNEL"] # KERNEL must be last in the list to locate it at the first place in PATH variables
modules_list.reverse()

modules_root_dir = {}
modules_root_dir_list = []
python_version="python%d.%d" % sys.version_info[0:2]

to_remove_list=[]
for module in modules_list :
    module_variable=module.upper()+"_ROOT_DIR"
    if not os.environ.has_key(module_variable):
        print "*******************************************************************************"
        print "*"
        print "* Environment variable",module_variable,"must be set"
        print "* Module", module, "will be not available"
        print "*"
        print "*******************************************************************************"
        to_remove_list.append(module)
        continue
        pass
    module_root_dir = os.environ[module_variable]
    modules_root_dir[module]=module_root_dir
    modules_root_dir_list[:0] = [module_root_dir]
    add_path(os.path.join(module_root_dir,"lib",args['appname']), "LD_LIBRARY_PATH")
    add_path(os.path.join(module_root_dir,"bin",args['appname']), "PATH")
    if os.path.exists(module_root_dir + "/examples") :
        add_path(os.path.join(module_root_dir,"examples"), "PYTHONPATH")

for to_remove in to_remove_list:
    modules_list.remove(to_remove)

while "KERNEL" in modules_list:
    modules_list.remove("KERNEL")
    pass

# KERNEL must be last in the list to locate it at the first place in PYTHONPATH variable
list_modules = modules_list[:] + ["KERNEL"] 
for module in list_modules:
    module_root_dir = modules_root_dir[module]
    add_path(os.path.join(module_root_dir,"bin",args['appname']), "PYTHONPATH")
    add_path(os.path.join(module_root_dir,"lib",python_version,"site-packages",args['appname']), "PYTHONPATH")
    add_path(os.path.join(module_root_dir,"lib",args['appname']), "PYTHONPATH")
    add_path(os.path.join(module_root_dir,"lib",python_version,"site-packages",args['appname'],"shared_modules"), "PYTHONPATH")
      
#os.environ["SALOMEPATH"]=":".join(modules_root_dir.values())
os.environ["SALOMEPATH"]=":".join(modules_root_dir_list)
if "SUPERV" in modules_list and not 'superv' in args['standalone']:
    args['standalone'].append("superv")
    pass


# -----------------------------------------------------------------------------
#
# Définition des classes d'objets pour le lancement des Server CORBA
#

class Server:
   CMD=[]
   if args['xterm']:
	ARGS=['xterm', '-iconic', '-sb', '-sl', '500', '-hold']
   else:
   	ARGS=[]	

   def run(self):
       global process_id
       myargs=self.ARGS
       if args['xterm']:
           # (Debian) Transfert variable LD_LIBRARY_PATH aux shells fils (xterm)
           env_ld_library_path=['env', 'LD_LIBRARY_PATH='+ os.getenv("LD_LIBRARY_PATH")]
           myargs = myargs +['-T']+self.CMD[:1]+['-e'] + env_ld_library_path
       command = myargs + self.CMD
       #print "command = ", command
       pid = os.spawnvp(os.P_NOWAIT, command[0], command)
       process_id[pid]=self.CMD

class CatalogServer(Server):
   SCMD1=['SALOME_ModuleCatalog_Server','-common']
   SCMD2=['-personal','${HOME}/Salome/resources/CatalogModulePersonnel.xml'] 

   def setpath(self,modules_list):
      cata_path=[]
      list_modules = modules_list[:]
      list_modules.reverse()
      for module in ["KERNEL"] + list_modules:
          module_root_dir=modules_root_dir[module]
          module_cata=module+"Catalog.xml"
          print "   ", module_cata
          cata_path.extend(glob.glob(os.path.join(module_root_dir,"share",args['appname'],"resources",module_cata)))
      self.CMD=self.SCMD1 + [string.join(cata_path,':')] + self.SCMD2

class SalomeDSServer(Server):
   CMD=['SALOMEDS_Server']

class RegistryServer(Server):
   CMD=['SALOME_Registry_Server', '--salome_session','theSession']

class ContainerCPPServer(Server):
   #CMD=['SALOME_Container','FactoryServer','-ORBInitRef','NameService=corbaname::localhost']
   CMD=['SALOME_Container','FactoryServer']

class ContainerPYServer(Server):
   #CMD=['SALOME_ContainerPy.py','FactoryServerPy','-ORBInitRef','NameService=corbaname::localhost']
   CMD=['SALOME_ContainerPy.py','FactoryServerPy']

class ContainerSUPERVServer(Server):
   #CMD=['SALOME_Container','SuperVisionContainer','-ORBInitRef','NameService=corbaname::localhost']
   CMD=['SALOME_Container','SuperVisionContainer']

class LoggerServer(Server):
   CMD=['SALOME_Logger_Server', 'logger.log']

class SessionLoader(Server):
   CMD=['SALOME_Session_Loader']
   if "cpp" in args['containers']:
       CMD=CMD+['CPP']
   if "python" in args['containers']:
       CMD=CMD+['PY']
   if "superv" in args['containers']:
       CMD=CMD+['SUPERV']
   if args['gui']:
       CMD=CMD+['GUI']

class SessionServer(Server):
   SCMD1=['SALOME_Session_Server']
   SCMD2=[]
   if 'registry' in args['embedded']:
       SCMD1+=['--with','Registry','(','--salome_session','theSession',')']
   if 'moduleCatalog' in args['embedded']:
       SCMD1+=['--with','ModuleCatalog','(','-common']
       SCMD2+=['-personal','${HOME}/Salome/resources/CatalogModulePersonnel.xml',')']
   if 'study' in args['embedded']:
       SCMD2+=['--with','SALOMEDS','(',')']
   if 'cppContainer' in args['embedded']:
       SCMD2+=['--with','Container','(','FactoryServer',')']

   def setpath(self,modules_list):
      cata_path=[]
      list_modules = modules_list[:]
      list_modules.reverse()
      for module in ["KERNEL"] + list_modules:
          module_root_dir=modules_root_dir[module]
          module_cata=module+"Catalog.xml"
          print "   ", module_cata
          cata_path.extend(glob.glob(os.path.join(module_root_dir,"share",args['appname'],"resources",module_cata)))
      if 'moduleCatalog' in args['embedded']:
          self.CMD=self.SCMD1 + [string.join(cata_path,':')] + self.SCMD2
      else:
          self.CMD=self.SCMD1 + self.SCMD2

class NotifyServer(Server):
    myLogName = os.environ["LOGNAME"]
    CMD=['notifd','-c',modules_root_dir["KERNEL"]+'/share/salome/resources/channel.cfg', '-DFactoryIORFileName=/tmp/'+myLogName+'_rdifact.ior', '-DChannelIORFileName=/tmp/'+myLogName+'_rdichan.ior']

# -----------------------------------------------------------------------------
#
# initialisation des variables d'environnement
#

os.environ["SALOME_trace"]="local"
if args['logger']:
   os.environ["SALOME_trace"]="with_logger"
   locdir=os.environ['PWD']
   libtracedir=os.path.join(locdir,"libSalomeTrace")
   libtrace = os.path.join(modules_root_dir["KERNEL"],"lib",args['appname'],"libSALOMELoggerClient.so.0.0.0")
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
        os.environ["SALOME_StdMeshersResources"] = modules_root_dir["SMESH"]+"/share/"+args["appname"]+"/resources"
        pass
    if args.has_key("SMESH_plugins"):
        for plugin in args["SMESH_plugins"]:
            if os.environ.has_key(plugin.upper()+"_ROOT_DIR"):
                os.environ["SMESH_MeshersList"]=os.environ["SMESH_MeshersList"]+":"+plugin
                plugin_root = os.environ[plugin.upper()+"_ROOT_DIR"]
                if not os.environ.has_key("SALOME_"+plugin+"Resources"):
                    os.environ["SALOME_"+plugin+"Resources"] = plugin_root+"/share/"+args["appname"]+"/resources"
                add_path(os.path.join(plugin_root,"lib",python_version,"site-packages",args['appname']), "PYTHONPATH")
                add_path(os.path.join(plugin_root,"lib",args['appname']), "PYTHONPATH")
                add_path(os.path.join(plugin_root,"lib",args['appname']), "LD_LIBRARY_PATH")
                add_path(os.path.join(plugin_root,"bin",args['appname']), "PYTHONPATH")
                add_path(os.path.join(plugin_root,"bin",args['appname']), "PATH")
        pass
    pass

# set environment for SUPERV module
os.environ["ENABLE_MACRO_NODE"]="1"

import orbmodule

#
# -----------------------------------------------------------------------------
#

def startGUI():
  import SALOME
  session=clt.waitNS("/Kernel/Session",SALOME.Session)

  #
  # Activation du GUI de Session Server
  #
	
  session.GetInterface()
  
#
# -----------------------------------------------------------------------------
#

def startSalome():

  #
  # Lancement Session Loader
  #

  if args['gui']:
     SessionLoader().run()

  #
  # Initialisation ORB et Naming Service
  #
   
  clt=orbmodule.client()

  # (non obligatoire) Lancement Logger Server et attente de sa disponibilite dans le naming service
  #

  if args['logger']:
	LoggerServer().run()
	clt.waitLogger("Logger")

  # Notify Server launch
  #
  NotifyServer().run()

  #
  # Lancement Registry Server, attente de la disponibilité du Registry dans le Naming Service
  #
  if 'registry' not in args['embedded']:
      RegistryServer().run()
      clt.waitNS("/Registry")

  #
  # Lancement Catalog Server, attente de la disponibilité du Catalog Server dans le Naming Service
  #

  if 'moduleCatalog' not in args['embedded']:
      cataServer=CatalogServer()
      cataServer.setpath(modules_list)
      cataServer.run()
      import SALOME_ModuleCatalog
      clt.waitNS("/Kernel/ModulCatalog",SALOME_ModuleCatalog.ModuleCatalog)

  #
  # Lancement SalomeDS Server, attente de la disponibilité du SalomeDS dans le Naming Service
  #

  os.environ["CSF_PluginDefaults"]=os.path.join(modules_root_dir["KERNEL"],"share",args['appname'],"resources")
  os.environ["CSF_SALOMEDS_ResourcesDefaults"]=os.path.join(modules_root_dir["KERNEL"],"share",args['appname'],"resources")

  if "GEOM" in modules_list:
	print "GEOM OCAF Resources"
	os.environ["CSF_GEOMDS_ResourcesDefaults"]=os.path.join(modules_root_dir["GEOM"],"share",args['appname'],"resources")
	print "GEOM Shape Healing Resources"
        os.environ["CSF_ShHealingDefaults"]=os.path.join(modules_root_dir["GEOM"],"share",args['appname'],"resources")

  if 'study' not in args['embedded']:
      SalomeDSServer().run()
      clt.waitNS("/myStudyManager")

  #
  # Lancement Session Server
  #

  mySessionServ=SessionServer()
  mySessionServ.setpath(modules_list)
  mySessionServ.run()

  #
  # Attente de la disponibilité du Session Server dans le Naming Service
  #

  #import SALOME
  #session=clt.waitNS("/Kernel/Session",SALOME.Session)

  if os.getenv("HOSTNAME") == None:
     if os.getenv("HOST") == None:
        os.environ["HOSTNAME"]="localhost"
     else:
        os.environ["HOSTNAME"]=os.getenv("HOST")

  theComputer = os.getenv("HOSTNAME")
  computerSplitName = theComputer.split('.')
  theComputer = computerSplitName[0]
  
  #
  # Lancement Container C++ local, attente de la disponibilité du Container C++ local dans le Naming Service
  #

  if 'cppContainer' in args['standalone']:
	  ContainerCPPServer().run()
	  clt.waitNS("/Containers/" + theComputer + "/FactoryServer")

  #
  # Lancement Container Python local, attente de la disponibilité du Container Python local dans le Naming Service
  #

  if 'pyContainer' in args['standalone']:
	  ContainerPYServer().run()
	  clt.waitNS("/Containers/" + theComputer + "/FactoryServerPy")

  #
  # Lancement Container Supervision local, attente de la disponibilité du Container Supervision local dans le Naming Service
  #

  if 'supervContainer' in args['standalone']:
      ContainerSUPERVServer().run()
      clt.waitNS("/Containers/" + theComputer + "/SuperVisionContainer")

  end_time = os.times()
  print
  print "Start SALOME, elapsed time : %5.1f seconds"% (end_time[4] - init_time[4])

  return clt

#
# -----------------------------------------------------------------------------
#

process_id = {}
if __name__ == "__main__":
    clt=None
    try:
        clt = startSalome()
    except:
        print
        print
        print "--- erreur au lancement Salome ---"
        
    #print process_id
    
    filedict='/tmp/'+os.getenv('USER')+"_"+str(args['port'])+'_'+args['appname'].upper()+'_pidict'
    #filedict='/tmp/'+os.getlogin()+'_SALOME_pidict'
    
    
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
      killLocalPort()  --> kill this session
      killAllPorts()   --> kill all sessions
    
    runSalome, with --killall option, starts with killing the processes resulting from the previous execution.
    """%filedict
    
    #
    #  Impression arborescence Naming Service
    #
    
    if clt != None:
        print
        print " --- registered objects tree in Naming Service ---"
        clt.showNS()
