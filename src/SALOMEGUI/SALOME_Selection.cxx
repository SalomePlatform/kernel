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
//  File   : SALOME_Selection.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

/*!
  \class SALOME_Selection SALOME_Selection.h
  \brief Selection Mechanism of Interactive Object.
*/

#include "SALOME_Selection.h"
#include "SALOME_Filter.hxx"
#include "SALOME_InteractiveObject.hxx"
#include "SALOME_ListIteratorOfListIO.hxx"
#include "SALOME_ListIteratorOfListOfFilter.hxx"

#include "QAD_Desktop.h"
#include "utilities.h"

#include <TColStd_MapIteratorOfMapOfInteger.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

using namespace std;

static QList<SALOME_Selection>&  SALOME_Sel_GetSelections()
{
  static QList<SALOME_Selection> Selections;
  return Selections;
}

//=======================================================================
//                       SELECTIONS MANAGEMENT
//=======================================================================

/*!
  Constructor
*/
SALOME_Selection::SALOME_Selection(const QString& aName) :
  myName(aName)
{
  myFilters.Clear();
  myIObjects.Clear();
  mySelectionMode = ActorSelection; /*4*/
  mySelActiveCompOnly = false;
  
  QAD_Desktop* aDesktop = QAD_Application::getDesktop();
  if (aDesktop) {
    QAD_Application *anActiveApplication = aDesktop->getActiveApp();
    if (anActiveApplication) QAD_ASSERT(connect(this, SIGNAL(currentSelectionChanged()),anActiveApplication, SLOT(updateActions())));
  }
}

/*!
  Destructor
*/
SALOME_Selection::~SALOME_Selection()
{
}

/*!
  Create a Selection with name \a aName
  \return TRUE if the Selection is created. Returns FALSE otherwise. 
*/
bool SALOME_Selection::CreateSelection(const QString& aName)
{ 
  SALOME_Selection* S = NULL;
  if ( SALOME_Selection::FindSelection( aName ) )
    S = SALOME_Selection::Selection( aName );
  else {
    S = new SALOME_Selection(aName);
    SALOME_Sel_GetSelections().prepend(S);
  }
  return true;
}


/*!
  Sets the current Selection with \a aName
*/
SALOME_Selection* SALOME_Selection::Selection(const QString& aName) 
{
  SALOME_Selection* Sel = NULL;
  if(SALOME_Sel_GetSelections().isEmpty()) return Sel;

  for( Sel=SALOME_Sel_GetSelections().first(); Sel!=0; Sel=SALOME_Sel_GetSelections().next() ){
    if( Sel->myName.compare(aName) == 0 ) 
      return Sel;
  }
  
  return Sel;
}

/*!
  Finds Selection with \a aName
  \return TRUE if the Selection is found. Returns FALSE otherwise. 
*/
bool SALOME_Selection::FindSelection(const QString& aName) 
{
  SALOME_Selection* Sel;
  for( Sel=SALOME_Sel_GetSelections().first(); Sel!=0; Sel=SALOME_Sel_GetSelections().next() ){
    if( Sel->myName.compare(aName) == 0 )
      return true;
  }

  return false;
}

/*!
  Removes Selection with \a aName
  \return TRUE if the Selection is removed. Returns FALSE otherwise. 
*/
bool SALOME_Selection::RemoveSelection(const QString& aName) 
{
  SALOME_Selection* Sel;
  for( Sel=SALOME_Sel_GetSelections().first(); Sel!=0; Sel=SALOME_Sel_GetSelections().next() ){
    if( Sel->myName.compare(aName) == 0 )
       return SALOME_Sel_GetSelections().remove(Sel);
  }
  return false;
}



//=======================================================================
//                                FILTERS MANAGEMENT
//=======================================================================

