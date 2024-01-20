#!/usr/bin/env python3
# -*- coding:utf-8 -*-
# Copyright (C) 2007-2024  CEA, EDF, OPEN CASCADE
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

#  File   : extension_utilities.py
#  Author : Konstantin Leontev, Open Cascade
#
#  @package SalomeOnDemandTK
#  @brief Utilities and constants those help to deal with salome extension files.

"""
Utilities and constants those help to deal with salome extension files.
"""

import os
import sys
import logging
import json
from traceback import format_exc
from pathlib import Path
import importlib.util
import fnmatch
import re

# Usually logging verbosity is set inside bin/runSalomeCommon.py when salome is starting.
# Here we do just the same for a case if we call this package stand alone.
FORMAT = '%(levelname)s : %(asctime)s : [%(filename)s:%(funcName)s:%(lineno)s] : %(message)s'
logging.basicConfig(format=FORMAT, level=logging.DEBUG)
logger = logging.getLogger()

# SalomeContext sets the logging verbosity level on its own,
# and we put it here, so it doesn't override the local format above.
#pylint: disable=wrong-import-position
import salomeContext
#pylint: enable=wrong-import-position

SALOME_EXTDIR = '__SALOME_EXT__'
ARCFILE_EXT = 'salomex'
BFILE_EXT = 'salomexb'
CFILE_EXT = 'salomexc'
DFILE_EXT = 'salomexd'
PYFILE_EXT = 'py'
ENVPYFILE_SUF = '_env.py'

EXTNAME_KEY = 'name'
EXTDESCR_KEY = 'descr'
EXTDEPENDSON_KEY = 'depends_on'
EXTAUTHOR_KEY = 'author'
EXTCOMPONENT_KEY = 'components'


def create_salomexd(name, descr='', depends_on=None, author='', components=None):
    """
    Create a basic salomexd file from provided args.
    Current version is a json file with function args as the keys.

    Args:
        name - the name of the corresponding salome extension.
        depends_on - list of the modules that current extension depends on.
        author - an author of the extension.
        components - the names of the modules those current extension was built from.

    Returns:
        None
    """

    logger.debug('Create salomexd file...')

    if depends_on is None:
        depends_on = []

    if components is None:
        components = []

    json_string = json.dumps(
        {
            EXTNAME_KEY: name,
            EXTDESCR_KEY: descr,
            EXTDEPENDSON_KEY: depends_on,
            EXTAUTHOR_KEY: author,
            EXTCOMPONENT_KEY: components
        },
        indent=4
    )

    try:
        with open(name + '.' + DFILE_EXT, "w", encoding="utf-8") as file:
            file.write(json_string)

    except OSError:
        logger.error(format_exc())


def read_salomexd(file_path):
    """
    Reads a content of a salomexd file. Current version is a json file.
    There's no check if the file_path is a valid salomexd file name.
    It's expected that user call isvalid_filename() in advance.

    Args:
        file_path - the path to the salomexd file.

    Returns:
        A dictionary that represents the content of the salomexd file.
    """

    logger.debug('Read salomexd file %s', file_path)

    try:
        with open(file_path, 'r', encoding='UTF-8') as file:
            return json.load(file)

    except OSError:
        logger.error(format_exc())
        return {}


def value_from_salomexd(file_path, key):
    """
    Reads a content of a salomexd file and return a value for the given key.

    Args:
        file_path - the path to the salomexd file.
        key - the key to search an assigned value.

    Returns:
        A value assigned to the given key if exist, otherwise None.
    """

    file_content = read_salomexd(file_path)
    if key in file_content and file_content[key]:
        logger.debug('Key: %s, value: %s', key, file_content[key])
        return file_content[key]

    logger.warning('Cannot get a value for key %s in salomexd file %s', key, file_path)
    return None


