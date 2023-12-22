#!/usr/bin/env python3
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2023  CEA, EDF, OPEN CASCADE
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

## @package runSalome
# \brief Module that provides services to launch SALOME
#

import sys, os, string, glob, time, pickle, re
import setenv
from server import process_id, Server
import json
import subprocess
from salomeContextUtils import ScriptAndArgsObjectEncoder
import platform
import logging
from salome_utils import positionVerbosityOfLogger

logger = logging.getLogger()

def setVerbose(verbose):
    verbose_map = { "0": logging.WARNING, "1": logging.INFO, "2": logging.DEBUG}
    positionVerbosityOfLogger( verbose_map[verbose] )
# -----------------------------------------------------------------------------
#
# Class definitions to launch CORBA Servers
#

class InterpServer(Server):
    def __init__(self,args):
        self.args=args
        if sys.platform == "win32":
          self.CMD=['cmd', '/c', 'start cmd.exe', '/K', 'python']
        elif sys.platform == "darwin":
          env_ld_library_path=['env', 'DYLD_LIBRARY_PATH=' + os.getenv("LD_LIBRARY_PATH")]
          self.CMD=['xterm', '-e'] + env_ld_library_path + ['python']
        else:
          env_ld_library_path=['env', 'LD_LIBRARY_PATH=' + os.getenv("LD_LIBRARY_PATH")]
          self.CMD=['xterm', '-e'] + env_ld_library_path + ['python']

    def run(self):
        global process_id
        command = self.CMD
        print("INTERPSERVER::command = ", command)
        import subprocess
        pid = subprocess.Popen(command).pid
        process_id[pid]=self.CMD
        self.PID = pid

# ---

def get_cata_path(list_modules,modules_root_dir):
    """Build a list of catalog paths (cata_path) to initialize the ModuleCatalog server
    """
    modules_cata={}
    cata_path=[]

    for module in list_modules:
        if module in modules_root_dir:
            module_root_dir=modules_root_dir[module]
            module_cata=module+"Catalog.xml"
            cata_file=os.path.join(module_root_dir, "share",setenv.salome_subdir, "resources",module.lower(), module_cata)

            if os.path.exists(cata_file):
                cata_path.append(cata_file)
                modules_cata[module]=cata_file
            else:
                cata_file=os.path.join(module_root_dir, "share",setenv.salome_subdir, "resources", module_cata)
                if os.path.exists(cata_file):
                    cata_path.append(cata_file)
                    modules_cata[module]=cata_file

    for path in os.getenv("SALOME_CATALOGS_PATH","").split(os.pathsep):
        if os.path.exists(path):
            for cata_file in glob.glob(os.path.join(path,"*Catalog.xml")):
                module_name= os.path.basename(cata_file)[:-11]
                if module_name not in modules_cata:
                    cata_path.append(cata_file)
                    modules_cata[module_name]=cata_file

    return cata_path

class CatalogServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.SCMD1=['SALOME_ModuleCatalog_Server']
        if 'launcher' in self.args:
            pos = args['launcher'].find(":")
            if pos != -1:
              self.SCMD1+=['-ORBInitRef']
              machine = args['launcher'][0:pos]
              port = args['launcher'][pos+1:]
              self.SCMD1+=["NameService=corbaname::" + machine + ":" + port]
        self.SCMD1+=['-common']
        self.SCMD2=[]
        home_dir=os.path.expanduser("~")
        if home_dir is not None:
            self.SCMD2=['-personal',os.path.join(home_dir,'Salome', 'resources', 'CatalogModulePersonnel.xml')]

    def setpath(self,modules_list,modules_root_dir):
        list_modules = modules_list[:]
        list_modules.reverse()
        if self.args["gui"] :
            list_modules = ["KERNEL", "GUI"] + list_modules
        else :
            list_modules = ["KERNEL"] + list_modules

        cata_path=get_cata_path(list_modules,modules_root_dir)

        self.CMD=self.SCMD1 + ['"' + '"::"'.join(cata_path) + '"'] + self.SCMD2

# ---

class SalomeDSServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.CMD=['SALOMEDS_Server']
        if 'launcher' in self.args:
            pos = args['launcher'].find(":")
            if pos != -1:
              self.CMD+=['-ORBInitRef']
              machine = args['launcher'][0:pos]
              port = args['launcher'][pos+1:]
              self.CMD+=["NameService=corbaname::" + machine + ":" + port]

# ---

class ConnectionManagerServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.CMD=['SALOME_ConnectionManagerServer']
        if 'launcher' in self.args:
            pos = args['launcher'].find(":")
            if pos != -1:
              self.CMD+=['-ORBInitRef']
              machine = args['launcher'][0:pos]
              port = args['launcher'][pos+1:]
              self.CMD+=["NameService=corbaname::" + machine + ":" + port]


