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

#  File   : extension_query.py
#  Author : Konstantin Leontev, Open Cascade
#
#  @package SalomeOnDemandTK
#  @brief An utility package that will allow you to know the size of an extension
# and generate a dependency tree.

"""
An utility package that will allow you to know the size of an extension
and generate a dependency tree.
"""

import os
import sys
from traceback import format_exc

from .extension_utilities import logger, \
    SALOME_EXTDIR, DFILE_EXT, EXTDEPENDSON_KEY, EXTDESCR_KEY, EXTAUTHOR_KEY, EXTCOMPONENT_KEY, DFILES_DIR, EXTVERSION_KEY, EXTSUFFIX_KEY, \
    isvalid_dirname, find_salomexc, list_files_ext, read_salomexd, comp_interaction_treat


def size_to_str(size, format_in_bytes=False):
    """
    Returns a string describing a size.

    Args:
        size - the size to represent as a string.
        format_in_bytes - if True, size is expressed in bytes,
            otherwise human readable units are used.

    Returns:
        The size as a string with units.
    """

    if format_in_bytes:
        return '%s' % size

    __units__ = ["", "Ki", "Mi", "Gi", "Ti"]

    kilo = 1024.0
    prefix_index = 0
    prefix_index_max = len(__units__) - 1
    while (size > kilo and prefix_index < prefix_index_max):
        prefix_index += 1
        size /= kilo

    unit = __units__[prefix_index]
    return '%.2f %sB' % (size, unit)


def dir_size(directory):
    """
    Calculate a total size of the given directory.

    Args:
        directory - the given directory

    Returns:
        Size of the directory.
    """

    logger.debug('Get the size of %s', directory)

    total_size = 0
    for root, _, files in os.walk(directory):
        for file in files:
            itempath = os.path.join(root, file)
            if os.path.islink(itempath):
                continue

            total_size += os.path.getsize(itempath)

    logger.debug('The size of %s: %s', directory, total_size)
    return total_size


def dir_size_str(directory, format_in_bytes=False):
    """
    Calculate a total size of the given directory and format as a string.

    Args:
        directory - the given directory
        format_in_bytes - if True, size is expressed in bytes,
            otherwise human readable units are used.

    Returns:
        Size of the directory as a formatted string.
    """

    size = dir_size(directory)
    size_str = size_to_str(size, format_in_bytes)

    logger.debug('The size of %s: %s', directory, size_str)
    return size_str


def size_bylist(root_dir, salomexc):
    """
    Calcualate the total size of files listed in the given salomexc file.

    Args:
        root_dir - a root dir for listed files
        salomexc - file that contents a list of files.

    Returns:
        The total size of listed files.
    """

    logger.debug('Calcualate the total size of files inside %s listed in %s...',
        root_dir, salomexc)

    try:
        with open(salomexc, 'r', encoding='UTF-8') as file:
            total_size = 0
            for line in file:
                path_to_file = os.path.join(root_dir, line.strip())
                #logger.debug('Check the file %s...', path_to_file)

                if os.path.isfile(path_to_file) and not os.path.islink(path_to_file):
                    size = os.path.getsize(path_to_file)
                    total_size += size
                    logger.debug('%s size: %s', path_to_file, size)

                elif os.path.islink(path_to_file):
                    logger.debug('%s is link. Skip.', path_to_file)
                    continue

                elif os.path.isdir(path_to_file):
                    logger.warning('Directories are not expected to be listed in %s file! '
                        'Skip.',
                        salomexc)
                    continue

                else:
                    logger.warning('Unexpected path %s '
                        'is not a file, link or directory. Skip.',
                        path_to_file)

            return total_size

    except OSError:
        logger.error(format_exc())

    return None


def ext_size(install_dir, salomex_name):
    """
    Calculate a total size of a salome extension from SALOME install root.

    Args:
        salome_root - path to SALOME install root directory.
        salomex_name - a name of the given salome extension.

    Returns:
        Size of the directory in bytes.
    """

    # Check if provided dirname is valid
    if not isvalid_dirname(install_dir):
        return None

    # Check if an extension with this name is installed
    salomexc = find_salomexc(install_dir, salomex_name)
    if not salomexc:
        logger.error('Cannot calculate the size of %s extension!',
            salomex_name)
        return None

    # Calculate the size
    return size_bylist(os.path.join(install_dir, SALOME_EXTDIR), salomexc)


