# -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

__author__="gboulant"
__date__ ="$1 avr. 2010 18:12:38$"

import time
import threading

# ===========================================================================
class Runner(threading.Thread):
    """
    This class provides a tool to run and drive a function in a dedicated thread.
    """
    def __init__(self, functionToRun=None,*argv):
        threading.Thread.__init__( self )
        self._stopevent = threading.Event()
        self.setFunction(functionToRun)
        self.setArguments(*argv)
        self._exception = None
        self._return    = None
        self._observer  = None
        self._callbackFunction = None
        # We create an id from the name and the time date in milliseconds
        self._id = functionToRun.__name__ +"/"+ str(time.time())

    def setFunction(self,functionToRun):
        """
        Positionne la fonction e executer. La fonction peut etre la
        methode d'un objet passee sous la forme 'monobjet.mamethode'.
        """
        self.clear()
        self._function = functionToRun

    def setArguments(self,*argv):
        """
        Positionne les arguments e passer e la fonction
        """
        self.clear()
        self._argv = argv

    def getReturn(self):
        """
        Retourne le resultat de la fonction. En cas d'erreur, on
        recuperera l'exception levee au moyen de la methode
        getException().
        """
        return self._return

    def setCallbackFunction(self, callbackFunction):
        self._callbackFunction = callbackFunction

    def setObserver(self, observerToNotify):
        """
        Permet de specifier un observateur e notifier en fin
        d'execution. L'observateur est suppose etre un objet qui
        implemente une methode processNotification()
        """
        try:
            observerToNotify.processNotification
        except AttributeError:
            raise DevelException("The observer should implement the method processNotification()")

        self._observer = observerToNotify

    def run(self):
        """
        Execution de la fonction. Implementation de la methode run
        declenchee par l'appel e Thread.start().
        """
        print("##################### threadhelper.run")
        if self._function is None: return
        try:
            self._return = self._function(*self._argv)
        except Exception as e:
            print(e)
            self._exception = e
        self._stopevent.set()
        self.notifyObserver()
        self.callback()
        # _GBO_ Maybe it's no use to have both observers and a callback function.
        # One of the two mechanism should be sufficient

    def notifyObserver(self):
        if self._observer is None:
            # Aucune notification prevue
            return
        try:
            self._observer.processNotification()
        except AttributeError as att:
            if str(att) == "processNotification":
                print("L'observateur n'implemente pas la methode processNotification()")
            else:
                print("La fonction processNotification() a leve une exception:")
                print(att)
        except Exception as e:
            print("La fonction processNotification() a leve une exception:")
            print(e)

    def callback(self):
        if self._callbackFunction is None: return
        self._callbackFunction()

    def isEnded(self):
        """
        Retourne true si la fonction s'est terminee naturellement
        (correctement ou en erreur). Utile pour les client qui
        pratique le pooling (interrogation repetee e intervalle
        regulier.
        """
        return self._stopevent.isSet()
        # _GBO_ On n'utilise pas isAlive() pour pouvoir maetriser
        # l'indicateur de stop (exemple de la fonction kill)
        # return not self.isAlive()

    def getException(self):
        return self._exception

    def getId(self):
        return self._id

    def wait(self,timeout=None):
        """
        Met fin au thread apres timeout seconde s'il est encore en
        execution.
        Si le compte-e-rebours est atteind sans que la fonction
        functionToRun soit terminee, alors le runner est laisse dans
        l'etat 'not Ended', c'est-e-dire que isEnded retourne
        false. On peut ainsi distinguer l'arret normal de l'arret e
        l'issue du timeout.
        """
        threading.Thread.join(self, timeout)


    def kill(self,message="Arret demande"):
        """
        Cette methode doit etre appeler pour interrombre le
        thread. Cet appel declare le thread comme en erreur (exception
        mise e disposition par la methode getException().
        """
        # On cree un exception indiquant la demande d'interruption
        self._exception = Exception(message)
        self._stopevent.set()
        # _GBO_ ATTENTION
        # Un thread python ne peut pas en realite etre interrompu.
        # La fonction reste donc en execution meme si on a rejoint le
        # thread appelant.

    def clear(self):
        self._stopevent.clear()
        self._exception = None
        self._return    = None

# ===========================================================================
CONTINUE=1
STOP=0

