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
//  File   : VTKViewer_RenderWindowInteractor.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "VTKViewer_RenderWindowInteractor.h"
#include "VTKViewer_RenderWindow.h"
#include "VTKViewer_InteractorStyleSALOME.h"

#include "QAD_Application.h"
#include "QAD_Desktop.h"

#include "SALOME_Selection.h"
#include "SALOME_Actor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// VTK Includes
#include <vtkAssemblyNode.h>
#include <vtkActor.h>
#include <vtkInteractorStyle.h>
#include <vtkObjectFactory.h>
#include <vtkPicker.h>
#include <vtkCellPicker.h>
#include <vtkPointPicker.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>
#include <vtkDataSet.h>
#include <vtkMaskPoints.h>
#include <vtkVertex.h>
#include <vtkRendererCollection.h>
#include <vtkPolyDataWriter.h>

// QT Includes
#include <qkeycode.h>

using namespace std;

VTKViewer_RenderWindowInteractor* VTKViewer_RenderWindowInteractor::New() {
  vtkObject *ret = vtkObjectFactory::CreateInstance("VTKViewer_RenderWindowInteractor") ;
  if( ret ) {
    return dynamic_cast<VTKViewer_RenderWindowInteractor *>(ret) ;
  }
  return new VTKViewer_RenderWindowInteractor;
}

VTKViewer_RenderWindowInteractor::VTKViewer_RenderWindowInteractor() {
  this->Enabled = 0 ;
  this->mTimer = new QTimer( this ) ;
  this->displaymode = 0;
  myGUIWindow = 0;
  vtkPicker* thepicker = vtkPicker::New();
  thepicker->SetTolerance(0);
  this->SetPicker(thepicker);

  connect(mTimer, SIGNAL(timeout()), this, SLOT(TimerFunc())) ;
}


VTKViewer_RenderWindowInteractor::~VTKViewer_RenderWindowInteractor() {
  delete mTimer ;
}

void VTKViewer_RenderWindowInteractor::PrintSelf(ostream& os, vtkIndent indent) {
  vtkRenderWindowInteractor::PrintSelf(os, indent) ;
  //
  // :NOTE: Fri Apr 21 21:51:05 2000 Pagey
  // QGL specific stuff goes here. One should add output 
  // lines here if any protected members are added to
  // the class. 
  //
}

//
// We never allow the VTKViewer_RenderWindowInteractor to control 
// the event loop. The application always has the control. 
//
void VTKViewer_RenderWindowInteractor::Initialize() {
  //
  // We cannot do much unless there is a render window 
  // associated with this interactor. 
  //
  if( ! RenderWindow ) {
    vtkErrorMacro(<< "VTKViewer_RenderWindowInteractor::Initialize(): No render window attached!") ;
    return ;
  }

  //
  // We cannot hand a render window which is not a VTKViewer_RenderWindow. 
  // One way to force this is to use dynamic_cast and hope that 
  // it works. If the dynamic_cast does not work, we flag an error
  // and get the hell out.
  //
  vtkRenderWindow *my_render_win = dynamic_cast<vtkRenderWindow *>(RenderWindow) ;
  if( !my_render_win ) {
    vtkErrorMacro(<< "VTKViewer_RenderWindowInteractor::Initialize() can only handle VTKViewer_RenderWindow.") ;
    return ;
  }

  //
  // If the render window has zero size, then set it to a default 
  // value of 300x300.
  // 
  int* aSize = my_render_win->GetSize();
  this->Size[0] = ((aSize[0] > 0) ? aSize[0] : 300);
  this->Size[1] = ((aSize[1] > 0) ? aSize[1] : 300);

  //
  // Enable the interactor. 
  //
  this->Enable() ;

  //
  // Start the rendering of the window. 
  //
  my_render_win->Start() ;

  //
  // The interactor has been initialized.
  //
  this->Initialized = 1 ;

  Cell_Actor = vtkActor::New(); 
  Cell_Actor->PickableOff();
  Cell_Actor->GetProperty()->SetColor(1,1,0);
  Cell_Actor->GetProperty()->SetLineWidth(5);
  Cell_Actor->GetProperty()->SetRepresentationToSurface();

  Edge_Actor = vtkActor::New(); 
  Edge_Actor->PickableOff();
  Edge_Actor->GetProperty()->SetColor(1,0,0);
  Edge_Actor->GetProperty()->SetLineWidth(5);
  Edge_Actor->GetProperty()->SetRepresentationToWireframe();

  Point_Actor = vtkActor::New(); 
  Point_Actor->PickableOff();
  Point_Actor->GetProperty()->SetColor(1,1,0);
  Point_Actor->GetProperty()->SetPointSize(5);
  Point_Actor->GetProperty()->SetRepresentationToPoints();

  return ;
}


