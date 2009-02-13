#!/usr/bin/env python
#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
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
## @package runSalome
# \brief Module that provides services to launch SALOME
#
#

import sys, os, string, glob, time, pickle, re
import orbmodule
import setenv
from server import *
from launchConfigureParser import verbose
#process_id = {} move to server.py

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

class InterpServer(Server):
    def __init__(self,args):
        self.args=args
        env_ld_library_path=['env', 'LD_LIBRARY_PATH=' + os.getenv("LD_LIBRARY_PATH")]
        self.CMD=['xterm', '-e']+ env_ld_library_path + ['python']
        #self.CMD=['xterm', '-e', 'python']
       
    def run(self):
        global process_id
        command = self.CMD
        print "INTERPSERVER::command = ", command
        if sys.platform == "win32":
          import win32pm
          pid = win32pm.spawnpid( string.join(command, " "),'-nc' )
        else:
          pid = os.spawnvp(os.P_NOWAIT, command[0], command)
        process_id[pid]=self.CMD
        self.PID = pid

# ---

class CatalogServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        #if sys.platform == "win32":
        #        self.SCMD1=[os.environ["KERNEL_ROOT_DIR"] + "/win32/" + os.environ["BIN_ENV"] + "/" + 'SALOME_ModuleCatalog_Server' + ".exe",'-common']
        #else:
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
                                               "share",setenv.salome_subdir,
                                               "resources",module.lower(),
                                               module_cata)):
                    cata_path.extend(
                        glob.glob(os.path.join(module_root_dir,
                                               "share",setenv.salome_subdir,
                                               "resources",module.lower(),
                                               module_cata)))
                else:
                    cata_path.extend(
                        glob.glob(os.path.join(module_root_dir,
                                               "share",setenv.salome_subdir,
                                               "resources",
                                               module_cata)))
                pass
            pass
        #self.CMD=self.SCMD1 + ['\"']+[string.join(cata_path,'\"::\"')] + ['\"'] + self.SCMD2
        self.CMD=self.SCMD1 + ['\"' + string.join(cata_path,'\"::\"') + '\"'] + self.SCMD2

# ---

class SalomeDSServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.CMD=['SALOMEDS_Server']

# ---

class ConnectionManagerServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.CMD=['SALOME_ConnectionManagerServer']

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
        if sys.platform == "win32":
          self.CMD=[os.environ["PYTHONBIN"], '\"'+os.environ["KERNEL_ROOT_DIR"] + '/bin/salome/SALOME_ContainerPy.py'+'\"','FactoryServerPy']
        else:
          self.CMD=['SALOME_ContainerPy.py','FactoryServerPy']

# ---

class ContainerSUPERVServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
   #  if sys.platform == "win32":
#          self.CMD=[os.environ["KERNEL_ROOT_DIR"] + "/win32/" + os.environ["BIN_ENV"] + "/" + 'SALOME_Container' + ".exe",'SuperVisionContainer']
# else:
        self.CMD=['SALOME_Container','SuperVisionContainer']

# ---

class LoggerServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        from salome_utils import generateFileName
        if sys.platform == "win32": dirpath = os.environ["HOME"]
        else:                       dirpath = "/tmp"
        logfile = generateFileName( dirpath,
                                    prefix="logger",
                                    extension="log",
                                    with_username=True,
                                    with_hostname=True,
                                    with_port=True)
        print "==========================================================="
        print "Logger server: put log to the file:"
        print logfile
        print "==========================================================="
        self.CMD=['SALOME_Logger_Server', logfile]
        pass
    pass # end of LoggerServer class

# ---

