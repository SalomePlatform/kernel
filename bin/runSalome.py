#!/usr/bin/env python

usage="""USAGE: runSalome.py [options]

[command line options] :
--help                        : affichage de l'aide
--gui                         : lancement du GUI
--logger		      : redirection des messages dans un fichier
--xterm			      : les serveurs ouvrent une fenêtre xterm et les messages sont affichés dans cette fenêtre
--modules=module1,module2,... : où modulen est le nom d'un module Salome à charger dans le catalogue
--containers=cpp,python,superv: lancement des containers cpp, python et de supervision
--killall	              : arrêt des serveurs de salome

 La variable d'environnement <modulen>_ROOT_DIR doit etre préalablement
 positionnée (modulen doit etre en majuscule).
 KERNEL_ROOT_DIR est obligatoire.
"""

# -----------------------------------------------------------------------------
#
# Fonction d'arrêt de salome
#

def killSalome():
   print "arret des serveurs SALOME"
   for pid, cmd in process_id.items():
      print "arret du process %s : %s"% (pid, cmd[0])
      try:
	os.kill(pid,signal.SIGKILL)
      except:
         print "  ------------------ process %s : %s inexistant"% (pid, cmd[0])
   print "arret du naming service"
   os.system("killall -9 omniNames")
   
# -----------------------------------------------------------------------------
#
# Fonction message
#

def message(code, msg=''):
    if msg: print msg
    sys.exit(code)

import sys,os,string,glob,time,signal,pickle,getopt

init_time=os.times()
opts, args=getopt.getopt(sys.argv[1:], 'hmglxck:', ['help','modules=','gui','logger','xterm','containers=','killall'])
modules_root_dir={}
process_id={}
liste_modules={}
liste_containers={}
with_gui=0
with_logger=0
with_xterm=0

with_container_cpp=0
with_container_python=0
with_container_superv=0

try:
  for o, a in opts:
    if o in ('-h', '--help'):
      print usage
      sys.exit(1)
    elif o in ('-g', '--gui'):
      with_gui=1
    elif o in ('-l', '--logger'):
      with_logger=1
    elif o in ('-x', '--xterm'):
      with_xterm=1
    elif o in ('-m', '--modules'):
      liste_modules = [x.upper() for x in a.split(',')]
    elif o in ('-c', '--containers'):
      liste_containers = [x.lower() for x in a.split(',')]
      for r in liste_containers:
        if r not in ('cpp', 'python', 'superv'):
	   message(1, 'Invalid -c/--containers option: %s' % a)
      if 'cpp' in liste_containers:
	   with_container_cpp=1
      else:
	   with_container_cpp=0
      if 'python' in liste_containers:
	   with_container_python=1
      else:
	   with_container_python=0
      if 'superv' in liste_containers:
	   with_container_superv=1
      else:
	   with_container_superv=0
    elif o in ('-k', '--killall'):
      filedict='/tmp/'+os.getenv('USER')+'_SALOME_pidict'
      #filedict='/tmp/'+os.getlogin()+'_SALOME_pidict'
      found = 0
      try:
         fpid=open(filedict, 'r')
	 found = 1
      except:
         print "le fichier %s des process SALOME n'est pas accessible"% filedict

      if found:
         process_id=pickle.load(fpid)
         fpid.close()
         killSalome()
	 process_id={}
         os.remove(filedict)
	
except getopt.error, msg:
  print usage
  sys.exit(1)

# -----------------------------------------------------------------------------
#
# Vérification des variables d'environnement
#
try:
  kernel_root_dir=os.environ["KERNEL_ROOT_DIR"]
  modules_root_dir["KERNEL"]=kernel_root_dir
except:
  print usage
  sys.exit(1)

for module in liste_modules :
   try:
      module=module.upper()
      module_root_dir=os.environ[module +"_ROOT_DIR"]
      modules_root_dir[module]=module_root_dir
   except:
      print usage
      sys.exit(1)

# il faut KERNEL en premier dans la liste des modules
# - l'ordre des modules dans le catalogue sera identique
# - la liste des modules presents dans le catalogue est exploitée pour charger les modules CORBA python,
#   il faut charger les modules python du KERNEL en premier

if "KERNEL" in liste_modules:liste_modules.remove("KERNEL")
liste_modules[:0]=["KERNEL"]
#print liste_modules
#print modules_root_dir

os.environ["SALOMEPATH"]=":".join(modules_root_dir.values())
if "SUPERV" in liste_modules:with_container_superv=1


# -----------------------------------------------------------------------------
#
# Définition des classes d'objets pour le lancement des Server CORBA
#

class Server:
   CMD=[]
   if with_xterm:
	ARGS=['xterm', '-iconic', '-sb', '-sl', '500', '-e']
   else:
   	ARGS=[]	

   def run(self):
      args = self.ARGS+self.CMD
      #print "args = ", args
      pid = os.spawnvp(os.P_NOWAIT, args[0], args)
      process_id[pid]=self.CMD