void VTKViewer_RenderWindowInteractor::SetInteractorStyle(vtkInteractorObserver *theInteractor){
  myInteractorStyle = dynamic_cast<VTKViewer_InteractorStyleSALOME*>(theInteractor);
  vtkRenderWindowInteractor::SetInteractorStyle(theInteractor);
}


void VTKViewer_RenderWindowInteractor::SetSelectionMode(int mode)
{
  Cell_Actor->VisibilityOff();
  Edge_Actor->VisibilityOff();
  Point_Actor->VisibilityOff();

  selectionmode = mode;
  if ( mode == 1 ) {
    vtkPointPicker* thepicker = vtkPointPicker::New();
    //    thepicker->SetTolerance(0.001);
    this->SetPicker(thepicker);
  } else if ( mode == 2 ) {
    vtkCellPicker* thepicker = vtkCellPicker::New();
    thepicker->SetTolerance(0.001);
    this->SetPicker(thepicker);
  } else if ( mode == 3 ) {
    vtkCellPicker* thepicker = vtkCellPicker::New();
    thepicker->SetTolerance(0.001); 
    this->SetPicker(thepicker);
  } else if ( mode == 4 ) {
    vtkPicker* thepicker = vtkPicker::New();
    thepicker->SetTolerance(0.001);
    this->SetPicker(thepicker);
  }
}


void VTKViewer_RenderWindowInteractor::Enable() {
  //
  // Do not need to do anything if already enabled.
  //
  if( this->Enabled ) {
    return ;
  }
  
  //
  // Attach slots to every useful signal of the render window. 
  //
  this->ConnectSlots() ;
  
  this->Enabled = 1 ;
  this->Modified() ;
}

void VTKViewer_RenderWindowInteractor::Disable() {
  if( ! this->Enabled ) {
    return ;
  }

  this->DisconnectSlots() ;
  this->Enabled = 0 ;
  this->Modified() ;
}

// ================================== 
void VTKViewer_RenderWindowInteractor::Start() {
  //
  // We do not allow this interactor to control the 
  // event loop. Only the QtApplication objects are
  // allowed to do that. 
  //
  vtkErrorMacro(<<"VTKViewer_RenderWindowInteractor::Start() not allowed to start event loop.") ;
  return ;
}

void VTKViewer_RenderWindowInteractor::UpdateSize(int w, int h) {
  // if the size changed send this on to the RenderWindow
  if ((w != this->Size[0])||(h != this->Size[1])) {
    this->Size[0] = w;
    this->Size[1] = h;
    this->RenderWindow->SetSize(w,h);
  }
}

int VTKViewer_RenderWindowInteractor::CreateTimer(int vtkNotUsed(timertype)) {
  //
  // Start a one-shot timer for 10ms. 
  //
  mTimer->start(10, TRUE) ;
  return 1 ;
}

int VTKViewer_RenderWindowInteractor::DestroyTimer(void) {
  //
  // :TRICKY: Tue May  2 00:17:32 2000 Pagey
  //
  // QTimer will automatically expire after 10ms. So 
  // we do not need to do anything here. In fact, we 
  // should not even Stop() the QTimer here because doing 
  // this will skip some of the processing that the TimerFunc()
  // does and will result in undesirable effects. For 
  // example, this will result in vtkLODActor to leave
  // the models in low-res mode after the mouse stops
  // moving. 
  //
  return 1 ;
}

void VTKViewer_RenderWindowInteractor::TimerFunc() {
  if( ! this->Enabled ) {
    return ;
  }

  ((vtkInteractorStyle*)this->InteractorStyle)->OnTimer() ;
  emit RenderWindowModified() ;
}

void VTKViewer_RenderWindowInteractor::ConnectSlots() {
  ProcessSlotConnections(true) ;
}

void VTKViewer_RenderWindowInteractor::DisconnectSlots() {
  ProcessSlotConnections(false) ;
}

