#! /usr/bin/env python3
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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

## @file killSalomeWithPort.py
#  @brief Forcibly stop %SALOME processes from given session(s).
#
#  The sessions are indicated by their ports on the command line as in below example:
#  @code
#  killSalomeWithPort.py 2811 2815
#  @endcode

"""
Forcibly stop given SALOME session(s).

To stop one or more SALOME sessions, specify network ports they are bound to,
for example:

* in shell

    $ killSalomeWithPort.py 2811 2815

* in Python script:

    from killSalomeWithPort import killMyPort
    killMyPort(2811, 2815)

"""

# pragma pylint: disable=invalid-name

import itertools
import os
import os.path as osp
import pickle
import re
import shutil
import sys
from contextlib import suppress
from glob import glob
from threading import Thread
from time import sleep

import psutil

from salome.kernel.salome_utils import (generateFileName, getHostName, getLogDir, getShortHostName,
                          getUserName, killOmniNames, killPid, verbose)
import logging
logger = logging.getLogger()

def getPiDict(port, appname='salome', full=True, hidden=True, hostname=None):
    """
    Get path to the file that stores the list of SALOME processes.

    This file is located in the user's home directory
    and named .<user>_<host>_<port>_SALOME_pidict
    where

    - <user> is user name
    - <host> is host name
    - <port> is port number

    :param port     : port number
    :param appname  : application name (default: 'salome')
    :param full     : if True, full path to the file is returned,
                      otherwise only file name is returned
    :param hidden   : if True, file name is prefixed with . (dot) symbol;
                      this internal parameter is only used to support
                      compatibility with older versions of SALOME
    :param hostname : host name (if not given, it is auto-detected)
    :return pidict file's name or path
    """
    # ensure port is an integer
    # warning: this function is also called with port='#####'!!!
    with suppress(ValueError):
        port = int(port)

    # hostname (if not specified via parameter)
    with suppress(AttributeError):
        hostname = hostname or os.getenv('NSHOST').split('.')[0]

    # directory to store pidict file (if `full` is True)
    # old style: pidict files aren't dot-prefixed, stored in the user's home directory
    # new style: pidict files are dot-prefixed, stored in the system-dependant temporary directory
    pidict_dir = getLogDir() if hidden else osp.expanduser('~')

    return generateFileName(pidict_dir if full else '',
                            suffix='pidict',
                            hidden=hidden,
                            with_username=True,
                            with_hostname=(hostname or True),
                            with_port=port,
                            with_app=appname.upper())

def appliCleanOmniOrbConfig(port):
    """
    Remove omniorb config files related to given `port` in SALOME application:
    - ${OMNIORB_USER_PATH}/.omniORB_${USER}_${HOSTNAME}_${NSPORT}.cfg
    - ${OMNIORB_USER_PATH}/.omniORB_${USER}_last.cfg
    the last is removed only if the link points to the first file.
    :param port : port number
    """
    omniorb_user_path = os.getenv('OMNIORB_USER_PATH')
    if not omniorb_user_path:
        # outside application context
        return

    logging.getLogger().debug("Cleaning OmniOrb config for port {}".format(port))

    omniorb_config = generateFileName(omniorb_user_path,
                                      prefix='omniORB',
                                      extension='cfg',
                                      hidden=True,
                                      with_username=True,
                                      with_hostname=True,
                                      with_port=port)
    last_running_config = generateFileName(omniorb_user_path,
                                           prefix='omniORB',
                                           suffix='last',
                                           extension='cfg',
                                           hidden=True,
                                           with_username=True)
    logging.getLogger().debug("Omniorb_config file deduced by port : {}".format(omniorb_config))
    logging.getLogger().debug("Omniorb_config file of last : {}".format(last_running_config))
    if osp.exists(last_running_config):
        if sys.platform == 'win32' or ( osp.exists(omniorb_config) and osp.samefile(last_running_config,omniorb_config) ):
            os.remove(last_running_config)

    if os.access(omniorb_config, os.F_OK):
        os.remove(omniorb_config)

    if osp.lexists(last_running_config):
        return

    # try to relink last.cfg to an existing config file if any
    cfg_files = [(cfg_file, os.stat(cfg_file)) for cfg_file in \
                     glob(osp.join(omniorb_user_path,
                                   '.omniORB_{}_*.cfg'.format(getUserName())))]
    next_config = next((i[0] for i in sorted(cfg_files, key=lambda i: i[1])), None)
    if next_config:
        if sys.platform == 'win32':
            shutil.copyfile(osp.normpath(next_config), last_running_config)
        else:
            os.symlink(osp.normpath(next_config), last_running_config)

