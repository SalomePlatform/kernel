using namespace std;
//  File      : VTKViewer_RenderWindowInteractor.cxx
//  Created   : Wed Mar 20 11:32:45 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : VTKViewer
//  Copyright : Open CASCADE 2002
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
#include <vtkDataSetMapper.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkSphereSource.h>
#include <vtkDataSet.h>
#include <vtkMaskPoints.h>
#include <vtkVertex.h>

// QT Includes
#include <qkeycode.h>

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
  VTKViewer_RenderWindow *my_render_win = dynamic_cast<VTKViewer_RenderWindow *>(RenderWindow) ;
  if( ! my_render_win ) {
    vtkErrorMacro(<< "VTKViewer_RenderWindowInteractor::Initialize() can only handle VTKViewer_RenderWindow.") ;
    return ;
  }

  //
  // If the render window has zero size, then set it to a default 
  // value of 300x300.
  // 
  int *size = my_render_win->GetSize() ;
  size[0] = ((size[0] > 0) ? size[0] : 300);
  size[1] = ((size[1] > 0) ? size[1] : 300);

  //
  // Enable the interactor. 
  //
  this->Enable() ;

  //
  // Start the rendering of the window. 
  //
  my_render_win->Start() ;

  //
  // Set our size to that of the render window. 
  //
  this->Size[0] = size[0] ;
  this->Size[1] = size[1] ;

  //
  // The interactor has been initialized.
  //
  this->Initialized = 1 ;

  VTKViewer_InteractorStyleSALOME* style = VTKViewer_InteractorStyleSALOME::New();
  this->SetInteractorStyle(style); 

  Cell_Actor = vtkActor::New(); 
  Cell_Actor->PickableOff();
  Cell_Actor->GetProperty()->SetColor(1,1,0);
  Cell_Actor->GetProperty()->SetRepresentationToSurface();

  Edge_Actor = vtkActor::New(); 
  Edge_Actor->PickableOff();
  Edge_Actor->GetProperty()->SetColor(1,0,0);
  Edge_Actor->GetProperty()->SetRepresentationToSurface();
  Edge_Actor->GetProperty()->SetLineWidth(20);

  Point_Actor = vtkActor::New(); 
  Point_Actor->PickableOff();
  Point_Actor->GetProperty()->SetColor(1,1,0);
  Point_Actor->GetProperty()->SetRepresentationToSurface();
  Point_Actor->GetProperty()->SetPointSize(3);

  return ;
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

  this->InteractorStyle->OnTimer() ;
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
  if( ! RenderWindow ) {
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
  VTKViewer_RenderWindow *my_render_win = dynamic_cast<VTKViewer_RenderWindow *>(RenderWindow) ;
  if( ! my_render_win ) {
    vtkErrorMacro(<< "VTKViewer_RenderWindowInteractor::Initialize() can only handle VTKViewer_RenderWindow.") ;
    return ;
  }

  slot_func ( my_render_win, SIGNAL(ButtonPressed(const QMouseEvent *)), 
	      this,          SLOT(ButtonPressed(const QMouseEvent *)) ) ;
  slot_func( my_render_win, SIGNAL(ButtonReleased(const QMouseEvent *)), 
	     this,          SLOT(ButtonReleased(const QMouseEvent *)) ) ;
  
  
  slot_func( my_render_win, SIGNAL(LeftButtonPressed(const QMouseEvent *)), 
	     this,          SLOT(LeftButtonPressed(const QMouseEvent *)) ) ;
  slot_func( my_render_win, SIGNAL(LeftButtonReleased(const QMouseEvent *)), 
	     this,          SLOT(LeftButtonReleased(const QMouseEvent *)) ) ;
  
  slot_func( my_render_win, SIGNAL(MiddleButtonPressed(const QMouseEvent *)), 
	     this,          SLOT(MiddleButtonPressed(const QMouseEvent *)) ) ;
  slot_func( my_render_win, SIGNAL(MiddleButtonReleased(const QMouseEvent *)), 
	     this,          SLOT(MiddleButtonReleased(const QMouseEvent *)) ) ;
  
  slot_func( my_render_win, SIGNAL(RightButtonPressed(const QMouseEvent *)), 
	     this,          SLOT(RightButtonPressed(const QMouseEvent *)) ) ;
  slot_func( my_render_win, SIGNAL(RightButtonReleased(const QMouseEvent *)), 
	     this,          SLOT(RightButtonReleased(const QMouseEvent *)) ) ;
  
  slot_func( my_render_win, SIGNAL(MouseMove(const QMouseEvent *)), 
	     this,          SLOT(MouseMove(const QMouseEvent *)) ) ;
  
  slot_func( my_render_win, SIGNAL(KeyPressed(QKeyEvent *)),
	     this,          SLOT(KeyPressed(QKeyEvent *)) ) ;
  
  slot_func( this,          SIGNAL(RenderWindowModified()), 
	     my_render_win, SLOT(updateGL()) ) ;

}

