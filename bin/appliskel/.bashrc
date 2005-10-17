#!/bin/bash

# If not running interactively, don't do anything:
[ -z "$PS1" ] && return

# keep original settings (debian)
. ~/.bashrc

# prefix prompt with naming server hostname and port
export PS1='[NS='${NSHOST}':'${NSPORT}']'${PS1}