def ext_info_bykey(salome_root, salomex_name, key):
    """
    Search a salomexd file by ext name and return a value for the given key.

    Args:
        install_dir - directory where the given extension is installed.
        salomex_name - the given extension's name.
        key - the key to search an assigned value.

    Returns:
        A value for key in the given ext salomexd file.
    """

    salomexd = find_salomexd(salome_root, salomex_name)
    if salomexd:
        return value_from_salomexd(salomexd, key)

    return None


def create_salomexb(name, included):
    """
    Create a salomexb file from a list of included file names.
    For example:
    */*.py
    doc/*.html
    doc/*.jp

    Args:
        name - the name of the corresponding salome extension.
        included - list of the directories those must be included inside a salomex.

    Returns:
        None
    """

    logger.debug('Create salomexb file...')

    try:
        with open(name + '.' + BFILE_EXT, "w", encoding="utf-8") as file:
            file.write('\n'.join(included[0:]))

    except OSError:
        logger.error(format_exc())


def read_salomexb(file_path):
    """
    Returns a content af a salomexb file as a list of strings.
    There's no check if the file_path is a valid salomexb file name.
    It's expected that user call isvalid_filename() in advance.

    Args:
        file_path - the path to the salomexb file.

    Returns:
        List of strings - paths to the directories those must be included in
        corresponding salomex archive file.
    """

    logger.debug('Read salomexb file %s', file_path)

    try:
        with open(file_path, 'r', encoding='UTF-8') as file:
            return [line.rstrip() for line in file]

    except OSError:
        logger.error(format_exc())
        return []


def list_files(dir_path):
    """
    Returns the recursive list of relative paths to files as strings
    in the dir_path root directory and all subdirectories.

    Args:
        dir_path - the path to the directory where you search for files.

    Raises:
        Raises OSError exception.

    Returns:
        A list of relative paths to files inside the given directory.
    """

    files_list = []
    for root, _, files in os.walk(dir_path):
        for file in files:
            files_list += os.path.relpath(os.path.join(root, file), dir_path)

    return files_list


def filter_to_regex(dir_path, filter_patterns):
    r"""
    Makes a regex pattern from a given filter.

    Args:
        dir_path - the path to the directory where you search for files.
        filter_patterns - list of expressions for matching file names.

    Returns:
        A regex string translated from the filter.
        For example:
        Filter:  ['SMESH/**.cmake', 'SMESH/share/s*.med']
        Regex:   (?s:SMESH/.*\.cmake)\Z|(?s:SMESH/share/s.*\.med)\Z
        Matches: SMESH/adm_local/cmake_files/SalomeSMESHConfig.cmake
                 SMESH/share/salome/resources/smesh/padderdata/ferraill.med
    """

    logger.debug('Convert given filter to regex...')

    # On Windows, it converts forward slashes to backward slashes.
    norm_filter = [os.path.normpath(pat) for pat in filter_patterns]

    # Make a regex pattern
    # Adding '*' at the end of the folders names to match all the files inside.
    regex_pattern = r'|'.join(
        [fnmatch.translate(pat + '*' if os.path.isdir(os.path.join(dir_path, pat)) else pat)
        for pat
        in norm_filter])

    logger.debug('Regex pattern: %s', regex_pattern)

    return regex_pattern


def list_files_filter(dir_path, filter_patterns):
    """
    Returns the recursive list of relative paths to files as strings
    in the dir_path root directory and all subdirectories.

    Args:
        dir_path - the path to the directory where you search for files.
        filter_patterns - list of expressions for matching file names.

    Returns:
        files_abs - a list of absolute paths to selected files.
        files_rel - a list of relative paths to selected files.
    """

    logger.debug('Get list of files to add into archive...')

    regex_pattern = filter_to_regex(dir_path, filter_patterns)

    files_abs = []
    files_rel = []

    for root, _, files in os.walk(dir_path):
        for file in files:
            filename_abs = os.path.join(root, file)
            filename_rel = os.path.relpath(filename_abs, dir_path)

            if re.match(regex_pattern, filename_rel):
                logger.debug('File name %s matches pattern', filename_rel)
                files_abs.append(filename_abs)
                files_rel.append(filename_rel)

    return files_abs, files_rel


