//  File      : SALOME_Selection.h
//  Created   : Wed Feb 20 17:24:59 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2002
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


	void                       SetSelectionMode(int mode, bool activeCompOnly = false);
	int                        SelectionMode();
        bool                       IsSelectActiveCompOnly() const;
	
	bool                        HasIndex( const Handle(SALOME_InteractiveObject)& IObject );
	void                       GetIndex( const Handle(SALOME_InteractiveObject)& IObject, 
					     TColStd_MapOfInteger& theIndex );
	
	bool                        AddOrRemoveIndex( const Handle(SALOME_InteractiveObject)& IObject, int index, bool modeShift, bool update=true );
	void                        RemoveIndex( const Handle(SALOME_InteractiveObject)& IObject, int index );
	bool                        IsIndexSelected(const Handle(SALOME_InteractiveObject)& IObject, int index);

signals:
	void                        currentSelectionChanged();

private:
	QString               myName;
	SALOME_ListOfFilter   myFilters;
	SALOME_ListIO         myIObjects;

	SALOME_DataMapOfIOMapOfInteger myMapIOSubIndex;

	int                   mySelectionMode;
        bool                  mySelActiveCompOnly;
};

#endif

