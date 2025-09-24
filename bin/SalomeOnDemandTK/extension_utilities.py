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
SOD_DEBUG=os.getenv("SOD_DEBUG")
if SOD_DEBUG:
    logging.basicConfig(format=FORMAT, level=logging.DEBUG)
else:
    logging.basicConfig(format=FORMAT, level=logging.INFO)

logger = logging.getLogger()

# version of extension metadata model
ModelVersion = "1.0.0"

EXT_MNG_DIR = "ext_mng"
DFILES_DIR = os.path.join(EXT_MNG_DIR,"metadata")
CFILES_DIR = os.path.join(EXT_MNG_DIR,"control_files")
ENVFILES_DIR = os.path.join(EXT_MNG_DIR,"env")
POSTINSTALL_DIR = os.path.join(EXT_MNG_DIR,"postinstall")

SALOME_EXTDIR = '__SALOME_EXT__'
ARCFILE_EXT = 'salomex'
BFILE_EXT = 'salomexb'
CFILE_EXT = 'salomexc'
DFILE_EXT = 'salomexd'
PYFILE_EXT = 'py'
ENVPYFILE_SUF = '_env.py'
INSTALLFILE_EXT = 'post_install'

EXTNAME_KEY = 'name'
EXTVERSION_KEY = 'version'
EXTDESCR_KEY = 'descr'
EXTDEPENDSON_KEY = 'depends_on'
EXTAUTHOR_KEY = 'author'
EXTCOMPONENT_KEY = 'ext_components'
EXTISGUI_KEY = 'salomegui'
EXTSMOGULENAME_KEY = 'salomemodule_name'
License_KEY = 'License'
MODELVERSION_KEY = 'ModelVersion'
EXTSUFFIX_KEY = "suffix"

EXTDEPNAME_KEY = 'name'
EXTDEPVERSION_KEY = 'version'
DKEY_LIST = [ EXTNAME_KEY, EXTDESCR_KEY, EXTDEPENDSON_KEY, EXTAUTHOR_KEY, EXTCOMPONENT_KEY, EXTSMOGULENAME_KEY, EXTISGUI_KEY ]
ITERACTIVE_EXTCOMPONENT_KEY = 'salome_interactive'

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


def override_salomexd(file_path, item):
    """
    Override a salomexd file

    Args:
        file_path - the file_path of salomexd file
        item - A dictionary containing all item modified
               Key of these item must be declared in DKEY_LIST

    Returns:
        None
    """
    file_dir = os.path.dirname(file_path)
    if file_dir:
        os.chdir(file_dir)
    data = read_salomexd(file_path)
    for key in item:
        if key not in DKEY_LIST:
            logger.warning('Key %s was not declared in DKEY_LIST'%key)
        else:   
            data[key] = item[key]
    create_salomexd(
                    data[EXTNAME_KEY],
                    descr = data[EXTNAME_KEY],
                    depends_on = data[EXTDEPENDSON_KEY],
                    author = data[EXTAUTHOR_KEY],
                    components = data[EXTCOMPONENT_KEY]
                    )


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

    logger.debug('Cannot get a value for key %s in salomexd file %s', key, file_path)
    return None

def get_module_name(salome_root, salomex_name):
    """
    Get salome module name from salomexd. If salome module name is not declared, the extension name will be given
    Normally, the extension name declared in the salomexd is the same of extension config files name (salomexd, salomexb, _env.py)

    Args:
        install_dir - directory where the given extension is installed.
        salomex_name - the given extension's name.
    """
    module_name = ext_info_bykey(salome_root, salomex_name, EXTSMOGULENAME_KEY)
    if not module_name:
        logger.debug(f'salomemodule_name is not declared in {salomex_name}.salomexd .')
    return module_name

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
    salomexd_files = list_files_ext(os.path.join(install_dir,DFILES_DIR), DFILE_EXT)

    for salomexd_file in salomexd_files:
        dependant_name, _ = os.path.splitext(os.path.basename(salomexd_file))

        # Don't process <salomex_name> extension itself
        if dependant_name == salomex_name:
            continue

        logger.debug('Check dependencies for %s...', salomexd_file)
        salomexd_content = read_salomexd(salomexd_file)

        if EXTDEPENDSON_KEY in salomexd_content and salomexd_content[EXTDEPENDSON_KEY]:
            depends_on = salomexd_content[EXTDEPENDSON_KEY]
            depends_on_no_version = [ext["name"] for ext in depends_on ]
            logger.debug('List of dependencies: %s', depends_on_no_version)

            if salomex_name in depends_on_no_version:
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

    return find_file(install_dir, os.path.join(DFILES_DIR, salomex_name + '.' + DFILE_EXT))