void VTKViewer_RenderWindowInteractor::MouseMove(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  this->InteractorStyle->OnMouseMove(0, 0, 
				     event->x(), 
				     event->y()/*this->Size[1] - event->y() - 1*/) ;
  VTKViewer_InteractorStyleSALOME* Style = 0;
  if (this->InteractorStyle->IsA("VTKViewer_InteractorStyleSALOME"))
    Style = VTKViewer_InteractorStyleSALOME::SafeDownCast(this->InteractorStyle);
  if (!Style || Style->needsRedrawing() )
    emit RenderWindowModified() ; 
}

void VTKViewer_RenderWindowInteractor::LeftButtonPressed(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  this->InteractorStyle->OnLeftButtonDown( (event->state() & ControlButton), 
					   (event->state() & ShiftButton), 
					   event->x(),
					   event->y()) ;
}

void VTKViewer_RenderWindowInteractor::LeftButtonReleased(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  this->InteractorStyle->OnLeftButtonUp( (event->state() & ControlButton), 
					 (event->state() & ShiftButton), 
					 event->x(),
					 event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::MiddleButtonPressed(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  this->InteractorStyle->OnMiddleButtonDown( (event->state() & ControlButton), 
					     (event->state() & ShiftButton), 
					     event->x(),
					     event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::MiddleButtonReleased(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  this->InteractorStyle->OnMiddleButtonUp( (event->state() & ControlButton), 
					   (event->state() & ShiftButton), 
					   event->x(),
					   event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::RightButtonPressed(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  this->InteractorStyle->OnRightButtonDown( (event->state() & ControlButton), 
					    (event->state() & ShiftButton), 
					    event->x(),
					    event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::RightButtonReleased(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  this->InteractorStyle->OnRightButtonUp( (event->state() & ControlButton), 
					  (event->state() & ShiftButton), 
					  event->x(),
					  event->y() ) ;
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
	    if ( anActor->GetMapper() == NULL ) {
	      return false;
	    }
	    //highlight or unhilight actors
	    if ( anActor->hasHighlight() ) {
	      anActor->highlight(hilight);
	    } else {
	      if ( anActor->GetVisibility() == 1 ) {
		if (hilight) {
		  vtkActor2D *actor2D;
		  vtkProp3D *prop3D;
		  if ( (prop3D=vtkProp3D::SafeDownCast(anActor)) != NULL ) {
		    this->InteractorStyle->HighlightProp3D(prop3D);
		  } else if ( (actor2D=vtkActor2D::SafeDownCast(anActor)) != NULL ) {
		    this->InteractorStyle->HighlightActor2D(actor2D);
		  }
		  //this->InteractorStyle->HighlightProp(anActor);
		} else {
		  this->InteractorStyle->HighlightProp3D(NULL);
		  this->InteractorStyle->HighlightActor2D(NULL);
		  //this->InteractorStyle->HighlightProp(NULL);
		}
	      }
	    }
	  }
	}
      }
      ac = theActors->GetNextActor();
    }
  }
  if (update) {
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
      ac->Update();
      ac = theActors->GetNextActor();
    }
    aren->ResetCamera();
    emit RenderWindowModified() ;  
  }
}


bool VTKViewer_RenderWindowInteractor::unHighlightAll()
{
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    theActors->InitTraversal();

    if ( theActors->IsItemPresent(Point_Actor) != 0 ) 
      aren->RemoveActor( Point_Actor );
    if ( theActors->IsItemPresent( Edge_Actor ) != 0 ) 
      aren->RemoveActor( Edge_Actor ); 
    if ( theActors->IsItemPresent( Cell_Actor ) != 0 ) 
      aren->RemoveActor( Cell_Actor );

    vtkActor *ac = theActors->GetNextActor();

    while(!(ac==NULL)) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  //highlight or unhilight actors
	  if ( anActor->hasHighlight() )
	    anActor->highlight(false);
	}
      }
      ac = theActors->GetNextActor();
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
	      return QColor(r*255,g*255,b*255);
	    }
	    else {
	      float color[3];
	      anActor->GetProperty()->GetColor(color);   
	      return QColor(color[0]*255,color[1]*255,color[2]*255);
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

bool VTKViewer_RenderWindowInteractor::highlightCell( const Handle(SALOME_InteractiveObject)& IObject, 
						      bool hilight, 
						      const TColStd_MapOfInteger& MapIndex, 
						      bool update )
{
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    
    if ( theActors->IsItemPresent(Cell_Actor) != 0 ) 
      aren->RemoveActor( Cell_Actor );
    
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    for ( ; ac; ac = theActors->GetNextActor() ) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    vtkUnstructuredGrid* Cell_UGrid = vtkUnstructuredGrid::New();
	    vtkUnstructuredGrid* UGrid = vtkUnstructuredGrid::SafeDownCast( ac->GetMapper()->GetInput() );
	    if (UGrid == 0) break;
	    vtkIdList *ptIds = vtkIdList::New(); ptIds->Allocate(12);
	    vtkDataSetMapper* Cell_Mapper = vtkDataSetMapper::New();
	    Cell_UGrid->SetPoints( UGrid->GetPoints() );
	    TColStd_MapIteratorOfMapOfInteger ite( MapIndex );
	    for ( ; ite.More(); ite.Next() ) {
	      UGrid->GetCellPoints( ite.Key(), ptIds );
	      Cell_UGrid->InsertNextCell(ac->GetMapper()->GetInput()->GetCellType( ite.Key() ), ptIds);
	    }
	    
	    
	    Cell_Mapper->SetInput(Cell_UGrid);
	    Cell_Actor->SetMapper(Cell_Mapper);
	    
	    if ( theActors->IsItemPresent(Cell_Actor) == 0 ) 
	      aren->AddActor( Cell_Actor );	      
	    
	    if ( hilight )
	      Cell_Actor->VisibilityOn();
	    else
	      Cell_Actor->VisibilityOff();
	    break;
	  }
	}
      }
    }
  }
  if (update) {
    this->RenderWindow->Render();  
    emit RenderWindowModified() ;
  }
  return 0;
}