void VTKViewer_RenderWindowInteractor::ProcessSlotConnections(bool conn) {
  //
  // We cannot do much unless there is a render window 
  // associated with this interactor. 
  //
  if( ! myGUIWindow ) {
    vtkErrorMacro(<< "VTKViewer_RenderWindowInteractor::Initialize(): No render window attached!") ;
    return ;
  }

  
  bool (*slot_func) ( const QObject * sender, 
		      const char * signal, 
		      const QObject * receiver, 
		      const char * member ) ;
  if( conn ) {
    slot_func = &QObject::connect ;
  } else {
    slot_func = &QObject::disconnect ;
  }
  //
  // We cannot hand a render window which is not a VTKViewer_RenderWindow. 
  // One way to force this is to use dynamic_cast and hope that 
  // it works. If the dynamic_cast does not work, we flag an error
  // and get the hell out.
  //
  if( ! myGUIWindow ) {
    vtkErrorMacro(<< "VTKViewer_RenderWindowInteractor::Initialize() can only handle VTKViewer_RenderWindow.") ;
    return ;
  }

  slot_func ( myGUIWindow, SIGNAL(ButtonPressed(const QMouseEvent *)), 
	      this,          SLOT(ButtonPressed(const QMouseEvent *)) ) ;
  slot_func( myGUIWindow, SIGNAL(ButtonReleased(const QMouseEvent *)), 
	     this,          SLOT(ButtonReleased(const QMouseEvent *)) ) ;
  
  
  slot_func( myGUIWindow, SIGNAL(LeftButtonPressed(const QMouseEvent *)), 
	     this,          SLOT(LeftButtonPressed(const QMouseEvent *)) ) ;
  slot_func( myGUIWindow, SIGNAL(LeftButtonReleased(const QMouseEvent *)), 
	     this,          SLOT(LeftButtonReleased(const QMouseEvent *)) ) ;
  
  slot_func( myGUIWindow, SIGNAL(MiddleButtonPressed(const QMouseEvent *)), 
	     this,          SLOT(MiddleButtonPressed(const QMouseEvent *)) ) ;
  slot_func( myGUIWindow, SIGNAL(MiddleButtonReleased(const QMouseEvent *)), 
	     this,          SLOT(MiddleButtonReleased(const QMouseEvent *)) ) ;
  
  slot_func( myGUIWindow, SIGNAL(RightButtonPressed(const QMouseEvent *)), 
	     this,          SLOT(RightButtonPressed(const QMouseEvent *)) ) ;
  slot_func( myGUIWindow, SIGNAL(RightButtonReleased(const QMouseEvent *)), 
	     this,          SLOT(RightButtonReleased(const QMouseEvent *)) ) ;
  
  slot_func( myGUIWindow, SIGNAL(MouseMove(const QMouseEvent *)), 
	     this,          SLOT(MouseMove(const QMouseEvent *)) ) ;
  
  slot_func( myGUIWindow, SIGNAL(KeyPressed(QKeyEvent *)),
	     this,          SLOT(KeyPressed(QKeyEvent *)) ) ;
  
  slot_func( this,          SIGNAL(RenderWindowModified()), 
	     myGUIWindow, SLOT(update()) ) ;

}

void VTKViewer_RenderWindowInteractor::MouseMove(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  VTKViewer_InteractorStyleSALOME* Style = 0;
  if (this->InteractorStyle->IsA("VTKViewer_InteractorStyleSALOME"))
    Style = VTKViewer_InteractorStyleSALOME::SafeDownCast(this->InteractorStyle);
  else {
    emit RenderWindowModified() ; 
    return;
  }
  Style->OnMouseMove(0, 0, event->x(), event->y()/*this->Size[1] - event->y() - 1*/) ;
  if (Style->needsRedrawing() )
    emit RenderWindowModified() ; 
}

void VTKViewer_RenderWindowInteractor::LeftButtonPressed(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  VTKViewer_InteractorStyleSALOME* Style = 0;
  if (this->InteractorStyle->IsA("VTKViewer_InteractorStyleSALOME"))
    Style = VTKViewer_InteractorStyleSALOME::SafeDownCast(this->InteractorStyle);
  else 
    return;
  Style->OnLeftButtonDown((event->state() & ControlButton), 
			  (event->state() & ShiftButton), 
			  event->x(), event->y());
}

