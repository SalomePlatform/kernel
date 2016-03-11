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

#=============================================================================
#  Author    : Guillaume Boulant (CSSI)
#  Rewritten by Renaud Barate (EDF R&D)
#  Project   : SALOME
#  Copyright : EDF 2001-2009
#  $Header$
#=============================================================================

## \defgroup logger logger
#  \{ 
#  \details
#  This module defines a class which provides logging facility in Salome.
#  \}

"""
This module defines a class which provides logging facility in Salome:
"""

import sys, os
import logging

from salome.kernel.deprecation import deprecated
from salome.kernel import termcolor
import salome.kernel.logconfig

## This class formats and displays log messages in Salome environment. It
#  inherits \b logging.Logger class defined in \b logging module from Python 
#  library, so all methods from \b logging.Logger can be used here. 
#  The format of the traces is:
#  LEVEL[keyword] : Message  
#  
#  ,where \em LEVEL is the level of the message (\em DEBUG, \em INFO, etc.),
#  \em keyword is the name of the logger, and \em Message is the message to log.
#    
#  When creating a new Logger object, the parameter \em keyword defines the
#  name of the logger, \em level defines the logging level (default is
#  \b logging.DEBUG if KERNEL module is configured with --enable-debug option 
#  or \b logging.WARNING otherwise), and \em color defines the color
#  of the log messages for this logger (log messages will appear in color
#  only when displayed on color - capable ASCII terminals). See module
#  \ref termcolor "salome.kernel.termcolor" for the color constants.
#    
#  By default, log messages will be displayed only on standard output. They
#  can also be recorded in a file (see method setLogFile()). For now,
#  the CORBA-based logging facility can not be used through this class.
#
#  A source filename \em sourceFileName can be defined. If this argument is
#  specified, then the \em keyword is modified to the basename of the 
#  \em sourceFileName
#    
#  Basic usage::
#  \code    
#  from salome.kernel.logger import Logger
#  log = Logger("Test")
#  log.debug("Debug message")
#  log.info("Information message")
#  log.warning("Warning message")
#  log.error("Error message")
#  log.critical("Fatal error message")
#  \endcode
#  \ingroup logger
class Logger(logging.Logger):
    """
    This class formats and displays log messages in Salome environment. It
    inherits :class:`Logger<logging.Logger>` class defined in :mod:`logging`
    module from Python library, so all methods from :class:`logging.Logger`
    can be used here. The format of the traces is:
    
    LEVEL    [keyword] : Message
    
    where `LEVEL` is the level of the message (`DEBUG`, `INFO`, etc.),
    `keyword` is the name of the logger, and `Message` is the message to log.
    
    When creating a new Logger object, the parameter `keyword` defines the
    name of the logger, `level` defines the logging level (default is
    :const:`logging.DEBUG` if KERNEL module is configured with --enable-debug
    option or :const:`logging.WARNING` otherwise), and `color` defines the color
    of the log messages for this logger (log messages will appear in color
    only when displayed on color-capable ASCII terminals). See module
    :mod:`salome.kernel.termcolor` for the color constants.
    
    By default, log messages will be displayed only on standard output. They
    can also be recorded in a file (see method :meth:`setLogFile`). For now,
    the CORBA-based logging facility can not be used through this class.

    A source filename `sourceFileName` can be defined. If this argument is
    specified, then the `keyword` is modified to the basename of the `sourceFileName`
    
    Basic usage::
    
        from salome.kernel.logger import Logger
        log = Logger("Test")
        log.debug("Debug message")
        log.info("Information message")
        log.warning("Warning message")
        log.error("Error message")
        log.critical("Fatal error message")

    """

    def __init__(self, keyword = "KEY", level = salome.kernel.logconfig.loggingLevel,
                 color = None, sourceFileName=None):

        if sourceFileName is not None:
            keyword = os.path.basename(sourceFileName).split('.')[0]
        logging.Logger.__init__(self, keyword, level)
        self._baseFormatString = "%(levelname)-8s [%(name)s] : %(message)s"
        self._baseFormatter = logging.Formatter(self._baseFormatString)
        if hasattr(sys.stdout, "flush"):
            self._stdoutStream = sys.stdout
        else:
            self._stdoutStream = _UnFlushableLogStream(sys.stdout)
        self._stdoutHandler = logging.StreamHandler(self._stdoutStream)
        self._stdoutHandler.setLevel(logging.DEBUG)
        self.setColor(color)
        self.addHandler(self._stdoutHandler)
        self._fileHandler = None

    ## Log all messages, including DEBUG level messages (equivalent to
    #  setLevel(logging.DEBUG)).
    def showDebug(self):
        """
        Log all messages, including DEBUG level messages (equivalent to
        ``setLevel(logging.DEBUG)``).
        """
        self.setLevel(logging.DEBUG)

    ## Define a log file to record the log messages (in addition to the
    #  standard output).
    def setLogFile(self, logFilename):
        """
        Define a log file to record the log messages (in addition to the
        standard output).
        """
        self.closeLogFile()
        self._fileHandler = logging.FileHandler(logFilename, 'w')
        self._fileHandler.setLevel(logging.DEBUG)
        self._fileHandler.setFormatter(self._baseFormatter)
        self.addHandler(self._fileHandler)

    ## Set the color of log messages on color-capable terminals. If \em color
    #  is \b None, the default color will be used.
    def setColor(self, color):
        """
        Set the color of log messages on color-capable terminals. If `color`
        is :const:`None`, the default color will be used.
        """
        if color is None or not termcolor.canDisplayColor(self._stdoutStream):
            stdoutFormatter = self._baseFormatter
        else:
            format = ("%s%s%s" %
                      (termcolor.getControlSequence(color),
                       self._baseFormatString,
                       termcolor.getControlSequence(termcolor.DEFAULT)))
            stdoutFormatter = logging.Formatter(format)
        self._stdoutHandler.setFormatter(stdoutFormatter)

    ## Close the log file.
    def closeLogFile(self):
        """Close the log file."""
        if self._fileHandler is not None:
            self.removeHandler(self._fileHandler)
            self._fileHandler.close()
            self._fileHandler = None

    ## Hide DEBUG level messages (equivalent to setLevel(logging.INFO)).
    def hideDebug(self):
        """
        Hide DEBUG level messages (equivalent to ``setLevel(logging.INFO)``).
        """
        self.setLevel(logging.INFO)

    @deprecated("Deprecated since version 5.1.5. Please replace with "
                "Logger.critical(message)")
    
    ## Log a message with CRITICAL level. This method only exists for
    #  backward compatibility and is equivalent to \b critical(message).
    def fatal(self, message):
        """
        Log a message with CRITICAL level. This method only exists for
        backward compatibility and is equivalent to ``critical(message)``.
        """
        self.critical(message)

