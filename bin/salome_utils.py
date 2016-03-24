#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

# ---
# File   : salome_utils.py
# Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
# ---

## @package salome_utils
# \brief Set of utility functions used by SALOME python scripts.

#
# Exported functions
#

__all__ = [
    'getORBcfgInfo',
    'getHostFromORBcfg',
    'getPortFromORBcfg',
    'getUserName',
    'getHostName',
    'getShortHostName',
    'getAppName',
    'getPortNumber',
    'getLogDir',
    'getTmpDir',
    'getHomeDir',
    'generateFileName',
    'makeTmpDir',
    'uniteFiles',
    ]

# ---

def _try_bool( arg ):
    """
    Check if specified parameter represents boolean value and returns its value.
    String values like 'True', 'TRUE', 'YES', 'Yes', 'y', 'NO', 'false', 'n', etc
    are supported.
    If <arg> does not represent a boolean, an exception is raised.
    """
    import types
    if type( arg ) == types.BooleanType  :
        return arg
    elif type( arg ) == types.StringType  :
        v = str( arg ).lower()
        if   v in [ "yes", "y", "true"  ]: return True
        elif v in [ "no",  "n", "false" ]: return False
        pass
    raise Exception("Not boolean value")

# ---

def getORBcfgInfo():
    """
    Get omniORB current configuration.
    Returns a list of three values: [ orb_version, host_name, port_number ].

    The information is retrieved from the omniORB configuration file defined
    by the OMNIORB_CONFIG environment variable.
    If omniORB configuration file can not be accessed, a list of three empty
    strings is returned.
    """
    import os, re
    ret = [ "", "", "" ]
    try:
        f = open( os.getenv( "OMNIORB_CONFIG" ) )
        lines = f.readlines()
        f.close()
        regvar = re.compile( "(ORB)?InitRef.*corbaname::(.*):(\d+)\s*$" )
        for l in lines:
            try:
                m = regvar.match( l )
                if m:
                    if m.group(1) is None:
                        ret[0] = "4"
                    else:
                        ret[0] = "3"
                        pass
                    ret[1] = m.group(2)
                    ret[2] = m.group(3)
                    break
                pass
            except:
                pass
            pass
        pass
    except:
        pass
    return ret

# ---

def getHostFromORBcfg():
    """
    Get current omniORB host.
    """
    return getORBcfgInfo()[1]
# ---

def getPortFromORBcfg():
    """
    Get current omniORB port.
    """
    return getORBcfgInfo()[2]

# ---

def getUserName():
    """
    Get user name:
    1. try USER environment variable (USERNAME on windows)
    2. if fails, try LOGNAME (un*x)
    3. if fails return 'unknown' as default user name
    """
    import os, sys
    if sys.platform == "win32":
        return os.getenv("USERNAME", "unknown")
    else:
        user = os.getenv("USER")
        if user:
            return user
        return os.getenv("LOGNAME", "unknown")
# ---

def getHostName():
    """
    Get host name:
    1. try socket python module gethostname() function
    2. if fails, try HOSTNAME environment variable
    3. if fails, try HOST environment variable
    4. if fails, return 'unknown' as default host name
    """
    import os
    try:
        import socket
        host = socket.gethostname()
    except:
        host = None
        pass
    if not host: host = os.getenv("HOSTNAME")
    if not host: host = os.getenv("HOST")
    if not host: host = "unknown"           # 'unknown' is default host name
    return host

# ---

def getShortHostName():
    """
    Get short host name:
    1. try socket python module gethostname() function
    2. if fails, try HOSTNAME environment variable
    3. if fails, try HOST environment variable
    4. if fails, return 'unknown' as default host name
    """
    try:
        return getHostName().split('.')[0]
    except:
        pass
    return "unknown"           # 'unknown' is default host name

# ---

def getAppName():
    """
    Get application name:
    1. try APPNAME environment variable
    2. if fails, return 'SALOME' as default application name
    """
    import os
    return os.getenv( "APPNAME", "SALOME" ) # 'SALOME' is default user name

# ---

def getPortNumber(use_default=True):
    """
    Get current naming server port number:
    1. try NSPORT environment variable
    1. if fails, try to parse config file defined by OMNIORB_CONFIG environment variable
    2. if fails, return 2809 as default port number (if use_default is True) or None (id use_default is False)
    """
    import os
    try:
        return int( os.getenv( "NSPORT" ) )
    except:
        pass
    try:
        port = int( getPortFromORBcfg() )
        if port is not None: return port
    except:
        pass
    if use_default: return 2809 # '2809' is default port number
    return None

# ---

def getHomeDir():
    """
    Get home directory.
    """
    import os
    return os.path.realpath(os.path.expanduser('~'))
# ---

def getLogDir():
    """
    Get directory to be used for the log files.
    """
    import os
    return os.path.join(getTmpDir(), "logs", getUserName())
# ---

