#==============================================================================
#  File      : supervisionexample.py
#  Created   : 23 nov 2001
#  Author    : Jean Rahuel
#  Project   : SALOME
#  Copyright : CEA
#==============================================================================

from SuperV import *



# load this object with Nodes, Links and Datas stored in GraphEssai.xml
# (GraphEssai.xml was created with python in SuperVisionTest and GraphEssai.py)
myGraph = Graph("@ROOT_BUILDDIR@/share/salome/resources/GraphEssai.xml")

#myGraph = Graph("/home/data/jr_HEAD/build/share/salome/resources/GraphEssai.xml")
# This DataFlow is "valid" : no loop, correct links between Nodes etc...

print myGraph.IsValid()

# Get Nodes
myGraph.PrintNodes()
Add,Sub,Mul,Div = myGraph.Nodes()

# Load Datas
Addx = Add.Input("x",3.)
Addy = Add.Input("y",4.5)
Subx = Sub.Input("x",1.5)

# Get Output Port
Addz = Add.Port('z')
Subz = Sub.Port('z')
Mulz = Mul.Port('z')
Divz = Div.Port('z')

# This DataFlow is "executable" : all pending Ports are defined with Datas
print myGraph.IsExecutable()

# Starts only execution of that DataFlow and gets control immediatly
print myGraph.Run()

# That DataFlow is running ==> 0 (false)
print myGraph.IsDone()

# Events of execution :
aStatus,aNode,anEvent,aState = myGraph.Event()
while aStatus :
    print aNode.Thread(),aNode.SubGraph(),aNode.Name(),anEvent,aState
    aStatus,aNode,anEvent,aState = myGraph.Event()
print myGraph.IsDone()

# Wait for Completion (but it is already done after event loop ...)
print "Done : ",myGraph.DoneW()

# Get result
print "Result : ",Divz.ToString()

# Intermediate results :
print "Intermediate Result Add\z : ",Addz.ToString()
print "Intermediate Result Sub\z : ",Subz.ToString()
print "Intermediate Result Mul\z : ",Mulz.ToString()

print " "
print "Type : print myGraph.IsDone()"
print "       If execution is finished ==> 1 (true)"
print " "
print "Type : print Divz.ToString()"
print "       You will get the result"
print " "
print "Type : myGraph.PrintPorts()"
print "       to see input and output values of the graph"
print " "
print "Type : Add.PrintPorts()"
print "Type : Sub.PrintPorts()"
print "Type : Mul.PrintPorts()"
print "Type : Div.PrintPorts()"
print "       to see input and output values of nodes"

# Export will create newsupervisionexample.xml and the corresponding .py file
print myGraph.Export("@ROOT_BUILDDIR@/share/salome/resources/newsupervisionexample.xml")

print " "
print "See file @ROOT_BUILDDIR@/share/salome/resources/newsupervisionexample.xml"
print "See file @ROOT_BUILDDIR@/share/salome/resources/newsupervisionexample.py"