class SessionServer(Server):
    def __init__(self,args):
        self.args = args.copy()
        # Bug 11512 (Problems with runSalome --xterm on Mandrake and Debian Sarge)
        #self.args['xterm']=0
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
        if 'supervContainer' in self.args['standalone']:
            self.SCMD2+=['SUPERV']
        if self.args['gui']:
            session_gui = True
            if self.args.has_key('session_gui'):
                session_gui = self.args['session_gui']
            if session_gui:
                self.SCMD2+=['GUI']
                if self.args['splash']:
                    self.SCMD2+=['SPLASH']
                    pass
                if self.args['study_hdf'] is not None:
                    self.SCMD2+=['--study-hdf=%s'%self.args['study_hdf']]
                    pass
                pass
            pass
        if self.args['noexcepthandler']:
            self.SCMD2+=['noexcepthandler']
        if self.args.has_key('user_config'):
            self.SCMD2+=['--resources=%s'%self.args['user_config']]
        if self.args.has_key('modules'):
            self.SCMD2+=['--modules (%s)'%":".join(self.args['modules'])]
        if self.args.has_key('pyscript') and len(self.args['pyscript']) > 0:
            self.SCMD2+=['--pyscript=%s'%(",".join(self.args['pyscript']))]

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
                                           "share",setenv.salome_subdir,
                                           "resources",module.lower(),
                                           module_cata)):
                cata_path.extend(
                    glob.glob(os.path.join(module_root_dir,"share",
                                           setenv.salome_subdir,"resources",
                                           module.lower(),module_cata)))
            else:
                cata_path.extend(
                    glob.glob(os.path.join(module_root_dir,"share",
                                           setenv.salome_subdir,"resources",
                                           module_cata)))
            pass
        if (self.args["gui"]) & ('moduleCatalog' in self.args['embedded']):
            #Use '::' instead ":" because drive path with "D:\" is invalid on windows platform
            #self.CMD=self.SCMD1 + ['\"']+[string.join(cata_path,'\"::\"')] + ['\"'] + self.SCMD2
            self.CMD=self.SCMD1 + ['\"' + string.join(cata_path,'\"::\"') + '\"'] + self.SCMD2
        else:
            self.CMD=self.SCMD1 + self.SCMD2
        if self.args.has_key('test'):
            self.CMD+=['-test'] + self.args['test']
        elif self.args.has_key('play'):
            self.CMD+=['-play'] + self.args['play']

        if self.args["gdb_session"]:
            f = open(".gdbinit4salome", "w")
            f.write("set args ")
            args = " ".join(self.CMD[1:])
            args = args.replace("(", "\(")
            args = args.replace(")", "\)")
            f.write(args)
            f.write("\n")
            f.close()
            self.CMD = ["xterm", "-e", "gdb", "--command=.gdbinit4salome", self.CMD[0]]
            pass
        
# ---

class LauncherServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.SCMD1=['SALOME_LauncherServer']
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
                                               "share",setenv.salome_subdir,
                                               "resources",module.lower(),
                                               module_cata)):
                    cata_path.extend(
                        glob.glob(os.path.join(module_root_dir,"share",
                                               setenv.salome_subdir,"resources",
                                               module.lower(),module_cata)))
                else:
                    cata_path.extend(
                        glob.glob(os.path.join(module_root_dir,"share",
                                               setenv.salome_subdir,"resources",
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
   
    clt=orbmodule.client(args)
    # Save Naming service port name into
    # the file args["ns_port_log_file"]
    if args.has_key('ns_port_log_file'):
      home = os.environ['HOME']
      appli= os.environ.get("APPLI")
      if appli is not None:
        home='%s/%s'%(home,appli)
        pass
      file_name= '%s/%s'%(home, args["ns_port_log_file"])
      f = open(file_name, "w")
      f.write(os.environ['NSPORT'])
      f.close()

    # (non obligatoire) Lancement Logger Server
    # et attente de sa disponibilite dans le naming service
    #

    if args['logger']:
        myServer=LoggerServer(args)
        myServer.run()
        clt.waitLogger("Logger")

    # Notify Server launch
    #

    if sys.platform != "win32":
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
        if sys.platform == "win32":
          clt.waitNS("/Registry")
        else:
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
        if sys.platform == "win32":
          clt.waitNS("/Kernel/ModulCatalog",SALOME_ModuleCatalog.ModuleCatalog)
        else:
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
        if sys.platform == "win32":
          clt.waitNS("/myStudyManager")
        else:
          clt.waitNSPID("/myStudyManager",myServer.PID)

    #
    # Lancement LauncherServer
    #
    
    myCmServer = LauncherServer(args)
    myCmServer.setpath(modules_list,modules_root_dir)
    myCmServer.run()

    #
    # Launch ConnectionManagerServer
    #

    myConnectionServer = ConnectionManagerServer(args)
    myConnectionServer.run()


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

    if ('cppContainer' in args['standalone']) | (args["gui"] == 0) : 
        myServer=ContainerCPPServer(args)
        myServer.run()
        if sys.platform == "win32":
          clt.waitNS("/Containers/" + theComputer + "/FactoryServer")
        else:
          clt.waitNSPID("/Containers/" + theComputer + "/FactoryServer",myServer.PID)

    #
    # Lancement Container Python local,
    # attente de la disponibilite du Container Python local
    # dans le Naming Service
    #

    if 'pyContainer' in args['standalone']:
        myServer=ContainerPYServer(args)
        myServer.run()
        if sys.platform == "win32":
          clt.waitNS("/Containers/" + theComputer + "/FactoryServerPy")
        else:
          clt.waitNSPID("/Containers/" + theComputer + "/FactoryServerPy",myServer.PID)

    #
    # Lancement Container Supervision local,
    # attente de la disponibilite du Container Supervision local
    # dans le Naming Service
    #

    if 'supervContainer' in args['standalone']:
        myServer=ContainerSUPERVServer(args)
        myServer.run()
        if sys.platform == "win32":
          clt.waitNS("/Containers/" + theComputer + "/SuperVisionContainer")
        else:
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
        if sys.platform == "win32":
          session=clt.waitNS("/Kernel/Session",SALOME.Session)
        else:
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
            nbaddi = args['interp']
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

    # set PYTHONINSPECT variable
    if args['pinter']:
        os.environ["PYTHONINSPECT"]="1"
        import readline
        
    return clt

# -----------------------------------------------------------------------------

def useSalome(args, modules_list, modules_root_dir):
    """
    Launch all SALOME servers requested by args,
    save list of process, give info to user,
    show registered objects in Naming Service.
    """
    global process_id
    
    clt=None
    try:
        clt = startSalome(args, modules_list, modules_root_dir)
    except:
        import traceback
        traceback.print_exc()
        print
        print
        print "--- Error during Salome launch ---"
        
    #print process_id

    from addToKillList import addToKillList
    from killSalomeWithPort import getPiDict

    filedict = getPiDict(args['port'])
    for pid, cmd in process_id.items():
        addToKillList(pid, cmd, args['port'])
        pass

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
        
        # run python scripts, passed via --execute option
        toimport = []
        if args.has_key('pyscript'):
            if args.has_key('gui') and args.has_key('session_gui'):
                if not args['gui'] or not args['session_gui']:
                    toimport = args['pyscript']
        i = 0
        while i < len( toimport ) :
            if toimport[ i ] == 'killall':
                clt.showNS()
                killAllPorts()
                import sys
                sys.exit(0)
            else:
                scrname = toimport[ i ]
                if len(scrname) > 2 and (len(scrname) - string.rfind(scrname, ".py") == 3):
                    print 'executing',scrname
                    doexec = 'execfile(\"%s\")'%scrname
                    exec doexec
                else:
                    print 'importing',scrname
                    doimport = 'import ' + scrname
                    exec doimport
            i = i + 1

    return clt

# -----------------------------------------------------------------------------

def registerEnv(args, modules_list, modules_root_dir):
    """
    Register args, modules_list, modules_root_dir in a file
    for further use, when SALOME is launched embedded in an other application.
    """
    if sys.platform == "win32":
      fileEnv = os.getenv('TEMP')
    else:
      fileEnv = '/tmp/'

    fileEnv += os.getenv('USER') + "_" + str(args['port']) \
            + '_' + args['appname'].upper() + '_env'
    fenv=open(fileEnv,'w')
    pickle.dump((args, modules_list, modules_root_dir),fenv)
    fenv.close()
    os.environ["SALOME_LAUNCH_CONFIG"] = fileEnv

# -----------------------------------------------------------------------------

def searchFreePort(args, save_config=1):
    print "Searching for a free port for naming service:",
    #
    if sys.platform == "win32":
        tmp_file = os.getenv('TEMP');
    else:
        tmp_file = '/tmp'
    tmp_file = os.path.join(tmp_file, '.netstat_%s'%os.getpid())
    #
    ###status = os.system("netstat -ltn | grep -E :%s > /dev/null 2>&1"%(NSPORT))
    os.system( "netstat -a -n > %s" % tmp_file );
    f = open( tmp_file, 'r' );
    ports = f.readlines();
    f.close();
    os.remove( tmp_file );
    #
    def portIsUsed(port, data):
        regObj = re.compile( "tcp.*:([0-9]+).*:.*listen", re.IGNORECASE );
        for item in data:
            try:
                p = int(regObj.match(item).group(1))
                if p == port: return True
                pass
            except:
                pass
            pass
        return False
    #
    NSPORT=2810
    limit=NSPORT+100
    #
    while 1:
        if not portIsUsed(NSPORT, ports):
            print "%s - OK"%(NSPORT)
            #
            from salome_utils import generateFileName, getHostName
            hostname = getHostName()
            #
            home  = os.getenv("HOME")
            appli = os.getenv("APPLI")
            kwargs={}
            if appli is not None: 
              home = os.path.join(home, appli,"USERS")
              kwargs["with_username"]=True
            #
            omniorb_config = generateFileName(home, prefix="omniORB",
                                              extension="cfg",
                                              hidden=True,
                                              with_hostname=True,
                                              with_port=NSPORT,
                                              **kwargs)
            orbdata = []
            initref = "NameService=corbaname::%s:%s"%(hostname, NSPORT)
            from omniORB import CORBA
            if CORBA.ORB_ID == "omniORB4":
                orbdata.append("InitRef = %s"%(initref))
                orbdata.append("giopMaxMsgSize = 2097152000  # 2 GBytes")
                orbdata.append("traceLevel = 0 # critical errors only")
            else:
                orbdata.append("ORBInitRef %s"%(initref))
                orbdata.append("ORBgiopMaxMsgSize = 2097152000  # 2 GBytes")
                orbdata.append("ORBtraceLevel = 0 # critical errors only")
                pass
            orbdata.append("")
            f = open(omniorb_config, "w")
            f.write("\n".join(orbdata))
            f.close()
            #
            os.environ['OMNIORB_CONFIG'] = omniorb_config
            os.environ['NSPORT'] = "%s"%(NSPORT)
            os.environ['NSHOST'] = "%s"%(hostname)
            args['port'] = os.environ['NSPORT']
            #
            if save_config:
                last_running_config = generateFileName(home, prefix="omniORB",
                                                       suffix="last",
                                                       extension="cfg",
                                                       hidden=True,
                                                       **kwargs)
                try:
                    if sys.platform == "win32":
                        import shutil       
                        shutil.copyfile(omniorb_config, last_running_config)
                    else:
                        try:
                            os.remove(last_running_config)
                        except OSError:
                            pass
                        os.symlink(omniorb_config, last_running_config)
                        pass
                    pass
                except:
                    pass
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
    from salome_utils import getHostName
    print "runSalome running on %s" % getHostName()
    args, modules_list, modules_root_dir = setenv.get_config()
    kill_salome(args)
    save_config = True
    if args.has_key('save_config'):
        save_config = args['save_config']
    searchFreePort(args, save_config)
    #setenv.main()
    setenv.set_env(args, modules_list, modules_root_dir)
    clt = useSalome(args, modules_list, modules_root_dir)
    return clt,args

# -----------------------------------------------------------------------------

if __name__ == "__main__":
   import user
   clt,args = main()
