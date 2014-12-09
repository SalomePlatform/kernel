# Copyright (C) 2013-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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

import ConfigParser
import os
import logging
import re
from io import StringIO
import subprocess
from salomeContextUtils import SalomeContextException #@UnresolvedImport

logging.basicConfig()
logConfigParser = logging.getLogger(__name__)

ADD_TO_PREFIX = 'ADD_TO_'
UNSET_KEYWORD = 'UNSET'


def _expandSystemVariables(key, val):
  expandedVal = os.path.expandvars(val) # expand environment variables
  # Search for not expanded variables (i.e. non-existing environment variables)
  pattern = re.compile('\${ ( [^}]* ) }', re.VERBOSE) # string enclosed in ${ and }
  expandedVal = pattern.sub(r'', expandedVal) # remove matching patterns

  if not "DLIM8VAR" in key: # special case: DISTENE licence key can contain double clons (::)
    expandedVal = _trimColons(expandedVal)
  return expandedVal
#

# :TRICKY: So ugly solution...
class MultiOptSafeConfigParser(ConfigParser.SafeConfigParser):
  def __init__(self):
    ConfigParser.SafeConfigParser.__init__(self)

  # copied from python 2.6.8 Lib.ConfigParser.py
  # modified (see code comments) to handle duplicate keys
  def _read(self, fp, fpname):
    """Parse a sectioned setup file.

    The sections in setup file contains a title line at the top,
    indicated by a name in square brackets (`[]'), plus key/value
    options lines, indicated by `name: value' format lines.
    Continuations are represented by an embedded newline then
    leading whitespace.  Blank lines, lines beginning with a '#',
    and just about everything else are ignored.
    """
    cursect = None                        # None, or a dictionary
    optname = None
    lineno = 0
    e = None                              # None, or an exception
    while True:
      line = fp.readline()
      if not line:
        break
      lineno = lineno + 1
      # comment or blank line?
      if line.strip() == '' or line[0] in '#;':
        continue
      if line.split(None, 1)[0].lower() == 'rem' and line[0] in "rR":
        # no leading whitespace
        continue
      # continuation line?
      if line[0].isspace() and cursect is not None and optname:
        value = line.strip()
        if value:
          cursect[optname].append(value)
      # a section header or option header?
      else:
        # is it a section header?
        mo = self.SECTCRE.match(line)
        if mo:
          sectname = mo.group('header')
          if sectname in self._sections:
            cursect = self._sections[sectname]
          elif sectname == ConfigParser.DEFAULTSECT:
            cursect = self._defaults
          else:
            cursect = self._dict()
            cursect['__name__'] = sectname
            self._sections[sectname] = cursect
          # So sections can't start with a continuation line
          optname = None
        # no section header in the file?
        elif cursect is None:
          raise ConfigParser.MissingSectionHeaderError(fpname, lineno, line)
        # an option line?
        else:
          mo = self.OPTCRE.match(line)
          if mo:
            optname, vi, optval = mo.group('option', 'vi', 'value')
            optname = self.optionxform(optname.rstrip())
            # This check is fine because the OPTCRE cannot
            # match if it would set optval to None
            if optval is not None:
              if vi in ('=', ':') and ';' in optval:
                # ';' is a comment delimiter only if it follows
                # a spacing character
                pos = optval.find(';')
                if pos != -1 and optval[pos-1].isspace():
                  optval = optval[:pos]
              optval = optval.strip()
              # ADD THESE LINES
              splittedComments = optval.split('#')
              s = _expandSystemVariables(optname, splittedComments[0])
              optval = s.strip().strip("'").strip('"')
              #if len(splittedComments) > 1:
              #  optval += " #" + " ".join(splittedComments[1:])
              # END OF ADD
              # allow empty values
              if optval == '""':
                optval = ''
              # REPLACE following line (original):
              #cursect[optname] = [optval]
              # BY THESE LINES:
              # Check if this optname already exists
              if (optname in cursect) and (cursect[optname] is not None):
                cursect[optname][0] += ','+optval
              else:
                cursect[optname] = [optval]
              # END OF SUBSTITUTION
            else:
              # valueless option handling
              cursect[optname] = optval
          else:
            # a non-fatal parsing error occurred.  set up the
            # exception but keep going. the exception will be
            # raised at the end of the file and will contain a
            # list of all bogus lines
            if not e:
              e = ConfigParser.ParsingError(fpname)
            e.append(lineno, repr(line))
    # if any parsing errors occurred, raise an exception
    if e:
      raise e

    # join the multi-line values collected while reading
    all_sections = [self._defaults]
    all_sections.extend(self._sections.values())
    for options in all_sections:
      for name, val in options.items():
        if isinstance(val, list):
          options[name] = '\n'.join(val)
  #