def ext_size_str(install_dir, salomex_name, format_in_bytes=False):
    """
    Calculate a total size of the given extension and format as a string.

    Args:
        install_dir - directory where the given extension is installed.
        salomex_name - the given extension's name.
        format_in_bytes - if True, size is expressed in bytes,
            otherwise human readable units are used.

    Returns:
        Size of the extension as a formatted string.
    """

    size = ext_size(install_dir, salomex_name)
    if size is None:
        return ''

    size_str = size_to_str(size, format_in_bytes)

    logger.debug('The size of %s: %s', salomex_name, size_str)
    return size_str


def dependency_tree(directory):
    r"""
    Create a dependency tree for all salome extensions
    installed in the given directory.

    Args:
        directory - the given directory

    Returns:
        A dictionary like that for extensions A, B, C, D and E:
          A
         /|\
        / B D
        \/ \/
        C   E

        { 'A': ['B', 'C', 'D'], 'B': ['C', 'E'], 'C': [], 'D': ['E'], 'E': [] }.
    """

    salomexd_dir = os.path.join(directory,DFILES_DIR)
    logger.debug('Build dependency tree for extensions in %s', salomexd_dir)

    tree = {}
    salomexd_files = list_files_ext(salomexd_dir, DFILE_EXT)
    logger.debug('There are %s extensions in %s', len(salomexd_files), salomexd_dir)

    for salomexd_file in salomexd_files:
        ext_name, _ = os.path.splitext(os.path.basename(salomexd_file))

        logger.debug('Check dependencies for %s...', salomexd_file)
        salomexd_content = read_salomexd(salomexd_file)

        if EXTDEPENDSON_KEY in salomexd_content:
            depends_on = salomexd_content[EXTDEPENDSON_KEY]
            depends_on_no_version = [ext["name"] for ext in depends_on ]
            logger.debug('List of dependencies: %s', depends_on_no_version)

            tree[ext_name] = depends_on_no_version

    logger.debug('Dependency tree: %s', tree)
    return tree

def ext_dependency_tree( salomexd_files_dir, ext_name ):
    r"""
    Create a dependency tree for one salome extensions
    Data is retrieved from the directory containning all of salomexd files.
    ext_name.salomexd must be present dans salomexd files directory

    Args:
        salomexd_files_dir - salomexd files directory
        ext_name - the extension name

    Returns:
        A dictionary like that for extensions A, B, C, D and E:
          A
         /|\
        / B D
        \/ \/
        C   E

        { 'A': ['B', 'C', 'D'], 'B': ['C', 'E'], 'C': [], 'D': ['E'], 'E': [] }.
    """

    tree = {}
    ext_salomexd_file = os.path.join(salomexd_files_dir,ext_name + "." + DFILE_EXT)
    salomexd_content = read_salomexd(ext_salomexd_file)
    depends_on = salomexd_content[EXTDEPENDSON_KEY]
    depends_on_no_version = [ext["name"] for ext in depends_on ]
    tree[ext_name] = depends_on_no_version
    if tree[ext_name]:
        for ext_dep_name in tree[ext_name]:
            tree.update(ext_dependency_tree(salomexd_files_dir, ext_dep_name))
    return tree

def get_depends_on_list( ext_list, salomexd_files_dir = ""):
    """
    Get an orderd list of depends_on of an extensions list from *.salomexd
    This list is used to automate the installation of an extension and its prerequis
    """
    if not salomexd_files_dir:
        salomexd_files_dir = os.getenv("SALOMEXD_FILES_DIR") if os.getenv("SALOMEXD_FILES_DIR") else ""
    if os.path.isdir(salomexd_files_dir):
        tree = {}
        for module_name in ext_list:
            tree.update(ext_dependency_tree(salomexd_files_dir,module_name))
        return ext_by_dependants(tree)
    else:
        logger.error(f"salomexd files directory \"{salomexd_files_dir}\" not found!!")
        return None