class PeriodicTimer( threading.Thread ):
    """
    Cette classe permet d'amorcer un compte-e-rebours periodique pour
    operer un mecanisme de pooling. On definit la fonction appellee
    periodiquement et la fonction terminale.
    """
    def __init__( self,  loopdelay, initdelay=0,
                  periodic_action=None,
                  ended_action=None,
                  *ended_argv ):

        if periodic_action is None:
            self._periodic_action = self.defaultPeriodicAction
        else:
            self._periodic_action = periodic_action
        self._ended_action = ended_action
        self._loopdelay = loopdelay
        self._initdelay = initdelay
        self._running = CONTINUE
        self._ended_argv    = ended_argv
        threading.Thread.__init__( self )

    def defaultPeriodicAction():
        """
        Les fonctions 'periodicAction' retournent CONTINU ou STOP
        apres avoir execute l'action de fin de boucle. Si STOP est
        retourne, le cycle est interrompu. 
        """
        return CONTINU

    def run( self ):
        if self._initdelay:
            time.sleep( self._initdelay )
        self._runtime = time.time()
        while self._running == CONTINUE:
            start = time.time()
            self._running  = self._periodic_action()
            self._runtime += self._loopdelay
            time.sleep( max( 0, self._runtime - start ) )
        if self._ended_action is not None:
            self._ended_action( *self._ended_argv )

    def stop( self ):
        self._running = STOP



#
# ======================================================
# Fonctions de test unitaire
# ======================================================
#

# ======================================================
import os
testfilename="/tmp/threadhelperTestFile"
def testIfContinue():
    print("On examine la presence du fichier ", testfilename)
    if os.path.exists(testfilename):
        return STOP
    else:
        return CONTINUE

def endedAction():
    print("FINI")

def TEST_PeriodicTimer():
    periodicTimer=PeriodicTimer(1,0,testIfContinue, endedAction)
    periodicTimer.start()


# ======================================================
def function_ok(nbsteps=5):
    """
    Fonction qui se termine correctement
    """
    print("Debut")
    cnt=0
    while ( cnt < nbsteps ):
        print("Etape ", cnt)
        time.sleep(0.6)
        cnt+=1

    print("Fin")

def function_with_exception():
    """
    Fonction qui aboutie e une levee d'exception
    """
    print("Debut")
    cnt=0
    while ( cnt < 5 ):
        print("Etape ", cnt)
        time.sleep(1)
        cnt+=1
    
    raise Exception("erreur d'execution de la fonction")
    print("Fin")

def infinite_function():
    """
    fonction de duree infinie (tant qu'il y a du courant electrique) pour
    le test du timeout.
    """
    print("Debut")
    cnt=0
    while ( 1 ):
        print("Etape ", cnt)
        time.sleep(1)
        cnt+=1
    
    raise Exception("erreur")
    print("Fin")


def runWithRunner(functionToRun):
    """
    Execute la fonction avec le runner. On illustre ici la modalite
    d'utilisation du Runner.
    """
    print("###########")
    runner = Runner(functionToRun)
    runner.start()

    while ( not runner.isEnded() ):
        print("La fonction est en cours")
        time.sleep(0.2)
    e = runner.getException()
    if e is not None:
        print("La fonction s'est terminee en erreur")
        print(e)
        # On peut en fait la relancer
        # raise e
    else:
        print("La fonction s'est terminee correctement")


def runWithTimeout(functionToRun, timeout=10):
    """
    Execute la fonction avec le runner. On illustre ici la modalite
    d'utilisation du Runner.
    """
    print("runWithTimeout : DEBUT")
    runner = Runner(functionToRun)
    runner.start()

    # On se fixe un temps au dele duquel on considere que la fonction
    # est en erreur => on tue le thread (timeout)
    runner.wait(timeout)
    print("Apres runner.timeout(timeout)")
    if not runner.isEnded():    
        runner.kill()
    e = runner.getException()
    if e is not None:
        print("La fonction s'est terminee en erreur")
        print(e)
        # On peut en fait la relancer
        # raise e
    else:
        print("La fonction s'est terminee correctement")

    print("runWithTimeout : FIN")
    import sys
    sys.exit(0)
    

def TEST_Timeout():
    #runWithTimeout(function_ok)
    #runWithTimeout(function_ok,2)
    runWithTimeout(function_with_exception)

    
def TEST_Runner():
    runWithRunner(function_ok)
    runWithRunner(function_with_exception)
    #runWithRunner(infinite_function)


def myCallbackFunction():
    print("myCallbackFunction: the job is ended")
    

def TEST_runWithCallback():
    runner = Runner(function_ok,8)
    runner.setCallbackFunction(myCallbackFunction)
    runner.start()

    if runner.getException() is not None:
        return False

    runnerId = runner.getId()
    print("A runner has been started with id="+str(runnerId))
    cpt = 0
    while ( not runner.isEnded() ):
        print("Waiting notification from process "+str(runner.getId())+", step n°"+str(cpt))
        time.sleep(0.2)
        cpt+=1

    return True

if __name__ == "__main__":
    from . import unittester
    #TEST_PeriodicTimer()
    #TEST_Runner()
    #TEST_Timeout()
    unittester.run("threadhelper","TEST_runWithCallback")

    
