#!/usr/bin/env python3
# -*- coding:utf-8 -*-
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
# See https://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

#  File   : extension_unpacker.py
#  Author : Konstantin Leontev, Open Cascade
#
#  @package SalomeOnDemandTK
#  @brief Set of utility to unpack SALOME python extensions.

"""Set of utility to unpack SALOME python extensions.
"""

import tarfile
import os
import sys
import json
from traceback import format_exc

import SalomeOnDemandTK.extension_remover as extension_remover

from .extension_utilities import logger, \
    DFILE_EXT, ARCFILE_EXT, EXTDEPENDSON_KEY, INSTALLFILE_EXT, EXTVERSION_KEY, DFILES_DIR, POSTINSTALL_DIR, \
    isvalid_filename, isvalid_dirname, ext_info_bykey, \
    get_app_root, check_if_installed, comp_interaction_treat, get_module_name, \
    EXTISGUI_KEY, MODELVERSION_KEY, value_from_salomexd, EXTDEPNAME_KEY, EXTDEPVERSION_KEY, \
    ModelVersion

class SalomeBootStrapInstallerException(Exception):
    def __init__(self, message):
        super().__init__(message)

def unpack_salomex(salome_root, salomex, remove_old_pkg):
    """
    Unpack a given salome extension archive into SALOME install root.

    Args:
        salome_root - path to SALOME install root directory.
        salomex - a given salomex file to unpack.

    Returns:
        True if an ext was successfully unpacked.
    """

    logger.debug('Starting unpack a salome extension file')

    # Check if provided filenames are valid
    if  not isvalid_dirname(salome_root) or \
        not isvalid_filename(salomex, ARCFILE_EXT):
        raise SalomeBootStrapInstallerException("Provided filename is not valid")

    try:
        with tarfile.open(salomex) as ext:

            # Read a list of dependencies, so let's check if they are present in salome_root
            ext_file_name, _ = os.path.splitext(os.path.basename(salomex))
            logger.debug('Try to read %s \'s %s file...', ext_file_name, DFILE_EXT)
            # Get salomexd file
            for member in ext.getmembers():
                if member.name.endswith(DFILE_EXT):
                    salomexd_file_name = os.path.basename(member.name)
                    salomexd_file = ext.extractfile(member).read().decode("utf-8")
                    break
            if not salomexd_file:
                raise SalomeBootStrapInstallerException('Cannot extract %s.%s file!', salome_ext_name, DFILE_EXT)

            # Check if the given extension is already installed
            salome_ext_name, _ = os.path.splitext(salomexd_file_name)
            _, salomexc = check_if_installed(salome_root, salome_ext_name)
            if salomexc:
                if remove_old_pkg:
                    logger.debug('%s is already installed on your application. It will be removed forcibly first to be able to reinstalled', salome_ext_name)
                    extension_remover.remove_salomex(salome_root, salome_ext_name, True)
                else:
                    logger.error('%s is already installed on your application. To reinstall an extension you need to remove it first!', salome_ext_name)
                    return None

            salomexd_content = json.loads(salomexd_file)
            logger.debug('Check dependencies...')
            if EXTDEPENDSON_KEY in salomexd_content and salomexd_content[EXTDEPENDSON_KEY]:
                depends_on = salomexd_content[EXTDEPENDSON_KEY]

                # Check every module if it's in the salome_root
                for depends in depends_on:
                    depends_filename = os.path.join(salome_root, DFILES_DIR, depends[EXTDEPNAME_KEY] + '.' + DFILE_EXT)
                    if not os.path.isfile(depends_filename):
                        logger.error('Cannot find %s for a module that extension depends on!',
                            depends_filename)
                        raise SalomeBootStrapInstallerException("Prequisite package missing!!")
                    else:
                        installed_version = value_from_salomexd(depends_filename, EXTVERSION_KEY)
                        if installed_version != depends[EXTDEPVERSION_KEY]:
                            logger.error('Installed version of %s is %s. But we need %s', depends[EXTDEPNAME_KEY], installed_version, depends[EXTDEPVERSION_KEY])
                            raise SalomeBootStrapInstallerException("Incompatible prerequisite version !!")


            # Check model version of extension. Its version must be the same with salome bootstrap version
            if salomexd_content[MODELVERSION_KEY] != ModelVersion:
                logger.error('Extension model version is not compatible \n\
                        %s ext-model version: %s \n\
                        SALOME ext-model version: %s',
                        salome_ext_name, salomexd_content[MODELVERSION_KEY], ModelVersion)
                raise SalomeBootStrapInstallerException("Incompatible Model version !!")

            # Unpack archive in the salome_root
            logger.debug('Extract all the files into %s...', salome_root)
            ext.extractall(salome_root)

            logger.debug('SALOME extension %s was installed.', salome_ext_name)

    except (OSError, KeyError):
        logger.error(format_exc())
        return None

    return salome_ext_name


def install_salomex(salomex, remove_old_pkg = False):
    """
    Install a given salome extension into SALOME_APPLICATION_DIR.

    Args:
        salomex - a given salomex file to unpack.

    Returns:
        salome module name or ext name if salome module name is not declared to be activated later or None if the function failed.
    """

    logger.debug('Starting install a salome extension from %s', salomex)

    # Check if we have the salome root path
    app_root = os.environ.get('SALOME_APPLICATION_DIR', '')
    if not app_root:
        # It should be set on the app start, but leave it here to run as a standalone script
        logger.warning(
            'Env var SALOME_APPLICATION_DIR is not set! Try to set it going up from cur location.')
        app_root = get_app_root()

    # Unpack an archive
    salome_ext_name = unpack_salomex(app_root, salomex, remove_old_pkg)
    if not salome_ext_name:
        return None

    # Execute post-install
    ext_post_install_script = os.path.join(app_root,POSTINSTALL_DIR,salome_ext_name + '.' + INSTALLFILE_EXT)

    if os.path.isfile(ext_post_install_script):
        os.chmod(ext_post_install_script, 0o755)
        ret = os.system(ext_post_install_script)
        if ret != 0:
            logger.error("Post installation is failed")
            return 1
        #os.remove(ext_post_install_script)

    # Get module name to activate later
    module_name = get_module_name(app_root, salome_ext_name)
    return module_name, ext_info_bykey(app_root, salome_ext_name, EXTISGUI_KEY)

if __name__ == '__main__':
    if len(sys.argv) == 3:
        arg_1, arg_2 = sys.argv[1:] # pylint: disable=unbalanced-tuple-unpacking
        unpack_salomex(arg_1, arg_2)
    elif len(sys.argv) == 2:
        install_salomex(sys.argv[1])
    else:
        logger.error('You must provide all the arguments!')
        logger.info(unpack_salomex.__doc__)
