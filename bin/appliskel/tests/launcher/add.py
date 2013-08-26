#! /usr/bin/env python

import sys
import logging

from getLogger import getLogger

if __name__ == "__main__":
  args = sys.argv[1:]
  logger, args = getLogger(args)

  if len(args)==0:
    logger.info("No args!")
  else:
    msg = "+".join(args)
    res = sum(map(int, args))
    logger.info("%s = %s"%(msg, res))
#