void VTKViewer_RenderWindowInteractor::LeftButtonReleased(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  VTKViewer_InteractorStyleSALOME* Style = 0;
  if (this->InteractorStyle->IsA("VTKViewer_InteractorStyleSALOME"))
    Style = VTKViewer_InteractorStyleSALOME::SafeDownCast(this->InteractorStyle);
  else 
    return;
  Style->OnLeftButtonUp( (event->state() & ControlButton), 
			 (event->state() & ShiftButton), 
			 event->x(), event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::MiddleButtonPressed(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  VTKViewer_InteractorStyleSALOME* Style = 0;
  if (this->InteractorStyle->IsA("VTKViewer_InteractorStyleSALOME"))
    Style = VTKViewer_InteractorStyleSALOME::SafeDownCast(this->InteractorStyle);
  else 
    return;
  Style->OnMiddleButtonDown((event->state() & ControlButton), 
			    (event->state() & ShiftButton), 
			    event->x(), event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::MiddleButtonReleased(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  VTKViewer_InteractorStyleSALOME* Style = 0;
  if (this->InteractorStyle->IsA("VTKViewer_InteractorStyleSALOME"))
    Style = VTKViewer_InteractorStyleSALOME::SafeDownCast(this->InteractorStyle);
  else 
    return;
  Style->OnMiddleButtonUp( (event->state() & ControlButton), 
			   (event->state() & ShiftButton), 
			   event->x(), event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::RightButtonPressed(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  VTKViewer_InteractorStyleSALOME* Style = 0;
  if (this->InteractorStyle->IsA("VTKViewer_InteractorStyleSALOME"))
    Style = VTKViewer_InteractorStyleSALOME::SafeDownCast(this->InteractorStyle);
  else 
    return;
  Style->OnRightButtonDown( (event->state() & ControlButton), 
			    (event->state() & ShiftButton), 
			    event->x(), event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::RightButtonReleased(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  VTKViewer_InteractorStyleSALOME* Style = 0;
  if (this->InteractorStyle->IsA("VTKViewer_InteractorStyleSALOME"))
    Style = VTKViewer_InteractorStyleSALOME::SafeDownCast(this->InteractorStyle);
  else 
    return;
  Style->OnRightButtonUp( (event->state() & ControlButton), 
			  (event->state() & ShiftButton), 
			  event->x(), event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::ButtonPressed(const QMouseEvent *event) {
  return ;
}

void VTKViewer_RenderWindowInteractor::ButtonReleased(const QMouseEvent *event) {
  return ;
}

void VTKViewer_RenderWindowInteractor::ChangeRepresentationToWireframe(vtkActorCollection* ac)
{
  vtkActor* anActor;

  for (ac->InitTraversal(); (anActor = ac->GetNextActor()); ) {
    if ( anActor->IsA("GEOM_Actor") ) {
      SALOME_Actor* SALOMEactor = SALOME_Actor::SafeDownCast(anActor);
      // Specific for GEOM actor
      if ( SALOMEactor->getDisplayMode() == 1 )
  	SALOMEactor->setDisplayMode(0);
    } else {
      anActor->GetProperty()->SetRepresentationToWireframe();
    }
  }
  this->RenderWindow->Render();
  emit RenderWindowModified() ;
}

int VTKViewer_RenderWindowInteractor::GetDisplayMode() {
  return displaymode;
}

void VTKViewer_RenderWindowInteractor::SetDisplayMode(int mode) {
  if(mode==0) ChangeRepresentationToWireframe();
  else ChangeRepresentationToSurface();
  displaymode = mode;
}

void VTKViewer_RenderWindowInteractor::SwitchRepresentation(const Handle(SALOME_InteractiveObject)& IObject,
							    bool update) {  
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    while(!(ac==NULL)) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    if ( anActor->IsA("GEOM_Actor") ) {
	      // GEOM actor
	      if ( anActor->getDisplayMode() == 0 ) anActor->setDisplayMode(1);
	      else anActor->setDisplayMode(0);
	    }
	    else {
	      if(anActor->GetProperty()->GetRepresentation() <= 1)
		anActor->GetProperty()->SetRepresentationToSurface();
	      else
		anActor->GetProperty()->SetRepresentationToWireframe();	
	    }
	  }
	}
      }
      ac = theActors->GetNextActor();
    }
  } 
  if (update) {
    this->RenderWindow->Render();
    emit RenderWindowModified() ;
  }
}



void VTKViewer_RenderWindowInteractor::ChangeRepresentationToWireframe()
  // change all actors to wireframe
{
  vtkActorCollection *ac;
  vtkActor *anActor, *aPart;
  
  ac = GetRenderer()->GetActors();
  ChangeRepresentationToWireframe(ac);
}



void VTKViewer_RenderWindowInteractor::ChangeRepresentationToSurface(vtkActorCollection* ac)
{
  vtkActor *anActor, *aPart;
  
  for (ac->InitTraversal(); (anActor = ac->GetNextActor()); ) {
    if ( anActor->IsA("GEOM_Actor") ) {
      SALOME_Actor* SALOMEactor = SALOME_Actor::SafeDownCast(anActor);
      // Specific for GEOM actor
      if ( SALOMEactor->getDisplayMode() == 0 )
  	SALOMEactor->setDisplayMode(1);
    } else {
      anActor->GetProperty()->SetRepresentationToSurface();
    }
  }
  this->RenderWindow->Render();
  emit RenderWindowModified() ;
}

void VTKViewer_RenderWindowInteractor::ChangeRepresentationToSurface()
  // change all actors to "surface" or solid
{
  vtkActorCollection *ac;
  vtkActor *anActor, *aPart;
  
  ac = GetRenderer()->GetActors();
  ChangeRepresentationToSurface(ac);
}

vtkRenderer* VTKViewer_RenderWindowInteractor::GetRenderer()
{
  vtkRendererCollection * theRenderers =  this->RenderWindow->GetRenderers();
  theRenderers->InitTraversal();
  return theRenderers->GetNextItem();
}

void VTKViewer_RenderWindowInteractor::EraseAll()
{   
  vtkActorCollection *ac;
  vtkActor *anActor, *aPart;
  
  ac = GetRenderer()->GetActors();
  for (ac->InitTraversal(); (anActor = ac->GetNextActor()); ) {
    if ( anActor->IsA("SALOME_Actor") ) {
      SALOME_Actor* SActor = SALOME_Actor::SafeDownCast( anActor );
      SActor->SetVisibility(false);
    }
  }
  emit RenderWindowModified() ;
}

void VTKViewer_RenderWindowInteractor::DisplayAll()
{   
  vtkActorCollection *ac;
  vtkActor *anActor, *aPart;
  
  ac = GetRenderer()->GetActors();
  for (ac->InitTraversal(); (anActor = ac->GetNextActor()); ) {
    if ( anActor->IsA("SALOME_Actor") ) {
      SALOME_Actor* SActor = SALOME_Actor::SafeDownCast( anActor );
      SActor->SetVisibility( true );
    }
  }
  emit RenderWindowModified() ;
}

void VTKViewer_RenderWindowInteractor::Erase(const Handle(SALOME_InteractiveObject)& IObject, bool update)
{
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    vtkActorCollection* theChildActors = vtkActorCollection::New(); 
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    while(!(ac==NULL)) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    anActor->SetVisibility( false );
	    anActor->GetChildActors(theChildActors);
	  }
	}
      }
      ac = theActors->GetNextActor();
    }

    // Erase dependent actors
    theChildActors->InitTraversal();
    ac = theChildActors->GetNextActor();
    while(!(ac==NULL)) {
      ac->SetVisibility( false );
      ac = theChildActors->GetNextActor();
    }
  } 
  if (update)
    emit RenderWindowModified() ;
}

