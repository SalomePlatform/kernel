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

#include "VTKViewer_InteractorStyleSALOME.h"
#include "VTKViewer_RenderWindow.h"
#include "VTKViewer_ViewFrame.h"

#include "VTKViewer_Algorithm.h"
#include "VTKViewer_Functor.h"

#include "QAD_Application.h"
#include "QAD_Desktop.h"

#include "VTKViewer_Actor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// VTK Includes
#include <vtkObjectFactory.h>
#include <vtkPicker.h>
#include <vtkCellPicker.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h>

// QT Includes
#include <qkeycode.h>

#include <TColStd_IndexedMapOfInteger.hxx>

#include "utilities.h"

using namespace std;

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


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
  myDisplayMode = 0;
  myGUIWindow = 0;

  myBasicPicker = vtkPicker::New();
  myCellPicker = vtkCellPicker::New();
  myPointPicker = vtkPointPicker::New();

  myCellActor = VTKViewer_Actor::New(); 
  myCellActor->PickableOff();
  myCellActor->GetProperty()->SetColor(1,1,0);
  myCellActor->GetProperty()->SetLineWidth(5);
  myCellActor->GetProperty()->SetRepresentationToSurface();

  myEdgeActor = VTKViewer_Actor::New(); 
  myEdgeActor->PickableOff();
  myEdgeActor->GetProperty()->SetColor(1,0,0);
  myEdgeActor->GetProperty()->SetLineWidth(5);
  myEdgeActor->GetProperty()->SetRepresentationToWireframe();

  myPointActor = VTKViewer_Actor::New(); 
  myPointActor->PickableOff();
  myPointActor->GetProperty()->SetColor(1,1,0);
  myPointActor->GetProperty()->SetPointSize(5);
  myPointActor->GetProperty()->SetRepresentationToPoints();

  connect(mTimer, SIGNAL(timeout()), this, SLOT(TimerFunc())) ;
}


VTKViewer_RenderWindowInteractor::~VTKViewer_RenderWindowInteractor() {
  if(MYDEBUG) INFOS("VTKViewer_RenderWindowInteractor::~VTKViewer_RenderWindowInteractor()");

  delete mTimer ;

  myViewFrame->RemoveActor(myCellActor);
  myViewFrame->RemoveActor(myEdgeActor);
  myViewFrame->RemoveActor(myPointActor);

  myCellActor->Delete();
  myEdgeActor->Delete();
  myPointActor->Delete();

  myBasicPicker->Delete();
  myCellPicker->Delete();
  myPointPicker->Delete();
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

  this->SetPicker(myBasicPicker);

  SetSelectionTolerance();

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

  return ;
}


//----------------------------------------------------------------------------
void VTKViewer_RenderWindowInteractor::setGUIWindow(QWidget* theWindow){
  myGUIWindow = theWindow;
}

//----------------------------------------------------------------------------
void VTKViewer_RenderWindowInteractor::setViewFrame(VTKViewer_ViewFrame* theViewFrame){
  myViewFrame = theViewFrame;

  myViewFrame->InsertActor(myCellActor);
  myViewFrame->InsertActor(myEdgeActor);
  myViewFrame->InsertActor(myPointActor);
}

//----------------------------------------------------------------------------
void VTKViewer_RenderWindowInteractor::MoveInternalActors()
{
  myViewFrame->MoveActor(myCellActor);
  myViewFrame->MoveActor(myEdgeActor);
  myViewFrame->MoveActor(myPointActor);
}

//----------------------------------------------------------------------------
void VTKViewer_RenderWindowInteractor::SetInteractorStyle(vtkInteractorObserver *theInteractor){
  myInteractorStyle = dynamic_cast<VTKViewer_InteractorStyleSALOME*>(theInteractor);
  vtkRenderWindowInteractor::SetInteractorStyle(theInteractor);
}


