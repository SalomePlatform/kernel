using namespace std;
//  File      : SALOME_AISShape.hxx
//  Created   : Thu Feb 21 14:37:54 CET 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOME
//  Copyright : Open CASCADE 2002
//  $Header$

#include "SALOME_AISShape.ixx"

SALOME_AISShape::SALOME_AISShape(const TopoDS_Shape& shape): AIS_Shape(shape) {
}