# Parse configuration file
# Input: filename, and a list of reserved keywords (environment variables)
# Output: a list of pairs (variable, value), and a dictionary associating a list of user-defined values to each reserved keywords
# Note: Does not support duplicate keys in a same section
def parseConfigFile(filename, reserved = None):
  if reserved is None:
    reserved = []
  config = MultiOptSafeConfigParser()
  config.optionxform = str # case sensitive

  # :TODO: test file existence

  # Read config file
  try:
    config.read(filename)
  except ConfigParser.MissingSectionHeaderError:
    logConfigParser.error("No section found in file: %s"%(filename))
    return []

  try:
    return __processConfigFile(config, reserved, filename)
  except ConfigParser.InterpolationMissingOptionError, e:
    msg = "A variable may be undefined in SALOME context file: %s\nParser error is: %s\n"%(filename, e)
    raise SalomeContextException(msg)
#

def __processConfigFile(config, reserved = None, filename="UNKNOWN FILENAME"):
  # :TODO: may detect duplicated variables in the same section (raise a warning)
  #        or even duplicate sections

  if reserved is None:
    reserved = []
  unsetVariables = []
  outputVariables = []
  # Get raw items for each section, and make some processing for environment variables management
  reservedKeys = [ADD_TO_PREFIX+str(x) for x in reserved] # produce [ 'ADD_TO_reserved_1', 'ADD_TO_reserved_2', ..., ADD_TO_reserved_n ]
  reservedValues = dict([str(i),[]] for i in reserved) # create a dictionary in which keys are the 'ADD_TO_reserved_i' and associated values are empty lists: { 'reserved_1':[], 'reserved_2':[], ..., reserved_n:[] }
  sections = config.sections()
  for section in sections:
    entries = config.items(section, raw=False) # use interpolation
    if len(entries) == 0: # empty section
      logConfigParser.warning("Empty section: %s in file: %s"%(section, filename))
      pass
    for key,val in entries:
      if key in reserved:
        logConfigParser.error("Invalid use of reserved variable: %s in file: %s"%(key, filename))
      elif key == UNSET_KEYWORD:
        unsetVariables += val.replace(',', ' ').split()
      else:
        expandedVal = _expandSystemVariables(key, val)

        if key in reservedKeys:
          shortKey = key[len(ADD_TO_PREFIX):]
          vals = expandedVal.split(',')
          reservedValues[shortKey] += vals
          # remove left&right spaces on each element
          vals = [v.strip(' \t\n\r') for v in vals]
        else:
          outputVariables.append((key, expandedVal))
          pass
        pass # end if key
      pass # end for key,val
    pass # end for section

  # remove duplicate values
  outVars = []
  for (var, values) in outputVariables:
    vals = values.split(',')
    vals = list(set(vals))
    outVars.append((var, ','.join(vals)))

  return unsetVariables, outVars, reservedValues
#

def _trimColons(var):
  v = var
  # Remove leading and trailing colons (:)
  pattern = re.compile('^:+ | :+$', re.VERBOSE)
  v = pattern.sub(r'', v) # remove matching patterns
  # Remove multiple colons
  pattern = re.compile('::+', re.VERBOSE)
  v = pattern.sub(r':', v) # remove matching patterns
  return v
#