def ext_info_dict(directory):
    r"""
    Get installed salome extensions info.

    Args:
        directory - the given ext install directory.

    Returns:
        A dictionary {name: {salomexd_content}}.
    """

    salomexd_dir = os.path.join(directory, DFILES_DIR)
    logger.debug('Build info dictionary for extensions in %s', salomexd_dir)

    ext_info = {}
    salomexd_files = list_files_ext(salomexd_dir, DFILE_EXT)
    logger.debug('There are %s extensions in %s', len(salomexd_files), salomexd_dir)

    for salomexd_file in salomexd_files:
        # Collect size info
        ext_name, _ = os.path.splitext(os.path.basename(salomexd_file))
        size = ext_size_str(directory, ext_name)

        # Collect salomexd info
        salomexd_content = read_salomexd(salomexd_file)

        ext_info[ext_name] = salomexd_content
        ext_info[ext_name]["size"] = size

    logger.debug('Installed extensions info dict: %s', ext_info)
    return ext_info

def ext_info_list(directory):
    r"""
    Get installed salome extensions info.

    Args:
        directory - the given ext install directory.

    Returns:
        A dictionary {name: [descr, author, components, size, version, suffix]}.
    """

    info_dict = ext_info_dict(directory)

    ext_info = {}
    for ext_name in info_dict:

        descr = ''
        if EXTDESCR_KEY in info_dict[ext_name]:
            descr = info_dict[ext_name][EXTDESCR_KEY]
            logger.debug('descr: %s', descr)

        author = ''
        if EXTAUTHOR_KEY in info_dict[ext_name]:
            author = info_dict[ext_name][EXTAUTHOR_KEY]
            logger.debug('author: %s', author)

        components = ''
        if EXTCOMPONENT_KEY in info_dict[ext_name]:
            components = ', '.join(comp_interaction_treat(info_dict[ext_name][EXTCOMPONENT_KEY]))
            logger.debug('components: %s', components)

        version = ''
        if EXTVERSION_KEY in info_dict[ext_name]:
            version = info_dict[ext_name][EXTVERSION_KEY]
            logger.debug('version: %s', version)

        suffix = 1
        if EXTSUFFIX_KEY in info_dict[ext_name]:
            suffix = info_dict[ext_name][EXTSUFFIX_KEY]
            logger.debug('suffix: %s', suffix)

        size = 0
        if "size" in info_dict[ext_name]:
            size = info_dict[ext_name]["size"]
            logger.debug('size: %s', size)
        else:
            logger.error(f'size of {ext_name} is not received')
            return None

        ext_info[ext_name] = [descr, author, components, size, version, suffix]

    logger.debug('Installed extensions info: %s', ext_info)
    return ext_info

def ext_by_dependants(dep_tree, depends_on=None):
    r"""
    Calcualate a list of extensions names sorted by dependencies.

    Args:
        dep_tree - a dependecy tree of all considered extensions.
        depends_on - a set of extensions names to check if the current one depends on them.

    Returns:
        A list of of extensions from dep_tree sorted by dependencies.
        For example, dictionary like that for extensions A, B, C, D and E:
          A
         /|\
        / B D
        \/ \/
        C   E

        represented { 'A': ['B', 'C', 'D'], 'B': ['C', 'E'], 'C': [], 'D': ['E'], 'E': [] }
        returns ['A', 'B', 'D', 'C', 'E'].
    """

    logger.debug('dep_tree: %s, depends_on: %s', dep_tree, depends_on)

    if not dep_tree:
        logger.debug('Dependency tree is empty! Return')
        return []

    # Check if we've got a default value
    if not depends_on:
        depends_on = set()

    # Collect all dependencies for the given dependency level in the tree
    cur_depends_on = set()
    for ext, dependendants in dep_tree.items():
        # Select all components with empty dependendants
        is_selected = not dependendants

        # Check dependendants intersection if the case
        if not is_selected:
            dep_set = set(dependendants)
            intersection = dep_set & depends_on
            logger.debug('ext: %s, dep_set: %s, intersection: %s', ext, dep_set, intersection)
            is_selected = intersection == dep_set

        if is_selected:
            cur_depends_on.add(ext)

    # Check collected dependencies
    logger.debug('cur_depends_on: %s', cur_depends_on)
    if not cur_depends_on:
        logger.warning(
            'Extensions in the tree doesnt rely on any other extensions! Return all of them')
        return [ext for ext, _ in dep_tree.items()]

    # Collect all extension for this level
    res_extensions = []
    for ext in cur_depends_on:
        res_extensions.append(ext)
        del dep_tree[ext]

    logger.debug('res_extensions: %s', res_extensions)

    # Get extensions from the next dependency level of the tree
    cur_depends_on |= depends_on
    return res_extensions + ext_by_dependants(dep_tree, cur_depends_on)