def shutdownMyPort(port, cleanup=True):
    """
    Shutdown SALOME session running on the specified port.
    :param port    : port number
    :param cleanup : perform additional cleanup actions (kill omniNames, etc.)
    """
    if not port:
        return

    # ensure port is an integer
    with suppress(ValueError):
        port = int(port)

    # release port
    with suppress(ImportError):
        # DO NOT REMOVE NEXT LINE: it tests PortManager availability!
        from .PortManager import releasePort
        releasePort(port)

    # set OMNIORB_CONFIG variable to the proper file (if not set yet)
    omniorb_user_path = os.getenv('OMNIORB_USER_PATH')
    kwargs = {}
    if omniorb_user_path is not None:
        kwargs['with_username'] = True
    else:
        omniorb_user_path = osp.realpath(osp.expanduser('~'))
    omniorb_config = generateFileName(omniorb_user_path,
                                      prefix='omniORB',
                                      extension='cfg',
                                      hidden=True,
                                      with_hostname=True,
                                      with_port=port,
                                      **kwargs)
    os.environ['OMNIORB_CONFIG'] = omniorb_config
    os.environ['NSPORT'] = str(port)

    # give the chance to the servers to shutdown properly
    with suppress(Exception):
        from omniORB import CORBA
        from .LifeCycleCORBA import LifeCycleCORBA
        orb = CORBA.ORB_init([''], CORBA.ORB_ID)
        lcc = LifeCycleCORBA(orb) # see (1) below
        # shutdown all
        if verbose():
            print("Terminating SALOME session on port {}...".format(port))
        lcc.shutdownServers()
        # give some time to shutdown to complete
        sleep(1)
        # shutdown omniNames
        if cleanup:
            killOmniNames(port)
            __killMyPort(port, getPiDict(port))
            # DO NOT REMOVE NEXT LINE: it tests PortManager availability!
            from .PortManager import releasePort
            releasePort(port)
            sleep(1)
    sys.exit(0) # see (1) below

# (1) If --shutdown-servers option is set to 1, session close procedure is
# called twice: first explicitly by salome command, second by automatic
# atexit to handle Ctrl-C. During second call, LCC does not exist anymore and
# a RuntimeError is raised; we explicitly exit this function with code 0 to
# prevent parent thread from crashing.

def __killProcesses(processes):
    '''
    Terminate and kill all given processes (inernal).
    :param processes : list of processes, each one is an instance of psutil.Process
    '''
    # terminate processes
    for process in processes:
        process.terminate()
    # wait a little, then check for alive
    _, alive = psutil.wait_procs(processes, timeout=5)
    # finally kill alive
    for process in alive:
        process.kill()

def __killPids(pids):
    """
    Kill processes with given `pids`.
    :param pids : processes IDs
    """
    processes = []
    for pid in pids:
        try:
            logger.debug("Add process with PID = {} into PIDList to kill".format(pid))
            processes.append(psutil.Process(pid))
        except psutil.NoSuchProcess:
            logger.debug("  ------------------ Process {} not found".format(pid))
    __killProcesses(processes)

def __killMyPort(port, filedict):
    """
    Kill processes for given port (internal).
    :param port     : port number
    :param filedict : pidict file
    """
    # ensure port is an integer
    with suppress(ValueError):
        port = int(port)
    logger.debug("Into __killMyPort with port {}. File containing PID to kill is {}".format(port,filedict))
    # read pids from pidict file
    with suppress(Exception), open(filedict, 'rb') as fpid:
        pids_lists = pickle.load(fpid)
        # note: pids_list is a list of tuples!
        for pids in pids_lists:
            __killPids(pids)

    # finally remove pidict file
    try:
        os.remove(filedict)
    except:
        pass