void VTKViewer_RenderWindowInteractor::SetSelectionMode(Selection_Mode theMode)
{
  myCellActor->SetVisibility(false);
  myEdgeActor->SetVisibility(false);
  myPointActor->SetVisibility(false);

  switch(theMode){
  case ActorSelection:
    this->SetPicker(myBasicPicker);
    break;
  case NodeSelection:
    this->SetPicker(myPointPicker);
    break;
  case CellSelection:
  case EdgeSelection:
  case FaceSelection:
  case VolumeSelection:
  case EdgeOfCellSelection:
    this->SetPicker(myCellPicker);
    break;
  }

  myInteractorStyle->OnSelectionModeChanged();
}

void VTKViewer_RenderWindowInteractor::SetSelectionProp(const double& theRed, const double& theGreen, 
							const double& theBlue, const int& theWidth) 
{
  myCellActor->GetProperty()->SetColor(theRed, theGreen, theBlue);
  myCellActor->GetProperty()->SetLineWidth(theWidth);

  myPointActor->GetProperty()->SetColor(theRed, theGreen, theBlue);
  myPointActor->GetProperty()->SetPointSize(theWidth);
}

void VTKViewer_RenderWindowInteractor::SetSelectionTolerance(const double& theTolNodes, const double& theTolItems)
{
  myTolNodes = theTolNodes;
  myTolItems = theTolItems;

  myBasicPicker->SetTolerance(myTolItems);
  myCellPicker->SetTolerance(myTolItems);
  myPointPicker->SetTolerance(myTolNodes);

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
  myInteractorStyle->OnMouseMove(0, 0, event->x(), event->y()/*this->Size[1] - event->y() - 1*/) ;
  if (myInteractorStyle->needsRedrawing() )
    emit RenderWindowModified() ; 
}

void VTKViewer_RenderWindowInteractor::LeftButtonPressed(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  myInteractorStyle->OnLeftButtonDown((event->state() & ControlButton), 
				      (event->state() & ShiftButton), 
				      event->x(), event->y());
}