def getTmpDir():
    """
    Get directory to be used for the temporary files.
    """
    import os, tempfile
    f = tempfile.NamedTemporaryFile()
    tmpdir = os.path.dirname(f.name)
    f.close()
    return tmpdir
# ---

def generateFileName( dir, prefix = None, suffix = None, extension = None,
                      unique = False, separator = "_", hidden = False, **kwargs ):
    """
    Generate file name by sepecified parameters. If necessary, file name
    can be generated to be unique.

    Parameters:
    - dir       : directory path
    - prefix    : file prefix (not added by default)
    - suffix    : file suffix (not added by default)
    - extension : file extension (not added by default)
    - unique    : if this parameter is True, the unique file name is generated:
    in this case, if the file with the generated name already exists
    in the <dir> directory, an integer suffix is added to the end of the
    file name. This parameter is False by default.
    - separator : separator of the words ('_' by default)
    - hidden    : if this parameter is True, the file name is prepended by . (dot)
    symbol. This parameter is False by default.

    Other keyword parameters are:
    - with_username : 'add user name' flag/option:
      * boolean value can be passed to determine user name automatically
      * string value to be used as user name
    - with_hostname : 'add host name' flag/option:
      * boolean value can be passed to determine host name automatically
      * string value to be used as host name
    - with_port     : 'add port number' flag/option:
      * boolean value can be passed to determine port number automatically
      * string value to be used as port number
    - with_app      : 'add application name' flag/option:
      * boolean value can be passed to determine application name automatically
      * string value to be used as application name
    All <with_...> parameters are optional.
    """
    supported = [ 'with_username', 'with_hostname', 'with_port', 'with_app' ]
    from launchConfigureParser import verbose
    filename = []
    # separator
    if separator is None:
        separator = ""
        pass
    else:
        separator = str( separator )
        pass
    # prefix (if specified)
    if prefix is not None:
        filename.append( str( prefix ) )
        pass
    # additional keywords
    ### check unsupported parameters
    for kw in kwargs:
        if kw not in supported and verbose():
            print 'Warning! salome_utilitie.py: generateFileName(): parameter %s is not supported' % kw
            pass
        pass
    ### process supported keywords
    for kw in supported:
        if kw not in kwargs: continue
        ### user name
        if kw == 'with_username':
            try:
                # auto user name ?
                if _try_bool( kwargs[kw] ): filename.append( getUserName() )
                pass
            except:
                # user name given as parameter
                filename.append( kwargs[kw] )
                pass
            pass
        ### host name
        elif kw == 'with_hostname':
            try:
                # auto host name ?
                if _try_bool( kwargs[kw] ): filename.append( getShortHostName() )
                pass
            except:
                # host name given as parameter
                filename.append( kwargs[kw] )
                pass
            pass
        ### port number
        elif kw == 'with_port':
            try:
                # auto port number ?
                if _try_bool( kwargs[kw] ): filename.append( str( getPortNumber() ) )
                pass
            except:
                # port number given as parameter
                filename.append( str( kwargs[kw] ) )
                pass
            pass
        ### application name
        elif kw == 'with_app':
            try:
                # auto application name ?
                if _try_bool( kwargs[kw] ): filename.append( getAppName() )
                pass
            except:
                # application name given as parameter
                filename.append( kwargs[kw] )
                pass
            pass
        pass
    # suffix (if specified)
    if suffix is not None:
        filename.append( str( suffix ) )
        pass
    # raise an exception if file name is empty
    if not filename:
        raise Exception("Empty file name")
    #
    if extension is not None and extension.startswith("."): extension = extension[1:]
    #
    import os
    name = separator.join( filename )
    if hidden: name = "." + name                       # add dot for hidden files
    if extension: name = name + "." + str( extension ) # add extension if defined
    name = os.path.join( dir, name )
    if unique:
        # create unique file name
        index = 0
        while os.path.exists( name ):
            index = index + 1
            name = separator.join( filename ) + separator + str( index )
            if hidden: name = "." + name                       # add dot for hidden files
            if extension: name = name + "." + str( extension ) # add extension if defined
            name = os.path.join( dir, name )
            pass
        pass
    return os.path.normpath(name)

# ---

def makeTmpDir( path, mode=0777 ):
    """
    Make temporary directory with the specified path.
    If the directory exists then clear its contents.

    Parameters:
    - path : absolute path to the directory to be created.
    - mode : access mode
    """
    import os
    if os.path.exists( path ):
        import sys
        if sys.platform == "win32":
            os.system( "rmdir /S /Q " + '"' + path + '"' )
            os.system( "mkdir " + '"' + path + '"' )
        else:
            os.system( "rm -rf " + path + "/*" )
    else:
        dirs = path.split("/")
        shift1 = shift2 = 0
        if not dirs[0]: shift1 = 1
        if dirs[-1]: shift2 = 1
        for i in range(1+shift1,len(dirs)+shift2):
            p = "/".join(dirs[:i])
            try:
                os.mkdir(p, mode)
                os.chmod(p, mode)
            except:
                pass

