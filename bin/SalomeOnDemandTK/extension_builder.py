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

#  File   : extension_builder.py
#  Author : Konstantin Leontev, Open Cascade
#
#  @package SalomeOnDemandTK
#  @brief Set of utility functions those help to build SALOME python extensions.

"""Build salome extension archive in tar.gz format.
"""

import tarfile
import os
import sys
import io
from traceback import format_exc

from .extension_utilities import logger, \
    BFILE_EXT, DFILE_EXT, PYFILE_EXT, EXTNAME_KEY, ARCFILE_EXT, SALOME_EXTDIR, CFILE_EXT, \
    isvalid_filename, isvalid_dirname, read_salomexd, read_salomexb, list_files_filter, \
    list_tonewline_str


def add_files(ext, files_abs, files_rel):
    """
    Add selected files into salome extension archive.

    Args:
        ext - a file oject to pack in.
        files_abs - a list of the files with abs file names.
        files_rel - a list of the files with rel file names.

    Returns:
        None.
    """

    logger.debug('Add selected files into archive %s directory...', SALOME_EXTDIR)

    # Set progress bar, because it can get some time for large archives
    progress_count = 0
    total_count = len(files_abs)
    default_terminator = logger.handlers[0].terminator
    logger.handlers[0].terminator = ''
    for (file_abs, file_rel) in zip(files_abs, files_rel):
        ext.add(file_abs, os.path.normpath(SALOME_EXTDIR + '/' + file_rel))

        # Progress bar's length is 100 symbols.
        progress_count += 1
        logger.debug('\r|%-100s|', '=' * int(100 * progress_count/(total_count - 1)))

    # Reset terminator to default value, otherwise all the followed logs will be in one line
    logger.debug('\n')
    logger.handlers[0].terminator = default_terminator


def create_salomex(salomexb, salomexd, env_py, top_repository):
    """
    Makes salome extension archive from provided in salomexb file directories.

    Args:
        salomexb - a path to the <extension>.salomexb file.
        salomexd - a path to the <extension>.salomexd file.
        env_py - a path to the <ext>_env.py file.
        top_repository - a root directory for all the paths listed inside salomexb file.

    Returns:
        None.
    """

    logger.debug('Starting create an salome extension file')

    # Check if provided filenames are valid
    if  not isvalid_filename(salomexb, BFILE_EXT) or \
        not isvalid_filename(salomexd, DFILE_EXT) or \
        not isvalid_filename(env_py, PYFILE_EXT) or \
        not isvalid_dirname(top_repository):
        return

    # Try to get info from salomexd file
    salome_ext_name = ''
    salomexd_content = read_salomexd(salomexd)
    if EXTNAME_KEY in salomexd_content and salomexd_content[EXTNAME_KEY]:
        salome_ext_name = salomexd_content[EXTNAME_KEY]
    else:
        # Now as a last resort we get a name right from salomexd filename
        # We need to decide if we can handle this case handsomely
        salome_ext_name = os.path.basename(salomexd)
        logger.warning('Cannot get a SALOME extension name from salomexd file! \
            Use salomexd file name as an extension name.')

    logger.debug('Set an extension name as: %s', salome_ext_name)

    try:
        with tarfile.open(salome_ext_name + '.' + ARCFILE_EXT, "w:gz") as ext:
            # Write all included files to the extension's dir

            # Get the file's matching pattern in the first place
            included_files_patterns = read_salomexb(salomexb)
            logger.debug('Included files pattern: %s', included_files_patterns)
            if not included_files_patterns:
                # We don't have any pattern, so we don't know what we must put inside an archive
                logger.error('Cannot create salomex file: \
                    a list of included files patterns is empty.')
                return

            # List of the files those actually written to the archive.
            # It goes to the salomexc file then.
            files_abs, files_rel = list_files_filter(top_repository, included_files_patterns)
            id = 0
            for f in files_rel:
                fsplit = f.split('/')
                del fsplit[0]
                files_rel[id] = '/'.join(fsplit)
                id +=1
            add_files(ext, files_abs, files_rel)

            # Write the control file - list of the files inside extension's dir
            logger.debug('Write the %s control file into archive root...', CFILE_EXT)
            included_files_data = list_tonewline_str(files_rel).encode('utf8')
            info = tarfile.TarInfo(salome_ext_name + '.' + CFILE_EXT)
            info.size = len(included_files_data)
            ext.addfile(info, io.BytesIO(included_files_data))

            # Write the description file as is
            logger.debug('Copy the %s file into archive root...', salomexd)
            ext.add(salomexd, os.path.basename(salomexd))

            # Write the env_py file as is
            logger.debug('Copy the %s file into archive root...', env_py)
            ext.add(env_py, os.path.basename(env_py))

            logger.debug('SALOME extension %s was created.', salome_ext_name)

    except OSError:
        logger.error(format_exc())


if __name__ == '__main__':
    if len(sys.argv) == 5:
        arg_1, arg_2, arg_3, arg_4 = sys.argv[1:5]
        create_salomex(arg_1, arg_2, arg_3, arg_4)
    else:
        logger.error('You must provide all the arguments!')
        logger.info(create_salomex.__doc__)
