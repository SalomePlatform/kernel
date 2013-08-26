#! /usr/bin/env python

import sys
import logging

from getLogger import getLogger


def file_len(fname):
  with open(fname) as f:
    for i, l in enumerate(f):
      pass
    return i + 1
#

if __name__ == "__main__":
  args = sys.argv[1:]
  logger, args = getLogger(args)

  if len(args)==0:
    logger.info("No files given")
  else:
    for filename in args:
      try:
        nb = file_len(filename)
        logger.info("%s is %s lines long"%(filename, nb))
      except IOError:
        logger.info("File '%s' cannot be read"%(filename))
#
