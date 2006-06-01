#  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
#  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org
#
#  File   : salome_version.py
#  Author : Vadim SANDLER
#  Module : SALOME

_salome_versions = {}

def getVersion( mod = "KERNEL" ):
    """
    Get SALOME module version number
    Returns: version number string or None if VERSION file is not found
    """
    global _salome_versions
    mod = mod.upper()
    if not _salome_versions.has_key( mod ):
        _salome_versions[ mod ] = None
        import os
        root_dir = os.getenv( "%s_ROOT_DIR" % mod )
        if root_dir:
            try:
                filename = root_dir + "/bin/salome/VERSION"
                if not os.path.exists( filename ):
                    filename = root_dir + "/bin/VERSION"
                file = open( filename )
                ver = file.readline()
                file.close()
                _salome_versions[ mod ] = ver.split( ":" )[ -1 ].strip()
            except:
                pass
    return _salome_versions[ mod ]

def getVersionMajor( mod = "KERNEL" ):
    """
    Get SALOME module major version number
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
    Returns: version release number string or None if VERSION file is not found
    """
    ver = getVersion( mod )
    try:
        return ver.split( "." )[ 2 ]
    except:
        pass
    return None
