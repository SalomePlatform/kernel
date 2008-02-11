#!/usr/bin/env python

import os

def relpath(target, base):
    """ Find relative path from base to target
        if target== "/local/chris/appli" and base== "/local/chris" the result is appli
        if target== /tmp/appli and base /local/chris the result is ../../tmp/appli
    """
    target=target.split(os.path.sep)
    base=base.split(os.path.sep)
    for i in xrange(len(base)):
      if base[i] != target[i]:
        i=i-1
        #not in base
        break
    p=['..']*(len(base)-i-1)+target[i+1:]
    if p == []:
      return '.'
    return os.path.join( *p )

def set_var(VAR, strpath):
    """Set VAR environment variable """
    value = "%r" % strpath
    shell = os.getenv('SHELL')
    if shell and shell.endswith('csh'):
        return "setenv %s %s" % (VAR, value)
    else:
        return "export %s=%s" % (VAR, value)
	

applipath=relpath(os.path.abspath(os.path.dirname(__file__)),os.path.abspath(os.getenv('HOME')))

#print set_var('APPLI', applipath)
print applipath