/*!
  Adds Filter
*/
void SALOME_Selection::AddFilter(const Handle(SALOME_Filter)& aFilter,
				 bool updateSelection) 
{
  if ( !FindFilter(aFilter) ) {
    myFilters.Append( aFilter );

    SALOME_ListIO removedIObjects;

    if ( updateSelection ) {
      SALOME_ListIteratorOfListIO It(myIObjects);
      for(;It.More();It.Next()){
	Handle(SALOME_InteractiveObject) Object = It.Value();
	if( !IsOk(Object) ) {
	  removedIObjects.Append( Object );
	  //	  RemoveIObject( Object );
	}
      }
      
      SALOME_ListIteratorOfListIO It1(removedIObjects);
      for(;It1.More();It1.Next()){
	Handle(SALOME_InteractiveObject) Object = It1.Value();
	RemoveIObject( Object );
      }
    }
  }
}

/*!
  Removes Filter
*/
bool SALOME_Selection::RemoveFilter(const Handle(SALOME_Filter)& aFilter) 
{
  SALOME_ListIteratorOfListOfFilter It(myFilters);
  for(;It.More();It.Next()){
    if (aFilter==It.Value()){ 
      myFilters.Remove(It);
      return true;
    }
  }
  return false;
}

//! Clears All Filters
void SALOME_Selection::ClearFilters()
{
  myFilters.Clear();
}

//! Finds aFilter 
bool SALOME_Selection::FindFilter(const Handle(SALOME_Filter)& aFilter) 
{
  SALOME_ListIteratorOfListOfFilter It(myFilters);
  for(;It.More();It.Next())
    if (aFilter==It.Value()) 
      return true;
  return false;
}

//! Returns the list of stored Filters
const SALOME_ListOfFilter& SALOME_Selection::StoredFilters()
{
  return myFilters;
}



//=======================================================================
//                     INTERACTIVE OBJECTS MANAGEMENT
//=======================================================================

void SALOME_Selection::Clear() 
{
  myIObjects.Clear();
}

//! Clears all Interactive Objects
void SALOME_Selection::ClearIObjects() 
{
  myIObjects.Clear();

  QAD_Desktop*   myDesktop = (QAD_Desktop*) QAD_Application::getDesktop();
  QAD_Study* myActiveStudy = myDesktop->getActiveStudy();
  myActiveStudy->unHighlightAll();

  myMapIOSubIndex.Clear();

  emit currentSelectionChanged();
}

//! Add an InteractiveObject
int SALOME_Selection::AddIObject(const Handle(SALOME_InteractiveObject)& anObject, bool update) 
{
  QAD_Desktop*   myDesktop = (QAD_Desktop*) QAD_Application::getDesktop();
  QAD_Study* myActiveStudy = myDesktop->getActiveStudy();

  if ( !IsOk(anObject) ) {
    if(MYDEBUG) INFOS ( "The Object not authorized by Filters" )
    myActiveStudy->highlight(anObject,false, update);
    return -1;
  }

  bool Found = false;
  SALOME_ListIteratorOfListIO It(myIObjects);
  for(;It.More();It.Next()) {
    if (anObject->isSame(It.Value())) {
      Found = true;
      break;
    }
  }

  // Il n'est pas dedans, on le rajoute....
  if(Found==false) {
    myIObjects.Append( anObject );
    myActiveStudy->highlight(anObject, true, update);
    emit currentSelectionChanged();
    return 1;
  }
  return 0;
}

//! Removes an InteractiveObject
int SALOME_Selection::RemoveIObject(const Handle(SALOME_InteractiveObject)& anObject, bool update) 
{
  SALOME_ListIteratorOfListIO It(myIObjects);
  for(;It.More();It.Next()) {
    if (anObject->isSame(It.Value())) {
      QAD_Desktop*   myDesktop = (QAD_Desktop*) QAD_Application::getDesktop();
      QAD_Study* myActiveStudy = myDesktop->getActiveStudy();
      myActiveStudy->highlight(anObject, false, update);

      if ( myMapIOSubIndex.IsBound( anObject ) ) {
	myMapIOSubIndex.UnBind( anObject );
      }
      
      myIObjects.Remove(It);

      emit currentSelectionChanged();
      return 1; 
    }
  }
  return 0;
}

//! Returns the list of InteractiveObjects
const SALOME_ListIO& SALOME_Selection::StoredIObjects()
{
  return myIObjects;
}

