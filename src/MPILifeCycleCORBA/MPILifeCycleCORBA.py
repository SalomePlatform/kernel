#==============================================================================
#  File      : MPILifeCycleCORBA.py
#  Created   : ven may 30 08:42:01 CEST 2003
#  Author    : Bernard SECHER, CEA
#  Project   : SALOME
#  Copyright : CEA 2003
#  $Header$
#==============================================================================

from LifeCycleCORBA import *

class MPILifeCycleCORBA(LifeCycleCORBA):
    #-------------------------------------------------------------------------

    def __init__(self, orb):
        MESSAGE( "MPILifeCycleCORBA::__init__" )
        LifeCycleCORBA.__init__(self, orb)

    #-------------------------------------------------------------------------

    def FindOrStartMPIContainer(self, theComputer , theMPIContainerRoot, nbproc ):
        theMPIContainer = theMPIContainerRoot + "_" + str(nbproc)
        MESSAGE( "FindOrStartMPIContainer" + theComputer + theMPIContainer )
        aMPIContainer = self.FindContainer( theComputer + "/" + theMPIContainer )
        if aMPIContainer is None :
            if (theMPIContainerRoot == "MPIFactoryServer") | (theMPIContainerRoot == "MPIFactoryServerPy") :
                if theComputer == os.getenv("HOSTNAME") :
                    rshstr = ""
                else :
                    rshstr = "rsh -n " + theComputer + " "
                path = self.ComputerPath( theComputer )
##                if path != "" :
##                    rshstr = rshstr + path + "/../bin/salome/"
##                else :
##                    rshstr = rshstr + os.getenv( "KERNEL_ROOT_DIR" ) + "/bin/"
#                    rshstr = rshstr + os.getenv( "PWD" ) + "/"
                if theMPIContainerRoot == "MPIFactoryServer" :
                    rshstr = rshstr + "mpirun -np " + str(nbproc) + " " + path + "SALOME_MPIContainer "
                else :
                    rshstr = rshstr + path + "SALOME_MPIContainerPy.py '"
                rshstr = rshstr + theMPIContainer + " -"
                omniORBcfg = os.getenv( "OMNIORB_CONFIG" )
