#!/usr/bin/env python

import sys,os,pickle,signal

process_id={}

# -----------------------------------------------------------------------------
#
# Fonction d'arrêt de salome
#

def killSalome():
   print "arret des serveurs SALOME"
   for pid, cmd in process_id.items():
      print "arret du process %s : %s"% (pid, cmd[0])
      try:
         os.kill(pid,signal.SIGKILL)
      except:
         print "  ------------------ process %s : %s inexistant"% (pid, cmd[0])
   print "arret du naming service"
   os.system("killall -9 omniNames")
   
# -----------------------------------------------------------------------------

filedict='/tmp/'+os.getenv('USER')+'_SALOME_pidict'
#filedict='/tmp/'+os.getlogin()+'_SALOME_pidict'
try:
   fpid=open(filedict, 'r')
except:
   print "le fichier %s des process SALOME n'est pas accessible"% filedict
   sys.exit(1)
   
process_id=pickle.load(fpid)
fpid.close()

killSalome()

os.remove(filedict)