bool VTKViewer_RenderWindowInteractor::highlightEdge( const Handle(SALOME_InteractiveObject)& IObject, 
						      bool hilight, 
						      const TColStd_MapOfInteger& MapIndex, 
						      bool update )
{
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    for( ; ac; ac = theActors->GetNextActor() ) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    
	    vtkUnstructuredGrid* Edge_UGrid = vtkUnstructuredGrid::New();
	    vtkUnstructuredGrid* UGrid = vtkUnstructuredGrid::SafeDownCast( ac->GetMapper()->GetInput() );
	    if (UGrid == 0) break;
	    
	    vtkDataSetMapper* Edge_Mapper = vtkDataSetMapper::New();
	    vtkCell* pickedCell;
	    vtkCell* pickedEdge;
	    int cellId, edgeId;
	    Edge_UGrid->SetPoints( UGrid->GetPoints() );
	    TColStd_MapIteratorOfMapOfInteger ite( MapIndex );
	    int numberOfIndexes = MapIndex.Extent();
	    int i, j, npts, *pts, aLen;

	    if ( theActors->IsItemPresent(Cell_Actor) != 0 ) {
	      Cell_Actor->VisibilityOff();
	      if ( theActors->IsItemPresent(Edge_Actor) != 0 ) {
		vtkUnstructuredGrid* EdgeGrid = vtkUnstructuredGrid::SafeDownCast( Edge_Actor->GetMapper()->GetInput() );
		if (EdgeGrid) {
		  aLen = EdgeGrid->GetNumberOfCells(); 
		  //SRN: modified traversal of edge cells.  
		  vtkCellArray* cells = EdgeGrid->GetCells();
		  for (i=0, cells->InitTraversal(); cells->GetNextCell(npts,pts); i++) {
		    if(MapIndex.Contains(i))
		      Edge_UGrid->InsertNextCell(VTK_LINE, npts, pts);
		  }
		}
	      }
	      vtkUnstructuredGrid* CellGrid = vtkUnstructuredGrid::SafeDownCast( Cell_Actor->GetMapper()->GetInput() );

	      bool isOneEdge = (MapIndex.Extent() <= 2);
	      int edgeId;

	      //SRN: modified finding the selected edges
	      if (CellGrid) {
		aLen = CellGrid->GetNumberOfCells();
		for (i=0; i<aLen; i++) {
		  pickedCell = CellGrid->GetCell(i);
		  int aLen2 = pickedCell->GetNumberOfEdges();
		  for(j=0; j<aLen2; j++) {
		    edgeId = (isOneEdge)?(-j):j;
		    if(MapIndex.Contains(edgeId))  {
		      pickedEdge = pickedCell->GetEdge(j);
		      Edge_UGrid->InsertNextCell(pickedEdge->GetCellType(), pickedEdge->GetPointIds());
		    }
		  }
		}
	      } 

	      Edge_Mapper->SetInput(Edge_UGrid);
	      Edge_Actor->SetMapper(Edge_Mapper);
	      
	      if ( theActors->IsItemPresent(Edge_Actor) == 0 ) 
		aren->AddActor( Edge_Actor );	
	      
	      if ( hilight )
		Edge_Actor->VisibilityOn();
	      else
		Edge_Actor->VisibilityOff();
	    }
	    
	    break;
	  }
	}
      }
    }
  }

  if (update) {
    this->RenderWindow->Render();
    emit RenderWindowModified();
  }

  return 0;
}