def ext_by_name(directory):
    """
    Calcualate a list of extensions installed in the given directory.

    Args:
        directory - a directory where extensions are installed.

    Returns:
        A list of extensions names sorted by name.
    """

    logger.debug('directory: %s', directory)
    salomexd_dir = os.path.join(directory, DFILES_DIR)

    # Get ext files
    salomexd_files = list_files_ext(salomexd_dir, DFILE_EXT)
    logger.debug('There are %s extensions in %s', len(salomexd_files), salomexd_dir)

    # Get ext names
    res_names = []
    for salomexd_file in salomexd_files:
        ext_name, _ = os.path.splitext(os.path.basename(salomexd_file))
        res_names.append(ext_name)

    # Sort
    res_names.sort()
    logger.debug('Installed extensions: %s', res_names)

    return res_names


def ext_canremove_flags(directory):
    r"""
    Calcualate a dict of extensions names paired with bool flag if able to be removed.

    Args:
        directory - the given ext install directory.

    Returns:
        For dependency tree for extensions A, B, C, D and E:
          A
         /|\
        / B D
        \/ \/
        C   E

        represented { 'A': ['B', 'C', 'D'], 'B': ['C', 'E'], 'C': [], 'D': ['E'], 'E': [] }
        returns ['A': True, 'B': False, 'D': False, 'C': False, 'E': False].
        We can remove only A here, because we don't have any other modules depend on it.
    """

    logger.debug('directory: %s', directory)

    dep_tree = dependency_tree(directory)

    res_dict = {}
    for ext in dep_tree:
        can_remove = True
        for dep in dep_tree.values():
            if ext in dep:
                # At least one ext depends on it, so can't remove
                can_remove = False
                break

        res_dict[ext] = can_remove

    # We shouldn't remove Base in any case
    if 'Base' in dep_tree:
        res_dict['Base'] = False

    logger.debug('res_dict: %s', res_dict)

    return res_dict

def get_ext_version(ext_name, root_dir = os.getenv("SALOME_APPLICATION_DIR")):
    r"""
    Get version de l'extension from salomexd

    Args:
        ext_name - the extension name

    Returns:
        version - string
    """

    if not root_dir or not os.path.isdir(root_dir):
        logger.error(f'Installation directory {root_dir} is not found')
        return None

    salomexd_dir = os.path.join(root_dir, DFILES_DIR)
    ext_salomexd_file = os.path.join(salomexd_dir,ext_name + "." + DFILE_EXT)
    salomexd_content = read_salomexd(ext_salomexd_file)
    return salomexd_content[EXTVERSION_KEY]

def get_ext_version_major( ext_name ):
    r"""
    Get version de l'extension from salomexd

    Args:
        ext_name - the extension name

    Returns:
        version major - string
    """
    ver = get_ext_version( ext_name )
    try:
        return ver.split( "." )[ 0 ]
    except Exception:
        pass
    return None

def get_ext_version_minor( ext_name ):
    r"""
    Get version de l'extension from salomexd

    Args:
        ext_name - the extension name

    Returns:
        version minor - string
    """
    ver = get_ext_version( ext_name )
    try:
        return ver.split( "." )[ 1 ]
    except Exception:
        pass
    return None

def get_ext_version_release( ext_name ):
    r"""
    Get version de l'extension from salomexd

    Args:
        ext_name - the extension name

    Returns:
        version release - string
    """
    ver = get_ext_version( ext_name )
    try:
        return ver.split( "." )[ 2 ]
    except Exception:
        pass
    return None

if __name__ == '__main__':
    if len(sys.argv) == 2:
        dir_size_str(sys.argv[1])
        ext_tree = dependency_tree(sys.argv[1])
        ext_list = ext_by_dependants(ext_tree)
        logger.info('ext_list: %s', ext_list)
        ext_by_name(sys.argv[1])
        ext_info_dict(sys.argv[1])
        ext_canremove_flags(sys.argv[1])
    elif len(sys.argv) == 3:
        arg_1, arg_2 = sys.argv[1:] # pylint: disable=unbalanced-tuple-unpacking
        ext_size_str(arg_1, arg_2)
    else:
        logger.error('You must provide all the arguments!')
        logger.info(dir_size_str.__doc__)
        logger.info(dependency_tree.__doc__)
        logger.info(ext_size_str.__doc__)
