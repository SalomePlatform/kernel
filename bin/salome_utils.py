#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2021  CEA/DEN, EDF R&D, OPEN CASCADE
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

## @package salome_utils
#  @brief Set of utility functions used by SALOME python scripts.

"""
Various utilities for SALOME.
"""

# pragma pylint: disable=invalid-name

import os
import os.path as osp
import re
import shutil
import socket
import sys
import tempfile
from contextlib import suppress

import psutil

def _try_bool(arg):
    """
    Convert given `arg` to a boolean value.
    String values like 'True', 'TRUE', 'YES', 'Yes', 'y', 'NO', 'false', 'n', etc.
    are supported.
    If `arg` does not represent a boolean, an exception is raised.
    :param arg : value being converted
    :return result of conversion: `True` or `False`
    """
    if isinstance(arg, bool):
        return arg
    if isinstance(arg, bytes):
        arg = arg.decode('utf-8', errors='ignore')
    if isinstance(arg, str):
        if arg.lower() in ('yes', 'y', 'true', 't'):
            return True
        if arg.lower() in ('no', 'n', 'false', 'f'):
            return False
    raise ValueError('Not a boolean value')

# ---

def getORBcfgInfo():
    """
    Get current omniORB configuration.

    The information is retrieved from the omniORB configuration file defined
    by the OMNIORB_CONFIG environment variable.
    If omniORB configuration file can not be accessed, a tuple of three empty
    strings is returned.

    :return tuple of three strings: (orb_version, host_name, port_number)
    """
    orb_version = ''
    hostname = ''
    port_number = ''
    with suppress(Exception), open(os.getenv('OMNIORB_CONFIG')) as forb:
        regvar = re.compile(r'(ORB)?InitRef.*corbaname::(.*):(\d+)\s*$')
        for line in forb.readlines():
            match = regvar.match(line)
            if match:
                orb_version = '4' if match.group(1) is None else '3'
                hostname = match.group(2)
                port_number = match.group(3)
                break
    return orb_version, hostname, port_number

# ---

def getHostFromORBcfg():
    """
    Get current omniORB host name.
    :return host name
    """
    return getORBcfgInfo()[1]

# ---

def getPortFromORBcfg():
    """
    Get current omniORB port.
    :return port number
    """
    return getORBcfgInfo()[2]

# ---

def getUserName():
    """
    Get user name.

    The following procedure is perfomed to deduce user name:
    1. try USER (USERNAME on Windows) environment variable.
    2. if (1) fails, try LOGNAME (un*x only).
    3. if (2) fails, return 'unknown' as default user name

    :return user name
    """
    return os.getenv('USERNAME', 'unknown') if sys.platform == 'win32' \
        else os.getenv('USER', os.getenv('LOGNAME', 'unknown'))

# ---

def getHostName():
    """
    Get host name.

    The following procedure is perfomed to deduce host name:
    1. try socket python module, gethostname() function
    2. if (1) fails, try HOSTNAME environment variable
    3. if (2) fails, try HOST environment variable
    4. if (3) fails, tries 'unknown' as default host name
    5. finally, checks that IP is configured for hostname; if not, returns 'localhost'

    :return host name
    """
    host = None
    with suppress(Exception):
        host = socket.gethostname()
    if not host:
        host = os.getenv('HOSTNAME', os.getenv('HOST', 'unknown'))
    try:
        # the following line just checks that IP is configured for hostname
        socket.gethostbyname(host)
    except (TypeError, OSError):
        host = 'localhost'
    return host

# ---

def getShortHostName():
    """
    Get short host name (with domain stripped).
    See `getHostName()` for more details.
    :return short host name
    """
    with suppress(AttributeError, IndexError):
        return getHostName().split('.')[0]
    return 'unknown' # default host name

# ---

def getAppName():
    """
    Get application name.
    The following procedure is perfomed to deduce application name:
    1. try APPNAME environment variable
    2. if (1) fails, return 'SALOME' as default application name
    :return application name
    """
    return os.getenv('APPNAME', 'SALOME') # 'SALOME' is default user name

# ---

def getPortNumber(use_default=True):
    """
    Get currently used omniORB port.
    The following procedure is perfomed to deduce port number:
    1. try NSPORT environment variable
    2. if (1) fails, try to parse config file defined by OMNIORB_CONFIG environment variable
    3. if (2) fails, return 2809 as default port number (if use_default is `True`) or `None`
       (if use_default is `False`)
    :return port number
    """
    with suppress(TypeError, ValueError):
        return int(os.getenv('NSPORT'))
    with suppress(TypeError, ValueError):
        port = int(getPortFromORBcfg())
        if port:
            return port
    return 2809 if use_default else None

# ---

def getHomeDir():
    """
    Get home directory.
    :return home directory path
    """
    return osp.realpath(osp.expanduser('~'))

# ---

def getLogDir():
    """
    Get directory that stores log files.
    :return path to the log directory
    """
    return osp.join(getTmpDir(), 'logs', getUserName())

# ---

def getTmpDir():
    """
    Get directory to store temporary files.
    :return temporary directory path
    """
    with tempfile.NamedTemporaryFile() as tmp:
        return osp.dirname(tmp.name)
    return None

# ---

