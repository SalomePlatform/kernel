#!/usr/bin/env python3
# -*- coding:utf-8 -*-
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
# See https://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

#  File   : extension_remover.py
#  Author : Konstantin Leontev, Open Cascade
#
#  @package SalomeOnDemandTK
#  @brief Set of utility to remove SALOME python extensions.

"""Set of utility to remove SALOME python extensions.
"""

import os
import sys
import shutil
from traceback import format_exc

from .extension_utilities import logger, \
    SALOME_EXTDIR, EXTCOMPONENT_KEY, \
    isvalid_dirname, list_dependants, is_empty_dir, \
    find_envpy, value_from_salomexd, check_if_installed


def remove_if_empty(top_dir, directory):
    """
    Recursively remove empty directories from the given one to the top.

    Args:
        top_dir - top parent directory that can be removed as well
        directory - the given directory

    Returns:
        None.
    """

    #logger.debug('Check if %s is empty...', directory)
    if not is_empty_dir(directory):
        return

    logger.debug('Directory %s is empty. Remove it.', directory)
    os.rmdir(directory)

    # Don't go up than top root
    if top_dir == directory:
        return

    # Remove the parent dir as well
    parent_dir = os.path.abspath(os.path.join(directory, os.pardir))
    remove_if_empty(top_dir, parent_dir)


def remove_bylist(root_dir, salomexc):
    """
    Remove files and directories listed in the given salomexc file.

    Args:
        root_dir - a root dir for listed files
        salomexc - file that contents a list of files to remove.

    Returns:
        True if all the files were deleted without critical errors.
    """

    logger.debug('Remove files from %s dir listed in %s...',
        root_dir, salomexc)

    try:
        with open(salomexc, 'r', encoding='UTF-8') as file:
            for line in file:
                path_to_remove = os.path.join(root_dir, line.strip())
                logger.debug('Remove file %s...', path_to_remove)

                if os.path.isfile(path_to_remove):
                    os.remove(path_to_remove)

                    # Remove the parent folder if empty
                    parent_dir = os.path.dirname(path_to_remove)
                    remove_if_empty(root_dir, parent_dir)

                elif os.path.islink(path_to_remove):
                    os.unlink(path_to_remove)

                    # Remove the parent folder if empty
                    parent_dir = os.path.dirname(path_to_remove)
                    remove_if_empty(root_dir, parent_dir)

                elif os.path.isdir(path_to_remove):
                    logger.warning('Directories are not expected to be listed in %s file! '
                        'Remove %s anyway.',
                        salomexc, path_to_remove)
                    # Use instead of rmdir here, because dir can be not empty
                    shutil.rmtree(path_to_remove)

                else:
                    logger.warning('Unexpected path %s!'
                        'It is not a file or directory. Skip.',
                        path_to_remove)

    except OSError:
        logger.error(format_exc())
        return False

    return True

def remove_salomex(install_dir, salomex_name):
    """
    Remove a salome extension from SALOME install root.

    Args:
        salome_root - path to SALOME install root directory.
        salomex_name - a name of salome extension to remove.

    Returns:
        List of deleted components or None if the functions fails.
    """

    logger.debug('Starting remove a salome extension %s', salomex_name)

    # Return value
    components = None

    # Check if provided dirname is valid
    if not isvalid_dirname(install_dir):
        return components

    # Check if the given extension is installed
    salomexd, salomexc = check_if_installed(install_dir, salomex_name)
    if not salomexc:
        logger.debug('Going to exit from extension removing process.')
        return components

    # Check if we cannot remove an extension because of dependencies
    dependants = list_dependants(install_dir, salomex_name)
    if len(dependants) > 0:
        logger.error('Cannot remove an extension %s because followed extensions depend on it: %s! '
            'Going to exit from extension removing process.',
            salomex_name, dependants)
        return components

    # Try to remove all the files listed in the control file
    if not remove_bylist(os.path.join(install_dir, SALOME_EXTDIR), salomexc):
        return components

    # Remove control file
    os.remove(salomexc)

    # Remove env file
    env_py = find_envpy(install_dir, salomex_name)
    if env_py:
        os.remove(env_py)
    else:
        logger.warning('Cannot find and remove %s file! ', env_py)

    # Remove description file
    if salomexd:
        # Get components to deactivate in UI if the case
        components = value_from_salomexd(salomexd, EXTCOMPONENT_KEY)
        os.remove(salomexd)

    logger.debug('An extension %s was removed from %s',
        salomex_name, install_dir)

    return components if components else []


if __name__ == '__main__':
    if len(sys.argv) == 3:
        arg_1, arg_2 = sys.argv[1:] # pylint: disable=unbalanced-tuple-unpacking
        remove_salomex(arg_1, arg_2)
    else:
        logger.error('You must provide all the arguments!')
        logger.info(remove_salomex.__doc__)
