// File:	SALOME_Prs.cxx
// Created:	Wed Apr 28 15:03:43 2004
// Author:	Sergey ANIKIN
//		<san@startrex.nnov.opencascade.com>


#include "SALOME_Prs.h"

#include "utilities.h"

using namespace std;

//===========================================================
/*!
 *  Function: SALOME_OCCPrs::DisplayIn
 *  Purpose:  Dispatches display operation to proper Display()
 *            method of SALOME_View
 */
//===========================================================
void SALOME_OCCPrs::DisplayIn( SALOME_View* v ) const
{
  if ( v ) v->Display( this );
}

//===========================================================
/*!
 *  Function: SALOME_OCCPrs::EraseIn
 *  Purpose:  Dispatches display operation to proper Erase()
 *            method of SALOME_View
 */
//===========================================================
void SALOME_OCCPrs::EraseIn( SALOME_View* v, const bool forced ) const
{
  if ( v ) v->Erase( this, forced );
}

//===========================================================
/*!
 *  Function: SALOME_OCCPrs::LocalSelectionIn
 *  Purpose:  Dispatches operation to proper LocalSelectionIn()
 *            method of SALOME_View
 */
//===========================================================
void SALOME_OCCPrs::LocalSelectionIn( SALOME_View* v, const int mode ) const
{
  if ( v ) v->LocalSelection( this, mode );
}

//===========================================================
/*!
 *  Function: SALOME_OCCPrs::Update
 *  Purpose:  Dispatches update operation to proper Update()
 *            method of SALOME_Displayer
 */
//===========================================================
void SALOME_OCCPrs::Update( SALOME_Displayer* d )
{
  if ( d ) d->Update( this );
}

//===========================================================
/*!
 *  Function: SALOME_VTKPrs::DisplayIn
 *  Purpose:  Dispatches display operation to proper Display()
 *            method of SALOME_View
 */
//===========================================================
void SALOME_VTKPrs::DisplayIn( SALOME_View* v ) const
{
  if ( v ) v->Display( this );
}

//===========================================================
/*!
 *  Function: SALOME_VTKPrs::EraseIn
 *  Purpose:  Dispatches display operation to proper Erase()
 *            method of SALOME_View
 */
//===========================================================
void SALOME_VTKPrs::EraseIn( SALOME_View* v, const bool forced ) const
{
  if ( v ) v->Erase( this, forced );
}

//===========================================================
/*!
 *  Function: SALOME_VTKPrs::LocalSelectionIn
 *  Purpose:  Dispatches operation to proper LocalSelectionIn()
 *            method of SALOME_View
 */
//===========================================================
void SALOME_VTKPrs::LocalSelectionIn( SALOME_View* v, const int mode ) const
{
  if ( v ) v->LocalSelection( this, mode );
}

//===========================================================
/*!
 *  Function: SALOME_VTKPrs::Update
 *  Purpose:  Dispatches update operation to proper Update()
 *            method of SALOME_Displayer
 */
//===========================================================
void SALOME_VTKPrs::Update( SALOME_Displayer* d )
{
  if ( d ) d->Update( this );
}

//===========================================================
/*!
 *  Function: SALOME_Prs2d::DisplayIn
 *  Purpose:  Dispatches display operation to proper Display()
 *            method of SALOME_View
 */
//===========================================================
void SALOME_Prs2d::DisplayIn( SALOME_View* v ) const
{
  if ( v ) v->Display( this );
}

//===========================================================
/*!
 *  Function: SALOME_Prs2d::EraseIn
 *  Purpose:  Dispatches display operation to proper Erase()
 *            method of SALOME_View
 */
//===========================================================
void SALOME_Prs2d::EraseIn( SALOME_View* v, const bool forced ) const
{
  if ( v ) v->Erase( this, forced );
}

