//  SALOME VTKViewer : build VTK viewer into Salome desktop
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
//  File   : VTKViewer_ViewFrame.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "VTKViewer_ViewFrame.h"
#include "VTKViewer_Utilities.h"
#include "VTKViewer_Trihedron.h"
#include "VTKViewer_RenderWindow.h"
#include "VTKViewer_InteractorStyleSALOME.h"

#include "SALOME_Transform.h"
#include "SALOME_TransformFilter.h"
#include "SALOME_PassThroughFilter.h"
#include "SALOME_GeometryFilter.h"

#include "QAD_Settings.h"
#include "QAD_Config.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "SALOME_Selection.h"
#include "SALOME_InteractiveObject.hxx"

#include "utilities.h"

//QT Include
#include <qlayout.h>
#include <qcolordialog.h>
#include <qfiledialog.h>
#include <qapplication.h>

// VTK Includes
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>

using namespace std;

/*!
    Constructor
*/
VTKViewer_ViewFrame::VTKViewer_ViewFrame(QWidget* parent, const char* name) 
  :  QAD_ViewFrame(parent, name)
{
  m_ViewUp[0] = 0; m_ViewUp[1] = 0; m_ViewUp[2] = -1;
  m_ViewNormal[0] = 0; m_ViewNormal[1] = 0; m_ViewNormal[2] = 1;
  m_Triedron = VTKViewer_Trihedron::New();
  m_Transform = SALOME_Transform::New();
  //m_Renderer = VTKViewer_Renderer::New() ;
  m_Renderer = vtkRenderer::New() ;

  m_Triedron->AddToRender(m_Renderer);
  InitialSetup();
}  

void VTKViewer_ViewFrame::InitialSetup() {
  m_RW = new VTKViewer_RenderWindow(this, "RenderWindow");
  m_RW->getRenderWindow()->AddRenderer(m_Renderer);

  m_Renderer->GetActiveCamera()->ParallelProjectionOn();
  m_Renderer->LightFollowCameraOn();
  m_Renderer->TwoSidedLightingOn();

  // Set BackgroundColor
  QString BgrColorRed   = QAD_CONFIG->getSetting("VTKViewer:BackgroundColorRed");
  QString BgrColorGreen = QAD_CONFIG->getSetting("VTKViewer:BackgroundColorGreen");
  QString BgrColorBlue  = QAD_CONFIG->getSetting("VTKViewer:BackgroundColorBlue");

  if( !BgrColorRed.isEmpty() && !BgrColorGreen.isEmpty() && !BgrColorBlue.isEmpty() ) 
    m_Renderer->SetBackground( BgrColorRed.toInt()/255., BgrColorGreen.toInt()/255., BgrColorBlue.toInt()/255. );
  else
    m_Renderer->SetBackground( 0, 0, 0 );
  
  // Create an interactor.
  m_RWInteractor = VTKViewer_RenderWindowInteractor::New();
  m_RWInteractor->setGUIWindow(m_RW);
  m_RWInteractor->SetRenderWindow(m_RW->getRenderWindow());

  VTKViewer_InteractorStyleSALOME* RWS = VTKViewer_InteractorStyleSALOME::New();
  RWS->setGUIWindow(m_RW);
  m_RWInteractor->SetInteractorStyle(RWS); 

  m_RWInteractor->Initialize();
  RWS->setTriedron(m_Triedron);
  //SRN: additional initialization, to init CurrentRenderer of vtkInteractorStyle 
  RWS->FindPokedRenderer(0, 0);

  setCentralWidget( m_RW );
  onViewReset();
}

VTKViewer_ViewFrame::~VTKViewer_ViewFrame() {
  m_Transform->Delete() ;
  // In order to ensure that the interactor unregisters
  // this RenderWindow, we assign a NULL RenderWindow to 
  // it before deleting it.
  m_RWInteractor->SetRenderWindow(NULL) ;
  m_RWInteractor->Delete() ;
  
  //m_RW->Delete() ;
  m_Renderer->RemoveAllProps();
  // NRI : BugID 1137:  m_Renderer->Delete() ;
  m_Triedron->Delete();
  MESSAGE("VTKViewer_ViewFrame::~VTKViewer_ViewFrame()");
}

