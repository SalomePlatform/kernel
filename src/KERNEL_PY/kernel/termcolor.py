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
#  Author : Renaud Barate (EDF R&D)
#  Date   : August 2009
#
"""
This module provides utility functions to display colored text in the
terminal. It is based on ISO 6429 standard that defines control codes to
change characters representation in color-capable ASCII terminals.

In this module, colors are represented as lists of codes, so they can be added
to obtain special effects (e.g. RED + GREEN_BG to display red text on green
background). Several constants are defined for the most usual codes to
facilitate the use of colors, but it is also possible to define colors
directly from the corresponding code from ISO 6429 standard. In fact it is
even necessary for less usual codes that don't have an associated constant
(e.g. PURPLE + ['09'] can be used to display a crossed-out purple text).

Example::

    import sys
    from pal import termcolor
    if termcolor.canDisplayColor(sys.stdout):
        print termcolor.makeColoredMessage("Hello world!", termcolor.BLUE)
    else:
        print "Hello world!"

"""

# Constants for color codes
DEFAULT    = ['00']
"""Default color for the terminal"""
BOLD       = ['01']
"""Bold text and brighter colors"""
UNDERLINED = ['04']
"""Underlined text"""
BLACK_FG   = ['30']
"""Black foreground"""
RED_FG     = ['31']
"""Red foreground"""
GREEN_FG   = ['32']
"""Green foreground"""
YELLOW_FG  = ['33']
"""Yellow foreground"""
BLUE_FG    = ['34']
"""Blue foreground"""
PURPLE_FG  = ['35']
"""Purple foreground"""
CYAN_FG    = ['36']
"""Cyan foreground"""
WHITE_FG   = ['37']
"""White foreground"""
BLACK_BG   = ['40']
"""Black background"""
RED_BG     = ['41']
"""Red background"""
GREEN_BG   = ['42']
"""Green background"""
YELLOW_BG  = ['43']
"""Yellow background"""
BLUE_BG    = ['44']
"""Blue background"""
PURPLE_BG  = ['45']
"""Purple background"""
CYAN_BG    = ['46']
"""Cyan background"""
WHITE_BG   = ['47']
"""White background"""

# Constants for common colored text
BLACK      = BLACK_FG
"""Black text (equivalent to BLACK_FG)"""
RED        = BOLD + RED_FG
"""Red text (equivalent to BOLD + RED_FG)"""
GREEN      = BOLD + GREEN_FG
"""Green text (equivalent to BOLD + GREEN_FG)"""
YELLOW     = BOLD + YELLOW_FG
"""Yellow text (equivalent to BOLD + YELLOW_FG)"""
BLUE       = BOLD + BLUE_FG
"""Blue text (equivalent to BOLD + BLUE_FG)"""
PURPLE     = BOLD + PURPLE_FG
"""Purple text (equivalent to BOLD + PURPLE_FG)"""
CYAN       = BOLD + CYAN_FG
"""Cyan text (equivalent to BOLD + CYAN_FG)"""
WHITE      = WHITE_FG
"""White text (equivalent to WHITE_FG)"""


def canDisplayColor(stream):
    """
    Return True if the stream can display colored text, False otherwise.
    """
    return hasattr(stream, "isatty") and stream.isatty()

def getControlSequence(color):
    """
    Return the control sequence for the color in parameter, i.e. the string
    telling the terminal to display the following text in the given color.
    """
    seq = "\x1b["
    for i in range(len(color)):
        seq += color[i]
        if i < len(color)-1:
            seq += ";"
    seq += "m"
    return seq

def makeColoredMessage(message, color):
    """
    Return a string that can be used to display the message in parameter with
    the given color.
    """
    return (getControlSequence(color) +
            str(message) +
            getControlSequence(DEFAULT))

def TEST_termcolor():
    """Test function for termcolor module."""
    import sys
    if not canDisplayColor(sys.stdout):
        print "Standard output does not support colors."
        return
    print makeColoredMessage("This message must appear in blue.", BLUE)
    print makeColoredMessage("This message must appear in red on green " +
                             "background.", RED + GREEN_BG)
    print makeColoredMessage("This message must appear in magenta and " +
                             "crossed-out.", PURPLE + ['09'])


# Main function only used to test the module
if __name__ == "__main__":
    TEST_termcolor()