# ---

class RegistryServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        self.CMD=['SALOME_Registry_Server', '--salome_session','theSession']
        if 'launcher' in self.args:
            pos = args['launcher'].find(":")
            if pos != -1:
              self.CMD+=['-ORBInitRef']
              machine = args['launcher'][0:pos]
              port = args['launcher'][pos+1:]
              self.CMD+=["NameService=corbaname::" + machine + ":" + port]

# ---

class ContainerCPPServer(Server):
    def __init__(self,args,with_gui=False):
        self.args=args
        self.initArgs()
        self.CMD=['SALOME_Container']
        if 'launcher' in self.args:
            pos = args['launcher'].find(":")
            if pos != -1:
              self.CMD+=['-ORBInitRef']
              machine = args['launcher'][0:pos]
              port = args['launcher'][pos+1:]
              self.CMD+=["NameService=corbaname::" + machine + ":" + port]
        self.CMD+=['FactoryServer']
        if not with_gui and self.args["valgrind_session"]:
            l = ["valgrind"]
            val = os.getenv("VALGRIND_OPTIONS")
            if val:
                l += val.split()
                pass
            self.CMD = l + self.CMD
            pass

# ---

class LoggerServer(Server):
    def __init__(self,args):
        self.args=args
        self.initArgs()
        from salome_utils import generateFileName, getLogDir
        logfile = generateFileName( getLogDir(),
                                    prefix="logger",
                                    extension="log",
                                    with_username=True,
                                    with_hostname=True,
                                    with_port=True)
        print("===========================================================")
        print("Logger server: put log to the file:")
        print(logfile)
        print("===========================================================")
        self.CMD=['SALOME_Logger_Server', logfile]
        pass
    pass # end of LoggerServer class

