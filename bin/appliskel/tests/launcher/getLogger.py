import os
import sys
import logging

def getLogger(args=[]):
  outfileOptionPrefix = "outfile="
  outfileArgs = [ str(x) for x in args if str(x).startswith(outfileOptionPrefix) ]
  allFiles = [ x.replace(outfileOptionPrefix, '') for x in outfileArgs ]
  args = [ x for x in args if not str(x).startswith(outfileOptionPrefix) ]

  logger = logging.getLogger(__name__)
  if len(allFiles) == 0:
    logger.addHandler(logging.StreamHandler())
  else:
    for currentFile in allFiles:
      elements = currentFile.split(',')
      for elt in elements:
        elt = os.path.realpath(os.path.expanduser(elt))
        hdlr = logging.FileHandler(elt)
        logger.addHandler(hdlr)
  #
  logger.level = logging.DEBUG
  return logger, args
#