//===========================================================
/*!
 *  Function: SALOME_Prs2d::LocalSelectionIn
 *  Purpose:  Dispatches operation to proper LocalSelectionIn()
 *            method of SALOME_View
 */
//===========================================================
void SALOME_Prs2d::LocalSelectionIn( SALOME_View* v, const int mode ) const
{
  if ( v ) v->LocalSelection( this, mode );
}

//===========================================================
/*!
 *  Function: SALOME_Prs2d::Update
 *  Purpose:  Dispatches update operation to proper Update()
 *            method of SALOME_Displayer
 */
//===========================================================
void SALOME_Prs2d::Update( SALOME_Displayer* d )
{
  if ( d ) d->Update( this );
}

//===========================================================
/*!
 *  Function: SALOME_View::Display
 *  Purpose:  Gives control to SALOME_Prs object, so that
 *            it could perform double dispatch
 */
//===========================================================
void SALOME_View::Display( const SALOME_Prs* prs )
{
  prs->DisplayIn( this );
}

//===========================================================
/*!
 *  Function: SALOME_View::Erase
 *  Purpose:  Gives control to SALOME_Prs object, so that
 *            it could perform double dispatch
 */
//===========================================================
void SALOME_View::Erase( const SALOME_Prs* prs, const bool forced )
{
  prs->EraseIn( this, forced );
}

//===========================================================
/*!
 *  Function: SALOME_View::LocalSelection
 *  Purpose:  Gives control to SALOME_Prs object, so that
 *            it could perform double dispatch
 */
//===========================================================
void SALOME_View::LocalSelection( const SALOME_Prs* prs, const int mode )
{
  prs->LocalSelectionIn( this, mode );
}

//===========================================================
/*!
 *  Function: SALOME_View::Display
 *  Purpose:  Virtual method, should be reimplemented in successors,
 *            by default issues a warning and does nothing.
 */
//===========================================================
void SALOME_View::Display( const SALOME_OCCPrs* )
{
  MESSAGE( "SALOME_View::Display( const SALOME_OCCPrs& ) called! Probably, presentation is being displayed in uncompatible viewframe." );
}

//===========================================================
/*!
 *  Function: SALOME_View::Display
 *  Purpose:  Virtual method, should be reimplemented in successors,
 *            by default issues a warning and does nothing.
 */
//===========================================================
void SALOME_View::Display( const SALOME_VTKPrs* )
{
  MESSAGE( "SALOME_View::Display( const SALOME_VTKPrs& ) called! Probably, presentation is being displayed in uncompatible viewframe." );
}

//===========================================================
/*!
 *  Function: SALOME_View::Display
 *  Purpose:  Virtual method, should be reimplemented in successors,
 *            by default issues a warning and does nothing.
 */
//===========================================================
void SALOME_View::Display( const SALOME_Prs2d* )
{
  MESSAGE( "SALOME_View::Display( const SALOME_Prs2d& ) called! Probably, presentation is being displayed in uncompatible viewframe." );
}

//===========================================================
/*!
 *  Function: SALOME_View::Erase
 *  Purpose:  Virtual method, should be reimplemented in successors,
 *            by default issues a warning and does nothing.
 */
//===========================================================
void SALOME_View::Erase( const SALOME_OCCPrs*, const bool )
{
  MESSAGE( "SALOME_View::Erase( const SALOME_OCCPrs& ) called! Probably, presentation is being erased in uncompatible viewframe." );
}

//===========================================================
/*!
 *  Function: SALOME_View::Erase
 *  Purpose:  Virtual method, should be reimplemented in successors,
 *            by default issues a warning and does nothing.
 */
//===========================================================
void SALOME_View::Erase( const SALOME_VTKPrs*, const bool )
{
  MESSAGE( "SALOME_View::Erase( const SALOME_VTKPrs& ) called! Probably, presentation is being erased in uncompatible viewframe." );
}