## This utility class allows to log messages to a stream with no \b flush
#  method. This is useful to send log messages to \b PyOut objects.
#  \ingroup logger
class _UnFlushableLogStream:
    """
    This utility class allows to log messages to a stream with no `flush`
    method. This is useful to send log messages to `PyOut` objects.
    """

    def __init__(self, stream):
        self._stream = stream

    def write(self, msg):
        self._stream.write(msg)

    def flush(self):
        pass

## This class extends Logger class and adds exception information
#  when DEBUG messages are recorded. It exists mainly for backward
#  compatibility, as the same thing can be done by calling
#  <em> Logger.debug(message, exc_info = True) </em>.
#  \ingroup logger
class ExtLogger(Logger):
    """
    This class extends :class:`Logger` class and adds exception information
    when DEBUG messages are recorded. It exists mainly for backward
    compatibility, as the same thing can be done by calling
    ``Logger.debug(message, exc_info = True)``.
    """
    
    @deprecated("Class ExtLogger is deprecated since version 5.1.5. See "
                "documentation for replacement.")
    def __init__(self, keyword = "KEY",
                 level = salome.kernel.logconfig.loggingLevel,
                 color = None, sourceFileName=None):
        Logger.__init__(self, keyword, level, color, sourceFileName)

    ## Log a DEBUG message with exception information (equivalent to
    #  <em> Logger.debug(message, exc_info = True) </em>).
    def debug( self, message ):
        """
        Log a DEBUG message with exception information (equivalent to
        ``Logger.debug(message, exc_info = True)``).
        """
        Logger.debug(self, message, exc_info = True)

## Test function for logger module
#  \ingroup logger
def TEST_Logger():
    """Test function for logger module"""
    log = Logger("TST")

    # Base methods
    log.info("Information message")
    log.debug("Debug message")
    log.fatal("Fatal error message")

    # Message building
    data = 12
    log.info("This message displays data = " + str(data))

    data = {}
    data["KERNEL"] = "V1"
    data["GEOM"] = "V2"
    log.info("This message displays data = " + str(data))

    # Test with a non-string parameter
    log.info(data)

    # Test with a default instance
    log = Logger()
    log.info("Default logger")

    # Test showDebug method
    log.setLogFile("test.log")
    log.debug("Debug trace")
    log.hideDebug()
    log.debug("This trace should NOT be displayed")
    log.showDebug()
    log.debug("This trace should be displayed")
    log.closeLogFile()
    log.info("After closing the log file")


# Main function only used to test the module
if __name__ == "__main__":
    TEST_Logger()
