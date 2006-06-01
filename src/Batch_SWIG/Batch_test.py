# Copyright (C) 2005  OPEN CASCADE, CEA, EDF R&D, LEG
#           PRINCIPIA R&D, EADS CCR, Lip6, BV, CEDRAT
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either 
# version 2.1 of the License.
# 
# This library is distributed in the hope that it will be useful 
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
#Batch_test.py

# pratique
import readline
import rlcompleter
readline.parse_and_bind('tab: complete')

# Importation de la bibliotheque de classes Batch
from libBatch_Swig import *

def work():
    # Definition d'un job...
    job=Job()
    # ... de ses parametres ...
    p={}
    p['EXECUTABLE']='/home/dutka/tmp/job'
    p['NAME']='MonJob'
    p['OUTFILE']=[('/tmp/stdout', 'stdout'), ('/tmp/stderr', 'stderr')]
    job.setParametre(p)
    # ... et de son environnement
    job.setEnvironnement({})
    print job

    # Appel au catalogue de BatchManager pour accéder au serveur cli70cu
    # Instanciation du catalogue (quasi-singleton)
    c=BatchManagerCatalog()
    # Instanciation d'une Factory de BatchManager de type 'PBS'
    # fbm=c('PBS')

    # Creation d'un BatchManager de type PBS sur le serveur cli70cu
    bm=c('PBS')('cli70cu')

    # Soumission du job au BatchManager
    jobid=bm.submitJob(job)
    print jobid

    # Interrogation de l'etat du job
    jobid.queryJob()

    # On attend que le job soit termine
    try:
        while 1: jinfo = jobid.queryJob()
    except:
        print "Job", jobid, "is done"

    pass

if __name__ == "__main__":
    work()
    pass