# ---
import abc
import tempfile
class CommonSessionServer(Server):
    def __init__(self,args,modules_list,modules_root_dir):
        self.args = args.copy()
        # Bug 11512 (Problems with runSalome --xterm on Mandrake and Debian Sarge)
        #self.args['xterm']=0
        #
        self.initArgs()
        self.SCMD1=[self.getSessionServerExe()]
        if "SQUISH_PREFIX" in os.environ:
            if platform.system() == "Windows" :
                self.SCMD1 = [os.path.join(os.getenv("SQUISH_PREFIX"), "bin", "dllpreload.exe"),os.path.join(os.getenv("SQUISH_SALOME_PATH"), "W64", "GUI", "bin", "salome", self.SCMD1[0])]
            else :
                os.environ["LD_LIBRARY_PATH"] = os.environ["SQUISH_PREFIX"] + "/lib:" + os.environ["LD_LIBRARY_PATH"]
        self.SCMD2=[]
        if 'launcher' in self.args:
            pos = args['launcher'].find(":")
            if pos != -1:
              self.SCMD1+=['-ORBInitRef']
              machine = args['launcher'][0:pos]
              port = args['launcher'][pos+1:]
              self.SCMD1+=["NameService=corbaname::" + machine + ":" + port]
        if 'registry' in self.args['embedded']:
            self.SCMD1+=['--with','Registry',
                         '(','--salome_session','theSession',')']
        if 'moduleCatalog' in self.args['embedded']:
            self.SCMD1+=['--with','ModuleCatalog','(','-common']
            home_dir=os.path.expanduser("~")
            if home_dir is not None:
                self.SCMD2+=['-personal',os.path.join(home_dir,'Salome','resources','CatalogModulePersonnel.xml')]
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
            raise Exception('Python containers no longer supported')
        if self.args['gui']:
            session_gui = self.args.get('session_gui', True)
            if not session_gui:
                self.SCMD2+=['--hide-desktop']
            else:
                if not self.args['splash']:
                    self.SCMD2+=['--hide-splash']
                    pass
                if self.args['study_hdf'] is not None:
                    self.SCMD2+=['--study-hdf=%s'%self.args['study_hdf']]
                    pass
                pass
                if 'pyscript' in self.args and len(self.args['pyscript']) > 0:
                    msg = json.dumps(self.args['pyscript'], cls=ScriptAndArgsObjectEncoder)
                    self.SCMD2+=['--pyscript=%s'%(msg)]
                    pass
                pass
                if self.args['gui_log_file'] is not None:
                    guilogfile = self.args['gui_log_file']
                    if os.path.exists(guilogfile) and os.path.isfile(guilogfile):
                        try:
                            os.remove(guilogfile)
                        except:
                            print("Error: cannot remove existing log file", guilogfile)
                            guilogfile = None
                    if guilogfile is not None:
                        self.SCMD2+=['--gui-log-file=%s'%guilogfile]
                    pass
                pass
            pass
        if self.args['noexcepthandler']:
            self.SCMD2+=['--no-exception-handler']
        if 'user_config' in self.args:
            self.SCMD2+=['--resources=%s'%self.args['user_config']]
        if 'modules' in self.args:
            list_modules = []
            #keep only modules with GUI
            for m in modules_list:
              if m not in modules_root_dir:
                list_modules.insert(0,m)
              else:
                fr1 = os.path.join(modules_root_dir[m],"share","salome","resources",m.lower(),"SalomeApp.xml")
                fr2 = os.path.join(modules_root_dir[m],"share","salome","resources","SalomeApp.xml")
                if os.path.exists(fr1) or os.path.exists(fr2):
                  list_modules.insert(0,m)
            list_modules.reverse()
            self.SCMD2+=['--modules (%s)' % ":".join(list_modules)]
            pass
        if 'language' in self.args:
            self.SCMD2+=['--language=%s' % self.args['language']]
        os_handle, iorfakens = tempfile.mkstemp()
        self.iorfakens = iorfakens
        os.close(os_handle)
        self.SCMD2+=["--iorfakens={}".format(iorfakens)]
        pass

    @abc.abstractmethod
    def getSessionServerExe(self):
        pass
    
    def setpath(self,modules_list,modules_root_dir):
        list_modules = modules_list[:]
        list_modules.reverse()
        if self.args["gui"] :
            list_modules = ["KERNEL", "GUI"] + list_modules
        else :
            list_modules = ["KERNEL"] + list_modules

        cata_path=get_cata_path(list_modules,modules_root_dir)

        if ("gui" in self.args) & ('moduleCatalog' in self.args['embedded']):
            #Use '::' instead ":" because drive path with "D:\" is invalid on windows platform
            self.CMD=self.SCMD1 + ['"' + '"::"'.join(cata_path) + '"'] + self.SCMD2
        else:
            self.CMD=self.SCMD1 + self.SCMD2
        if 'test' in self.args:
            self.CMD+=['-test'] + self.args['test']
        elif 'play' in self.args:
            self.CMD+=['-play'] + self.args['play']

        if self.args["gdb_session"] or self.args["ddd_session"]:
            f = open(".gdbinit4salome", "w")
            f.write("set args ")
            args = " ".join(self.CMD[1:])
            args = args.replace("(", "\(")
            args = args.replace(")", "\)")
            f.write(args)
            f.write("\n")
            f.close()
            if self.args["ddd_session"]:
                self.CMD = ["ddd", "--command=.gdbinit4salome", self.CMD[0]]
            elif self.args["gdb_session"]:
                self.CMD = ["xterm", "-e", "gdb", "--command=.gdbinit4salome", self.CMD[0]]
                pass
            pass

        if self.args["valgrind_session"]:
            l = ["valgrind"]
            val = os.getenv("VALGRIND_OPTIONS")
            if val:
                l += val.split()
                pass
            self.CMD = l + self.CMD
            pass

class SessionServer(CommonSessionServer):
    def __init__(self,args,modules_list,modules_root_dir):
        super().__init__(args,modules_list,modules_root_dir)
        import KernelBasis
        KernelBasis.setSSLMode(False)
    
    def getSessionServerExe(self):
        return "SALOME_Session_Server"
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
                home_dir=os.path.expanduser("~")
                if home_dir is not None:
                    self.SCMD2=['-personal',os.path.join(home_dir,'Salome','resources','CatalogModulePersonnel.xml')]
                self.SCMD2+=[')']
            if 'study' in self.args['embedded']:
                self.SCMD2+=['--with','SALOMEDS','(',')']
            if 'cppContainer' in self.args['embedded']:
                self.SCMD2+=['--with','Container','(','FactoryServer',')']

    def setpath(self,modules_list,modules_root_dir):
        list_modules = modules_list[:]
        list_modules.reverse()
        if self.args["gui"] :
            list_modules = ["KERNEL", "GUI"] + list_modules
        else :
            list_modules = ["KERNEL"] + list_modules

        cata_path=get_cata_path(list_modules,modules_root_dir)

        if ("gui" in self.args) & ('moduleCatalog' in self.args['embedded']):
            #Use '::' instead ":" because drive path with "D:\" is invalid on windows platform
            self.CMD=self.SCMD1 + ['"' + '"::"'.join(cata_path) + '"'] + self.SCMD2
        else:
            self.CMD=self.SCMD1 + self.SCMD2