def find_salomexc(install_dir, salomex_name):
    """
    Finds a salomexc file for the given extension.

    Args:
        install_dir - path to directory to check.
        salomex_name - extension's name.

    Returns:
        Abs path if the file exist, otherwise None.
    """

    return find_file(install_dir, os.path.join(CFILES_DIR, salomex_name + '.' + CFILE_EXT))


def find_envpy(install_dir, salomex_name):
    """
    Finds a _env.py file for the given extension.

    Args:
        install_dir - path to directory to check.
        salomex_name - extension's name.

    Returns:
        Abs path if the file exist, otherwise None.
    """

    return find_file(install_dir, os.path.join(ENVFILES_DIR, salomex_name + ENVPYFILE_SUF))


def find_postinstall(install_dir, salomex_name):
    """
    Finds a _env.py file for the given extension.

    Args:
        install_dir - path to directory to check.
        salomex_name - extension's name.

    Returns:
        Abs path if the file exist, otherwise None.
    """

    return find_file(install_dir, os.path.join(POSTINSTALL_DIR, salomex_name + '.' +INSTALLFILE_EXT))


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


def comp_interaction_treat(components):
    """
    Convert a dict of modules groups into dict containing modules names as key and its interactive_mode as value
    All the modules belong group salome_interactive has interactive_mode == True, so it has salome gui.

    Args:
        A dict {string: string list} containing group name and modules lists associated

    Returns:
        A dict {string, bool} containing modules name and interative mode associated
    """
    interactive = False
    dict_treated = {}
    if type(components) == dict:
        for group in components:
            if group != ITERACTIVE_EXTCOMPONENT_KEY:
                interactive = False
            else:
                interactive = True
            for component in components[group]:
                dict_treated[component] = interactive
    elif type(components) == list:
        interactive = True
        for component in components:
            dict_treated[component] = interactive
    return dict_treated

def findReplace(directory, find, replace, filePattern):
    for path, dirs, files in os.walk(os.path.abspath(directory)):
        for filename in fnmatch.filter(files, filePattern):
            filepath = os.path.join(path, filename)
            with open(filepath) as f:
                s = f.read()
            for s_find in re.findall(find,s):
                s = s.replace(s_find, replace)
            with open(filepath, "w") as f:
                f.write(s)

def get_OS_ID():
    """
    Get identification of operating system

    return: on Linux: <id>_<version_number> (Ex: debian_12)
            on Windows: windows
    """
    os_name = sys.platform
    if os_name  == "linux":
        import distro
        return distro.id() + "_" + distro.version()
    else:
        return os_name

def get_salome_version():
    """
    Get SALOME APP version from SALOME_APPLICATION_DIR/__RUN_SALOME__/bin/salome/VERSION
    """
    salome_app_dir = os.getenv( "SALOME_APPLICATION_DIR" )
    if salome_app_dir:
        try:
            filename = os.path.join(salome_app_dir, "__RUN_SALOME__","bin","salome","VERSION")
            f = open( filename )
            data = f.readlines()
            f.close()
            for l in data:
                if l.strip().startswith("#") or ":" not in l: continue
                key = ":".join( l.split( ":" )[ :-1 ] ).strip()
                val = l.split( ":" )[ -1 ].strip()
                if "salomeapp" in key.lower():
                    return val
        except Exception:
            pass
    return ""
