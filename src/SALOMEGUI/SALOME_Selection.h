//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
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
//  File   : SALOME_Selection.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef SALOME_SELECTION_H
#define SALOME_SELECTION_H

#include "SALOME_Filter.hxx"
#include "SALOME_ListIO.hxx"
#include "SALOME_ListOfFilter.hxx"
#include "SALOME_InteractiveObject.hxx"
#include "SALOME_DataMapOfIOMapOfInteger.hxx"

// Qt Includes
#include <qlist.h>
#include <qstring.h>
#include <qobject.h>

// Open CASCADE Include
#include <TColStd_MapOfInteger.hxx>

#include <set>

enum Selection_Mode { 
 NodeSelection, 
 CellSelection,
 EdgeOfCellSelection,
 EdgeSelection, 
 FaceSelection, 
 VolumeSelection, 
 ActorSelection };

class SALOME_Selection : public QObject
{
  Q_OBJECT
    
public:

	SALOME_Selection(const QString& aName);
	~SALOME_Selection();

	static bool                CreateSelection(const QString& aName) ;
	static SALOME_Selection*   Selection      (const QString& aName) ;
	static bool                RemoveSelection(const QString& aName) ;
	static bool                FindSelection  (const QString& aName) ;

	void Clear () ;

	void ClearIObjects () ;
	int  AddIObject    ( const Handle(SALOME_InteractiveObject)& anObject, bool update = true) ;
	int  RemoveIObject ( const Handle(SALOME_InteractiveObject)& anObject, bool update = true) ;

	const SALOME_ListIO&             StoredIObjects() ;
	int                              IObjectCount() ;

	Handle(SALOME_InteractiveObject) firstIObject() ;
	Handle(SALOME_InteractiveObject) lastIObject() ;

	void                       AddFilter(const Handle(SALOME_Filter)& aFilter,
					     bool updateSelection = true ) ;
	bool                       RemoveFilter(const Handle(SALOME_Filter)& aFilter) ;
	void                       ClearFilters();
	const SALOME_ListOfFilter& StoredFilters();
	bool                       FindFilter(const Handle(SALOME_Filter)& aFilter);

	bool                       IsOk( const Handle(SALOME_InteractiveObject)& IObject ) ;


	void                       SetSelectionMode(Selection_Mode mode, bool activeCompOnly = false);
	Selection_Mode             SelectionMode();
        bool                       IsSelectActiveCompOnly() const;
	
	bool                        HasIndex( const Handle(SALOME_InteractiveObject)& IObject );
	void                       GetIndex( const Handle(SALOME_InteractiveObject)& IObject, 
					     TColStd_IndexedMapOfInteger& theIndex );
	
	typedef std::set<int> TContainerOfId;

	bool                        AddOrRemoveIndex( const Handle(SALOME_InteractiveObject)& IObject, int index, bool modeShift, bool update=true );
	bool                        AddOrRemoveIndex( const Handle(SALOME_InteractiveObject)& IObject, const TColStd_MapOfInteger& theIndices, bool modeShift, bool update=true );
	bool                        AddOrRemoveIndex( const Handle(SALOME_InteractiveObject)& IObject, const TContainerOfId& theIndices, bool modeShift, bool update=true );
	void                        RemoveIndex( const Handle(SALOME_InteractiveObject)& IObject, int index );
	bool                        IsIndexSelected(const Handle(SALOME_InteractiveObject)& IObject, int index);
	void                        ClearIndex();

	void                        BlockSignals( const bool );
	void                        SelectionChanged();

signals:
	void                        currentSelectionChanged();

private:
	QString               myName;
	SALOME_ListOfFilter   myFilters;
	SALOME_ListIO         myIObjects;

	SALOME_DataMapOfIOMapOfInteger myMapIOSubIndex;

	Selection_Mode        mySelectionMode;
        bool                  mySelActiveCompOnly;
};

#endif