# pragma pylint: disable=too-many-arguments
def generateFileName(path, prefix=None, suffix=None, extension=None,
                     unique=False, separator='_', hidden=False, **kwargs):
    """
    Generate file name.

    :param path      : directory path
    :param prefix    : file name prefix (none by default)
    :param suffix    : file name suffix (none by default)
    :param extension : file extension (none by default)
    :param unique    : if `True`, function generates unique file name -
                       in this case, if file with the generated name already
                       exists in `path` directory, an integer suffix is appended
                       to the file name (`False` by default)
    :param separator : words separator ('_' by default)
    :param hidden    : if `True`, file name is prepended with dot symbol
                       (`False` by default)
    :param kwargs    : additional keywrods arguments (see below)
    :return generated file name

    Additionally supported keyword parameters:
    - with_username : use user name:
    - with_hostname : use host name:
    - with_port : use port number:
    - with_app      : use application name:

    Any of these keyword arguments can accept either explicit string value,
    or `True` to automatically deduce value from current configuration.
    """
    filename = []

    def _with_str(_str):
        _str = '' if _str is None else str(_str)
        if _str:
            filename.append(_str)

    def _with_kwarg(_kwarg, _func):
        _value = kwargs.get(_kwarg, False)
        try:
            if _try_bool(_value):
                filename.append(str(_func()))
        except ValueError:
            _with_str(_value)

    _with_str(prefix)
    _with_kwarg('with_username', getUserName)
    _with_kwarg('with_hostname', getShortHostName)
    _with_kwarg('with_port', getPortNumber)
    _with_kwarg('with_app', getAppName)
    _with_str(suffix)

    # raise an exception if file name is empty
    if not filename:
        raise ValueError('Empty file name')

    # extension
    extension = '' if extension is None else str(extension)
    if extension.startswith('.'):
        extension = extension[1:]

    # separator
    separator = '' if separator is None else str(separator)

    def _generate(_index=None):
        # join all components together, add index if necessary
        if _index is not None:
            _name = separator.join(filename+[str(_index)])
        else:
            _name = separator.join(filename)
        # prepend with dot if necessary
        if hidden:
            _name = '.' + _name
        # append extension if ncessary
        if extension:
            _name = _name + '.' + extension
        # now get full path
        return osp.join(path, _name)

    name = _generate()
    if unique:
        index = 0
        while osp.exists(name):
            index = index + 1
            name = _generate(index)
    return osp.normpath(name)

# ---

def cleanDir(path):
    """
    Clear contents of directory.
    :param path directory path
    """
    if osp.exists(path):
        for filename in os.listdir(path):
            file_path = osp.join(path, filename)
            with suppress(OSError):
                if osp.isdir(file_path):
                    shutil.rmtree(file_path)
                else:
                    os.unlink(file_path)

# ---

def makeDir(path, mode=0o777):
    """
    Make directory with the specified path.
    :param path : directory path
    :param mode : access mode
    """
    try:
        oldmask = os.umask(0)
        os.makedirs(path, mode=mode, exist_ok=True)
    except IOError:
        pass
    finally:
        os.umask(oldmask)

# ---

def makeTmpDir(path, mode=0o777):
    """
    Make temporary directory with the specified path.
    If the directory exists, clear all its contents.
    :param path : directory path
    :param mode : access mode
    """
    makeDir(path, mode)
    cleanDir(path)

# ---

def uniteFiles(src_file, dest_file):
    """
    Join contents of `src_file` and `dest_file` and put result to `dest_file`.
    File `dest_file` may not exist.
    :param src_file  : source file path
    :param dest_file : destination file path
    """
    if not osp.exists(src_file):
        return

    if osp.exists(dest_file):
        with suppress(OSError), open(src_file, 'rb') as src, open(dest_file, 'ab') as dest:
            dest.write(b'\n')
            dest.write(src.read())
    else:
        with suppress(OSError):
            shutil.copy(src_file, dest_file)

# --

def verbose():
    """
    Get current verbosity level.

    Default verbosity level is specified via the environment variable SALOME_VERBOSE,
    e.g. in bash:

        $ export SALOME_VERBOSE=1

    The function `setVerbose()` can be used to explicitly set verbosity level.

    :return current verbosity level
    """
    if not hasattr(verbose, 'verbosity_level'):
        verbose.verbosity_level = 0 # default value
        with suppress(TypeError, ValueError):
            # from SALOME_VERBOSE environment variable
            verbose.verbosity_level = int(os.getenv('SALOME_VERBOSE', '0'))
    return verbose.verbosity_level
# --

def setVerbose(level):
    """
    Change verbosity level.
    The function `verbose()` can be used to get current verbosity level.
    :param level : verbosity level
    """
    with suppress(TypeError, ValueError):
        verbose.verbosity_level = int(level)
# --

def killPid(pid, sig=9):
    """
    Send signal `sig` to the process with given `pid`.

    :param pid : PID of the process
    :param sig : signal to send; some of possible values:
       - 9 : kill process
       - 0 : do nothing, just check process existence (see below)
       NOTE: other values are not processed on Windows
    :return result of execution:
       -  1 : success
       -  0 : fail, no such process
       - -1 : fail, another reason
    """
    if not pid:
        return -1

    with suppress(ValueError):
        pid = int(pid)

    if sig == 0:
        ret = 1 if psutil.pid_exists(pid) else 0
    else:
        if verbose():
            print("######## killPid pid = ", pid)
        try:
            process = psutil.Process(pid)
            process.terminate()
            _, alive = psutil.wait_procs([process], timeout=5)
            for proc in alive:
                proc.kill()
            ret = 1
        except psutil.NoSuchProcess:
            ret = 0
        except OSError:
            ret = -1
    return ret
# --

def getOmniNamesPid(port):
    """
    Get PID of omniNames process running on given `port`.
    :param port : port number
    :return omniNames process's PID
    """
    processes = {p.info['pid']: p.info['name'] for p in psutil.process_iter(['pid', 'name'])}
    return next((c.pid for c in psutil.net_connections(kind='inet') \
                     if c.laddr.port == port and processes.get(c.pid) == 'omniNames'), None)
# --

def killOmniNames(port):
    """
    Kill omniNames process running on given `port`.
    :param port : port number
    """
    with suppress(Exception):
        killPid(getOmniNamesPid(port))
# --