#                omniORBcfg = os.getenv( "HOME" ) + "/.omniORB.cfg"
                file = os.open( omniORBcfg , os.O_RDONLY )
                ORBInitRef = os.read(file,132)
                if ORBInitRef[len(ORBInitRef)-1] == '\n' :
                    ORBInitRef,bsn = ORBInitRef.split('\n')
                os.close( file )
                rshstr = rshstr + ORBInitRef
                if theMPIContainerRoot == "MPIFactoryServerPy" :
                    rshstr = rshstr + "'"
                rshstr = rshstr + " > /tmp/" + theMPIContainer + "_"
                rshstr = rshstr + theComputer
                rshstr = rshstr + ".log 2>&1 &"
                os.system( rshstr )
                MESSAGE( "FindOrStartMPIContainer" + rshstr + " done" )
            else :
                if theMPIContainer.find('Py') == -1 :
                    aMPIContainer = self.FindContainer( theComputer + "/" + "MPIFactoryServer_" + str(nbproc) )
                else :
                    aMPIContainer = self.FindContainer( theComputer + "/" + "MPIFactoryServerPy_" + str(nbproc) )
                aMPIContainer = aMPIContainer.start_impl( theMPIContainer )

            count = 21
            while aMPIContainer is None :
                time.sleep(1)
                count = count - 1
                MESSAGE( str(count) + ". Waiting for " + theComputer + "/" + theMPIContainer )
                aMPIContainer = self.FindContainer( theComputer + "/" + theMPIContainer )
                if count == 0 :
                    return aMPIContainer
            
        return  aMPIContainer       
        #os.system("rsh -n dm2s0017 /export/home/SALOME_ROOT/bin/runSession SALOME_Container -ORBInitRef NameService=corbaname::dm2s0017:1515")

    #-------------------------------------------------------------------------

    def FindOrLoadMPIComponent(self, MPIcontainerName, MPIcomponentName, nbproc):

        theComputer,theMPIContainerRoot = self.ContainerName( MPIcontainerName )
        theMPIContainer = theMPIContainerRoot + "_" + str(nbproc)
        name = [CosNaming.NameComponent(theComputer,"dir"),
                CosNaming.NameComponent(theMPIContainer,"dir"),
                CosNaming.NameComponent(MPIcomponentName,"object")]
        try:
            obj = self._containerRootContext.resolve(name)
        except CosNaming.NamingContext.NotFound, ex:
            MESSAGE( "component " + MPIcomponentName + " not found, trying to load" )
            MPIcontainer = self.FindContainer(theComputer + "/" + theMPIContainer)
            if MPIcontainer is None:
                MESSAGE( "MPIcontainer " + theComputer + "/" + theMPIContainer + " not found in Naming Service, trying to start" )
                if (theMPIContainerRoot != "MPIFactoryServer") & (theMPIContainerRoot != "MPIFactoryServerPy") :
                    if theMPIContainer.find('Py') == -1 :
                        theMPIFactorycontainerRoot = "MPIFactoryServer"
                        theMPIFactorycontainer = theMPIFactorycontainerRoot + "_" + str(nbproc)
                    else :
                        theMPIFactorycontainerRoot = "MPIFactoryServerPy"
                        theMPIFactorycontainer = theMPIFactorycontainerRoot + "_" + str(nbproc)
                    MPIFactorycontainer = self.FindContainer(theComputer + "/" + theMPIFactorycontainer)
                    if MPIFactorycontainer is None:
                        MESSAGE( "MPIcontainer " + theComputer + "/" + theMPIFactorycontainer + " not found in Naming Service, trying to start" )
                        MPIFactorycontainer = self.FindOrStartMPIContainer(theComputer,theMPIFactorycontainerRoot,nbproc)
                else:
                    MPIFactorycontainer = self.FindOrStartMPIContainer(theComputer,theMPIContainerRoot,nbproc)
                if MPIFactorycontainer != None :
                    MPIcontainer = self.FindOrStartMPIContainer(theComputer,theMPIContainerRoot,nbproc)

            if MPIcontainer != None:
                compoinfo = self._catalog.GetComponent(MPIcomponentName)
                if compoinfo is None:
                    MESSAGE( "MPIcomponent " + MPIcomponentName + " not found in Module Catalog" )
                else:
                    try:
                        machineName = theComputer
                        path = compoinfo.GetPathPrefix(machineName) + "/"
                    except SALOME_ModuleCatalog.NotFound, ex:
                        MESSAGE( "machine " + machineName + " not found in Module Catalog" )
                        MESSAGE( "trying localhost" )
                        try:
                            path = compoinfo.GetPathPrefix("localhost") + "/"
                        except SALOME_ModuleCatalog.NotFound, ex:
                            path = ""
                    implementation = path + "lib" + MPIcomponentName + "Engine.so"
                    MESSAGE( "Trying to load " + implementation )
                    try:
                        MPIcomponent = MPIcontainer.load_impl(MPIcomponentName, implementation)
                        MESSAGE( "component " + MPIcomponent._get_instanceName() + " launched !" )
                        return MPIcomponent
                    except:
                        MESSAGE( "component " + MPIcomponentName + " NOT launched !" )

        else:
            try:
                MPIcomponent = obj._narrow(Engines.Component)
                if MPIcomponent is None:
                    MESSAGE( MPIcomponentName + " is not a component !" )
                else:
                    MESSAGE( "MPIcomponent " + MPIcomponent._get_instanceName() + " found !" )
                return MPIcomponent
            except:
                MESSAGE( MPIcomponentName + " failure" )
                return None
