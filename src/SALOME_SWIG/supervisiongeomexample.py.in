
# Generated python file of Graph GraphGeom2Essai

from SuperV import *

import salome

import geompy


geom = lcc.FindOrLoadComponent("FactoryServer", "Geometry")
geom.GetCurrentStudy(salome.myStudyId)

myBuilder = salome.myStudy.NewBuilder()

# Graph creation 
GraphGeom2Essai = Graph( 'GraphGeom2Essai' )
print GraphGeom2Essai.SetAuthor( '' )
print GraphGeom2Essai.SetComment( '' )
GraphGeom2Essai.Coords( 0 , 0 )

# Creation of Nodes
MakeSphere = GraphGeom2Essai.Node( 'Geometry' , 'Geometry' , 'MakeSphere' )
print MakeSphere.SetAuthor( '' )
print MakeSphere.SetContainer( 'FactoryServer' )
print MakeSphere.SetComment( '' )
MakeSphere.Coords( 26 , 13 )
MakeCopy = GraphGeom2Essai.Node( 'Geometry' , 'Geometry' , 'MakeCopy' )
print MakeCopy.SetAuthor( '' )
print MakeCopy.SetContainer( 'FactoryServer' )
print MakeCopy.SetComment( '' )
MakeCopy.Coords( 219 , 12 )
MakeTranslation = GraphGeom2Essai.Node( 'Geometry' , 'Geometry' , 'MakeTranslation' )
print MakeTranslation.SetAuthor( '' )
print MakeTranslation.SetContainer( 'FactoryServer' )
print MakeTranslation.SetComment( '' )
MakeTranslation.Coords( 219 , 159 )
MakeFuse = GraphGeom2Essai.Node( 'Geometry' , 'Geometry' , 'MakeFuse' )
print MakeFuse.SetAuthor( '' )
print MakeFuse.SetContainer( 'FactoryServer' )
print MakeFuse.SetComment( '' )
MakeFuse.Coords( 465 , 106 )

# Creation of intermediate Output variables and of links
MakeSphereshape = MakeSphere.Port( 'shape' )
MakeCopyshape1 = GraphGeom2Essai.Link( MakeSphereshape , MakeCopy.Port( 'shape1' ) )
MakeTranslationshape1 = GraphGeom2Essai.Link( MakeSphereshape , MakeTranslation.Port( 'shape1' ) )
MakeCopyshape = MakeCopy.Port( 'shape' )
MakeFuseshape1 = GraphGeom2Essai.Link( MakeCopyshape , MakeFuse.Port( 'shape1' ) )
MakeTranslationshape = MakeTranslation.Port( 'shape' )
MakeFuseshape2 = GraphGeom2Essai.Link( MakeTranslationshape , MakeFuse.Port( 'shape2' ) )

# Creation of Input datas
MakeSpherex1 = MakeSphere.Input( 'x1' , 0)
MakeSpherey1 = MakeSphere.Input( 'y1' , 0)
MakeSpherez1 = MakeSphere.Input( 'z1' , 0)
MakeSphereradius = MakeSphere.Input( 'radius' , 50)
MakeTranslationx1 = MakeTranslation.Input( 'x1' , 80)
MakeTranslationy1 = MakeTranslation.Input( 'y1' , 0)
MakeTranslationz1 = MakeTranslation.Input( 'z1' , 0)

# Missing Input datas : None

# Creation of Output variables
MakeFuseshape = MakeFuse.Port( 'shape' )

# State of Ports of the Graph
GraphGeom2Essai.PrintPorts()

# Start asynchronous run
print GraphGeom2Essai.Run()

# Done ?
print GraphGeom2Essai.IsDone()

# Events of execution :
aStatus,aNode,anEvent,aState = GraphGeom2Essai.Event()
while aStatus :
    print aNode.Thread(),aNode.SubGraph(),aNode.Name(),anEvent,aState
    aStatus,aNode,anEvent,aState = GraphGeom2Essai.Event()
print GraphGeom2Essai.IsDone()

# Waiting end of execution :
print GraphGeom2Essai.DoneW()

print " "
print "Type print GraphGeom2Essai.IsDone()"
print "1 <==> It's done"
print " "
print "Type print GraphGeom2Essai.DoneW()"
print "Wait until end of execution : 1(success)"
print " "
print "Type GraphGeom2Essai.PrintPorts()"
print "     to see input and output values of the graph"
print " "
print "Type MakeSphere.PrintPorts()"
print "Type MakeCopy.PrintPorts()"
print "Type MakeTranslation.PrintPorts()"
print "Type MakeFuse.PrintPorts()"
print "     to see input and output values of nodes"
print " "
print "Type print GraphGeom2Essai.Export('File-Name')"
print "You will get a .py and a .xml file of this graph"
