#  Copyright (C) 2003  CEA/DEN, EDF R&D
#
#
#
#  File   : test_big_table.py
#  Author : Vadim SANDLER
#  Module : SALOME
#  $Header$

import salome
import math
import SALOMEDS

# >>> Getting study builder ==================================================
myStudy = salome.myStudy
myBuilder = myStudy.NewBuilder()

# >>> Creating virtual component =============================================
myComponent = myStudy.FindComponent("VirtualComponent")
if not myComponent:
   myComponent = myBuilder.NewComponent("VirtualComponent")
   aName = myBuilder.FindOrCreateAttribute(myComponent, "AttributeName")
   aName.SetValue("VirtualComponent")

# >>> Creating object with Table of real[ 200 * 20 ] ========================
myTRealObject = myBuilder.NewObject(myComponent)
AName = myBuilder.FindOrCreateAttribute(myTRealObject, "AttributeName")
AName.SetValue("Table Of Real")
ARealTable = myBuilder.FindOrCreateAttribute(myTRealObject, "AttributeTableOfReal")
myHorNb = 10
myVerNb = 200

k={}
for j in range(0,myHorNb):
   k[j] = j*10+1
ARealTable.AddRow(k.values())
ARealTable.SetRowTitle(1, "Frequency")
ARealTable.SetRowUnit(1, "Hz")

for i in range(1,myVerNb+1):
   for j in range(0,myHorNb):
      if j % 2 == 1:
         k[j] = math.log10(j*30*math.pi/180) * 20 + i * 15 + j*5
      else:
         k[j] = math.sin(j*30*math.pi/180) * 20 + i * 15 + j*5 
   ARealTable.AddRow(k.values())
   ARealTable.SetRowTitle(i+1, "Power " + str(i))
   ARealTable.SetRowUnit(i+1, "Wt")
ARealTable.SetTitle("Very useful data")

# >>> Updating Object Browser ================================================
salome.sg.updateObjBrowser(1)

# ============================================================================
