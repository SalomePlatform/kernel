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