def list_files_ext(dir_path, ext):
    """
    Returns a list of abs paths to files with a given extension
    in the dir_path directory.

    Args:
        dir_path - the path to the directory where you search for files.
        ext - a given extension.

    Returns:
        A list of absolute paths to selected files.
    """

    logger.debug('Get list of files with extension %s...', ext)

    dot_ext = '.' + ext
    return [os.path.join(dir_path, f) for f in os.listdir(dir_path) if f.endswith(dot_ext)]


def list_tonewline_str(str_list):
    """
    Converts the given list of strings to a newline separated string.

    Args:
        dir_path - the path to the directory where you search for files.

    Returns:
        A newline separated string.
    """
    return '\n'.join(file for file in str_list)


def isvalid_filename(filename, extension):
    """
    Checks if a given filename is valid in a sense that it exists and have a given extension.

    Args:
        filename - the name of the file to check.
        extension - expected file name extension.

    Returns:
        True if the given filename is valid for given extension.
    """

    logger.debug('Check if the filename %s exists and has an extension %s', filename, extension)

    # First do we even have something to check here
    if filename == '' or extension == '':
        logger.error('A filename and extension cannot be empty! Args: filename=%s, extension=%s',
            filename, extension)
        return False

    # Check if the filename matchs the provided extension
    _, ext = os.path.splitext(filename)
    ext = ext.lstrip('.')
    if ext != extension:
        logger.error('The filename %s doesnt have a valid extension! \
            The valid extension must be: %s, but get: %s',
            filename, extension, ext)
        return False

    # Check if the file base name is not empty
    base_name = os.path.basename(filename)
    if base_name == '':
        logger.error('The file name %s has an empty base name!', filename)
        return False

    # Check if a file with given filename exists
    if not os.path.isfile(filename):
        logger.error('The filename %s is not an existing regular file!', filename)
        return False

    logger.debug('Filename %s exists and has an extension %s', filename, extension)
    return True


def isvalid_dirname(dirname):
    """
    Checks if a given directory name exists.

    Args:
        dirname - the name of the directory to check.

    Returns:
        True if the given dirname is valid.
    """

    logger.debug('Check if the dirname %s exists', dirname)

    # First do we even have something to check here
    if dirname == '':
        logger.error('A dirname argument cannot be empty! dirname=%s', dirname)
        return False

    # Check if a file with given filename exists
    if not os.path.isdir(dirname):
        logger.error('The dirname %s is not an existing regular file!', dirname)
        return False

    logger.debug('Directory %s exists', dirname)
    return True


def list_dependants(install_dir, salomex_name):
    """
    Checks if we have installed extensions those depend on a given extension.

    Args:
        install_dir - path to SALOME install root directory.
        salomex_name - a name of salome extension to check.

    Returns:
        True if the given extension has dependants.
    """

    logger.debug('Check if there are other extensions that depends on %s...', salomex_name)
    dependants = []
    salomexd_files = list_files_ext(install_dir, DFILE_EXT)

    for salomexd_file in salomexd_files:
        dependant_name, _ = os.path.splitext(os.path.basename(salomexd_file))

        # Don't process <salomex_name> extension itself
        if dependant_name == salomex_name:
            continue

        logger.debug('Check dependencies for %s...', salomexd_file)
        salomexd_content = read_salomexd(salomexd_file)

        if EXTDEPENDSON_KEY in salomexd_content and salomexd_content[EXTDEPENDSON_KEY]:
            depends_on = salomexd_content[EXTDEPENDSON_KEY]
            logger.debug('List of dependencies: %s', depends_on)

            if salomex_name in depends_on:
                dependants.append(dependant_name)

    if len(dependants) > 0:
        logger.debug('Followed extensions %s depend on %s',
            dependants, salomex_name)

    return dependants


