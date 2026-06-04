# Copyright (C) 2013-2026  CEA, EDF, OPEN CASCADE
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

""" This file must be compatible with python 3.6 as it is used by the salome boostrap."""

import configparser
import logging
import os
import re
import sys
from collections import OrderedDict, namedtuple

from salomeContextUtils import SalomeContextException # type: ignore

if sys.version_info[:2] >= (3, 2):
    from configparser import ConfigParser
else:
    from configparser import SafeConfigParser as ConfigParser

logging.basicConfig()
logConfigParser = logging.getLogger(__name__)

ADD_TO_PREFIX = "ADD_TO_"
UNSET_KEYWORD = "UNSET"
# variables defined in this section are set only if not already defined
DEFAULT_VARS_SECTION_NAME = "SALOME DEFAULT VALUES"


def _trimColons(var):
    v = var
    # Remove leading and trailing colons (:)
    pattern = re.compile(r"^:+ | :+$", re.VERBOSE)
    v = pattern.sub(r"", v)  # remove matching patterns
    # Remove multiple colons
    pattern = re.compile("::+", re.VERBOSE)
    v = pattern.sub(r":", v)  # remove matching patterns
    return v


def _expandSystemVariables(key, val):
    if not isinstance(val, str):
        return val
    splittedComments = val.split("#")
    expandedVal = os.path.expandvars(
        splittedComments[0]
    )  # expand environment variables
    # Search for not expanded variables (i.e. non-existing environment variables)
    pattern = re.compile(r"${ ( [^}]* ) }", re.VERBOSE)  # string enclosed in ${ and }
    expandedVal = pattern.sub(r"", expandedVal)  # remove matching patterns

    if "DLIM8VAR" not in key:
        # special case: DISTENE licence key can contain double clons (::)
        expandedVal = _trimColons(expandedVal)

    expandedVal = expandedVal.strip().strip("'").strip('"')
    if expandedVal == '""':
        expandedVal = ""
    return expandedVal


def __processConfigFile(config, reserved=None, filename="UNKNOWN FILENAME"):
    # :TODO: may detect duplicated variables in the same section (raise a warning)
    #        or even duplicate sections

    if reserved is None:
        reserved = []
    unsetVariables = []
    outputVariables = []
    defaultValues = []
    # Get raw items for each section, and make some processing for environment variables management
    reservedKeys = [
        ADD_TO_PREFIX + str(x) for x in reserved
    ]  # produce [ 'ADD_TO_reserved_1', 'ADD_TO_reserved_2', ..., ADD_TO_reserved_n ]
    reservedValues = dict(
        [str(i), []] for i in reserved
    )  # create a dictionary in which keys are the 'ADD_TO_reserved_i' and associated values are empty lists: { 'reserved_1':[], 'reserved_2':[], ..., reserved_n:[] }

    sections = config.sections()
    for section in sections:
        entries = config.items(section, raw=False)  # use interpolation
        if len(entries) == 0:  # empty section
            logConfigParser.warning(
                "Empty section: %s in file: %s" % (section, filename)
            )
            pass
        for key, val in entries:
            if key in reserved:
                logConfigParser.error(
                    "Invalid use of reserved variable: %s in file: %s" % (key, filename)
                )
            elif key == UNSET_KEYWORD:
                unsetVariables += val.replace(",", " ").split()
            else:
                if key in reservedKeys:
                    shortKey = key[len(ADD_TO_PREFIX) :]
                    vals = re.split("[,\n]", val)
                    reservedValues[shortKey] += vals
                    # remove left&right spaces on each element
                    vals = [v.strip(" \t\n\r") for v in vals]
                else:
                    if DEFAULT_VARS_SECTION_NAME == section.upper():
                        defaultValues.append((key, val))
                    else:
                        outputVariables.append((key, val))
                    pass
                pass  # end if key
            pass  # end for key,val
        pass  # end for section

    # remove duplicate values
    outVars = []
    for var, values in outputVariables:
        vals = re.split("[,\n]", values)
        vals = list(set(vals))
        outVars.append((var, ",".join(vals)))

    ConfigInfo = namedtuple(
        "ConfigInfo",
        ["unsetVariables", "outputVariables", "reservedValues", "defaultValues"],
    )

    return ConfigInfo(unsetVariables, outVars, reservedValues, defaultValues)


class MultiOrderedDict(OrderedDict):
    def __setitem__(self, key, value):
        if isinstance(value, list):
            value = [_expandSystemVariables(key, v) for v in value]
        else:
            value = _expandSystemVariables(key, value)
        if isinstance(value, list) and key in self:
            self[key].extend(value)
        else:
            super().__setitem__(key, value)


class MultiOptSafeConfigParser(ConfigParser):
    def __init__(self):
        super().__init__(strict=False, dict_type=MultiOrderedDict)

    def optionxform(self, optionstr):
        return optionstr


# Parse configuration file
# Input: filename, and a list of reserved keywords (environment variables)
# Output: a list of pairs (variable, value), and a dictionary associating a list of user-defined values to each reserved keywords
# Note: Does not support duplicate keys in a same section
def parseConfigFile(filename, reserved=None):
    config = MultiOptSafeConfigParser()
    # Read config file
    try:
        config.read(filename)
    except configparser.MissingSectionHeaderError:
        logConfigParser.error("No section found in file: %s" % (filename))
        return []

    try:
        return __processConfigFile(config, reserved, filename)
    except configparser.InterpolationMissingOptionError as e:
        msg = f"A variable may be undefined in SALOME context file: {filename}\nParser error is: {e}\n"
        raise SalomeContextException(msg)