def __guessPiDictFilename(port):
    """
    Guess and return pidict file for given `port` (internal).
    :param port : port number
    :return pidict file's path
    """
    # Check all possible versions of pidict file
    # new-style - dot-prefixed pidict file: hidden is True, auto hostname
    # old-style - not dot-prefixed pidict file: hidden is False, auto hostname
    # old-style - dot-prefixed pidict file: hidden is True, short hostname
    # old-style - not dot-prefixed pidict file: hidden is False, short hostname
    # old-style - dot-prefixed pidict file: hidden is True, long hostname
    # old-style - not dot-prefixed pidict file: hidden is False, long hostname
    for hostname, hidden in itertools.product((None, getShortHostName(), getHostName()),
                                              (True, False)):
        filedict = getPiDict(port, hidden=hidden, hostname=hostname)
        if not osp.exists(filedict):
            if verbose():
                print('Trying {}... not found'.format(filedict))
            continue
        if verbose():
            print('Trying {}... OK'.format(filedict))
        return filedict

    return None

def killProcessSSL(port, pids_list):
    """ Called by runSalome.py after CTRL-C.
    This method :
    - Kill all PIDS in pids
    - update file of pidict 
    """
    __killPids(pids_list)

    with suppress(ValueError):
        port = int(port)
        
    for filedict in glob('{}*'.format(getPiDict(port))):
        with suppress(Exception), open(filedict, 'rb') as fpid:
            logging.getLogger().debug("Removing following PIDS from file \"{}\" : {}"
            .format(filedict,pids_list))
            pids_lists_in_file = pickle.load(fpid)
        for dico_of_pids in pids_lists_in_file:
            for pid in pids_list:
                if pid in dico_of_pids:
                    del dico_of_pids[pid]
        pids_lists_in_file = [elt for elt in pids_lists_in_file if len(elt)>0]
        if len(pids_lists_in_file) == 0:
            try:
                logging.getLogger().debug("List of PIDS to Kill is now empty -> Remove file \"{}\"".format(filedict))
                os.remove(filedict)
            except:
                pass
            continue
        with suppress(Exception), open(filedict, 'wb') as fpid:
            logging.getLogger().debug("Writing back into file \"{}\"".format(filedict))
            pickle.dump(pids_lists_in_file,fpid)
    # clear-up omniOrb config files
    appliCleanOmniOrbConfig(port)

def killMyPort(*ports):
    """
    Kill SALOME session running on the specified port.
    :param ports : port numbers
    """
    for port in ports:
        # ensure port is an integer
        with suppress(ValueError):
            port = int(port)

        with suppress(Exception):
            # DO NOT REMOVE NEXT LINE: it tests PortManager availability!
            from .PortManager import releasePort
            # get pidict file
            filedict = getPiDict(port)
            if not osp.isfile(filedict): # removed by previous call, see (1) above
                if verbose():
                    print("SALOME session on port {} is already stopped".format(port))
                # remove port from PortManager config file
                with suppress(ImportError):
                    if verbose():
                        print("Removing port from PortManager configuration file")
                    releasePort(port)
                return

        # try to shutdown session normally
        Thread(target=shutdownMyPort, args=(port, True)).start()
        # wait a little...
        sleep(3)
        # ... then kill processes (should be done if shutdown procedure hangs up)
        try:
            # DO NOT REMOVE NEXT LINE: it tests PortManager availability!
            from . import PortManager # pragma pylint: disable=unused-import
            for file_path in glob('{}*'.format(getPiDict(port))):
                __killMyPort(port, file_path)
        except ImportError:
            __killMyPort(port, __guessPiDictFilename(port))

        # clear-up omniOrb config files
        appliCleanOmniOrbConfig(port)

def cleanApplication(port):
    """
    Clean application running on the specified port.
    :param port : port number
    """
    # ensure port is an integer
    with suppress(ValueError):
        port = int(port)

    # remove pidict file
    with suppress(Exception):
        filedict = getPiDict(port)
        os.remove(filedict)

    # clear-up omniOrb config files
    appliCleanOmniOrbConfig(port)