void VTKViewer_RenderWindowInteractor::LeftButtonReleased(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  myInteractorStyle->OnLeftButtonUp( (event->state() & ControlButton), 
				     (event->state() & ShiftButton), 
				     event->x(), event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::MiddleButtonPressed(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  myInteractorStyle->OnMiddleButtonDown((event->state() & ControlButton), 
					(event->state() & ShiftButton), 
					event->x(), event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::MiddleButtonReleased(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  myInteractorStyle->OnMiddleButtonUp( (event->state() & ControlButton), 
				       (event->state() & ShiftButton), 
				       event->x(), event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::RightButtonPressed(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  myInteractorStyle->OnRightButtonDown( (event->state() & ControlButton), 
					(event->state() & ShiftButton), 
					event->x(), event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::RightButtonReleased(const QMouseEvent *event) {
  if( ! this->Enabled ) {
    return ;
  }
  myInteractorStyle->OnRightButtonUp( (event->state() & ControlButton), 
				      (event->state() & ShiftButton), 
				      event->x(), event->y() ) ;
}

void VTKViewer_RenderWindowInteractor::ButtonPressed(const QMouseEvent *event) {
  return ;
}

void VTKViewer_RenderWindowInteractor::ButtonReleased(const QMouseEvent *event) {
  return ;
}


int VTKViewer_RenderWindowInteractor::GetDisplayMode() {
  return myDisplayMode;
}

void VTKViewer_RenderWindowInteractor::SetDisplayMode(int theMode) {
  if(theMode == 0) 
    ChangeRepresentationToWireframe();
  else 
    ChangeRepresentationToSurface();
  myDisplayMode = theMode;
}


void VTKViewer_RenderWindowInteractor::SetDisplayMode(const Handle(SALOME_InteractiveObject)& theIObject, int theMode){
  using namespace SALOME::VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,int>
			  (&SALOME_Actor::setDisplayMode,theMode));
}


void VTKViewer_RenderWindowInteractor::ChangeRepresentationToWireframe()
{
  ChangeRepresentationToWireframe(GetRenderer()->GetActors());
}

void VTKViewer_RenderWindowInteractor::ChangeRepresentationToSurface()
{
  ChangeRepresentationToSurface(GetRenderer()->GetActors());
}


void VTKViewer_RenderWindowInteractor::ChangeRepresentationToWireframe(vtkActorCollection* theCollection)
{
  using namespace SALOME::VTK;
  ForEach<SALOME_Actor>(theCollection,
			TSetFunction<SALOME_Actor,int>
			(&SALOME_Actor::setDisplayMode,0));
  emit RenderWindowModified();
}

void VTKViewer_RenderWindowInteractor::ChangeRepresentationToSurface(vtkActorCollection* theCollection)
{
  using namespace SALOME::VTK;
  ForEach<SALOME_Actor>(theCollection,
			TSetFunction<SALOME_Actor,int>
			(&SALOME_Actor::setDisplayMode,1));
  emit RenderWindowModified();
}


vtkRenderer* VTKViewer_RenderWindowInteractor::GetRenderer()
{
  vtkRendererCollection * theRenderers =  this->RenderWindow->GetRenderers();
  theRenderers->InitTraversal();
  return theRenderers->GetNextItem();
}


struct TErase{
  SALOME::VTK::TSetFunction<vtkActor,int> mySetFunction;
  TErase():
    mySetFunction(&vtkActor::SetVisibility,false)
  {}
  void operator()(SALOME_Actor* theActor){
    theActor->SetVisibility(false);
    // Erase dependent actors
    vtkActorCollection* aCollection = vtkActorCollection::New(); 
    theActor->GetChildActors(aCollection);
    SALOME::VTK::ForEach<vtkActor>(aCollection,mySetFunction);
    aCollection->Delete();
  }
};

void VTKViewer_RenderWindowInteractor::EraseAll()
{   
  using namespace SALOME::VTK;
  ForEach<SALOME_Actor>(GetRenderer()->GetActors(),
			TErase());

  emit RenderWindowModified() ;
}


void VTKViewer_RenderWindowInteractor::DisplayAll()
{ 
  vtkActorCollection* aCollection = GetRenderer()->GetActors();
  using namespace SALOME::VTK;
  ForEach<SALOME_Actor>(aCollection,TSetVisibility<SALOME_Actor>(true));

  emit RenderWindowModified() ;
}


void VTKViewer_RenderWindowInteractor::Erase(SALOME_Actor* theActor, bool update)
{
  TErase()(theActor);

  if(update)
    emit RenderWindowModified();
}


void VTKViewer_RenderWindowInteractor::Erase(const Handle(SALOME_InteractiveObject)& theIObject, bool update)
{
  using namespace SALOME::VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TErase());

  if(update)
    emit RenderWindowModified();
}


struct TRemoveAction{
  vtkRenderer* myRen;
  TRemoveAction(vtkRenderer* theRen): myRen(theRen){}
  void operator()(SALOME_Actor* theActor){
    myRen->RemoveActor(theActor);
  }
};

void VTKViewer_RenderWindowInteractor::Remove(const Handle(SALOME_InteractiveObject)& theIObject, bool update)
{
  vtkRenderer* aRen = GetRenderer();

  using namespace SALOME::VTK;
  ForEachIf<SALOME_Actor>(aRen->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TRemoveAction(aRen));

  if(update)
    emit RenderWindowModified();
}


float VTKViewer_RenderWindowInteractor::GetTransparency(const Handle(SALOME_InteractiveObject)& theIObject) 
{
  using namespace SALOME::VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  if(anActor)
    return 1.0 - anActor->GetOpacity();
  return -1.0;
}


void VTKViewer_RenderWindowInteractor::SetTransparency(const Handle(SALOME_InteractiveObject)& theIObject, float theTrans)
{
  float anOpacity = 1.0 - theTrans;
  using namespace SALOME::VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,float>
			  (&SALOME_Actor::SetOpacity,anOpacity));
}


void VTKViewer_RenderWindowInteractor::Display(SALOME_Actor* theActor, bool update)
{
  GetRenderer()->AddActor(theActor);
  theActor->SetVisibility(true);

  if(update)
    emit RenderWindowModified();
}


void VTKViewer_RenderWindowInteractor::Display(const Handle(SALOME_InteractiveObject)& theIObject, bool update)
{
  using namespace SALOME::VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetVisibility<SALOME_Actor>(true));

  if(update)
    emit RenderWindowModified() ;
}


void VTKViewer_RenderWindowInteractor::KeyPressed(QKeyEvent *event){}


struct THighlightAction{
  bool myIsHighlight;
  VTKViewer_InteractorStyleSALOME* myInteractorStyle;
  THighlightAction(VTKViewer_InteractorStyleSALOME* theInteractorStyle,
		   bool theIsHighlight): 
    myInteractorStyle(theInteractorStyle),
    myIsHighlight(theIsHighlight)
  {}
  void operator()(SALOME_Actor* theActor){
    if(theActor->GetMapper()){
      if(theActor->hasHighlight())
	theActor->highlight(myIsHighlight);
      else{
	if(theActor->GetVisibility() && myIsHighlight)
	  myInteractorStyle->HighlightProp(theActor);
	else if(!myIsHighlight)
	  myInteractorStyle->HighlightProp(NULL);
      }
    }
  }
};

bool VTKViewer_RenderWindowInteractor::highlight( const Handle(SALOME_InteractiveObject)& theIObject, 
						  bool hilight, 
						  bool update)
{
  using namespace SALOME::VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  THighlightAction(myInteractorStyle,hilight));

  if(update)
    emit RenderWindowModified();

  return false;
}


struct TUpdateAction{
  void operator()(vtkActor* theActor){
    theActor->ApplyProperties();
  }
};

void VTKViewer_RenderWindowInteractor::Update() {
  vtkRenderer* aRen = GetRenderer();

  using namespace SALOME::VTK;
  ForEach<vtkActor>(aRen->GetActors(),TUpdateAction());

  aRen->ResetCamera();

  emit RenderWindowModified();  
}


void VTKViewer_RenderWindowInteractor::unHighlightSubSelection(){
  myPointActor->SetVisibility(false);
  myEdgeActor->SetVisibility(false);
  myCellActor->SetVisibility(false);
}


struct TUnHighlightAllAction{
  void operator()(SALOME_Actor* theActor){
    if(theActor->hasIO()){
      if(theActor->hasHighlight())
	theActor->highlight(false);
    }
  }
};

bool VTKViewer_RenderWindowInteractor::unHighlightAll(){
  unHighlightSubSelection();

  using namespace SALOME::VTK;
  ForEach<SALOME_Actor>(GetRenderer()->GetActors(),
			TUnHighlightAllAction());

  emit RenderWindowModified() ;
  return false;
}

//-----------------
// Color methods
//-----------------

void VTKViewer_RenderWindowInteractor::SetColor(const Handle(SALOME_InteractiveObject)& theIObject,QColor theColor) 
{
  float aColor[3] = {theColor.red()/255., theColor.green()/255., theColor.blue()/255.};
  using namespace SALOME::VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,const float*>
			  (&SALOME_Actor::SetColor,aColor));
}


QColor VTKViewer_RenderWindowInteractor::GetColor(const Handle(SALOME_InteractiveObject)& theIObject) 
{
  using namespace SALOME::VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  if(anActor){
    float r,g,b;
    anActor->GetColor(r,g,b);
    return QColor(int(r*255),int(g*255),int(b*255));
  }
  return QColor(0,0,0);
}


bool VTKViewer_RenderWindowInteractor::isInViewer(const Handle(SALOME_InteractiveObject)& theIObject)
{
  using namespace SALOME::VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  return anActor != NULL;
}


bool VTKViewer_RenderWindowInteractor::isVisible(const Handle(SALOME_InteractiveObject)& theIObject)
{
  using namespace SALOME::VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(GetRenderer()->GetActors(),
		       TIsSameIObject<SALOME_Actor>(theIObject));
  return anActor != NULL && anActor->GetVisibility();
}


void VTKViewer_RenderWindowInteractor::rename(const Handle(SALOME_InteractiveObject)& theIObject, QString theName)
{
  using namespace SALOME::VTK;
  ForEachIf<SALOME_Actor>(GetRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TSetFunction<SALOME_Actor,const char*,QString>
			  (&SALOME_Actor::setName,theName.latin1()));
}


//----------------------------------------------------------------------------
bool VTKViewer_RenderWindowInteractor::highlight(const TColStd_IndexedMapOfInteger& theMapIndex,
						 SALOME_Actor* theMapActor, VTKViewer_Actor* theActor,
						 TUpdateActor theFun, bool hilight, bool update)
{
  if(theMapIndex.Extent() == 0) return false;
  
  if (hilight) {
    setActorData(theMapIndex,theMapActor,theActor,theFun);
    theActor->SetVisibility(true);
  }
  else {
    theActor->SetVisibility(false);
  }

  if(update){
    this->RenderWindow->Render();  
    emit RenderWindowModified() ;
  }

  return false;
}
  
void VTKViewer_RenderWindowInteractor::setActorData(const TColStd_IndexedMapOfInteger& theMapIndex,
						    SALOME_Actor* theMapActor,
						    VTKViewer_Actor *theActor,
						    TUpdateActor theFun)
{
  (*theFun)(theMapIndex,theMapActor,theActor);
}


//----------------------------------------------------------------------------
static void CellsUpdateActor(const TColStd_IndexedMapOfInteger& theMapIndex,
			     SALOME_Actor* theMapActor, 
			     VTKViewer_Actor* theActor)
{
  theActor->MapCells(theMapActor,theMapIndex);
}
  
bool VTKViewer_RenderWindowInteractor::highlightCell(const TColStd_IndexedMapOfInteger& theMapIndex,
						     SALOME_Actor* theMapActor, 
						     bool hilight, 
						     bool update)
{
  return highlight(theMapIndex,theMapActor,myCellActor,&CellsUpdateActor,hilight,update);
}
  
void VTKViewer_RenderWindowInteractor::setCellData(const int& theIndex, 
						   SALOME_Actor* theMapActor,
						   VTKViewer_Actor* theActor)
{
  TColStd_IndexedMapOfInteger MapIndex; 
  MapIndex.Add(theIndex);
  theActor->MapCells(theMapActor,MapIndex);
}


//----------------------------------------------------------------------------
static void PointsUpdateActor(const TColStd_IndexedMapOfInteger& theMapIndex,
			      SALOME_Actor* theMapActor, 
			      VTKViewer_Actor* theActor)
{
  theActor->MapPoints(theMapActor,theMapIndex);
}
  
bool VTKViewer_RenderWindowInteractor::highlightPoint(const TColStd_IndexedMapOfInteger& theMapIndex,
						      SALOME_Actor* theMapActor, 
						      bool hilight, 
						      bool update)
{
  return highlight(theMapIndex,theMapActor,myPointActor,&PointsUpdateActor,hilight,update);
}
  
void VTKViewer_RenderWindowInteractor::setPointData(const int& theIndex, 
						    SALOME_Actor* theMapActor,
						    VTKViewer_Actor* theActor)
{
  TColStd_IndexedMapOfInteger MapIndex; 
  MapIndex.Add(theIndex);
  theActor->MapPoints(theMapActor,MapIndex);
}

  
//----------------------------------------------------------------------------
static void EdgesUpdateActor(const TColStd_IndexedMapOfInteger& theMapIndex,
			     SALOME_Actor* theMapActor, 
			     VTKViewer_Actor* theActor)
{
  theActor->MapEdge(theMapActor,theMapIndex);
}
  
bool VTKViewer_RenderWindowInteractor::highlightEdge(const TColStd_IndexedMapOfInteger& theMapIndex,
						     SALOME_Actor* theMapActor, 
						     bool hilight, 
						     bool update)
{
  return highlight(theMapIndex,theMapActor,myEdgeActor,&EdgesUpdateActor,hilight,update);
}
  
void VTKViewer_RenderWindowInteractor::setEdgeData(const int& theCellIndex, 
						   SALOME_Actor* theMapActor,
						   const int& theEdgeIndex, 
						   VTKViewer_Actor* theActor )
{
  TColStd_IndexedMapOfInteger MapIndex; 
  MapIndex.Add(theCellIndex); 
  MapIndex.Add(theEdgeIndex);
  theActor->MapEdge(theMapActor,MapIndex);
}
