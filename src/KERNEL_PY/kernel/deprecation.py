# -*- coding: utf-8 -*-
#
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

## \defgroup deprecation deprecation
#  \{ 
#  \details
#  This module provides several functions to indicate the deprecation of a
#  module, a method or a function.
#  \code
#  from salome.kernel.deprecation import deprecated
#  deprecated("Deprecated since version 6.3.0. Consider replacement with 
#              newFunction()")
#  def oldFunction():
#  ...
#  \endcode
#  \}

"""
This module provides several functions to indicate the deprecation of a
module, a method or a function.

Example::

    from salome.kernel.deprecation import deprecated

    @deprecated("Deprecated since version 6.3.0. Consider replacement with "
                "newFunction()")
    def oldFunction():
      ...

"""

import sys
import warnings
import inspect
import os

from salome.kernel import termcolor

msg_seedoc = "See documentation for possible replacements."

def __deprecated_with_msg(func, msg):

    def new_func(*args, **kwargs):
        if len(inspect.stack()) > 1:
            callingfunc = inspect.stack()[1][3]
        else:
            callingfunc = "CORBA middleware"
        warnings.warn(
            ("Call to deprecated function %(funcname)s of module " +
             "%(modname)s (called from %(callingfunc)s).\n  %(msg)s") % {
                'funcname': func.__name__,
                'modname': func.__module__,
                'callingfunc': callingfunc,
                'msg': msg,
            },
            category = DeprecationWarning,
            stacklevel = 2
        )
        return func(*args, **kwargs)
    return new_func

## This is a decorator which can be used to mark functions
#  as deprecated. It will result in a warning being emitted
#  when the function is used. The message in parameter will
#  be displayed and should indicate how this function can be
#  replaced. If the terminal can display colors, the warning
#  messages will appear in blue.
#  \ingroup deprecation
def deprecated(msg = msg_seedoc):
    """
    This is a decorator which can be used to mark functions
    as deprecated. It will result in a warning being emitted
    when the function is used. The message in parameter will
    be displayed and should indicate how this function can be
    replaced. If the terminal can display colors, the warning
    messages will appear in blue.
    """
    def make_dep(f):
        if is_called_by_sphinx():
            return f
        else:
            g = __deprecated_with_msg(f, msg)
            g.__name__ = f.__name__
            g.__doc__ = f.__doc__
            g.__dict__.update(f.__dict__)
            return g
    return make_dep

## This function can be used to mark a module as deprecated.
#  It must be called explicitly at the beginning of the deprecated
#  module. It will result in a warning being emitted. The message
#  in parameter will be displayed and should indicate how this
#  module can be replaced. If the terminal can display colors,
#  the warning messages will appear in blue.
#  \ingroup deprecation
def deprecated_module(msg = msg_seedoc):
    """
    This function can be used to mark a module as deprecated.
    It must be called explicitly at the beginning of the deprecated
    module. It will result in a warning being emitted. The message
    in parameter will be displayed and should indicate how this
    module can be replaced. If the terminal can display colors,
    the warning messages will appear in blue.
    """
    if not is_called_by_sphinx():
        warnings.warn(
            "Importation of deprecated module %(modname)s.\n  %(msg)s" % {
                'modname': inspect.getmodulename(inspect.stack()[1][1]),
                'msg': msg,
            },
            category = DeprecationWarning,
            stacklevel = 5
        )

## Determine if the calling code is ultimately called by sphinx to generate
#  documentation. The result can be used to conditionally inhibit the
#  decorators or some Salome-related imports that fail when called outside
#  Salome.
#  \ingroup deprecation
def is_called_by_sphinx():
    """
    Determine if the calling code is ultimately called by sphinx to generate
    documentation. The result can be used to conditionally inhibit the
    decorators or some Salome-related imports that fail when called outside
    Salome.
    """
    calling_file = inspect.stack()[len(inspect.stack())-1][1]
    basename = os.path.basename(calling_file)
    return (basename == "sphinx-build")


def __show_colored_warning(message, category, filename,
                           lineno, file = None, line = None):
    if file is None:
        file = sys.stderr
    str = warnings.formatwarning(message, category, filename, lineno, line)
    if category == DeprecationWarning and termcolor.canDisplayColor(file):
        file.write(termcolor.makeColoredMessage(str, termcolor.BLUE))
    else:
        file.write(str)

# Enable warnings for deprecated functions and modules (in Python 2.7, they
# are disabled by default)
warnings.filterwarnings("always", "Call to *", DeprecationWarning)
warnings.filterwarnings("always", "Importation of *", DeprecationWarning)
warnings.showwarning = __show_colored_warning
