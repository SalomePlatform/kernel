#!/bin/bash

# Copyright (C) 2013-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

# Bash completion support for salome command.
#
# How to use this file?
# 1. If you have a root access, copy the file to /etc/bash_completion.d/
# 2. If you do not have root access or want to make a user-speficic installation:
#    2.1. Copy this file to somewhere (e.g. ~/.salome-completion.sh).
#    2.2. Add the following line to your .bashrc:
#         source ~/.salome-completion.sh
#

_salome_long_options()
{
    case $1 in
        --show-desktop|--splash|--catch-exceptions|--shutdown-servers|--foreground)
            options='1 0'
            ;;
        --server-launch-mode)
            options='daemon fork'
            ;;
    esac
    COMPREPLY=( $( compgen -W "$options" -- "$2" ) )
}

# Function used to compute the completion
_salome()
{
    # COMP_WORDS is an array that contains all command line elements (the script and its arguments)
    # COMP_CWORD is the index of current element in command line
    # COMPREPLY is an array of strings that are the proposals as returned by auto-completion
    local cur prev command options
    COMPREPLY=( )
    _get_comp_words_by_ref -n = cur prev
    commands='start context shell connect kill killall test info doc help coffee car'

    # Algorithm:
    # If cursor is at index 1
    #    Complete with commands
    # Else
    #    Find current command
    #    If word under cursor starts with '-'
    #       Complete options regarding current command

    if [[ $COMP_CWORD -eq 1 ]] ; then
        COMPREPLY=( $( compgen -W "$commands" -- "$cur" ) )
    elif [[ "$cur" == --*=* ]]; then
        _split_longopt
        _salome_long_options "$prev" "$cur"
    elif [[ "$cur" == -* ]]; then
        command=${COMP_WORDS[1]}
        if [[ "$cur" == -* ]]; then
            case $command in
                start)
                    options='-t --terminal -g --gui -d --show-desktop= -o --hide-desktop -b --batch -l --logger -f --log-file= -r --resources= -x --xterm -m --modules= -e --embedded= -s --standalone= -p --portkill -k --killall -i --interp= -z --splash= -c --catch-exceptions= --print-port --nosave-config --pinter --ns-port-log= --test= --play= --gdb-session --ddd-session --valgrind-session -w --shutdown-servers= --foreground= --wake-up-session --server-launch-mode= --port= --version -h --help --with-mpi-module= --config='
                    ;;
                shell)
                    options='-h --help -p --port= -m --machine= -d --directory= -u --user= --with-mpi-module= --config='
                    ;;
                info)
                    options='-h --help -p --ports -s --softwares -v --version'
                    ;;
                connect)
                    options='-h --help -c -p'
                    ;;
                test)
                    options='-h --help --print-labels'
                    ;;
            esac
            COMPREPLY=( $( compgen -W "$options" -- "$cur" ) )
        fi
    fi

}

# The-F option gives the name of the function that calculates the completion; moreover it tells the complete command that it must seek the results in the COMPREPLY variable.
complete -o bashdefault -o default -o nospace -F _salome salome 2>/dev/null \
    || complete -o default -o nospace -F _salome salome

# The following are necessary only for Cygwin, and only are needed
# when the user has tab-completed the executable name and consequently
# included the '.exe' suffix.
#
if [ Cygwin = "$(uname -o 2>/dev/null)" ]; then
complete -o bashdefault -o default -o nospace -F _salome salome.exe 2>/dev/null \
    || complete -o default -o nospace -F _salome salome.exe
fi