def is_empty_dir(directory):
    """
    Checks if the given directory is empty.

    Args:
        directory - path to directory to check.

    Returns:
        True if the given directory is empty.
    """

    return not next(os.scandir(directory), None)


def find_file(directory, file_name):
    """
    Finds a file in the given directory.

    Args:
        directory - path to directory to check.
        file_name - given base filename with extension

    Returns:
        Abs path if the file exist, otherwise None.
    """

    logger.debug('Try to find %s file in %s...', file_name, directory)
    file = os.path.join(directory, file_name)
    if os.path.isfile(file):
        logger.debug('File %s exists.', file)
        return file

    logger.debug('File %s doesnt\'t exist. Return None.', file)
    return None


def find_salomexd(install_dir, salomex_name):
    """
    Finds a salomexd file for the given extension.

    Args:
        install_dir - path to directory to check.
        salomex_name - extension's name.

    Returns:
        Abs path if the file exist, otherwise None.
    """

    return find_file(install_dir, salomex_name + '.' + DFILE_EXT)


def find_salomexc(install_dir, salomex_name):
    """
    Finds a salomexc file for the given extension.

    Args:
        install_dir - path to directory to check.
        salomex_name - extension's name.

    Returns:
        Abs path if the file exist, otherwise None.
    """

    return find_file(install_dir, salomex_name + '.' + CFILE_EXT)


def find_envpy(install_dir, salomex_name):
    """
    Finds a _env.py file for the given extension.

    Args:
        install_dir - path to directory to check.
        salomex_name - extension's name.

    Returns:
        Abs path if the file exist, otherwise None.
    """

    return find_file(install_dir, salomex_name + ENVPYFILE_SUF)


def module_from_filename(filename):
    """
    Create and execute a module by filename.

    Args:
        filename - a given python filename.

    Returns:
        Module.
    """

    # Get the module from the filename
    basename = os.path.basename(filename)
    module_name, _ = os.path.splitext(basename)

    spec = importlib.util.spec_from_file_location(module_name, filename)
    if not spec:
        logger.error('Could not get a spec for %s file!')
        return None

    module = importlib.util.module_from_spec(spec)
    if not module:
        logger.error('Could not get a module for %s file!')
        return None

    sys.modules[module_name] = module

    logger.debug('Execute %s module', module_name)
    if not spec.loader:
        logger.error('spec.loader is None for %s file!')
        return None

    spec.loader.exec_module(module)

    return module


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


def get_app_root(levels_up=5):
    """
    Finds an app root by going up on the given steps.

    Args:
        levels_up - steps up in dir hierarchy relative to the current file.

    Returns:
        Path to the app root.
    """

    app_root = str(Path(__file__).resolve().parents[levels_up - 1])
    logger.debug('App root: %s', app_root)

    return app_root


def check_if_installed(install_dir, salomex_name):
    """
    Check if a given salome extension is installed in install_dir.
    Now for install|remove process we consider an ext is installed
    if we have at least salomexc file with list of files to remove
    if we need to clean up.

    Args:
        install_dir - path to SALOME install root directory.
        salomex_name - a given ext name.

    Returns:
        salomexd, salomexc file names.
    """

    logger.debug('Check if %s extension is installed in %s...', salomex_name, install_dir)

    salomexd = find_salomexd(install_dir, salomex_name)
    if not salomexd:
        logger.debug('Extension has been already removed or %s file was deleted by mistake. '
            'In the former case we can use %s file to clean up.', DFILE_EXT, CFILE_EXT)

    salomexc = find_salomexc(install_dir, salomex_name)
    if salomexc:
        logger.debug('An extension %s IS installed.', salomex_name)
    else:
        logger.debug('An extension %s IS NOT installed.', salomex_name)

    return salomexd, salomexc