# ---

def uniteFiles( src_file, dest_file ):
    """
    Unite contents of the source file with contents of the destination file
    and put result of the uniting to the destination file.
    If the destination file does not exist then the source file is simply
    copied to its path.

    Parameters:
    - src_file  : absolute path to the source file
    - dest_file : absolute path to the destination file
    """
    import os

    if not os.path.exists( src_file ):
        return
        pass

    if os.path.exists( dest_file ):
        # add a symbol of new line to contents of the destination file (just in case)
        dest = open( dest_file, 'r' )
        dest_lines = dest.readlines()
        dest.close()

        dest_lines.append( "\n" )

        dest = open( dest_file, 'w' )
        dest.writelines( dest_lines )
        dest.close()

        import sys
        if sys.platform == "win32":
            command = "type " + '"' + src_file + '"' + " >> " + '"' + dest_file + '"'
        else:
            command = "cat " + src_file + " >> " + dest_file
            pass
        pass
    else:
        import sys
        if sys.platform == "win32":
            command = "copy " + '"' + src_file + '"' + " " + '"' + dest_file + '"' + " > nul"
        else:
            command = "cp " + src_file + " " + dest_file
            pass
        pass

    os.system( command )

# --

_verbose = None

def verbose():
    """
    Get verbosity level. Default verbosity level is specified via the environment variable
    SALOME_VERBOSE, e.g.:
    [bash %] export SALOME_VERBOSE=1
    The function setVerbose() can be used to change verbosity level explicitly.
    """
    global _verbose
    # verbose has already been called
    if _verbose is not None:
        return _verbose
    # first time
    try:
        from os import getenv
        _verbose = int(getenv('SALOME_VERBOSE'))
    except:
        _verbose = 0
        pass
    #
    return _verbose
# --

def setVerbose(level):
    """
    Change verbosity level. The function verbose() can be used to get current verbosity level.
    """
    global _verbose
    _verbose = level
    return
# --

import signal
def killpid(pid, sig = 9):
    """
    Send signal sig to the process by pid.

    Parameters:
    - pid : PID of process
    - sig : signal for sending
            Possible values of signals: 
            9 means kill the process
            0 only check existing of the process
            NOTE: Other values are not processed on Windows
    Returns:
     1 Success
     0 Fail, no such process
    -1 Fail, another reason

    """
    if not pid: return
    import os, sys
    if sig != 0:
        if verbose(): print "######## killpid pid = ", pid
    try:
        if sys.platform == "win32":
            import ctypes
            if sig == 0:
                # PROCESS_QUERY_INFORMATION (0x0400)	Required to retrieve certain information about a process
                handle = ctypes.windll.kernel32.OpenProcess(0x0400, False, int(pid))
                if handle: 
                    ret = 1
                    ctypes.windll.kernel32.CloseHandle(handle)
                else:
                    ret = 0
            if sig == 9:
                # PROCESS_TERMINATE (0x0001)	Required to terminate a process using TerminateProcess.
                handle = ctypes.windll.kernel32.OpenProcess(0x0001, False, int(pid))
                ret = ctypes.windll.kernel32.TerminateProcess(handle, -1)
                ctypes.windll.kernel32.CloseHandle(handle)
                pass
            pass
        else:
            # Default: signal.SIGKILL = 9
            os.kill(int(pid),sig)
            ret = 1
            pass
        pass
    except OSError, e:
        # errno.ESRCH == 3 is 'No such process'
        if e.errno == 3:
            ret = 0
        else:
            ret = -1
            pass
        pass
    return ret
# --

def getOmniNamesPid(port):
    """
    Return OmniNames pid by port number.
    """
    import sys,subprocess,re
    if sys.platform == "win32":
        # Get process list by WMI Command Line Utility(WMIC)
        # Output is formatted with each value listed on a separate line and with the name of the property:
        #   ...
        #   Caption=<caption0>
        #   CommandLine=<commandline0>
        #   ProcessId=<processid0>
        #
        #
        #
        #   Caption=<caption1>
        #   CommandLine=<commandline1>
        #   ProcessId=<processid1>
        #   ...
        cmd = 'WMIC PROCESS get Caption,Commandline,Processid /VALUE'
        proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        # Get stdout
        allProc = proc.communicate()[0]
        # find Pid of omniNames
        pid = re.findall(r'Caption=.*omniNames.*\n?CommandLine=.*omniNames.*\D%s\D.*\n?ProcessId=(\d*)'%(port),allProc)[0]
    else:        
        cmd = "ps -eo pid,command | grep -v grep | grep -E \"omniNames.*%s\" | awk '{print $1}'"%(port)
        proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        pid = proc.communicate()[0]
        pass

    return pid
# --

def killOmniNames(port):
    """
    Kill OmniNames process by port number.
    """
    try:
        pid = getOmniNamesPid(port)
        if pid: killpid(pid)
    except:
        pass
    pass
# --