//! Returns the number of InteractiveObjects in the selection. 
int SALOME_Selection::IObjectCount()
{
  return myIObjects.Extent();
}

//!  Returns the first InteractiveObject in the selection.
Handle(SALOME_InteractiveObject) SALOME_Selection::firstIObject()
{
  return myIObjects.Extent() > 0 ? myIObjects.First() : Handle(SALOME_InteractiveObject)();
}

//! Returns the last InteractiveObject in the selection.
Handle(SALOME_InteractiveObject) SALOME_Selection::lastIObject()
{
  return myIObjects.Extent() > 0 ? myIObjects.Last() : Handle(SALOME_InteractiveObject)();
}

/*!
  Returns TRUE if the InteractiveObject is authorized by Filters. Returns FALSE otherwise.
*/
bool SALOME_Selection::IsOk(const Handle(SALOME_InteractiveObject)& anObj)
{
  SALOME_ListIteratorOfListOfFilter It(myFilters);
  for(;It.More();It.Next()){
    Handle(SALOME_Filter) theFilter = It.Value();
    if ( !theFilter->IsOk(anObj) ) 
      return false;
  }
  return true;
}

void SALOME_Selection::SetSelectionMode(Selection_Mode mode, bool activeCompOnly)
{
  mySelectionMode = mode;
  mySelActiveCompOnly = activeCompOnly;
}

Selection_Mode SALOME_Selection::SelectionMode()
{
  return mySelectionMode;
}

bool SALOME_Selection::IsSelectActiveCompOnly() const
{
  return mySelActiveCompOnly;
}

bool SALOME_Selection::HasIndex( const Handle(SALOME_InteractiveObject)& IObject )
{
  return myMapIOSubIndex.IsBound(IObject);
}

void SALOME_Selection::GetIndex( const Handle(SALOME_InteractiveObject)& IObject, TColStd_IndexedMapOfInteger& theIndex )
{
  if ( myMapIOSubIndex.IsBound(IObject) ) {
    theIndex = myMapIOSubIndex.Find(IObject);
  }
  else {
    theIndex.Clear();
  }
}



bool SALOME_Selection::IsIndexSelected(const Handle(SALOME_InteractiveObject)& IObject, int index)
{
  if ( !myMapIOSubIndex.IsBound( IObject ) ) {
    return false;
  }
  TColStd_IndexedMapOfInteger& MapIndex = myMapIOSubIndex.ChangeFind( IObject );
  return MapIndex.Contains( index );
}

static bool removeIndex( TColStd_IndexedMapOfInteger& MapIndex,
                        const int                     Index)
{
  int i = MapIndex.FindIndex( Index ); // i==0 if Index is not in the MapIndex
  if ( i ) {
    // only the last key can be removed
    int indexLast = MapIndex.FindKey( MapIndex.Extent() );
    if ( indexLast == Index )
      MapIndex.RemoveLast();
    else {
      TColStd_IndexedMapOfInteger aNewMap;
      aNewMap.ReSize( MapIndex.Extent() - 1 );
      for ( int j = 1; j <= MapIndex.Extent(); j++ ) {
        int ind = MapIndex( j );
        if ( ind != Index )
          aNewMap.Add( ind );
      }
      MapIndex = aNewMap;
    }
  }
  return i;
}


bool SALOME_Selection::AddOrRemoveIndex( const Handle(SALOME_InteractiveObject)& IObject, 
					 int index, 
					 bool modeShift,
					 bool update)
{
  if(MYDEBUG) INFOS( " SALOME_Selection::AddOrRemoveIndex " << index << " - " << modeShift );
  QAD_Desktop*   myDesktop = (QAD_Desktop*) QAD_Application::getDesktop();
  QAD_Study* myActiveStudy = myDesktop->getActiveStudy();

  if ( !myMapIOSubIndex.IsBound( IObject ) ) {
    TColStd_IndexedMapOfInteger Empty;
    myMapIOSubIndex.Bind( IObject, Empty );
  }
  TColStd_IndexedMapOfInteger& MapIndex = myMapIOSubIndex.ChangeFind( IObject );

  bool anIsConatains = MapIndex.Contains( index );

  if (anIsConatains)
    removeIndex( MapIndex, index );
  
  if (!modeShift)
    MapIndex.Clear();
  
  if(!anIsConatains)
    MapIndex.Add( index );

  if ( MapIndex.IsEmpty() ) {
    myMapIOSubIndex.UnBind( IObject );
    RemoveIObject( IObject, update );
  }

  myActiveStudy->highlight( IObject, true, update );	

  emit currentSelectionChanged();
  return false;
}

