#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

#  File   : salome_version.py
#  Author : Vadim SANDLER
#  Module : SALOME
#
__ALL__ = [
    "getVersion",
    "getVersionMajor",
    "getVersionMinor",
    "getVersionRelease",
    "getVersions",
    "getXVersion",
    "isDevelopmentVersion",
    ]

_salome_versions = {}

def getVersion( mod = "KERNEL", full = False ):
    """
    Get SALOME module version number
    Parameters:
    - mod  : SALOME module name, default is 'KERNEL'
    - full : take into account development marker (in this case 'dev' is added
             to the end of version value for development version), False by default
    Returns: version number string or None if VERSION file is not found
    """
    global _salome_versions
    mod = mod.upper()
    dev_flag = { True : "dev", False : "" }
    if not _salome_versions.has_key( mod ):
        _salome_versions[ mod ] = [ None, "" ]
        import os
        root_dir = os.getenv( "%s_ROOT_DIR" % mod )
        if root_dir:
            try:
                filename = root_dir + "/bin/salome/VERSION"
                if not os.path.exists( filename ):
                    filename = root_dir + "/bin/VERSION"
                f = open( filename )
                data = f.readlines()
                f.close()
                for l in data:
                    if l.strip().startswith("#") or ":" not in l: continue
                    key = ":".join( l.split( ":" )[ :-1 ] ).strip()
                    val = l.split( ":" )[ -1 ].strip()
                    if "development" in key.lower():
                        _salome_versions[ mod ][ 1 ] = dev_flag[ val == "1" ]
                    elif "version" in key.lower() or mod in key:
                        _salome_versions[ mod ][ 0 ] = val
                    pass
            except:
                pass
    v = _salome_versions[ mod ][ 0 ]
    if full and v is not None:
        v += _salome_versions[ mod ][ 1 ]
    return v is not None and v or ""

def getVersionMajor( mod = "KERNEL" ):
    """
    Get SALOME module major version number
    Parameters:
    - mod  : SALOME module name, default is 'KERNEL'
    Returns: version major number string or None if VERSION file is not found
    """
    ver = getVersion( mod )
    try:
        return ver.split( "." )[ 0 ]
    except:
        pass
    return None

def getVersionMinor( mod = "KERNEL" ):
    """
    Get SALOME module minor version number
    Parameters:
    - mod  : SALOME module name, default is 'KERNEL'
    Returns: version minor number string or None if VERSION file is not found
    """
    ver = getVersion( mod )
    try:
        return ver.split( "." )[ 1 ]
    except:
        pass
    return None

def getVersionRelease( mod = "KERNEL" ):
    """
    Get SALOME module release version number
    Parameters:
    - mod  : SALOME module name, default is 'KERNEL'
    Returns: version release number string or None if VERSION file is not found
    """
    ver = getVersion( mod )
    try:
        return ver.split( "." )[ 2 ]
    except:
        pass
    return None

def getVersions( mod = "KERNEL" ):
    """
    Get SALOME module version as list of [major, minor, release] numbers
    Parameters:
    - mod  : SALOME module name, default is 'KERNEL'
    Returns: version numbers list
    """
    try:
        major = int( getVersionMajor( mod ) )
    except:
        major = 0
        pass
    try:
        minor = int( getVersionMinor( mod ) )
    except:
        minor = 0
        pass
    try:
        rel = int( getVersionRelease( mod ) )
    except:
        rel = 0
        pass
    return [ major, minor, rel ]
    
def getXVersion( mod = "KERNEL" ):
    """
    Get SALOME module version as list of [major, minor, release] numbers
    Parameters:
    - mod  : SALOME module name, default is 'KERNEL'
    Returns: version numbers list
    """
    major, minor, rel = getVersions( mod )
    return hex( (major<<16) + (minor<<8) + rel )

def isDevelopmentVersion( mod = "KERNEL" ):
    """
    Checks if the version of SALOME module is marked as development one
    Parameters:
    - mod  : SALOME module name, default is 'KERNEL'
    Returns: Return True for delopment version of module or False otherwise
    """
    ver = getVersion( mod, True )
    return ver.endswith( "dev" )
