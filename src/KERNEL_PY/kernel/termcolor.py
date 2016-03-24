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
#  Author : Renaud Barate (EDF R&D)
#  Date   : August 2009
#

## \defgroup termcolor termcolor
#  \{ 
#  \details
#  This module provides utility functions to display colored text in the
#  terminal. It is based on ISO 6429 standard that defines control codes to
#  change characters representation in color-capable ASCII terminals.
#
#  In this module, colors are represented as lists of codes, so they can be added
#  to obtain special effects (e.g. RED + GREEN_BG to display red text on green
#  background). Several constants are defined for the most usual codes to
#  facilitate the use of colors, but it is also possible to define colors
#  directly from the corresponding code from ISO 6429 standard. In fact it is
#  even necessary for less usual codes that don't have an associated constant
#  (e.g. PURPLE + ['09'] can be used to display a crossed-out purple text).
#
#  \code
#  import sys
#  from salome.kernel import termcolor
#  if termcolor.canDisplayColor(sys.stdout):
#      print termcolor.makeColoredMessage("Hello world!", termcolor.BLUE)
#  else:
#      print "Hello world!"
#  \endcode
#  \}

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
    from salome.kernel import termcolor
    if termcolor.canDisplayColor(sys.stdout):
        print termcolor.makeColoredMessage("Hello world!", termcolor.BLUE)
    else:
        print "Hello world!"

"""

# Constants for color codes
## Default color for the terminal
#  \ingroup termcolor
DEFAULT    = ['00']
"""Default color for the terminal"""
## Bold text and brighter colors
#  \ingroup termcolor
BOLD       = ['01']
"""Bold text and brighter colors"""
## Underlined text
#  \ingroup termcolor
UNDERLINED = ['04']
"""Underlined text"""
## Black foreground
#  \ingroup termcolor
BLACK_FG   = ['30']
"""Black foreground"""
## Red foreground
#  \ingroup termcolor
RED_FG     = ['31']
"""Red foreground"""
## Green foreground
#  \ingroup termcolor
GREEN_FG   = ['32']
"""Green foreground"""
## Yellow foreground
#  \ingroup termcolor
YELLOW_FG  = ['33']
"""Yellow foreground"""
## Blue foreground
#  \ingroup termcolor
BLUE_FG    = ['34']
"""Blue foreground"""
## Purple foreground
#  \ingroup termcolor
PURPLE_FG  = ['35']
"""Purple foreground"""
## Cyan foreground
#  \ingroup termcolor
CYAN_FG    = ['36']
"""Cyan foreground"""
## White foreground
#  \ingroup termcolor
WHITE_FG   = ['37']
"""White foreground"""
## Black background
#  \ingroup termcolor
BLACK_BG   = ['40']
"""Black background"""
## Red background
#  \ingroup termcolor
RED_BG     = ['41']
"""Red background"""
## Green background
#  \ingroup termcolor
GREEN_BG   = ['42']
"""Green background"""
## Yellow background
#  \ingroup termcolor
YELLOW_BG  = ['43']
"""Yellow background"""
## Blue background
#  \ingroup termcolor
BLUE_BG    = ['44']
"""Blue background"""
## Purple background
#  \ingroup termcolor
PURPLE_BG  = ['45']
"""Purple background"""
## Cyan background
#  \ingroup termcolor
CYAN_BG    = ['46']
"""Cyan background"""
## White background
#  \ingroup termcolor
WHITE_BG   = ['47']
"""White background"""

# Constants for common colored text
## Black text (equivalent to BLACK_FG)
#  \ingroup termcolor
BLACK      = BLACK_FG
"""Black text (equivalent to BLACK_FG)"""
## Red text (equivalent to BOLD + RED_FG)
#  \ingroup termcolor
RED        = BOLD + RED_FG
"""Red text (equivalent to BOLD + RED_FG)"""
## Green text (equivalent to BOLD + GREEN_FG)
#  \ingroup termcolor
GREEN      = BOLD + GREEN_FG
"""Green text (equivalent to BOLD + GREEN_FG)"""
## Yellow text (equivalent to BOLD + YELLOW_FG)
#  \ingroup termcolor
YELLOW     = BOLD + YELLOW_FG
"""Yellow text (equivalent to BOLD + YELLOW_FG)"""
## Blue text (equivalent to BOLD + BLUE_FG)
#  \ingroup termcolor
BLUE       = BOLD + BLUE_FG
"""Blue text (equivalent to BOLD + BLUE_FG)"""
## Purple text (equivalent to BOLD + PURPLE_FG)
#  \ingroup termcolor
PURPLE     = BOLD + PURPLE_FG
"""Purple text (equivalent to BOLD + PURPLE_FG)"""
## Cyan text (equivalent to BOLD + CYAN_FG)
#  \ingroup termcolor
CYAN       = BOLD + CYAN_FG
"""Cyan text (equivalent to BOLD + CYAN_FG)"""
## White text (equivalent to WHITE_FG)
#  \ingroup termcolor
WHITE      = WHITE_FG
"""White text (equivalent to WHITE_FG)"""

## Return True if the stream can display colored text, False otherwise.
#  \ingroup termcolor
def canDisplayColor(stream):
    """
    Return True if the stream can display colored text, False otherwise.
    """
    return hasattr(stream, "isatty") and stream.isatty()

## Return the control sequence for the color in parameter, i.e. the string
#  telling the terminal to display the following text in the given color.
#  \ingroup termcolor
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

## Return a string that can be used to display the message in parameter with
#  the given color.
#  \ingroup termcolor
def makeColoredMessage(message, color):
    """
    Return a string that can be used to display the message in parameter with
    the given color.
    """
    return (getControlSequence(color) +
            str(message) +
            getControlSequence(DEFAULT))

## Test function for termcolor module.
#  \ingroup termcolor
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
