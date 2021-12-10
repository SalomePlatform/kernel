#!/usr/bin/env python3
# -*- coding: utf-8 *-

##############################################################
# This script reduce the pythonpath of salome environement.  #
# The single input of this script is install path of salome. #                                  #
##############################################################

import argparse
import glob
import logging
import os
import shutil
import sys

from parseConfigFile import parseConfigFile

logging.basicConfig()
logger = logging.getLogger(os.path.basename(__file__))
logger.setLevel(logging.INFO)

IGNORE = ['__pycache__', '__init__.py', '.yamm', 'NEWS', 'README']


def remove(path):
    logger.debug('Removing %r' % (path))
    if os.path.islink(path):
        os.unlink(path)
    elif os.path.isdir(path):
        shutil.rmtree(path)
    else:
        os.remove(path)


def copy(src, dst):
    logger.debug('Copy %r to %r' % (src, dst))
    if os.path.exists(dst):
        remove(dst)
    if not os.path.isdir(src):
        shutil.copyfile(src, dst)
    else:
        shutil.copytree(src, dst)


def copy_or_link(src, dst):
    if sys.platform in ('linux', 'linux2'):
        if os.path.exists(dst):
            current_dst = os.readlink(dst)
            logger.warning('Destination link %r already exists and links to %r.' % (dst, current_dst))
            logger.warning('It is overwritten to %r' % (src))
            remove(dst)
        os.symlink(src, dst)
    else:
        copy(src, dst)


if sys.version_info[0] < 3:
    raise Exception("Must be using Python 3")


def main(salome_install_dir, context_file_name, env_file_name, ignore=None):
    if ignore:
        ignore = IGNORE + ignore
    else:
        ignore = IGNORE[::]

    # new pythonpath initiation; creation a directory containing all python module for salome
    salome_install_dir=os.path.abspath(salome_install_dir)
    pythonpath_common = os.path.join(salome_install_dir, 'python_modules')
    if os.path.exists(pythonpath_common):
        remove(pythonpath_common)
    os.mkdir(pythonpath_common)
    new_pythonpath_list = []
    new_pythonpath_list.append(pythonpath_common)

    # parsing configuration file (cfg) to get old pythonpath
    reserved_key = ['PYTHONPATH']
    context_file = os.path.join(salome_install_dir, context_file_name)
    context_file_sh = os.path.join(salome_install_dir, env_file_name)
    backup_context_file = context_file + '.backup'
    backup_context_file_sh = context_file_sh + '.backup'
    if os.path.exists(backup_context_file):
        logger.info("Recover context file %r from backup" % (context_file))
        copy(backup_context_file, context_file)
    if os.path.exists(backup_context_file_sh):
        logger.info("Recover env file %r from backup" % (context_file_sh))
        copy(backup_context_file_sh, context_file_sh)

    if not os.path.exists(context_file) or not os.path.exists(context_file_sh):
        raise Exception("Context and env files are not found in the salome install. "
                        "Verify if %s and %s are present in the salome install path: %s"
                        % (context_file_name, env_file_name, salome_install_dir))
    configInfo = parseConfigFile(context_file, reserved_key)
    configVars = configInfo.outputVariables
    reservedDict = configInfo.reservedValues
    configVarsDict = {k: v for (k, v) in configVars}

    # Synchronize new pythonpath with the old
    # Copy all the .egg files to the new directory of python module and list them in new single easy-install.pth
    # If the module hasn't .egg file, we must copy all files from its pythonpath and paste them in the python module directory
    # It's complicated to reduce pythonpath of paraview so for now we keep the old paraview pythonpath
    pattern = "*.egg"
    with open(os.path.join(pythonpath_common, 'easy-install.pth'), 'w') as easy_install:
        for dirs in reservedDict[reserved_key[0]]:
            for d in dirs.split(':'):
                egg_dir_list = glob.glob(os.path.join(d, pattern))
                if egg_dir_list:
                    for egg_dir in egg_dir_list:
                        egg_file = egg_dir.split('/')[-1]
                        new_dir = os.path.join(pythonpath_common, egg_file)
                        copy_or_link(egg_dir, new_dir)
                        easy_install.write("./%s\n" % egg_file)
                else:
                    for f in os.listdir(d):
                        if f in ignore:
                            continue
                        full_file_srcpath = os.path.join(d, f)
                        full_file_dstpath = os.path.join(pythonpath_common, f)
                        copy_or_link(full_file_srcpath, full_file_dstpath)
    # In the case of matplotlib, some prerequis are found matplotlib_root. They also need to be put in new directory of module python
    try:
        with open(os.path.join(pythonpath_common, 'easy-install.pth'), 'a') as easy_install:
            MATPLOTLIB_ROOT = configVarsDict['MATPLOTLIB_HOME']
            for d in os.listdir(MATPLOTLIB_ROOT):
                if ".egg" in d:
                    copy_or_link(os.path.join(MATPLOTLIB_ROOT, d), os.path.join(pythonpath_common, d))
                    easy_install.write("./%s\n" % d)
    except KeyError:
        logger.debug("Not found MATPLOTLIB_HOME variable in the .cfg")
        logger.debug("Matplotlib isn't compiled with salome or maybe it is in the system package ")
        logger.debug("If matplotlib is compiled with salome, we need define its install path with variable MATPLOTLIB_HOME")

    # Fix for correct imports
    import setuptools
    try:
        setup_tools_path = os.path.dirname(setuptools.__file__)
    except AttributeError:
        setup_tools_path = os.path.dirname(setuptools.__path__._path[0])
    del setuptools
    site_patch = os.path.join(setup_tools_path, 'site-patch.py')
    shutil.copyfile(site_patch, os.path.join(pythonpath_common, "site.py"))

    # Overwrite salome_context.cfg
    # Backup context and env files
    copy(context_file, backup_context_file)
    copy(context_file_sh, backup_context_file_sh)
    with open(context_file, 'r') as f:
        lines = f.readlines()
    with open(context_file, 'w') as f:
        for line in lines:
            if "ADD_TO_PYTHONPATH" not in line:
                f.write(line)
    with open(context_file, 'a') as f:
        f.write("#------ NEWPYTHONPATH ------\n")
        for p in new_pythonpath_list:
            f.write("ADD_TO_PYTHONPATH: %s\n" % p)

    # Overwrite salome_prerequisites.sh
    with open(context_file_sh, 'r') as f:
        lines = f.readlines()
    with open(context_file_sh, 'w') as f:
        for line in lines:
            if "PYTHONPATH" not in line:
                f.write(line)
    with open(context_file_sh, 'a') as f:
        f.write("#------ NEWPYTHONPATH ------\n")
        for p in new_pythonpath_list:
            f.write("export PYTHONPATH=%s:${PYTHONPATH}\n" % p)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', '--verbose', action='store_true', help='Verbose mode')
    parser.add_argument('-c', '--context-file', default='salome_context.cfg',
                        help='Context file name (default: %(default)s)')
    parser.add_argument('-e', '--env-file', default='salome_prerequisites.sh',
                        help='Env file name (default: %(default)s)')
    parser.add_argument('-i', '--ignore', nargs='*',
                        help='List of comma separated files to ignore')
    parser.add_argument(dest='salome_install_dir', help='Directory of context and env files')
    args = parser.parse_args()
    if args.verbose:
        logger.setLevel(logging.DEBUG)
    main(args.salome_install_dir, args.context_file, args.env_file, args.ignore)
