#  Copyright (C) 2003  CEA/DEN, EDF R&D
#
#
#
#  File   : visu_many_objects.py
#  Author : Vadim SANDLER
#  Module : SALOME
#  $Header$

import salome
import SALOMEDS

# >>> Getting study builder ==================================================
myStudy = salome.myStudy
myBuilder = myStudy.NewBuilder()

# >>> Creating virtual component =============================================
myComponent = myBuilder.NewComponent("VIRTUAL")
AName = myBuilder.FindOrCreateAttribute(myComponent, "AttributeName")
AName.SetValue("Virtual")
ACmt = myBuilder.FindOrCreateAttribute(myComponent, "AttributeComment")
ACmt.SetValue("Virtual")

# >>> Creating objects =======================================================
# TO CHANGE TOTAL NUMBER OF OBJECTS, SET PROPER <myNb1>, <myNb2>, <myNb3>
# IF YOU DON'T WANT TO ADD REFERENCES, SET <myAddRef> = 0
myNb1 = 20
myNb2 = 5
myNb3 = 3
myAddRef = 1

if myAddRef == 1:
    myContainer =  myBuilder.NewObject(myComponent)
    AName = myBuilder.FindOrCreateAttribute(myContainer, "AttributeName")
    AName.SetValue("References")
    ACmt = myBuilder.FindOrCreateAttribute(myContainer, "AttributeComment")
    ACmt.SetValue("Container for objects")

for i in range(0,myNb1):
    myObject1 = myBuilder.NewObject(myComponent)
    AName = myBuilder.FindOrCreateAttribute(myObject1, "AttributeName")
    AName.SetValue("Object "+str(i))
    ACmt = myBuilder.FindOrCreateAttribute(myObject1, "AttributeComment")
    ACmt.SetValue("Virtual object "+str(i))
    if myAddRef == 1:
        myRefObject = myBuilder.NewObject(myContainer)
        myBuilder.Addreference(myRefObject,myObject1);

    for j in range(0,myNb2):
        myObject2 = myBuilder.NewObject(myObject1)
        AName = myBuilder.FindOrCreateAttribute(myObject2, "AttributeName")
        AName.SetValue("Object "+str(i)+"-"+str(j))
        ACmt = myBuilder.FindOrCreateAttribute(myObject2, "AttributeComment")
        ACmt.SetValue("Virtual object "+str(i)+"-"+str(j))
        if myAddRef == 1:
            myRefObject = myBuilder.NewObject(myContainer)
            myBuilder.Addreference(myRefObject,myObject2);

        for k in range(0,myNb3):
            myObject3 = myBuilder.NewObject(myObject2)
            AName = myBuilder.FindOrCreateAttribute(myObject3, "AttributeName")
            AName.SetValue("Object "+str(i)+"-"+str(j)+"-"+str(k))
            ACmt = myBuilder.FindOrCreateAttribute(myObject3, "AttributeComment")
            ACmt.SetValue("Virtual object "+str(i)+"-"+str(j)+"-"+str(k))
            if myAddRef == 1:
                myRefObject = myBuilder.NewObject(myContainer)
                myBuilder.Addreference(myRefObject,myObject3);

# >>> Updating Object Browser ================================================
salome.sg.updateObjBrowser(1)

# ============================================================================