void VTKViewer_RenderWindowInteractor::Remove(const Handle(SALOME_InteractiveObject)& IObject, bool update)
{
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    while(!(ac==NULL)) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    aren->RemoveActor(anActor);
	  }
	}
      }
      ac = theActors->GetNextActor();
    }
  } 
  if (update)
    emit RenderWindowModified() ;
}

float VTKViewer_RenderWindowInteractor::GetTransparency(const Handle(SALOME_InteractiveObject)& IObject) {
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    while(!(ac==NULL)) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    if ( anActor->IsA("GEOM_Actor") ) {
	      // GEOM actor
	      return (1-(anActor->GetOpacity()));
	    }
	    else return (1-anActor->GetProperty()->GetOpacity());
	  }
	}
      }
      ac = theActors->GetNextActor();
    }
  } 
  return -1;
}

void VTKViewer_RenderWindowInteractor::SetTransparency(const Handle(SALOME_InteractiveObject)& IObject, float trans) {
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    while(!(ac==NULL)) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    if ( anActor->IsA("GEOM_Actor") ) {
	      // GEOM actor
	      anActor->SetOpacity(1-trans);
	    }
	    else anActor->GetProperty()->SetOpacity(1-trans);
	  }
	}
      }
      ac = theActors->GetNextActor();
    }
  } 
}

void VTKViewer_RenderWindowInteractor::Display( SALOME_Actor* SActor, bool update)
{
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    aren->AddActor( SActor );
    SActor->SetVisibility( true );
  }

  if (update) {
    Render();
    emit RenderWindowModified();
  }
}

void VTKViewer_RenderWindowInteractor::Display(const Handle(SALOME_InteractiveObject)& IObject, bool update)
{
  Standard_Boolean isalreadydisplayed = false;

  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    while(!(ac==NULL)) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    anActor->SetVisibility( true );
	  }
	}
      }
      ac = theActors->GetNextActor();
    }
  }
  if (update)
    emit RenderWindowModified() ;
}

void VTKViewer_RenderWindowInteractor::KeyPressed(QKeyEvent *event) {
//    MESSAGE ( " vtkQGLRenderWindowInteractor::KeyPressed " )

//    if (!QAD_Application::getDesktop()->getActiveComponent().isEmpty()) {
//      QAD_Application::getDesktop()->onKeyPress( event );
//    }
}

