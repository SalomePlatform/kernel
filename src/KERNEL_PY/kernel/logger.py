# -*- coding: utf-8 -*-
#
#  Copyright (C) 2007-2009	 EDF R&D
# 
#    This file is part of PAL_SRC.
#
#    PAL_SRC is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    PAL_SRC is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with PAL_SRC; if not, write to the Free Software
#    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
#

#=============================================================================
#  Author    : Guillaume Boulant (CSSI)
#  Rewritten by Renaud Barate (EDF R&D)
#  Project   : SALOME
#  Copyright : EDF 2001-2009
#  $Header$
#=============================================================================
"""
This module defines a class which provides logging facility in Salome:
"""

import sys, os
import logging

from salome.kernel import termcolor
import salome.kernel.logconfig

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
    :const:`logging.WARNING` if PAL module is configured with --disable-debug
    option or :const:`logging.DEBUG` otherwise), and `color` defines the color
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

    def showDebug(self):
        """
        Log all messages, including DEBUG level messages (equivalent to
        ``setLevel(logging.DEBUG)``).
        """
        self.setLevel(logging.DEBUG)

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

    def closeLogFile(self):
        """Close the log file."""
        if self._fileHandler is not None:
            self.removeHandler(self._fileHandler)
            self._fileHandler.close()
            self._fileHandler = None

    def hideDebug(self):
        """
        Hide DEBUG level messages (equivalent to ``setLevel(logging.INFO)``).
        """
        self.setLevel(logging.INFO)

    def fatal(self, message):
        """
        Log a message with CRITICAL level. This method only exists for
        backward compatibility and is equivalent to ``critical(message)``.
        """
        self.critical(message)


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


class ExtLogger(Logger):
    """
    This class extends :class:`Logger` class and adds exception information
    when DEBUG messages are recorded. It exists mainly for backward
    compatibility, as the same thing can be done by calling
    ``Logger.debug(message, exc_info = True)``.
    """

    def debug( self, message ):
        """
        Log a DEBUG message with exception information (equivalent to
        ``Logger.debug(message, exc_info = True)``).
        """
        Logger.debug(self, message, exc_info = True)


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