bool VTKViewer_RenderWindowInteractor::highlightPoint(const Handle(SALOME_InteractiveObject)& IObject, 
						      bool hilight, const TColStd_MapOfInteger& MapIndex, 
						      bool update)
{
  vtkRenderer* aren;
  for (this->RenderWindow->GetRenderers()->InitTraversal(); 
       (aren = this->RenderWindow->GetRenderers()->GetNextItem()); ) {
    vtkActorCollection* theActors = aren->GetActors();
    
    if ( theActors->IsItemPresent(Point_Actor) != 0 ) 
      aren->RemoveActor( Point_Actor );

    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    for ( ; ac; ac = theActors->GetNextActor() ) {
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
	if ( anActor->hasIO() ) {
	  if ( IObject->isSame( anActor->getIO() ) ) {
	    vtkUnstructuredGrid* UGrid = vtkUnstructuredGrid::SafeDownCast( ac->GetMapper()->GetInput() );
            if (UGrid == 0) break;
	    vtkUnstructuredGrid* Point_UGrid = vtkUnstructuredGrid::New();
	    float pts[3];

	    vtkPoints *Points = vtkPoints::New();
	    vtkMaskPoints* verts = vtkMaskPoints::New();
	    vtkPolyDataMapper* vertMapper = vtkPolyDataMapper::New();

	    TColStd_MapIteratorOfMapOfInteger ite( MapIndex );
	    int i = 0;
	    //  	    Points->SetNumberOfPoints( MapIndex.Extent() );
	    if ( MapIndex.Extent() == 1 )
	      Points->SetNumberOfPoints( MapIndex.Extent() + 1 );
	    else
	      Points->SetNumberOfPoints( MapIndex.Extent() );

	    for ( ; ite.More(); ite.Next() ) {
	      UGrid->GetPoint( ite.Key(), pts) ;
	      Points->SetPoint(i, pts );
	      i++;
	      // DEBUG : it's not possible to see only one point !!!!
	      if ( MapIndex.Extent() == 1 )
  		Points->SetPoint(i, 0., 0., 0. );
	    }
	    
	    Point_UGrid->SetPoints(Points);
	    verts->SetInput(Point_UGrid);
	    verts->SetGenerateVertices(1);
	    verts->SetOnRatio(1);
	    vertMapper->SetInput(verts->GetOutput());
	    Point_Actor->SetMapper(vertMapper);
	    
	    if ( theActors->IsItemPresent(Point_Actor) == 0 ) {
	      aren->AddActor( Point_Actor );
	    }
	    
	    if ( hilight )
	      Point_Actor->VisibilityOn();
	    else
	      Point_Actor->VisibilityOff();
	    
	    break;
	  }
	}
      }
    }
  }
  
  if (update) {
    this->RenderWindow->Render();
    emit RenderWindowModified();
  }
  return 0;
}
