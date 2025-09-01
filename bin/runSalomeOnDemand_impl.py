#!/usr/bin/env python3
# -*- coding:utf-8 -*-
# Copyright (C) 2022-2025  CEA, EDF, OPEN CASCADE
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

#  File   : runSalomeOnDemand.py
#  Author : Konstantin Leontev, Open Cascade
#
## @package runSalomeOnDemand
# \brief Module that provides services to launch SALOME with custom set of modules
#

"""Run SALOME app in the context of adding modules as extensions.
"""

import os,sys
import salomeContext
from SalomeOnDemandTK.extension_utilities import logger, \
    find_envpy, module_from_filename, get_app_root, find_file, \
    SALOME_EXTDIR
from SalomeOnDemandTK.extension_query import ext_by_dependants, dependency_tree

def set_selext_env(install_dir, salomex_name, context=None):
    """
    Finds and run _env.py file for the given extension.

    Args:
        install_dir - path to directory to check.
        salomex_name - extension's name.
        context - SalomeContext object.

    Returns:
        True if an envpy file was found and run its init func.
    """

    logger.debug('Set an env for salome extension: %s...', salomex_name)

    # Set the root dir as env variable
    if not context:
        context = salomeContext.SalomeContext(None)
        context.setVariable('SALOME_APPLICATION_DIR', install_dir, overwrite=False)


    # Find env file
    ext_envpy = find_envpy(install_dir, salomex_name)
    if not ext_envpy:
        return False

    # Get a module
    envpy_module = module_from_filename(ext_envpy)
    if not envpy_module:
        return False

    # Set env if we have something to set
    ext_dir = os.path.join(install_dir, SALOME_EXTDIR)
    if hasattr(envpy_module, 'init'):
        envpy_module.init(context, ext_dir)
        return True
    else:
        logger.warning('Env file %s doesnt have init func:!', ext_envpy)

    logger.warning('Setting an env for salome extension %s failed!', salomex_name)
    return False

def set_ext_env(app_name='', version=''):
    """
    Set an environment to start SALOME as a set of extensions.

    Args:
        app_name - an application's name.
        version - a version of an application.

    Returns:
        None.
    """

    logger.debug('Set an env for app: %s, version: %s...', app_name, version)

    # Get the root directory
    app_root = os.environ.get('SALOME_APPLICATION_DIR', '')
    if not app_root:
        app_root = get_app_root()
        logger.warning(
            'Env var SALOME_APPLICATION_DIR is not set! It will be setted by default in %s.'%app_root)

    # Set the root dir as env variable
    context = salomeContext.SalomeContext(None)
    context.setVariable('SALOME_APPLICATION_DIR', app_root, overwrite=True)

    # Find and source all _env.py files for installed extensions
    tree = dependency_tree(app_root)
    installed_ext = ext_by_dependants(tree)
    logger.debug('Installed extensions: %s', installed_ext)
    if not installed_ext:
        logger.debug('There are not any extensions in %s!', app_root)
        return

    # Execute env file as a module
    for ext_name in installed_ext:
        set_selext_env(app_root, ext_name, context)
    for python_path in os.environ["PYTHONPATH"].split(':'):
        sys.path[:0] = [python_path]