bool VTKViewer_RenderWindowInteractor::highlight( const Handle(SALOME_InteractiveObject)& IObject, 
						  bool hilight, 
						  bool update)
{
  VTKViewer_InteractorStyleSALOME* aStyle = 
    VTKViewer_InteractorStyleSALOME::SafeDownCast(this->InteractorStyle);
  if(!aStyle) return false;
  vtkRendererCollection* aRenColl = this->RenderWindow->GetRenderers();
  aRenColl->InitTraversal();
  while(vtkRenderer* aRen = this->RenderWindow->GetRenderers()->GetNextItem()){
      vtkActorCollection* theActors = aRen->GetActors();
      theActors->InitTraversal();
      while(vtkActor *anAct = theActors->GetNextActor()) {
	if(SALOME_Actor* anActor = SALOME_Actor::SafeDownCast(anAct)){
	  if(anActor->hasIO()){
	    if(IObject->isSame(anActor->getIO())){
	      if(anActor->GetMapper() == NULL)
		return false;
	      //highlight or unhilight actors
	      if(anActor->hasHighlight())
		anActor->highlight(hilight);
	      else{
		if(anActor->GetVisibility() == 1 && hilight)
		  aStyle->HighlightProp(anActor);
		else if(!hilight)
		  aStyle->HighlightProp(NULL);
	      }
	    }
	  }
	}
      }
    }
  if(update){
    Render();
    emit RenderWindowModified();
  }
  return false;
}

void VTKViewer_RenderWindowInteractor::Update() {
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();

    while(!(ac==NULL)) {
      ac->ApplyProperties();
      //VSV: No in VTK: ac->Update();
      ac = theActors->GetNextActor();
    }
    aren->ResetCamera();
    emit RenderWindowModified() ;  
  }
}


bool VTKViewer_RenderWindowInteractor::unHighlightAll(){
  VTKViewer_InteractorStyleSALOME* aStyle = 
    VTKViewer_InteractorStyleSALOME::SafeDownCast(this->InteractorStyle);
  if(aStyle) aStyle->HighlightProp(NULL);
  vtkRendererCollection* aRenColl = this->RenderWindow->GetRenderers();
  aRenColl->InitTraversal();
  while(vtkRenderer* aRen = this->RenderWindow->GetRenderers()->GetNextItem()){
    vtkActorCollection* theActors = aRen->GetActors();
    theActors->InitTraversal();
    if(theActors->IsItemPresent(Point_Actor)) 
      aRen->RemoveActor(Point_Actor);
    if(theActors->IsItemPresent(Edge_Actor)) 
      aRen->RemoveActor(Edge_Actor); 
    if(theActors->IsItemPresent(Cell_Actor)) 
      aRen->RemoveActor(Cell_Actor);
    vtkActor *anActor = theActors->GetNextActor();
    while(vtkActor *anAct = theActors->GetNextActor()) {
      if(SALOME_Actor* anActor = SALOME_Actor::SafeDownCast(anAct)){
	if(anActor->hasIO()){
	  //highlight or unhilight actors
	  if(anActor->hasHighlight())
	    anActor->highlight(false);
	}
      }
    }
  }
  emit RenderWindowModified() ;
  return false;
}

//-----------------
// Color methods
//-----------------

void VTKViewer_RenderWindowInteractor::SetColor(const Handle(SALOME_InteractiveObject)& IObject,QColor thecolor) {
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    while(!(ac==NULL)) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    if ( anActor->IsA("GEOM_Actor") ) {
	      // GEOM actor
	      anActor->SetColor(float(thecolor.red())/255,float(thecolor.green())/255,float(thecolor.blue())/255);
	    }
	    else {
	      anActor->GetProperty()->SetColor(float(thecolor.red())/255,float(thecolor.green())/255,float(thecolor.blue())/255);
	    }
	  }
	}
      }
      ac = theActors->GetNextActor();
    }
  } 
}

QColor VTKViewer_RenderWindowInteractor::GetColor(const Handle(SALOME_InteractiveObject)& IObject) {
  vtkRenderer* aren;
  QColor emptycolor(0,0,0);
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    while(!(ac==NULL)) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    if ( anActor->IsA("GEOM_Actor") ) {
	      // GEOM actor
	      float r,g,b;
	      anActor->GetColor(r,g,b);
	      return QColor(int(r*255),int(g*255),int(b*255));
	    }
	    else {
	      float color[3];
	      anActor->GetProperty()->GetColor(color);   
	      return QColor(int(color[0]*255),int(color[1]*255),int(color[2]*255));
	    }
	  }
	}
      }
      ac = theActors->GetNextActor();
    }
  } 
  return emptycolor;
}