# This class is used to parse .sh environment file
# It deals with specific treatments:
#    - virtually add a section to configuration file
#    - process shell keywords (if, then...)
class EnvFileConverter(object):
  def __init__(self, fp, section_name, reserved = None, outputFile=None):
    if reserved is None:
      reserved = []
    self.fp = fp
    self.sechead = '[' + section_name + ']\n'
    self.reserved = reserved
    self.outputFile = outputFile
    self.allParsedVariableNames = []
    # exclude line that begin with:
    self.exclude = [ 'if', 'then', 'else', 'fi', '#', 'echo', 'exit' ]
    self.exclude.append('$gconfTool') # QUICK FIX :TODO: provide method to extend this variable
    # discard the following keywords if at the beginning of line:
    self.discard = [ 'export' ]
    # the following keywords imply a special processing if at the beginning of line:
    self.special = [ 'unset' ]

  def readline(self):
    if self.sechead:
      try:
        if self.outputFile is not None:
          self.outputFile.write(self.sechead)
        return self.sechead
      finally:
        self.sechead = None
    else:
      line = self.fp.readline()
      # trim  whitespaces
      line = line.strip(' \t\n\r')
      # line of interest? (not beginning by a keyword of self.exclude)
      for k in self.exclude:
        if line.startswith(k):
          return '\n'
      # look for substrinsg beginning with sharp charcter ('#')
      line = re.sub(r'#.*$', r'', line)
      # line to be pre-processed? (beginning by a keyword of self.special)
      for k in self.special:
        if k == "unset" and line.startswith(k):
          line = line[len(k):]
          line = line.strip(' \t\n\r')
          line = UNSET_KEYWORD + ": " + line
      # line to be pre-processed? (beginning by a keyword of self.discard)
      for k in self.discard:
        if line.startswith(k):
          line = line[len(k):]
          line = line.strip(' \t\n\r')
      # process reserved keywords
      for k in self.reserved:
        if line.startswith(k) and "=" in line:
          variable, value = line.split('=')
          value = self._purgeValue(value, k)
          line = ADD_TO_PREFIX + k + ": " + value
      # Update list of variable names
      # :TODO: define excludeBlock variable (similar to exclude) and provide method to extend it
      if "cleandup()" in line:
        print "WARNING: parseConfigFile.py: skip cleandup and look for '# PRODUCT environment'"
        while True:
          line = self.fp.readline()
          if "# PRODUCT environment" in line:
            print "WARNING: parseConfigFile.py: '# PRODUCT environment' found"
            break
      while "clean " in line[0:6]: #skip clean calls with ending ";" crash
        line = self.fp.readline()
      # Extract variable=value
      if "=" in line:
        try:
          variable, value = line.split('=')
        except: #avoid error for complicated sh line xx=`...=...`, but warning
          print "WARNING: parseConfigFile.py: line with multiples '=' character are hazardous: '"+line+"'"
          variable, value = line.split('=',1)
          pass

        # Self-extending variables that are not in reserved keywords
        # Example: FOO=something:${FOO}
        # In this case, remove the ${FOO} in value
        if variable in value:
          value = self._purgeValue(value, variable)
          line = "%s=%s"%(variable,value)

        self.allParsedVariableNames.append(variable)
      # End of extraction

      if not line:
        return line

      #
      # replace "${FOO}" and "$FOO" and ${FOO} and $FOO by %(FOO)s if FOO is
      # defined in current file (i.e. it is not an external environment variable)
      for k in self.allParsedVariableNames:
        key = r'\$\{?'+k+'\}?'
        pattern = re.compile(key, re.VERBOSE)
        line = pattern.sub(r'%('+k+')s', line)
        # Remove quotes (if line does not contain whitespaces)
        try:
          variable, value = line.split('=', 1)
        except ValueError:
          variable, value = line.split(':', 1)
        if not ' ' in value.strip():
          pattern = re.compile(r'\"', re.VERBOSE)
          line = pattern.sub(r'', line)
      #

      # Replace `shell_command` by its result
      def myrep(obj):
        obj = re.sub('`', r'', obj.group(0)) # remove quotes
        obj = obj.split()
        res = subprocess.Popen(obj, stdout=subprocess.PIPE).communicate()[0]
        res = res.strip(' \t\n\r') # trim whitespaces
        return res
      #
      line = re.sub('`[^`]+`', myrep, line)
      #
      if self.outputFile is not None:
        self.outputFile.write(line+'\n')
      return line

  def _purgeValue(self, value, name):
    # Replace foo:${PATTERN}:bar or foo:$PATTERN:bar by foo:bar
    key = r'\$\{?'+name+'\}?'
    pattern = re.compile(key, re.VERBOSE)
    value = pattern.sub(r'', value)

    # trim colons
    value = _trimColons(value)

    return value
  #

# Convert .sh environment file to configuration file format
def convertEnvFileToConfigFile(envFilename, configFilename, reserved=None):
  if reserved is None:
    reserved = []
  logConfigParser.debug('convert env file %s to %s'%(envFilename, configFilename))
  fileContents = open(envFilename, 'r').read()

  pattern = re.compile('\n[\n]+', re.VERBOSE) # multiple '\n'
  fileContents = pattern.sub(r'\n', fileContents) # replace by a single '\n'

  finput = StringIO(unicode(fileContents))
  foutput = open(configFilename, 'w')

  config = MultiOptSafeConfigParser()
  config.optionxform = str # case sensitive
  config.readfp(EnvFileConverter(finput, 'SALOME Configuration', reserved, outputFile=foutput))

  foutput.close()

  logConfigParser.info('Configuration file generated: %s'%configFilename)
#
