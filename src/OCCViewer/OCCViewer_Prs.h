//  SALOME OCCViewer : build OCC Viewer into Salome desktop
//
//  Copyright (C) 2004  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.salome-platform.org or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : OCCViewer_Prs.h
//  Author : Sergey ANIKIN
//  Module : SALOME
//  $Header$

#ifndef OCCVIEWER_PRS_H
#define OCCVIEWER_PRS_H

#include "SALOME_Prs.h"

#include <AIS_InteractiveObject.hxx>
#include <AIS_ListOfInteractive.hxx>

class OCCViewer_Prs : public SALOME_OCCPrs
{
public:
  OCCViewer_Prs();
  // Default constructor
  OCCViewer_Prs( const Handle(AIS_InteractiveObject)& obj );
  // Standard constructor
  ~OCCViewer_Prs();
  // Destructor

  void GetObjects( AIS_ListOfInteractive& list ) const;
  // Get interactive objects list
  void AddObject( const Handle(AIS_InteractiveObject)& obj );
  // Add interactive object

  bool IsNull() const;
  // Reimplemented from SALOME_Prs

  /* This method is used for activisation/deactivisation of
     objects in the moment of displaying */
  void         SetToActivate( const bool );
  bool         ToActivate() const;

private:
  AIS_ListOfInteractive myObjects;   // list of interactive objects
  bool                  myToActivate;
};

#endif