bool VTKViewer_RenderWindowInteractor::isInViewer(const Handle(SALOME_InteractiveObject)& IObject)
{
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    while(!(ac==NULL)) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    //MESSAGE ( " VTKViewer_RenderWindowInteractor::isInViewer = TRUE" )
	    return true;
	  }
	}
      }
      ac = theActors->GetNextActor();
    }
  }
  //MESSAGE ( " VTKViewer_RenderWindowInteractor::isInViewer = FALSE" )
  return false;
}

bool VTKViewer_RenderWindowInteractor::isVisible(const Handle(SALOME_InteractiveObject)& IObject)
{
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    while(!(ac==NULL)) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    return anActor->GetVisibility();
	  }
	}
      }
      ac = theActors->GetNextActor();
    }
  }
  return false;
}

void VTKViewer_RenderWindowInteractor::rename(const Handle(SALOME_InteractiveObject)& IObject, QString newName)
{
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    while(!(ac==NULL)) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    anActor->setName(strdup(newName));
	  }
	}
      }
      ac = theActors->GetNextActor();
    }
  }  

  emit RenderWindowModified() ;
}

bool VTKViewer_RenderWindowInteractor::highlight(const Handle(SALOME_InteractiveObject)& IObject, 
						 const TColStd_MapOfInteger& MapIndex, 
						 VTKViewer_RenderWindowInteractor::TCreateMapperFun theFun,
						 vtkActor *theActor, bool hilight, bool update )
{
  if(MapIndex.Extent() == 0) return false;
  vtkRenderer* aRen;
  this->RenderWindow->GetRenderers()->InitTraversal();
  for (; aRen = this->RenderWindow->GetRenderers()->GetNextItem(); ) {
    vtkActorCollection* anActorColl = aRen->GetActors();
    if ( anActorColl->IsItemPresent(theActor) != 0 ) 
      aRen->RemoveActor(theActor);
    anActorColl->InitTraversal();
    vtkActor *ac = NULL;
    for(; (ac = anActorColl->GetNextActor()) != NULL; ){
      if(ac->IsA("SALOME_Actor")){
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast(ac);
	if(anActor->hasIO()){
	  if(IObject->isSame(anActor->getIO())){
	    if(vtkPolyData* aSourcePolyData = anActor->GetPolyDataInput()){
	      vtkPolyDataMapper* aMapper = vtkPolyDataMapper::New();
	      (*theFun)(aSourcePolyData,aMapper,MapIndex);
	      theActor->SetMapper(aMapper);   
	      aMapper->Delete();
	      if(anActorColl->IsItemPresent(theActor) == 0) 
		aRen->AddActor(theActor);	      
	      if(hilight)
		theActor->VisibilityOn();
	      else
		theActor->VisibilityOff();
	      break;
	    }
	  }
	}
      }
    }
  }
  if(update){
    this->RenderWindow->Render();  
    emit RenderWindowModified() ;
  }
  return false;
}

static void CellCreateMapper(vtkPolyData *theSourcePolyData, vtkPolyDataMapper* theMapper, 
		      const TColStd_MapOfInteger& theMapIndex)
{
  vtkPolyData* aPolyData = vtkPolyData::New();
  aPolyData->Allocate();

  vtkIdList *ptIds = vtkIdList::New(); 
  ptIds->Allocate(theSourcePolyData->GetMaxCellSize());
  vtkPolyDataMapper* aMapper = vtkPolyDataMapper::New();
  aPolyData->SetPoints(theSourcePolyData->GetPoints());
  TColStd_MapIteratorOfMapOfInteger ite(theMapIndex);
  int aNbOfParts = theSourcePolyData->GetNumberOfCells();
  for(; ite.More(); ite.Next()){
    int aPartId = ite.Key();
    if(0 > aPartId || aPartId >= aNbOfParts) break;
    theSourcePolyData->GetCellPoints(aPartId,ptIds);
    vtkCell* aCell = theSourcePolyData->GetCell(aPartId);
    aPolyData->InsertNextCell(aCell->GetCellType(),ptIds);
    for (int i = 0, iEnd = aCell->GetNumberOfEdges(); i < iEnd; i++){
      vtkCell* anEdgeCell = aCell->GetEdge(i);
      aPolyData->InsertNextCell(VTK_LINE,anEdgeCell->GetPointIds());
    }
  }
  ptIds->Delete();
  theMapper->SetInput(aPolyData);  
  aPolyData->Delete();
}
  