class CatalogServer(Server):
   SCMD1=['SALOME_ModuleCatalog_Server','-common']
   SCMD2=['-personal','${HOME}/Salome/resources/CatalogModulePersonnel.xml'] 

   def setpath(self,liste_modules):
      cata_path=[]
      for module in liste_modules:
          module_root_dir=modules_root_dir[module]
          module_cata=module+"Catalog.xml"
          print "   ", module_cata
          cata_path.extend(glob.glob(os.path.join(module_root_dir,"share","salome","resources",module_cata)))
      self.CMD=self.SCMD1 + [string.join(cata_path,':')] + self.SCMD2

class SalomeDSServer(Server):
   CMD=['SALOMEDS_Server']

class RegistryServer(Server):
   CMD=['SALOME_Registry_Server', '--salome_session','theSession']

class ContainerCPPServer(Server):
   CMD=['SALOME_Container','FactoryServer','-ORBInitRef','NameService=corbaname::localhost']

class ContainerPYServer(Server):
   CMD=['SALOME_ContainerPy.py','FactoryServerPy','-ORBInitRef','NameService=corbaname::localhost']

class ContainerSUPERVServer(Server):
   CMD=['SALOME_Container','SuperVisionContainer','-ORBInitRef','NameService=corbaname::localhost']

class LoggerServer(Server):
   CMD=['SALOME_Logger_Server', 'logger.log']

class SessionLoader(Server):
   CMD=['SALOME_Session_Loader']
   if with_container_cpp:
       CMD=CMD+['CPP']
   if with_container_python:
       CMD=CMD+['PY']
   if with_container_superv:
       CMD=CMD+['SUPERV']
   if with_gui:
       CMD=CMD+['GUI']

class SessionServer(Server):
   CMD=['SALOME_Session_Server']

class NotifyServer(Server):
   CMD=['notifd','-c','${KERNEL_ROOT_DIR}/share/salome/resources/channel.cfg -DFactoryIORFileName=/tmp/${LOGNAME}_rdifact.ior -DChannelIORFileName=/tmp/${LOGNAME}_rdichan.ior']

# -----------------------------------------------------------------------------
#
# Fonction de test
#

def test(clt):
   # create an LifeCycleCORBA instance
   import LifeCycleCORBA 
   lcc = LifeCycleCORBA.LifeCycleCORBA(clt.orb)
   med = lcc.FindOrLoadComponent("FactoryServer", "MED")
   #pycalc = lcc.FindOrLoadComponent("FactoryServerPy", "CalculatorPy")

# -----------------------------------------------------------------------------
#
# Fonctions helper pour ajouter des variables d'environnement
#

def add_path(directory):
   os.environ["PATH"]=directory + ":" + os.environ["PATH"]

def add_ld_library_path(directory):
   os.environ["LD_LIBRARY_PATH"]=directory + ":" + os.environ["LD_LIBRARY_PATH"]

def add_python_path(directory):
   os.environ["PYTHONPATH"]=directory + ":" + os.environ["PYTHONPATH"]
   sys.path[:0]=[directory]

# -----------------------------------------------------------------------------
#
# initialisation des variables d'environnement
#

python_version="python%d.%d" % sys.version_info[0:2]

#
# Ajout du chemin d'acces aux executables de KERNEL dans le PATH
#

add_path(os.path.join(kernel_root_dir,"bin","salome"))
#print "PATH=",os.environ["PATH"]

#
# Ajout des modules dans le LD_LIBRARY_PATH
#
for module in liste_modules:
    module_root_dir=modules_root_dir[module]
    add_ld_library_path(os.path.join(module_root_dir,"lib","salome"))

if with_logger:
   locdir=os.environ['PWD']
   libtracedir=os.path.join(locdir,"libSalomeTrace")
   libtrace = os.path.join(kernel_root_dir,"lib","salome","libSALOMELoggerClient.so.0.0.0")
   libtraceln = os.path.join(libtracedir,"libSALOMELocalTrace.so")
   aCommand = 'rm -rf ' + libtracedir + "; "
   aCommand += 'mkdir ' + libtracedir + "; "
   aCommand += 'ln -s ' + libtrace + " " + libtraceln + "; "
   aCommand += 'ln -s ' + libtrace + " " + libtraceln + ".0; "
   aCommand += 'ln -s ' + libtrace + " " + libtraceln + ".0.0.0; "
   os.system(aCommand)
   add_ld_library_path(libtracedir)
   
#print "LD_LIBRARY_PATH=",os.environ["LD_LIBRARY_PATH"]

#
# Ajout des modules dans le PYTHONPATH (KERNEL prioritaire, donc en dernier)
#