//===========================================================
/*!
 *  Function: SALOME_View::Erase
 *  Purpose:  Virtual method, should be reimplemented in successors,
 *            by default issues a warning and does nothing.
 */
//===========================================================
void SALOME_View::Erase( const SALOME_Prs2d*, const bool )
{
  MESSAGE( "SALOME_View::Erase( const SALOME_Prs2d& ) called! Probably, presentation is being erased in uncompatible viewframe." );
}

//===========================================================
/*!
 *  Function: SALOME_View::LocalSelection
 *  Purpose:  Virtual method, should be reimplemented in successors,
 *            by default issues a warning and does nothing.
 */
//===========================================================
void SALOME_View::LocalSelection( const SALOME_OCCPrs*, const int )
{
  MESSAGE( "SALOME_View::LocalSelection( const SALOME_OCCPrs* ) called! \
   Probably, selection is being activated in uncompatible viewframe." );
}

//===========================================================
/*!
 *  Function: SALOME_View::LocalSelection
 *  Purpose:  Virtual method, should be reimplemented in successors,
 *            by default issues a warning and does nothing.
 */
//===========================================================
void SALOME_View::LocalSelection( const SALOME_VTKPrs*, const int )
{
  MESSAGE( "SALOME_View::LocalSelection( const SALOME_VTKPrs* ) called! \
   Probably, selection is being activated in uncompatible viewframe." );
}

//===========================================================
/*!
 *  Function: SALOME_View::LocalSelection
 *  Purpose:  Virtual method, should be reimplemented in successors,
 *            by default issues a warning and does nothing.
 */
//===========================================================
void SALOME_View::LocalSelection( const SALOME_Prs2d*, const int )
{
  MESSAGE( "SALOME_View::LocalSelection( const SALOME_Prs2d* ) called! \
   Probably, selection is being activated in uncompatible viewframe." );
}

//===========================================================
/*!
 *  Function: SALOME_View::GlobalSelection
 *  Purpose:  Virtual method, should be reimplemented in successors,
 *            by default issues a warning and does nothing.
 */
//===========================================================
void SALOME_View::GlobalSelection( const bool ) const
{
  MESSAGE( "SALOME_View::GlobalSelection() called! \
   Probably, selection is being activated in uncompatible viewframe." );
}

//===========================================================
/*!
 *  Function: SALOME_Displayer::UpdatePrs
 *  Purpose:  Gives control to SALOME_Prs object, so that
 *            it could perform double dispatch
 */
//===========================================================
void SALOME_Displayer::UpdatePrs( SALOME_Prs* prs )
{
  prs->Update( this );
}

//===========================================================
/*!
 *  Function: SALOME_Displayer::Update
 *  Purpose:  Virtual method, should be reimplemented in successors,
 *            by default issues a warning and does nothing.
 */
//===========================================================
void SALOME_Displayer::Update( SALOME_OCCPrs* )
{
  MESSAGE( "SALOME_Displayer::Update( SALOME_OCCPrs* ) called! Probably, presentation is being updated in uncompatible viewframe." );
}

//===========================================================
/*!
 *  Function: SALOME_Displayer::Update
 *  Purpose:  Virtual method, should be reimplemented in successors,
 *            by default issues a warning and does nothing.
 */
//===========================================================
void SALOME_Displayer::Update( SALOME_VTKPrs* )
{
  MESSAGE( "SALOME_Displayer::Update( SALOME_VTKPrs* ) called! Probably, presentation is being updated in uncompatible viewframe." );
}

//===========================================================
/*!
 *  Function: SALOME_Displayer::Update
 *  Purpose:  Virtual method, should be reimplemented in successors,
 *            by default issues a warning and does nothing.
 */
//===========================================================
void SALOME_Displayer::Update( SALOME_Prs2d* )
{
  MESSAGE( "SALOME_Displayer::Update( SALOME_Prs2d* ) called! Probably, presentation is being updated in uncompatible viewframe." );
}