bool VTKViewer_RenderWindowInteractor::highlightCell(const Handle(SALOME_InteractiveObject)& IObject, 
						     bool hilight, 
						     const TColStd_MapOfInteger& MapIndex, 
						     bool update )
{
  return highlight(IObject,MapIndex,&CellCreateMapper,Cell_Actor,hilight,update);
}


static void PointCreateMapper(vtkPolyData *theSourcePolyData, vtkPolyDataMapper* theMapper, 
		       const TColStd_MapOfInteger& theMapIndex)
{
  vtkPolyData* aPolyData = vtkPolyData::New();
  aPolyData->Allocate();

  vtkPoints *aPoints = vtkPoints::New();
  vtkMaskPoints* aMaskPoints = vtkMaskPoints::New();
  TColStd_MapIteratorOfMapOfInteger ite(theMapIndex);
  aPoints->SetNumberOfPoints(theMapIndex.Extent());
  int aNbOfParts = theSourcePolyData->GetNumberOfPoints();
  for(int i = 0; ite.More(); ite.Next(),i++){
    int aPartId = ite.Key();
    if(0 > aPartId || aPartId >= aNbOfParts) break;
    aPoints->SetPoint(i,theSourcePolyData->GetPoint(aPartId));
  }
  aPolyData->SetPoints(aPoints);
  aPoints->Delete();
  aMaskPoints->SetInput(aPolyData);
  aMaskPoints->SetGenerateVertices(1);
  aMaskPoints->SetOnRatio(1);
  theMapper->SetInput(aMaskPoints->GetOutput());  
  aMaskPoints->Delete();
}
  
bool VTKViewer_RenderWindowInteractor::highlightPoint(const Handle(SALOME_InteractiveObject)& IObject, 
						      bool hilight, const TColStd_MapOfInteger& MapIndex, 
						      bool update)
{
  return highlight(IObject,MapIndex,&PointCreateMapper,Point_Actor,hilight,update);
}

  
static void EdgeCreateMapper(vtkPolyData *theSourcePolyData, vtkPolyDataMapper* theMapper, 
		      const TColStd_MapOfInteger& theMapIndex)
{
  vtkPolyData* aPolyData = vtkPolyData::New();
  aPolyData->Allocate();

  vtkIdList *ptIds = vtkIdList::New(); 
  ptIds->Allocate(theSourcePolyData->GetMaxCellSize());
  vtkPolyDataMapper* aMapper = vtkPolyDataMapper::New();
  aPolyData->SetPoints(theSourcePolyData->GetPoints());
  int aCellId = -1, aCellCounter = 0;
  for(TColStd_MapIteratorOfMapOfInteger ite(theMapIndex); ite.More(); ite.Next()){
    if(ite.Key() > 0){
      aCellId = ite.Key();
      aCellCounter++;
    }
  }
  if(aCellCounter == 1){
    vtkCell* aCell = theSourcePolyData->GetCell(aCellId);
    if(aCell->GetCellType() <= VTK_LINE){
      aPolyData->InsertNextCell(aCell->GetCellType(),aCell->GetPointIds());
    }else{
      TColStd_MapIteratorOfMapOfInteger ite(theMapIndex);
      int aNbOfParts = aCell->GetNumberOfEdges();
      for(; ite.More(); ite.Next()){
	if(ite.Key() < 0){
	  int aPartId = -ite.Key()-1;
	  if(0 > aPartId || aPartId >= aNbOfParts) break;
	  vtkCell* anEdgeCell = aCell->GetEdge(aPartId);
	  aPolyData->InsertNextCell(VTK_LINE,anEdgeCell->GetPointIds());
	}
      }
    }
  }else{
    int aNbOfParts = theSourcePolyData->GetNumberOfCells();
    for(TColStd_MapIteratorOfMapOfInteger ite(theMapIndex); ite.More(); ite.Next()){
      if(ite.Key() > 0){
	int aPartId = ite.Key();
	if(0 > aPartId || aPartId >= aNbOfParts) break;
	vtkCell* aCell = theSourcePolyData->GetCell(aPartId);
	aPolyData->InsertNextCell(aCell->GetCellType(),aCell->GetPointIds());
      }
    }
  }
  ptIds->Delete();
  theMapper->SetInput(aPolyData);  
  aPolyData->Delete();
}
  

bool VTKViewer_RenderWindowInteractor::highlightEdge( const Handle(SALOME_InteractiveObject)& IObject, 
						      bool hilight, 
						      const TColStd_MapOfInteger& MapIndex, 
						      bool update )
{
  return highlight(IObject,MapIndex,&EdgeCreateMapper,Edge_Actor,hilight,update);
}