bool SALOME_Selection::AddOrRemoveIndex( const Handle(SALOME_InteractiveObject)& IObject, 
					 const TColStd_MapOfInteger& theIndices, 
					 bool modeShift,
					 bool update)
{
  QAD_Desktop*   myDesktop = (QAD_Desktop*) QAD_Application::getDesktop();
  QAD_Study* myActiveStudy = myDesktop->getActiveStudy();
  
  if ( !myMapIOSubIndex.IsBound( IObject ) ) {
    TColStd_IndexedMapOfInteger Empty;
    myMapIOSubIndex.Bind( IObject, Empty );
  }
  TColStd_IndexedMapOfInteger& MapIndex = myMapIOSubIndex.ChangeFind( IObject );
  TColStd_MapIteratorOfMapOfInteger It;
  It.Initialize(theIndices);
  
  if (!modeShift)
    MapIndex.Clear();
  
  for(;It.More();It.Next())
    MapIndex.Add(It.Key());
  
  if ( MapIndex.IsEmpty() ) {
    myMapIOSubIndex.UnBind( IObject );
    RemoveIObject( IObject, update );
  }

  myActiveStudy->highlight( IObject, true, update );	

  emit currentSelectionChanged();
  
  return !MapIndex.IsEmpty();
}

bool SALOME_Selection::AddOrRemoveIndex( const Handle(SALOME_InteractiveObject)& IObject, 
					 const TContainerOfId& theIndices, 
					 bool modeShift,
					 bool update)
{
  QAD_Desktop*   myDesktop = (QAD_Desktop*) QAD_Application::getDesktop();
  QAD_Study* myActiveStudy = myDesktop->getActiveStudy();
  
  if ( !myMapIOSubIndex.IsBound( IObject ) ) {
    TColStd_IndexedMapOfInteger Empty;
    myMapIOSubIndex.Bind( IObject, Empty );
  }
  TColStd_IndexedMapOfInteger& MapIndex = myMapIOSubIndex.ChangeFind( IObject );
      
  if (!modeShift)
    MapIndex.Clear();
  
  TContainerOfId::const_iterator anIter = theIndices.begin();
  TContainerOfId::const_iterator anIterEnd = theIndices.end();
  for(; anIter != anIterEnd; anIter++)
    MapIndex.Add(*anIter); 

  if ( MapIndex.IsEmpty() ) {
    myMapIOSubIndex.UnBind( IObject );
    RemoveIObject( IObject, update );
  }

  myActiveStudy->highlight( IObject, true, update );	

  emit currentSelectionChanged();
  
  return !MapIndex.IsEmpty();
}

void SALOME_Selection::RemoveIndex( const Handle(SALOME_InteractiveObject)& IObject, int index )
{
  if ( myMapIOSubIndex.IsBound( IObject ) ) {
    TColStd_IndexedMapOfInteger& MapIndex = myMapIOSubIndex.ChangeFind( IObject );
    removeIndex( MapIndex, index );
  }
}

void SALOME_Selection::ClearIndex()
{
  myMapIOSubIndex.Clear();  
}


//================================================================
// Function : GEOMBase_Helper
// Purpose  : Block signal currentSelectionChanged. Use this method to
//            deactivate signal before big modification of selection.
//            After this modification this signal must be activated and
//            method SelectionChanged must be called to notify other oblects
//            ( dialogs for example )
//================================================================
void SALOME_Selection::BlockSignals( const bool theState )
{
  blockSignals( theState );
}

//================================================================
// Function : SelectionChanged
// Purpose  : Enit signal currentSelectionChanged()
//================================================================
void SALOME_Selection::SelectionChanged()
{
  emit currentSelectionChanged();
}
