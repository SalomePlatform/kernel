#! /usr/bin/env python

import sys
import logging

from getLogger import getLogger

if __name__ == "__main__":
  args = sys.argv[1:]
  logger, args = getLogger(args)

  if len(args)==0:
    logger.info("Hello!")
  else:
    logger.info("Hello to: %s"%(", ".join(args)))
#
