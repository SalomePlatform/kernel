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
//  File   : SALOMEGUI.cxx
//  Author : Sergey ANIKIN
//  Module : SALOME
//  $Header$

#include "SALOMEGUI.h"
#include "QAD_Desktop.h"


//=============================================================================
/*!
 *
 */
//=============================================================================
SALOMEGUI::SALOMEGUI( const QString& name, QObject* parent )
: QObject( parent ),
  myName( name )
{
}

//=============================================================================
/*!
 *
 */
//=============================================================================
SALOMEGUI::~SALOMEGUI()
{
}

//=============================================================================
/*!
 *
 */
//=============================================================================
bool SALOMEGUI::OnGUIEvent(int theCommandID, QAD_Desktop* parent)
{
  return true;
}

//=============================================================================
/*!
 *
 */
//=============================================================================
bool SALOMEGUI::OnKeyPress(QKeyEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
{
  return true;
}

//=============================================================================
/*!
 *
 */
//=============================================================================
bool SALOMEGUI::OnMousePress(QMouseEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
{
  return true;
}

//=============================================================================
/*!
 *
 */
//=============================================================================
bool SALOMEGUI::OnMouseMove(QMouseEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
{
  return true;
}

//=============================================================================
/*!
 *
 */
//=============================================================================
bool SALOMEGUI::SetSettings( QAD_Desktop* parent )
{
  return true;
}

//=============================================================================
/*!
 *
 */
//=============================================================================
bool SALOMEGUI::SetSettings( QAD_Desktop* parent, char* compName )
{
  return SetSettings( parent);
}

//=============================================================================
/*!
 *
 */
//=============================================================================
bool SALOMEGUI::CustomPopup( QAD_Desktop* parent, QPopupMenu* popup, const QString & theContext,
                                   const QString & theParent, const QString & theObject )
{
  return true;
}

//=============================================================================
/*!
 *
 */
//=============================================================================
void SALOMEGUI::DefinePopup( QString & theContext, QString & theParent, QString & theObject )
{
}

//=============================================================================
/*!
 *
 */
//=============================================================================
bool SALOMEGUI::ActiveStudyChanged( QAD_Desktop* parent )
{
  //static QAD_Study* aPrevStudy = NULL;
  //QAD_Study* aStudy = parent->getActiveStudy();
  //if(!aPrevStudy)
  //  aPrevStudy = aStudy;
  //if(aStudy != aPrevStudy){
  //  emit SignalCloseAllDialogs();
  //  aPrevStudy = aStudy;
  //}
  emit SignalCloseAllDialogs();
  return true;
}

//=============================================================================
/*!
 *
 */
//=============================================================================
void SALOMEGUI::BuildPresentation( const Handle(SALOME_InteractiveObject)& theIO )
{
}

//=============================================================================
/*!
 *
 */
//=============================================================================
void SALOMEGUI::SupportedViewType(int* buffer, int bufferSize)
{
}

//=============================================================================
/*!
 *
 */
//=============================================================================
void SALOMEGUI::Deactivate()
{
  emit SignalCloseAllDialogs();
}

