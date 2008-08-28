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
# ---
#
# File   : salome_utils.py
# Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
#
# ---

"""
Set of utility functions used by SALOME python scripts.
"""

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
    'getTmpDir',
    'generateFileName',
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
    1. try USER environment variable
    2. if fails, return 'unknown' as default user name
    """
    import os
    return os.getenv( "USER", "unknown" ) # 'unknown' is default user name

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

def getPortNumber():
    """
    Get current naming server port number:
    1. try NSPORT environment variable
    1. if fails, try to parse config file defined by OMNIORB_CONFIG environment variable
    2. if fails, return 2809 as default port number
    """
    import os
    try:
        return int( os.getenv( "NSPORT" ) )
    except:
        pass
    port = getPortFromORBcfg()
    if port is not None: return port
    return 2809      # '2809' is default port number

# ---

def getTmpDir():
    """
    Get directory to be used for the temporary files.
    """
    import os, sys
    if sys.platform == "win32":
        # for Windows: temporarily using home directory for tmp files;
        # to be replaced with TEMP environment variable later...
        dir = os.getenv("HOME")
    else:
        # for Linux: use /tmp/logs/{user} folder
        dir = os.path.join( '/tmp', 'logs', getUserName() )
        pass
    return dir

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
    return name
