#!/usr/bin/env python

import sys, re
outfile = open(sys.argv[1], 'wb')
isCom = False
for line in open(sys.argv[2], 'rb').readlines():
    if re.match('class '+sys.argv[3]+'DC', line): 
        continue
    n = line.find('"""')
    n1 = line[(n+2):].find('"""')
    if (n > -1) and (n1 > -1):
        continue
    if isCom:
        if n > -1:
            isCom = False
        continue
    else:
        if n > -1:
            isCom = True
            continue       
    line = re.sub(r'^\s+#', '#', line) 
    line = re.sub(r'^\s+def', 'def', line) 
    line = re.sub(sys.argv[3]+'DC', sys.argv[3], line)
    outfile.write(line)
outfile.close()