/*!
  Returns widget containing 3D-Viewer
*/
QWidget* VTKViewer_ViewFrame::getViewWidget(){
  return m_RW;
}

bool VTKViewer_ViewFrame::isTrihedronDisplayed(){
  return m_Triedron->GetVisibility() == VTKViewer_Trihedron::eOn;
}

void VTKViewer_ViewFrame::onAdjustTrihedron(){   
  if(!isTrihedronDisplayed()) 
    return;
  int aVisibleNum = m_Triedron->GetVisibleActorCount(m_Renderer);
  if(aVisibleNum){
    // calculating diagonal of visible props of the renderer
    float bnd[6];
    m_Triedron->VisibilityOff();
    ::ComputeVisiblePropBounds(m_Renderer,bnd);
    m_Triedron->VisibilityOn();
    float aLength = 0;
    static bool CalcByDiag = false;
    if(CalcByDiag){
      aLength = sqrt((bnd[1]-bnd[0])*(bnd[1]-bnd[0])+
		     (bnd[3]-bnd[2])*(bnd[3]-bnd[2])+
		     (bnd[5]-bnd[4])*(bnd[5]-bnd[4]));
    }else{
      aLength = bnd[1]-bnd[0];
      aLength = max((bnd[3]-bnd[2]),aLength);
      aLength = max((bnd[5]-bnd[4]),aLength);
    }
   
    static float aSizeInPercents = 105;
    QString aSetting = QAD_CONFIG->getSetting("Viewer:TrihedronSize");
    if(!aSetting.isEmpty()) aSizeInPercents = aSetting.toFloat();

    static float EPS_SIZE = 5.0E-3;
    float aSize = m_Triedron->GetSize();
    float aNewSize = aLength*aSizeInPercents/100.0;
    // if the new trihedron size have sufficient difference, then apply the value
    if(fabs(aNewSize-aSize) > aSize*EPS_SIZE || fabs(aNewSize-aSize) > aNewSize*EPS_SIZE)
      m_Triedron->SetSize(aNewSize);
  }
  m_Triedron->Render(m_Renderer);
  ::ResetCameraClippingRange(m_Renderer);
}

/*!
  Display/hide Trihedron
*/
void VTKViewer_ViewFrame::onViewTrihedron(){
  if(!m_Triedron) return;
  if(isTrihedronDisplayed())
    m_Triedron->VisibilityOff();
  else{
    m_Triedron->VisibilityOn();
  }
  Repaint();
}

/*!
  Provides top projection of the active view
*/
void VTKViewer_ViewFrame::onViewTop(){
  vtkCamera* camera = m_Renderer->GetActiveCamera();
  camera->SetPosition(0,0,1);
  camera->SetViewUp(0,1,0);
  camera->SetFocalPoint(0,0,0);
  onViewFitAll();
}

/*!
  Provides bottom projection of the active view
*/
void VTKViewer_ViewFrame::onViewBottom(){
  vtkCamera* camera = m_Renderer->GetActiveCamera();
  camera->SetPosition(0,0,-1);
  camera->SetViewUp(0,1,0);
  camera->SetFocalPoint(0,0,0);
  onViewFitAll();
}

/*!
  Provides left projection of the active view
*/
void VTKViewer_ViewFrame::onViewLeft(){
  vtkCamera* camera = m_Renderer->GetActiveCamera(); 
  camera->SetPosition(0,1,0);
  camera->SetViewUp(0,0,1);
  camera->SetFocalPoint(0,0,0);
  onViewFitAll();
}

/*!
  Provides right projection of the active view
*/
void VTKViewer_ViewFrame::onViewRight(){
  vtkCamera* camera = m_Renderer->GetActiveCamera();
  camera->SetPosition(0,-1,0);
  camera->SetViewUp(0,0,1);
  camera->SetFocalPoint(0,0,0);
  onViewFitAll();
}

/*!
  Provides back projection of the active view
*/
void VTKViewer_ViewFrame::onViewBack(){
  vtkCamera* camera = m_Renderer->GetActiveCamera();
  camera->SetPosition(-1,0,0);
  camera->SetViewUp(0,0,1);
  camera->SetFocalPoint(0,0,0);
  onViewFitAll();
}