liste_modules_reverse=liste_modules[:]
liste_modules_reverse.reverse()
#print liste_modules
#print liste_modules_reverse
for module in liste_modules_reverse:
    module_root_dir=modules_root_dir[module]
    add_python_path(os.path.join(module_root_dir,"bin","salome"))
    add_python_path(os.path.join(module_root_dir,"lib",python_version,"site-packages","salome"))
    add_python_path(os.path.join(module_root_dir,"lib","salome"))
    add_python_path(os.path.join(module_root_dir,"lib",python_version,"site-packages","salome","shared_modules"))

#print "PYTHONPATH=",sys.path

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

  if with_gui:
     SessionLoader().run()

  #
  # Initialisation ORB et Naming Service
  #
   
  clt=orbmodule.client()

  # (non obligatoire) Lancement Logger Server et attente de sa disponibilite dans le naming service
  #

  if with_logger:
	LoggerServer().run()
	clt.waitLogger("Logger")


  #
  # Lancement Registry Server
  #

  RegistryServer().run()

  #
  # Attente de la disponibilité du Registry dans le Naming Service
  #

  clt.waitNS("/Registry")

  #
  # Lancement Catalog Server
  #

  cataServer=CatalogServer()
  cataServer.setpath(liste_modules)
  cataServer.run()

  #
  # Attente de la disponibilité du Catalog Server dans le Naming Service
  #

  import SALOME_ModuleCatalog
  clt.waitNS("/Kernel/ModulCatalog",SALOME_ModuleCatalog.ModuleCatalog)

  #
  # Lancement SalomeDS Server
  #

  os.environ["CSF_PluginDefaults"]=os.path.join(kernel_root_dir,"share","salome","resources")
  os.environ["CSF_SALOMEDS_ResourcesDefaults"]=os.path.join(kernel_root_dir,"share","salome","resources")
  SalomeDSServer().run()

  if "GEOM" in liste_modules:
	print "GEOM OCAF Resources"
	os.environ["CSF_GEOMDS_ResourcesDefaults"]=os.path.join(modules_root_dir["GEOM"],"share","salome","resources")


  #
  # Attente de la disponibilité du SalomeDS dans le Naming Service
  #

  clt.waitNS("/myStudyManager")

  #
  # Lancement Session Server
  #

  SessionServer().run()

  #
  # Attente de la disponibilité du Session Server dans le Naming Service
  #

  import SALOME
  session=clt.waitNS("/Kernel/Session",SALOME.Session)


  theComputer = os.getenv("HOSTNAME")
  computerSplitName = theComputer.split('.')
  theComputer = computerSplitName[0]
  
  #
  # Lancement Container C++ local
  #
  if with_container_cpp:
	  ContainerCPPServer().run()

	  #
	  # Attente de la disponibilité du Container C++ local dans le Naming Service
	  #

	  clt.waitNS("/Containers/" + theComputer + "/FactoryServer")

  #
  # Lancement Container Python local
  #

  if with_container_python:
	  ContainerPYServer().run()

	  #
	  # Attente de la disponibilité du Container Python local dans le Naming Service
	  #
	
	  clt.waitNS("/Containers/" + theComputer + "/FactoryServerPy")

  if with_container_superv:

	#
	# Lancement Container Supervision local
	#

	ContainerSUPERVServer().run()

	#
	# Attente de la disponibilité du Container Supervision local dans le Naming Service
	#

	clt.waitNS("/Containers/" + theComputer + "/SuperVisionContainer")


  #
  # Activation du GUI de Session Server
  #
	
  #session.GetInterface()

  end_time = os.times()
  print
  print "Start SALOME, elpased time : %5.1f seconds"% (end_time[4] - init_time[4])

  return clt

#
# -----------------------------------------------------------------------------
#

if __name__ == "__main__":
   clt=None
   try:
      clt = startSalome()
   except:
      print
      print
      print "--- erreur au lancement Salome ---"
   
   #print process_id
   
   
   filedict='/tmp/'+os.getenv('USER')+'_SALOME_pidict'
   #filedict='/tmp/'+os.getlogin()+'_SALOME_pidict'
   
   fpid=open(filedict, 'w')
   pickle.dump(process_id,fpid)
   fpid.close()
   
   print
   print "Sauvegarde du dictionnaire des process dans ", filedict
   print "Pour tuer les process SALOME, executer : python killSalome.py depuis"
   print "une console, ou bien killSalome() depuis le present interpreteur,"
   print "s'il n'est pas fermé."
   print
   print "runSalome, avec l'option --killall, commence par tuer les process restants d'une execution précédente."
   print
   print "Pour lancer uniquement le GUI, executer startGUI() depuis le present interpreteur,"
   print "s'il n'est pas fermé."
   
   #
   #  Impression arborescence Naming Service
   #
   
   if clt != None:
     print
     print " --- registered objects tree in Naming Service ---"
     clt.showNS()
   
