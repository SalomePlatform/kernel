//  SALOME OCCViewer : build OCC Viewer into Salome desktop
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : OCCViewer_Selector.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef OCCViewer_SELECTOR_H
#define OCCViewer_SELECTOR_H

#include "QAD.h"

// QT Include
#include <qobject.h>

// Open CASCADE Include
#include <AIS_InteractiveObject.hxx>
#include <AIS_ListOfInteractive.hxx>

class QAD_EXPORT OCCViewer_Selector : public QObject
{
  Q_OBJECT	
		
 public:
  OCCViewer_Selector();
  virtual ~OCCViewer_Selector();

 public:
  void		enableSelection( bool );
  void		enableMultipleSelection( bool );

 signals:

  void		selSelectionDone( bool bAdded );
  void		selSelectionCancel( bool bAdded );
	
 protected:

  int		myNumSelected;
  bool		myEnableSelection;
  bool		myEnableMultipleSelection;
};

#endif