/*!
  Provides front projection of the active view
*/
void VTKViewer_ViewFrame::onViewFront(){
  vtkCamera* camera = m_Renderer->GetActiveCamera();
  camera->SetPosition(1,0,0);
  camera->SetViewUp(0,0,1);
  camera->SetFocalPoint(0,0,0);
  onViewFitAll();
}

/*!
  Fits all objects in the active view
*/
void VTKViewer_ViewFrame::onViewFitAll(){
  m_RWInteractor->GetInteractorStyleSALOME()->ViewFitAll();
//   int aTriedronWasVisible = isTrihedronDisplayed();
//   if(m_Triedron->GetVisibleActorCount(m_Renderer)){
//     m_Triedron->VisibilityOff();
//     ::ResetCamera(m_Renderer);
//   }else{
//     m_Triedron->SetVisibility(VTKViewer_Trihedron::eOnlyLineOn);
//     ::ResetCamera(m_Renderer,true);
//   }
//   if(aTriedronWasVisible) m_Triedron->VisibilityOn();
//   else m_Triedron->VisibilityOff();

  Repaint();
}

/*!
  Reset the active view
*/
void VTKViewer_ViewFrame::onViewReset(){
  int aTriedronIsVisible = isTrihedronDisplayed();
  m_Triedron->SetVisibility(VTKViewer_Trihedron::eOnlyLineOn);
  ::ResetCamera(m_Renderer,true);  
  vtkCamera* aCamera = m_Renderer->GetActiveCamera();
  aCamera->SetPosition(1,-1,1);
  aCamera->SetViewUp(0,0,1);
  ::ResetCamera(m_Renderer,true);  
  if(aTriedronIsVisible) m_Triedron->VisibilityOn();
  else m_Triedron->VisibilityOff();
  static float aCoeff = 3.0;
  aCamera->SetParallelScale(aCoeff*aCamera->GetParallelScale());
  Repaint();
}

/*!
  Rotates the active view
*/
void VTKViewer_ViewFrame::onViewRotate(){
  m_RWInteractor->GetInteractorStyleSALOME()->startRotate();
}

/*!
  Sets a new center of the active view
*/
void VTKViewer_ViewFrame::onViewGlobalPan(){
  if(m_Triedron->GetVisibleActorCount(m_Renderer))
    m_RWInteractor->GetInteractorStyleSALOME()->startGlobalPan();
}

/*!
  Zooms the active view
*/
void VTKViewer_ViewFrame::onViewZoom(){
  m_RWInteractor->GetInteractorStyleSALOME()->startZoom();
}

/*!
  Moves the active view
*/
void VTKViewer_ViewFrame::onViewPan(){
  m_RWInteractor->GetInteractorStyleSALOME()->startPan();
}

/*!
  Fits all obejcts within a rectangular area of the active view
*/
void VTKViewer_ViewFrame::onViewFitArea(){
  m_RWInteractor->GetInteractorStyleSALOME()->startFitArea();
}

/*!
    Set background of the viewport
*/
void VTKViewer_ViewFrame::setBackgroundColor( const QColor& color)
{
  if ( m_Renderer )
    m_Renderer->SetBackground( color.red()/255., color.green()/255., color.blue()/255. );
}

/*!
    Returns background of the viewport
*/
QColor VTKViewer_ViewFrame::backgroundColor() const
{
  float backint[3];
  if ( m_Renderer ) {
    m_Renderer->GetBackground(backint);
    return QColorDialog::getColor ( QColor(int(backint[0]*255), int(backint[1]*255), int(backint[2]*255)), NULL );
  }
  return QMainWindow::backgroundColor();
}


void VTKViewer_ViewFrame::SetSelectionMode( int mode )
{
  m_RWInteractor->SetSelectionMode( mode );
}

void VTKViewer_ViewFrame::rename( const Handle(SALOME_InteractiveObject)& IObject, QString newName )
{
  m_RWInteractor->rename(IObject, newName);
}

void VTKViewer_ViewFrame::unHighlightAll() 
{
  m_RWInteractor->unHighlightAll();
}