def killMyPortSpy(pid, port):
    """
    Start daemon process which watches for given process and kills session when process exits.
    :param pid  : process ID
    :param port : port number (to kill)
    """
    while True:
        ret = killPid(int(pid), 0) # 0 is used to check process existence without actual killing it
        if ret == 0:
            break # process does not exist: exit loop
        elif ret < 0:
            return # something got wrong
        sleep(1)

    filedict = getPiDict(port)
    if not osp.exists(filedict):
        return

    # check Session server
    try:
        import omniORB
        orb = omniORB.CORBA.ORB_init(sys.argv, omniORB.CORBA.ORB_ID)
        from . import SALOME_NamingServicePy
        naming_service = SALOME_NamingServicePy.SALOME_NamingServicePy_i(orb, 3, True)
        from . import SALOME #@UnresolvedImport @UnusedImport # pragma pylint: disable=unused-import
        session = naming_service.Resolve('/Kernel/Session')
        assert session
    except: # pragma pylint: disable=bare-except
        killMyPort(port)
        return
    try:
        status = session.GetStatSession()
    except: # pragma pylint: disable=bare-except
        # -- session is in naming service but likely crashed
        status = None
    if status is not None and not status.activeGUI:
        return
    killMyPort(port)

def __checkUnkilledProcesses():
    '''
    Check all unkilled SALOME processes (internal).
    :return: list of unkilled processes
    '''
    def _checkUserName(_process):
        # The following is a workaround for Windows on which
        # psutil.Process().username() returns 'usergroup' + 'username'
        return getUserName() == _process.username().split('\\')[-1]

    def _getDictfromOutput(_processes, _wildcard=None):
        for _process in psutil.process_iter(['name', 'username']):
            with suppress(psutil.AccessDenied):
                if _checkUserName(_process) and re.match(_wildcard, _process.info['name']):
                    _processes.append(_process)

    processes = list()
    _getDictfromOutput(processes, '(SALOME_*)')
    _getDictfromOutput(processes, '(omniNames)')
    _getDictfromOutput(processes, '(ghs3d)')
    _getDictfromOutput(processes, '(ompi-server)')

    return processes

def killUnkilledProcesses():
    """
    Kill processes which could remain even after shutdowning SALOME sessions.
    """
    __killProcesses(__checkUnkilledProcesses())

def main():
    '''
    Main function
    '''
    from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter
    formatter = lambda prog: ArgumentDefaultsHelpFormatter(prog, max_help_position=50, width=120)
    parser = ArgumentParser(description='Forcibly stop given SALOME session(s)',
                            formatter_class=formatter)
    parser.add_argument('ports',
                        help='ports to kill',
                        nargs='*', type=int)
    group = parser.add_mutually_exclusive_group()
    group.add_argument('-s', '--spy',
                       help='start daemon to watch PID and then kill given PORT',
                       nargs=2, type=int, metavar=('PID', 'PORT'))
    group.add_argument('-l', '--list',
                       help='list unkilled SALOME processes',
                       action='store_true')
    args = parser.parse_args()

    if args.ports and (args.spy or args.list):
        print("{}: error: argument ports cannot be used with -s/--spy or -l/--list"
              .format(parser.prog), file=sys.stderr)
        sys.exit(1)

    if args.spy:
        killMyPortSpy(*args.spy)
        sys.exit(0)

    if args.list:
        processes = __checkUnkilledProcesses()
        if processes:
            print("Unkilled processes: ")
            print("---------------------")
            print("   PID : Process name")
            print("--------------------")
            for process in processes:
                print("{0:6} : {1}".format(process.pid, process.name()))
        else:
            print("No SALOME related processed found")
        sys.exit(0)

    try:
        from salomeContextUtils import setOmniOrbUserPath #@UnresolvedImport
        setOmniOrbUserPath()
    except Exception as exc: # pragma pylint: disable=broad-except
        if verbose():
            print(exc)
        sys.exit(1)

    killMyPort(*args.ports)