void VTKViewer_ViewFrame::highlight( const Handle(SALOME_InteractiveObject)& IObject, 
				     bool highlight, 
				     bool update ) 
{
  QAD_Study* ActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel = SALOME_Selection::Selection( ActiveStudy->getSelection() );
  if ( Sel->SelectionMode() == 4 )
    m_RWInteractor->highlight(IObject, highlight, update);
  else if ( Sel->SelectionMode() == 3 ) {
    m_RWInteractor->highlight(IObject, highlight, update);
    if ( Sel->HasIndex( IObject ) ) {
      TColStd_MapOfInteger MapIndex;
      Sel->GetIndex( IObject, MapIndex );
      m_RWInteractor->highlightCell(IObject, highlight, MapIndex, update);
    }
  } 
  else if ( Sel->SelectionMode() == 2 ) {
    m_RWInteractor->highlight(IObject, highlight, update);
    if ( Sel->HasIndex( IObject ) ) {
      TColStd_MapOfInteger MapIndex;
      Sel->GetIndex( IObject, MapIndex );
      m_RWInteractor->highlightEdge(IObject, highlight, MapIndex, update);
    }
  }
  else if ( Sel->SelectionMode() == 1 ) {
    m_RWInteractor->highlight(IObject, highlight, update);
    if ( Sel->HasIndex( IObject ) ) {
      TColStd_MapOfInteger MapIndex;
      Sel->GetIndex( IObject, MapIndex );
      m_RWInteractor->highlightPoint(IObject, highlight, MapIndex, update);
    }
  }
}

bool VTKViewer_ViewFrame::isInViewer( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  return m_RWInteractor->isInViewer( IObject );
}

bool VTKViewer_ViewFrame::isVisible( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  return m_RWInteractor->isVisible( IObject );
}

void VTKViewer_ViewFrame::setPopupServer( QAD_Application* App )
{
  m_RW->setPopupServer( App );
}

void VTKViewer_ViewFrame::undo(SALOMEDS::Study_var aStudy,
			       const char* StudyFrameEntry)
{
  vtkActorCollection* theActors = m_Renderer->GetActors();
  theActors->InitTraversal();
  vtkActor *ac = theActors->GetNextActor();
  while(!(ac==NULL)) {
    if ( ac->IsA("SALOME_Actor") ) {
      SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
      if ( anActor->hasIO() ) {
	Handle(SALOME_InteractiveObject) IO = anActor->getIO();
	if ( IO->hasEntry() ) { 
	  /*if (!QAD_ViewFrame::isInViewer(aStudy, IO->getEntry(), StudyFrameEntry)) {
	    m_RWInteractor->Erase(IO);
	    }*/
	}
      }
    }
    ac = theActors->GetNextActor();
  }
}

void VTKViewer_ViewFrame::redo(SALOMEDS::Study_var aStudy,
			       const char* StudyFrameEntry)
{
  SALOMEDS::SObject_var RefSO;
  SALOMEDS::SObject_var SO = aStudy->FindObjectID( StudyFrameEntry );
  SALOMEDS::ChildIterator_var it = aStudy->NewChildIterator(SO);
  for (; it->More();it->Next()){
    SALOMEDS::SObject_var CSO= it->Value();
    if (CSO->ReferencedObject(RefSO)) {
      vtkActorCollection* theActors = m_Renderer->GetActors();
      theActors->InitTraversal();
      vtkActor *ac = theActors->GetNextActor();
      while(!(ac==NULL)) {
	if ( ac->IsA("SALOME_Actor") ) {
	  SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	  if ( anActor->hasIO() ) {
	    Handle(SALOME_InteractiveObject) IO = anActor->getIO();
	    if ( IO->hasEntry() ) { 
	      /*if ( strcmp(IO->getEntry(),RefSO->GetID()) == 0 )
		m_RWInteractor->Display(IO);*/
	    }
	  }
	}
	ac = theActors->GetNextActor();
      }
    }
  }
}


/* selection */
Handle(SALOME_InteractiveObject) VTKViewer_ViewFrame::FindIObject(const char* Entry)
{
  Handle(SALOME_InteractiveObject) IO;
  vtkActorCollection* theActors = m_Renderer->GetActors();
  theActors->InitTraversal();
  vtkActor *ac = theActors->GetNextActor();
  while(!(ac==NULL)) {
    if ( ac->IsA("SALOME_Actor") ) {
      SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
      if ( anActor->hasIO() ) {
	IO = anActor->getIO();
	if ( IO->hasEntry() ) {
	  if ( strcmp( IO->getEntry(), Entry ) == 0 ) {
	    return IO;
	  }
	}
      }
    }
    ac = theActors->GetNextActor();
  }
  return IO;
}

/* display */		
void VTKViewer_ViewFrame::Display(const Handle(SALOME_InteractiveObject)& IObject, bool update)
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );

  vtkActorCollection* theActors = m_Renderer->GetActors();
  theActors->InitTraversal();
  vtkActor *ac = theActors->GetNextActor();
  while(!(ac==NULL))
    {
      if ( ac->IsA("SALOME_Actor") )
	{
	  SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	  if ( anActor->hasIO() ) 
	    {
	      Handle(SALOME_InteractiveObject) IO = anActor->getIO();
	      if ( IO->isSame(IObject) )
		{
		  m_RWInteractor->Display(IO, false);
		  Sel->AddIObject(IO, false);
		  break;
		}
	    }
	}
      ac = theActors->GetNextActor();
    }
  if (update)
    Repaint();
}


void VTKViewer_ViewFrame::DisplayOnly(const Handle(SALOME_InteractiveObject)& IObject)
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );

  vtkActorCollection* theActors = m_Renderer->GetActors();
  theActors->InitTraversal();
  vtkActor *ac = theActors->GetNextActor();
  while(!(ac==NULL)) {
    if ( ac->IsA("SALOME_Actor") ) {
      SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
      if ( anActor->hasIO() ) {
	Handle(SALOME_InteractiveObject) IO = anActor->getIO();
	if ( !IO->isSame(IObject) ) {
	  m_RWInteractor->Erase(IO, false);
	  Sel->RemoveIObject(IO, false);
	} else {
	  anActor->SetVisibility(true);
	  Sel->AddIObject(IO, false);
	}
      }
    }
    ac = theActors->GetNextActor();
  }
  Repaint();
}

void VTKViewer_ViewFrame::Erase(const Handle(SALOME_InteractiveObject)& IObject, bool update)
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );

  vtkActorCollection* theActors = m_Renderer->GetActors();
  theActors->InitTraversal();
  vtkActor *ac = theActors->GetNextActor();
  while(!(ac==NULL)) 
    {
      if ( ac->IsA("SALOME_Actor") )
	{
	  SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	  if ( anActor->hasIO() )
	    {
	      Handle(SALOME_InteractiveObject) IO = anActor->getIO();
	      if ( IO->isSame( IObject ) ) {
		m_RWInteractor->Erase(IO, false);
		Sel->RemoveIObject(IO, false);
		break;
	      }
	    }
	}
      ac = theActors->GetNextActor();
    }
  if (update)
    Repaint();
}


void VTKViewer_ViewFrame::DisplayAll()
{
  m_RWInteractor->DisplayAll();
}


void VTKViewer_ViewFrame::EraseAll()
{
  m_RWInteractor->EraseAll();
}


void VTKViewer_ViewFrame::Repaint(bool theUpdateTrihedron)
{
  if (theUpdateTrihedron) onAdjustTrihedron();
  m_RW->update();
}

void VTKViewer_ViewFrame::GetScale(double theScale[3]){
  m_Transform->GetScale(theScale);
}

void VTKViewer_ViewFrame::SetScale(double theScale[3]){
  m_Transform->SetScale(theScale[0], theScale[1], theScale[2]);
  m_RWInteractor->Render();
  Repaint();
}

void VTKViewer_ViewFrame::AddActor( SALOME_Actor* theActor, bool update /*=false*/ ){
  theActor->SetVisibility(true);
  theActor->AddToRender(m_Renderer);
  theActor->SetTransform(m_Transform);
  if(update) Repaint();
}

void VTKViewer_ViewFrame::RemoveActor( SALOME_Actor* theActor, bool update /*=false*/ ){
  theActor->RemoveFromRender(m_Renderer);
  if(update) Repaint();
}


