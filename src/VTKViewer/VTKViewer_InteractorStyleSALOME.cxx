using namespace std;
//  File      : VTKViewer_InteractorStyleSALOME.cxx
//  Created   : Wed Mar 20 11:37:34 2002
//  Author    : Christophe ATTANASIO
//  Project   : SALOME
//  Module    : VTKViewer
//  Copyright : Open CASCADE 2002
//  $Header$

#include "VTKViewer_InteractorStyleSALOME.h"
#include "VTKViewer_RenderWindow.h"

#include <qapplication.h>
#include "QAD_Application.h"
#include "QAD_Desktop.h"

#include "SALOME_Selection.h"
#include "SALOME_Actor.h"
#include "SALOME_ListIteratorOfListIO.hxx"

#include <vtkObjectFactory.h>
#include <vtkMath.h>
#include <vtkCommand.h>
#include <vtkAssemblyNode.h>
#include <vtkPicker.h>
#include <vtkPointPicker.h>
#include <vtkCellPicker.h>
#include <vtkLine.h> 
#include <vtkUnstructuredGrid.h> 
#include <vtkExtractEdges.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataSetCollection.h>

//VRV: porting on Qt 3.0.5
#if QT_VERSION >= 0x030005
#include <qpainter.h>
#endif
//VRV: porting on Qt 3.0.5

//----------------------------------------------------------------------------
VTKViewer_InteractorStyleSALOME *VTKViewer_InteractorStyleSALOME::New() 
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("VTKViewer_InteractorStyleSALOME");
  if(ret)
    {
      return (VTKViewer_InteractorStyleSALOME*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new VTKViewer_InteractorStyleSALOME;
}


//----------------------------------------------------------------------------
VTKViewer_InteractorStyleSALOME::VTKViewer_InteractorStyleSALOME() 
{
  m_Triedron = 0;
  this->MotionFactor = 10.0;
  this->State = VTK_INTERACTOR_STYLE_CAMERA_NONE;
  this->RadianToDegree = 180.0 / vtkMath::Pi();
  this->ForcedState = VTK_INTERACTOR_STYLE_CAMERA_NONE;
  loadCursors();
}

//----------------------------------------------------------------------------
VTKViewer_InteractorStyleSALOME::~VTKViewer_InteractorStyleSALOME() 
{
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::setTriedron( vtkActorCollection* triedron )
{
  m_Triedron = triedron;
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::RotateXY(int dx, int dy)
{
  double rxf;
  double ryf;
  vtkCamera *cam;
  
  if (this->CurrentRenderer == NULL)
    {
      return;
    }
  
  int *size = this->CurrentRenderer->GetRenderWindow()->GetSize();
  this->DeltaElevation = -20.0 / size[1];
  this->DeltaAzimuth = -20.0 / size[0];
  
  rxf = (double)dx * this->DeltaAzimuth *  this->MotionFactor;
  ryf = (double)dy * this->DeltaElevation * this->MotionFactor;
  
  cam = this->CurrentRenderer->GetActiveCamera();
  cam->Azimuth(rxf);
  cam->Elevation(ryf);
  cam->OrthogonalizeViewUp();
  this->CurrentRenderer->ResetCameraClippingRange();
  vtkRenderWindowInteractor *rwi = this->Interactor;
  if (this->CurrentLight)
    {
      // get the first light
      this->CurrentLight->SetPosition(cam->GetPosition());
      this->CurrentLight->SetFocalPoint(cam->GetFocalPoint());
    }	
  rwi->Render();
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::PanXY(int x, int y, int oldX, int oldY)
{
  TranslateView(x, y, oldX, oldY);   
  vtkRenderWindowInteractor *rwi = this->Interactor;
  if (this->CurrentLight)
    {
      /* get the first light */
      vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();
      this->CurrentLight->SetPosition(cam->GetPosition());
      this->CurrentLight->SetFocalPoint(cam->GetFocalPoint());
    }
    
  rwi->Render();
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::DollyXY(int dx, int dy)
{
  vtkCamera *cam;
  double dxf = this->MotionFactor * (double)(dx) / (double)(this->Center[1]);
  double dyf = this->MotionFactor * (double)(dy) / (double)(this->Center[1]);

  double zoomFactor = pow((double)1.1, dxf + dyf);
  
  if (this->CurrentRenderer == NULL)
    {
      return;
    }
  
  cam = this->CurrentRenderer->GetActiveCamera();
    if (cam->GetParallelProjection())
    {
      cam->SetParallelScale(cam->GetParallelScale()/zoomFactor);
    }
  else
    {
      cam->Dolly(zoomFactor);
      this->CurrentRenderer->ResetCameraClippingRange();
    }
  
  if (this->CurrentLight)
    {
      /* get the first light */
      this->CurrentLight->SetPosition(cam->GetPosition());
      this->CurrentLight->SetFocalPoint(cam->GetFocalPoint());
    }
  
  this->Interactor->Render();
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::SpinXY(int x, int y, int oldX, int oldY)
{
  vtkRenderWindowInteractor *rwi = this->Interactor;
  vtkCamera *cam;

  if (this->CurrentRenderer == NULL)
    {
      return;
    }

  double newAngle = atan2((double)(y - this->Center[1]),
			  (double)(x - this->Center[0]));
  double oldAngle = atan2((double)(oldY -this->Center[1]),
			  (double)(oldX - this->Center[0]));
  
  newAngle *= this->RadianToDegree;
  oldAngle *= this->RadianToDegree;

  cam = this->CurrentRenderer->GetActiveCamera();
  cam->Roll(newAngle - oldAngle);
  cam->OrthogonalizeViewUp();
      
  rwi->Render();
}


//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::OnMouseMove(int vtkNotUsed(ctrl), 
						  int shift,
						  int x, int y) 
{
  myShiftState = shift;
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
    onOperation(QPoint(x, y));
  else if (ForcedState == VTK_INTERACTOR_STYLE_CAMERA_NONE)
    onCursorMove(QPoint(x, y));
}


//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::OnLeftButtonDown(int ctrl, int shift, 
						       int x, int y) 
{
  if (this->HasObserver(vtkCommand::LeftButtonPressEvent)) 
    {
    this->InvokeEvent(vtkCommand::LeftButtonPressEvent,NULL);
    return;
    }
  this->FindPokedRenderer(x, y);
  if (this->CurrentRenderer == NULL)
    {
    return;
    }
  myShiftState = shift;
  // finishing current viewer operation
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  myOtherPoint = myPoint = QPoint(x, y);
  if (ForcedState != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    startOperation(ForcedState);
  }
  else {
    if (ctrl)
      startOperation(VTK_INTERACTOR_STYLE_CAMERA_ZOOM);
    else
      startOperation(VTK_INTERACTOR_STYLE_CAMERA_SELECT);
  }
  return;
}
//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::OnLeftButtonUp(int vtkNotUsed(ctrl),
						     int shift, 
						     int vtkNotUsed(x),
						     int vtkNotUsed(y))
{
  myShiftState = shift;
  // finishing current viewer operation
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::OnMiddleButtonDown(int ctrl,
							 int shift, 
							 int x, int y) 
{
  if (this->HasObserver(vtkCommand::MiddleButtonPressEvent)) 
    {
      this->InvokeEvent(vtkCommand::MiddleButtonPressEvent,NULL);
      return;
    }
  this->FindPokedRenderer(x, y);
  if (this->CurrentRenderer == NULL)
    {
      return;
    }
  myShiftState = shift;
  // finishing current viewer operation
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  myOtherPoint = myPoint = QPoint(x, y);
  if (ForcedState != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    startOperation(ForcedState);
  }
  else {
    if (ctrl)
      startOperation(VTK_INTERACTOR_STYLE_CAMERA_PAN);
  }
}
//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::OnMiddleButtonUp(int vtkNotUsed(ctrl),
						       int shift, 
						       int vtkNotUsed(x),
						       int vtkNotUsed(y))
{
  myShiftState = shift;
  // finishing current viewer operation
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::OnRightButtonDown(int ctrl,
							int shift, 
							int x, int y) 
{
  if (this->HasObserver(vtkCommand::RightButtonPressEvent)) 
    {
      this->InvokeEvent(vtkCommand::RightButtonPressEvent,NULL);
      return;
    }
  this->FindPokedRenderer(x, y);
  if (this->CurrentRenderer == NULL)
    {
      return;
    }
  myShiftState = shift;
  // finishing current viewer operation
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  myOtherPoint = myPoint = QPoint(x, y);
  if (ForcedState != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    startOperation(ForcedState);
  }
  else {
    if (ctrl)
      startOperation(VTK_INTERACTOR_STYLE_CAMERA_ROTATE);  
  }
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::OnRightButtonUp(int vtkNotUsed(ctrl),
						      int shift, 
						      int vtkNotUsed(x),
						      int vtkNotUsed(y))
{
  myShiftState = shift;
  // finishing current viewer operation
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE) {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkInteractorStyle::PrintSelf(os,indent);

}

/* XPM */
const char* imageZoomCursor[] = { 
"32 32 3 1",
". c None",
"a c #000000",
"# c #ffffff",
"................................",
"................................",
".#######........................",
"..aaaaaaa.......................",
"................................",
".............#####..............",
"...........##.aaaa##............",
"..........#.aa.....a#...........",
".........#.a.........#..........",
".........#a..........#a.........",
"........#.a...........#.........",
"........#a............#a........",
"........#a............#a........",
"........#a............#a........",
"........#a............#a........",
".........#...........#.a........",
".........#a..........#a.........",
".........##.........#.a.........",
"........#####.....##.a..........",
".......###aaa#####.aa...........",
"......###aa...aaaaa.......#.....",
".....###aa................#a....",
"....###aa.................#a....",
"...###aa...............#######..",
"....#aa.................aa#aaaa.",
".....a....................#a....",
"..........................#a....",
"...........................a....",
"................................",
"................................",
"................................",
"................................"};

const char* imageRotateCursor[] = { 
"32 32 3 1",
". c None",
"a c #000000",
"# c #ffffff",
"................................",
"................................",
"................................",
"................................",
"........#.......................",
".......#.a......................",
"......#######...................",
".......#aaaaa#####..............",
"........#..##.a#aa##........##..",
".........a#.aa..#..a#.....##.aa.",
".........#.a.....#...#..##.aa...",
".........#a.......#..###.aa.....",
"........#.a.......#a..#aa.......",
"........#a.........#..#a........",
"........#a.........#a.#a........",
"........#a.........#a.#a........",
"........#a.........#a.#a........",
".........#.........#a#.a........",
"........##a........#a#a.........",
"......##.a#.......#.#.a.........",
"....##.aa..##.....##.a..........",
"..##.aa.....a#####.aa...........",
"...aa.........aaa#a.............",
"................#.a.............",
"...............#.a..............",
"..............#.a...............",
"...............a................",
"................................",
"................................",
"................................",
"................................",
"................................"};

// loads cursors for viewer operations - zoom, pan, etc...
void VTKViewer_InteractorStyleSALOME::loadCursors()
{
  myDefCursor       = QCursor(ArrowCursor);
  myHandCursor      = QCursor(PointingHandCursor);
  myPanCursor       = QCursor(SizeAllCursor);
  myZoomCursor      = QCursor(QPixmap(imageZoomCursor));
  myRotateCursor    = QCursor(QPixmap(imageRotateCursor));
  mySpinCursor      = QCursor(QPixmap(imageRotateCursor)); // temporarly !!!!!!
  myGlobalPanCursor = QCursor(CrossCursor);
  myCursorState     = false;
}

// event filter - controls mouse and keyboard events during viewer operations
bool VTKViewer_InteractorStyleSALOME::eventFilter(QObject* object, QEvent* event)
{
  VTKViewer_RenderWindow* wnd = dynamic_cast<VTKViewer_RenderWindow*>(GetInteractor()->GetRenderWindow());
  if ( (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::KeyPress) && object != wnd)
  {
    qApp->removeEventFilter(this);
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  return QObject::eventFilter(object, event);
}

// starts Zoom operation (e.g. through menu command)
void VTKViewer_InteractorStyleSALOME::startZoom()
{
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
  {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  setCursor(VTK_INTERACTOR_STYLE_CAMERA_ZOOM);
  ForcedState = VTK_INTERACTOR_STYLE_CAMERA_ZOOM;
  qApp->installEventFilter(this);
}

// starts Pan operation (e.g. through menu command)
void VTKViewer_InteractorStyleSALOME::startPan()
{
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
  {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  setCursor(VTK_INTERACTOR_STYLE_CAMERA_PAN);
  ForcedState = VTK_INTERACTOR_STYLE_CAMERA_PAN;
  qApp->installEventFilter(this);
}

// starts Rotate operation (e.g. through menu command)
void VTKViewer_InteractorStyleSALOME::startRotate()
{
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
  {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  setCursor(VTK_INTERACTOR_STYLE_CAMERA_ROTATE);
  ForcedState = VTK_INTERACTOR_STYLE_CAMERA_ROTATE;
  qApp->installEventFilter(this);
}

// starts Spin operation (e.g. through menu command)
void VTKViewer_InteractorStyleSALOME::startSpin()
{
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
  {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  setCursor(VTK_INTERACTOR_STYLE_CAMERA_SPIN);
  ForcedState = VTK_INTERACTOR_STYLE_CAMERA_SPIN;
  qApp->installEventFilter(this);
}


// starts Fit Area operation (e.g. through menu command)
void VTKViewer_InteractorStyleSALOME::startFitArea()
{
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
  {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  setCursor(VTK_INTERACTOR_STYLE_CAMERA_FIT);
  ForcedState = VTK_INTERACTOR_STYLE_CAMERA_FIT;
  qApp->installEventFilter(this);
}

// starts Global Panning operation (e.g. through menu command)
void VTKViewer_InteractorStyleSALOME::startGlobalPan()
{
  if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
  {
    onFinishOperation();
    startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
  }
  setCursor(VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN);
  ForcedState = VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN;

  // store current zoom scale
  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();
  myScale = cam->GetParallelScale();

  // make fit all
  Standard_Boolean TriedronWasVisible = false;
  if ( m_Triedron ) {
    m_Triedron->InitTraversal();
    vtkActor *ac = m_Triedron->GetNextActor();
    while(!(ac==NULL)) {
      if(ac->GetVisibility()) {
	TriedronWasVisible = true;
	ac->VisibilityOff();
      }
      ac = m_Triedron->GetNextActor();
    }
  }
  this->CurrentRenderer->ResetCamera();
  this->CurrentRenderer->ResetCameraClippingRange();
  if( m_Triedron && TriedronWasVisible ) {
    m_Triedron->InitTraversal();
    vtkActor *ac = m_Triedron->GetNextActor();
    while(!(ac==NULL)) {
      ac->VisibilityOn();
      ac = m_Triedron->GetNextActor();
    }
  }
  VTKViewer_RenderWindow* aRW = dynamic_cast<VTKViewer_RenderWindow*>(this->Interactor->GetRenderWindow());
  if (aRW) aRW->updateGL();
  
  qApp->installEventFilter(this);
}

// returns TRUE if needs redrawing
bool VTKViewer_InteractorStyleSALOME::needsRedrawing()
{
  return State == VTK_INTERACTOR_STYLE_CAMERA_ZOOM   ||
         State == VTK_INTERACTOR_STYLE_CAMERA_PAN    ||
         State == VTK_INTERACTOR_STYLE_CAMERA_ROTATE ||
         State == VTK_INTERACTOR_STYLE_CAMERA_SPIN   ||
         State == VTK_INTERACTOR_STYLE_CAMERA_NONE;
}

// fits viewer contents to rect
void VTKViewer_InteractorStyleSALOME::fitRect(const int left, 
                                       const int top, 
                                       const int right, 
                                       const int bottom)
{
  if (this->CurrentRenderer == NULL) {
    return;
  }
  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();

  // move camera
  int x = (left + right)/2;
  int y = (top + bottom)/2;
  int *aSize = this->CurrentRenderer->GetRenderWindow()->GetSize();
  int oldX = aSize[0]/2;
  int oldY = aSize[1]/2;
  TranslateView(oldX, oldY, x, y);
      
  // zoom camera
  double dxf = (double)(aSize[0]) / (double)(abs(right - left));
  double dyf = (double)(aSize[1]) / (double)(abs(bottom - top));
  double zoomFactor = (dxf + dyf)/2 ;

  if (cam->GetParallelProjection()) {
    cam->SetParallelScale(cam->GetParallelScale()/zoomFactor);
  } else {
    cam->Dolly(zoomFactor);
    this->CurrentRenderer->ResetCameraClippingRange();
  }
  
  vtkRenderWindowInteractor *rwi = this->Interactor;
  if (this->CurrentLight) {
    /* get the first light */
    this->CurrentLight->SetPosition(cam->GetPosition());
    this->CurrentLight->SetFocalPoint(cam->GetFocalPoint());
  }
  //  rwi->Render();
  VTKViewer_RenderWindow* aRW = dynamic_cast<VTKViewer_RenderWindow*>(rwi->GetRenderWindow());
  if (aRW) aRW->updateGL();
}



// starts viewer operation (!internal usage!)
void VTKViewer_InteractorStyleSALOME::startOperation(int operation)
{
  switch(operation)
  { 
  case VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN:
  case VTK_INTERACTOR_STYLE_CAMERA_ZOOM:
  case VTK_INTERACTOR_STYLE_CAMERA_PAN:
  case VTK_INTERACTOR_STYLE_CAMERA_ROTATE:
  case VTK_INTERACTOR_STYLE_CAMERA_SPIN:
  case VTK_INTERACTOR_STYLE_CAMERA_FIT:
  case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
    if (State != VTK_INTERACTOR_STYLE_CAMERA_NONE)
      startOperation(VTK_INTERACTOR_STYLE_CAMERA_NONE);
    State = operation;
    if (State != VTK_INTERACTOR_STYLE_CAMERA_SELECT)
      setCursor(operation);
    onStartOperation();
    break;
  case VTK_INTERACTOR_STYLE_CAMERA_NONE:
  default:
    setCursor(VTK_INTERACTOR_STYLE_CAMERA_NONE);
    State = ForcedState = VTK_INTERACTOR_STYLE_CAMERA_NONE;
    break;
  }
}

// sets proper cursor for window when viewer operation is activated
void VTKViewer_InteractorStyleSALOME::setCursor(const int operation)
{
   VTKViewer_RenderWindow* wnd = dynamic_cast< VTKViewer_RenderWindow*>(GetInteractor()->GetRenderWindow());
  switch (operation)
  {
    case VTK_INTERACTOR_STYLE_CAMERA_ZOOM:
      wnd->setCursor(myZoomCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_PAN:
      wnd->setCursor(myPanCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_ROTATE:
      wnd->setCursor(myRotateCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_SPIN:
      wnd->setCursor(mySpinCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN:
      wnd->setCursor(myGlobalPanCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
      wnd->setCursor(myHandCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_NONE:
    default:
      wnd->setCursor(myDefCursor); 
      myCursorState = false;
      break;
  }
}

// called when viewer operation started (!put necessary initialization here!)
void VTKViewer_InteractorStyleSALOME::onStartOperation()
{
   VTKViewer_RenderWindow* wnd = dynamic_cast< VTKViewer_RenderWindow*>(GetInteractor()->GetRenderWindow());
  switch (State) {
    case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
    case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    {
      QPainter p(wnd);
      p.setPen(Qt::lightGray);
      p.setRasterOp(Qt::XorROP);
      p.drawRect(QRect(myPoint, myOtherPoint));
      break;
    }
    case VTK_INTERACTOR_STYLE_CAMERA_ZOOM:
    case VTK_INTERACTOR_STYLE_CAMERA_PAN:
    case VTK_INTERACTOR_STYLE_CAMERA_ROTATE:
    case VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN:
    case VTK_INTERACTOR_STYLE_CAMERA_SPIN:
      break;
  }
}

// called when viewer operation finished (!put necessary post-processing here!)
void VTKViewer_InteractorStyleSALOME::onFinishOperation() 
{
  QAD_Study* aActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* aSel    = SALOME_Selection::Selection( aActiveStudy->getSelection() );
  VTKViewer_RenderWindow* wnd = dynamic_cast< VTKViewer_RenderWindow*>(GetInteractor()->GetRenderWindow());
  vtkRenderWindowInteractor *rwi = this->Interactor;

  int aSelectionMode = aSel->SelectionMode();
  bool aSelActiveCompOnly = aSel->IsSelectActiveCompOnly();
  SALOMEDS::SComponent_var aActiveComponent = SALOMEDS::SComponent::_narrow(
     aActiveStudy->getStudyDocument()->FindObject(QAD_Application::getDesktop()->getActiveComponent()));

  switch (State) {
    case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
    case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    {
      QPainter p(wnd);
      p.setPen(Qt::lightGray);
      p.setRasterOp(Qt::XorROP);
      QRect rect(myPoint, myOtherPoint);
      p.drawRect(rect);
      rect = rect.normalize();
      if (State == VTK_INTERACTOR_STYLE_CAMERA_FIT) {
        // making fit rect opeation 
        int w, h, x, y;
        rwi->GetSize(w, h);
        int x1, y1, x2, y2;
        x1 = rect.left(); 
        y1 = h - rect.top() - 1;
        x2 = rect.right(); 
        y2 = h - rect.bottom() - 1;
        fitRect(x1, y1, x2, y2);
      }
      else {
        if (myPoint == myOtherPoint) {
        // process point selection
          int w, h, x, y;
          rwi->GetSize(w, h);
          x = myPoint.x(); 
          y = h - myPoint.y() - 1;
          vtkActorCollection* listactors = NULL;
          this->FindPokedRenderer(x, y);
	  rwi->StartPickCallback();
          rwi->GetPicker()->Pick(x, y, 0.0, this->CurrentRenderer);
    
          if ( rwi->GetPicker()->IsA("vtkCellPicker") ) {
            vtkCellPicker* picker;
            if ( (picker = vtkCellPicker::SafeDownCast(rwi->GetPicker())) ) {
	      MESSAGE ( " CellId : " << picker->GetCellId() );
              if ( picker->GetCellId() >= 0 ) {
                vtkActor* ac = picker->GetActor();
                if ( ac->IsA("SALOME_Actor") ) {
                  SALOME_Actor* SActor = SALOME_Actor::SafeDownCast( ac );
                  MESSAGE ( " NAME Actor : " << SActor->getName() );

                  //Cell selection //////////////////////////////////// NB
                  if ( aSelectionMode == 3 ) {
                    if ( SActor->hasIO() ) {
                      Handle(SALOME_InteractiveObject) IO = SActor->getIO();
                      // Look in the current selection
                      SALOME_ListIteratorOfListIO It(aSel->StoredIObjects());
                      Standard_Boolean IsSelected = false;
                      for(;It.More();It.Next()) {
                        Handle(SALOME_InteractiveObject) IOS = It.Value();
                        if(IO->isSame(IOS)) {
                          IsSelected = true;
			  IO = IOS; //Added by SRN, fix SAL1307
                          break;
	                }
                      }
		      if(IsSelected) {
                        // This IO is already in the selection
		        //if(shift) {
		          bool add = aSel->AddOrRemoveIndex( IO, picker->GetCellId(), myShiftState, false );
		          //Sel->RemoveIObject(IO);
		        //}
		      } else {
		        if(!myShiftState) {
		          this->HighlightProp( NULL );
		          aSel->ClearIObjects();
                        }
                        bool add = aSel->AddOrRemoveIndex( IO, picker->GetCellId(), myShiftState, false );
                        aSel->AddIObject( IO, false );
		      }
                    }
	          }
                  //Edge selection ////////////////////////// NB
                  else if ( aSelectionMode == 2 ) {
                    if ( SActor->hasIO() ) {
                      Handle(SALOME_InteractiveObject) IO = SActor->getIO();
                      float pickPosition[3],pcoords[3],closestPoint[3],weights[3],dist1=1000000.0,dist2=0;
                      int subId,edgeId,pickedID,result;
                      pickedID = picker->GetCellId();
                      picker->GetPickPosition(pickPosition);
                       MESSAGE("Position = "<<pickPosition[0]<<" "<<pickPosition[1]<<" "<<pickPosition[2]);
                      vtkUnstructuredGrid* UGrid = vtkUnstructuredGrid::SafeDownCast( SActor->GetMapper()->GetInput());
		      if (!UGrid) break;
                      vtkCell* pickedCell = UGrid->GetCell(pickedID);
		      if (!pickedCell) break;
                      vtkCell* edge;
                      vtkLine* line;
                      for (int i=0;i<pickedCell->GetNumberOfEdges();i++) {
                        edge = pickedCell->GetEdge(i);
                        if (edge->GetCellType() == 3) { //3 for VTK_LINE
                          line = (vtkLine*)edge;
                          result = line->EvaluatePosition(pickPosition,closestPoint,subId,pcoords,dist2,weights);
                          MESSAGE("edge "<< i <<" dist = " << dist2);
                          if (dist2 < dist1) {
                            dist1  = dist2;
                            edgeId = i;
                          }
                        }
                      }
                      MESSAGE("edgeID transformed = "<<edgeId);
                      // Look in the current selection
                      SALOME_ListIteratorOfListIO It(aSel->StoredIObjects());
                      Standard_Boolean IsSelected = false;
                      for(;It.More();It.Next()) {
                        Handle(SALOME_InteractiveObject) IOS = It.Value();
                        if(IO->isSame(IOS)) {
			  IO = IOS; //Added by SRN, fix SAL1307
                          IsSelected = true;
                          break;
                        }
                      }
                      if(!myShiftState) {
                        this->HighlightProp( NULL );
                        aSel->ClearIObjects();
                      }
                      aSel->SetSelectionMode(3, true);
                      bool add = aSel->AddOrRemoveIndex( IO, pickedID, 0 );
                      aSel->SetSelectionMode(2, true);
		      edgeId = -edgeId;
                      add = aSel->AddOrRemoveIndex( IO, edgeId, 0, false );
                      aSel->AddIObject( IO, false );
                    }
                  }
                } else {
                  this->HighlightProp( NULL );
                  aSel->ClearIObjects();
                }
              }
            }
          } else if ( rwi->GetPicker()->IsA("vtkPointPicker") ) {
            vtkPointPicker* picker;
            if ( (picker = vtkPointPicker::SafeDownCast(rwi->GetPicker())) ) {
              MESSAGE ( " PointId : " << picker->GetPointId() );
              if ( picker->GetPointId() >= 0 ) {
                vtkActor* ac = picker->GetActor();
                if ( ac->IsA("SALOME_Actor") ) {
                  SALOME_Actor* SActor = SALOME_Actor::SafeDownCast( ac );
                  MESSAGE ( " NAME Actor : " << SActor->getName() );
                  if ( SActor->hasIO() ) {
                    Handle(SALOME_InteractiveObject) IO = SActor->getIO();
/*		    if (IO.IsNull()) 
		      break;
		    if (aSelActiveCompOnly && 
			strcmp(aActiveComponent->ComponentDataType(), IO->getComponentDataType()) != 0) {
		      break;
		    }*/
                    // Look in the current selection
                    SALOME_ListIteratorOfListIO It(aSel->StoredIObjects());
                    Standard_Boolean IsSelected = false;
                    for(;It.More();It.Next()) {
                      Handle(SALOME_InteractiveObject) IOS = It.Value();
                      if(IO->isSame(IOS)) {
			IO = IOS; //Added by SRN, fix SAL1307
                        IsSelected = true;
                        break;
                      }
                    }
                    if(IsSelected) {
                      // This IO is already in the selection
                      bool add = aSel->AddOrRemoveIndex( IO, picker->GetPointId(), myShiftState, false );
                    } else {
                      if(!myShiftState) {
                        this->HighlightProp( NULL );
                        aSel->ClearIObjects();
                      }
                      bool add = aSel->AddOrRemoveIndex( IO, picker->GetPointId(), myShiftState, false );
                      aSel->AddIObject( IO, false );
                    }
                  }
                } 
              } else {
                this->HighlightProp( NULL );
                aSel->ClearIObjects();
              } 
            }
          } else {
            vtkPicker* picker;
            if ( (picker = vtkPicker::SafeDownCast(rwi->GetPicker())) )	{
              listactors = picker->GetActors();
            }
            if ( listactors->GetNumberOfItems() == 0 ) {
              // No selection clear all
              this->PropPicked = 0;
              this->HighlightProp( NULL );
              aSel->ClearIObjects();
            } else {
              vtkActor* ac;
              listactors->InitTraversal();
              ac = listactors->GetNextActor();
              if ( ac->IsA("SALOME_Actor") ) {
                SALOME_Actor* SActor = SALOME_Actor::SafeDownCast( ac );
                if ( SActor->hasIO() ) {      
                  this->PropPicked++;
                  Handle(SALOME_InteractiveObject) IO = SActor->getIO();
                  // Look in the current selection
                  SALOME_ListIteratorOfListIO It(aSel->StoredIObjects());
                  Standard_Boolean IsSelected = false;
                  for(;It.More();It.Next()) {
                    Handle(SALOME_InteractiveObject) IOS = It.Value();
                    if( IO->isSame(IOS) ) {
		      IO = IOS; //Added by SRN, fix SAL1307
                      IsSelected = true;
                      break;
                    }
                  }
                  if(IsSelected) {
                    // This IO is already in the selection
                    if(myShiftState) {
                      aSel->RemoveIObject(IO);
                    }
                  }
                  else {
                    if(!myShiftState) {
                      this->HighlightProp( NULL );
                      aSel->ClearIObjects();
                    }
                    aSel->AddIObject( IO, false );
                  }
                }
              }
            }
	    rwi->EndPickCallback();
          }
        } else {
          //processing rectangle selection
	  rwi->StartPickCallback();

	  if (!myShiftState) {
	    this->PropPicked = 0;
	    this->HighlightProp( NULL );
	    aSel->ClearIObjects();
	  }

	  // Compute bounds
	  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();
	  QRect rect(myPoint, myOtherPoint);
	  rect = rect.normalize();
	  int w, h, x, y;
	  rwi->GetSize(w, h);
	  int x1, y1, x2, y2;
	  x1 = rect.left(); 
	  y1 = h - rect.top() - 1;
	  x2 = rect.right(); 
	  y2 = h - rect.bottom() - 1;

	  switch (aSelectionMode) {
	  case 1: // Nodes selection
	    {
	      if (! rwi->GetPicker()->IsA("vtkPointPicker") ) break;
	      vtkPointPicker* aPointPicker = vtkPointPicker::SafeDownCast(rwi->GetPicker());
	      vtkActorCollection* aListActors = this->CurrentRenderer->GetActors();
	      aListActors->InitTraversal();
	      vtkActor* aActor;
	      for (int k = 0; k < aListActors->GetNumberOfItems(); k++) {
		aActor = aListActors->GetNextActor();
		if (aActor != NULL) {
		  if (aActor->GetVisibility() == 0) 
		    continue;
		  vtkAbstractMapper3D* aMapper3D = aActor->GetMapper();
		  if ((aMapper3D != NULL) && (aActor->IsA("SALOME_Actor"))) {
		    SALOME_Actor* SActor = SALOME_Actor::SafeDownCast(aActor);

		    if ((SActor != NULL) && (SActor->hasIO())) {
		      Handle(SALOME_InteractiveObject) IO = SActor->getIO();
		      if (IO.IsNull()) 
			continue;
		      if (aSelActiveCompOnly && 
                              strcmp(aActiveComponent->ComponentDataType(), IO->getComponentDataType()) != 0) {
			continue;
		      }

		      vtkMapper*       aMapper;
		      vtkVolumeMapper* aVolumeMapper;
		      vtkDataSet*      aDataSet;
		      
		      if ( (aMapper = vtkMapper::SafeDownCast(aMapper3D)) != NULL ) {
			aDataSet = aMapper->GetInput();
		      } else if ((aVolumeMapper = vtkVolumeMapper::SafeDownCast(aMapper3D)) != NULL ){
			aDataSet = aVolumeMapper->GetInput();
		      } else {
			continue;
		      }
		      if (aDataSet) {
			for (int i=0; i < aDataSet->GetNumberOfPoints(); i++) {
			  float* aPoint;
			  aPoint = aDataSet->GetPoint(i);
			  if (IsInRect(aPoint,  x1, y1, x2, y2)) {
			    float aDisp[3];
			    ComputeWorldToDisplay(aPoint[0],
						  aPoint[1],
						  aPoint[2], aDisp);
			    aPointPicker->Pick(aDisp[0], aDisp[1], 0.0, CurrentRenderer);
			    if ( aPointPicker->GetPointId() >= 0) { // && (!aSel->IsIndexSelected(IO, aPointPicker->GetPointId()))) {
			      aSel->AddOrRemoveIndex(IO, aPointPicker->GetPointId(), true, false);
			      aSel->AddIObject(IO, false);
			    }
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	    break;
	  case 2: // edges selection
	  case 3: // triangles selection
	    {
	      if (! rwi->GetPicker()->IsA("vtkCellPicker") ) break;
	      vtkCellPicker* aCellPicker = vtkCellPicker::SafeDownCast(rwi->GetPicker());
	      vtkActorCollection* aListActors = this->CurrentRenderer->GetActors();
	      aListActors->InitTraversal();
	      vtkActor* aActor;
	      for (int k = 0; k < aListActors->GetNumberOfItems(); k++) {
		aActor = aListActors->GetNextActor();
		if (aActor != NULL) {
		  if (aActor->GetVisibility() == 0) 
		    continue;
		  vtkAbstractMapper3D* aMapper3D = aActor->GetMapper();
		  if ((aMapper3D != NULL) && (aActor->IsA("SALOME_Actor"))) {
		    SALOME_Actor* SActor = SALOME_Actor::SafeDownCast(aActor);

		    if ((SActor != NULL) && (SActor->hasIO())) {
		      Handle(SALOME_InteractiveObject) IO = SActor->getIO();
		      if (IO.IsNull()) 
			continue;
		      if (aSelActiveCompOnly && 
                              strcmp(aActiveComponent->ComponentDataType(), IO->getComponentDataType()) != 0) {
			continue;
		      }

		      vtkMapper*       aMapper;
		      vtkVolumeMapper* aVolumeMapper;
		      vtkDataSet*      aDataSet;
		      
		      if ( (aMapper = vtkMapper::SafeDownCast(aMapper3D)) != NULL ) {
			aDataSet = aMapper->GetInput();
		      } else if ((aVolumeMapper = vtkVolumeMapper::SafeDownCast(aMapper3D)) != NULL ){
			aDataSet = aVolumeMapper->GetInput();
		      } else {
			continue;
		      }
		      if (aDataSet) {
			for (int i=0; i < aDataSet->GetNumberOfCells(); i++) {
			  vtkCell* aCell = aDataSet->GetCell(i);
			  if (aCell != NULL) {
			    if (IsInRect(aCell,  x1, y1, x2, y2)) {
			      float* aBounds = aCell->GetBounds();
			      float aCenter[3];
			      aCenter[0] =(aBounds[0] + aBounds[1])/2; // Center X
			      aCenter[1] =(aBounds[2] + aBounds[3])/2; // Center Y
			      aCenter[2] =(aBounds[4] + aBounds[5])/2; // Center Z
			      float aDisp[3];
			      ComputeWorldToDisplay(aCenter[0],
						    aCenter[1],
						    aCenter[2], aDisp);
			      aCellPicker->Pick(aDisp[0], aDisp[1], 0.0, CurrentRenderer);
			      if (aSelectionMode == 3) {
				if ( aCellPicker->GetCellId() >= 0 && 
				    (!aSel->IsIndexSelected(IO, aCellPicker->GetCellId()))) {
				  aSel->AddOrRemoveIndex( IO, aCellPicker->GetCellId(), true, false);
				  aSel->AddIObject( IO, false );
				}
			      } else {
				vtkCell* aTriangleCell = aDataSet->GetCell(aCellPicker->GetCellId());
				vtkCell* aEdge;
				if (!aSel->IsIndexSelected(IO, aCellPicker->GetCellId())) {
				  aSel->SetSelectionMode(3, true);
				  aSel->AddOrRemoveIndex(IO, aCellPicker->GetCellId(), true, false);
				}
				aSel->SetSelectionMode(2, true);
				for (int i=0; i < aTriangleCell->GetNumberOfEdges(); i++) {
				  aSel->AddOrRemoveIndex( IO, i, true, false);
				}
				aSel->AddIObject( IO, false );
			      }
			    }
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	    break;
	  case 4: // objects selection
	    {
	      vtkActorCollection* aListActors = this->CurrentRenderer->GetActors();
	      aListActors->InitTraversal();
	      vtkActor* aActor;
	      SALOME_ListIO aListIO;
	      for (int k = 0; k < aListActors->GetNumberOfItems(); k++) {
		aActor = aListActors->GetNextActor();
		if (aActor) {
		  if (aActor->GetVisibility() == 0) 
		    continue;
		  if ( aActor->IsA("SALOME_Actor") ) {
		    SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast(aActor);
		    if ( aSActor->hasIO() && IsInRect(aSActor, x1, y1, x2, y2)) {   
		      Handle(SALOME_InteractiveObject) aIO = aSActor->getIO();
		      if (aIO.IsNull())
			continue;
		      if (aSelActiveCompOnly && 
                              strcmp(aActiveComponent->ComponentDataType(), aIO->getComponentDataType()) != 0) {
			continue;
		      }
		      if (aListIO.IsEmpty()) {
			aListIO.Append( aIO );
		      } else {
			SALOME_ListIteratorOfListIO It(aListIO);
			bool isStored = false;
			for(;It.More();It.Next()) {
			  Handle(SALOME_InteractiveObject) IOS = It.Value();
			  if( aIO->isSame(IOS) ) {
			    aIO = IOS; //Added by SRN, fix SAL1307
			    isStored = true;
			    break;
			  }
			}
			if (!isStored)
			  aListIO.Append( aIO );
		      }
		    }
		  }
		}
	      }
	      if (!aListIO.IsEmpty()) {
		SALOME_ListIteratorOfListIO It(aListIO);
		for(;It.More();It.Next()) {
		  Handle(SALOME_InteractiveObject) IOS = It.Value();
		  this->PropPicked++;
		  aSel->AddIObject( IOS, false );
		}
	      }
	    } // end case 4
	  } //end switch
	  rwi->EndPickCallback();
	}
	aActiveStudy->update3dViewers();
      } 
    } 
    break;
  case VTK_INTERACTOR_STYLE_CAMERA_ZOOM:
  case VTK_INTERACTOR_STYLE_CAMERA_PAN:
  case VTK_INTERACTOR_STYLE_CAMERA_ROTATE:
  case VTK_INTERACTOR_STYLE_CAMERA_SPIN:
    break;
  case VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN: 
    {
      int w, h, x, y;
      rwi->GetSize(w, h);
      x = myPoint.x(); 
      y = h - myPoint.y() - 1;
      Place(x, y);
    }
    break;
  }
}

// called during viewer operation when user moves mouse (!put necessary processing here!)
void VTKViewer_InteractorStyleSALOME::onOperation(QPoint mousePos) 
{
  int w, h;
  GetInteractor()->GetSize(w, h);
   VTKViewer_RenderWindow* wnd = dynamic_cast< VTKViewer_RenderWindow*>(GetInteractor()->GetRenderWindow());
  switch (State) {
  case VTK_INTERACTOR_STYLE_CAMERA_PAN: 
    {
      // processing panning
      this->FindPokedCamera(mousePos.x(), mousePos.y());
      this->PanXY(mousePos.x(), myPoint.y(), myPoint.x(), mousePos.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_ZOOM: 
    {    
      // processing zooming
      this->FindPokedCamera(mousePos.x(), mousePos.y());
      this->DollyXY(mousePos.x() - myPoint.x(), mousePos.y() - myPoint.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_ROTATE: 
    {
      // processing rotation
      this->FindPokedCamera(mousePos.x(), mousePos.y());
      this->RotateXY(mousePos.x() - myPoint.x(), myPoint.y() - mousePos.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_SPIN: 
    {
      // processing spinning
      this->FindPokedCamera(mousePos.x(), mousePos.y());
      this->SpinXY(mousePos.x(), mousePos.y(), myPoint.x(), myPoint.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN: 
    {    
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
    {
      if (!myCursorState)
        setCursor(VTK_INTERACTOR_STYLE_CAMERA_SELECT);
    }
  case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    {
      QPainter p(wnd);
      p.setPen(Qt::lightGray);
      p.setRasterOp(Qt::XorROP);
      p.drawRect(QRect(myPoint, myOtherPoint));
      myOtherPoint = mousePos;
      p.drawRect(QRect(myPoint, myOtherPoint));
      break;
    }
  }
  this->LastPos[0] = mousePos.x();
  this->LastPos[1] = h - mousePos.y() - 1;
}

// called when user moves mouse inside viewer window and there is no active viewer operation 
// (!put necessary processing here!)
void VTKViewer_InteractorStyleSALOME::onCursorMove(QPoint mousePos) {
  // processing highlighting
  QAD_Study* myActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
      
  vtkRenderWindowInteractor *rwi = this->Interactor;
  int w, h, x, y;
  rwi->GetSize(w, h);
  x = mousePos.x(); y = h - mousePos.y() - 1;

  this->FindPokedRenderer(x,y);
  rwi->StartPickCallback();
  rwi->GetPicker()->Pick(x, y, 0.0, this->CurrentRenderer);

  if ( rwi->GetPicker()->IsA("vtkPicker") ) {
    vtkPicker* picker = vtkPicker::SafeDownCast(rwi->GetPicker());
    vtkActor* ac = picker->GetActor();
    
    if ( ac != NULL ) {
      if ( ac->IsA("SALOME_Actor") ) {
        SALOME_Actor* SActor = SALOME_Actor::SafeDownCast( ac );
        if ( preview != SActor ) {
          if ( preview != NULL ) {
            preview->SetPreSelected( false );
          }
	  preview = SActor;
	      
	  if ( SActor->hasIO() ) {
            Handle( SALOME_InteractiveObject) IO = SActor->getIO();

            SALOME_ListIteratorOfListIO It(Sel->StoredIObjects());
            Standard_Boolean IsSelected = false;
            for(;It.More();It.Next()) {
              Handle(SALOME_InteractiveObject) IOS = It.Value();
              if(IO->isSame(IOS)) {
                IsSelected = true;
                break;
              }
            }

            if ( !IsSelected ) {
            // Find All actors with same IO
              vtkActorCollection* theActors = this->CurrentRenderer->GetActors();
              theActors->InitTraversal();
              vtkActor *ac = theActors->GetNextActor();
              while( ac ) {
                if ( ac->IsA("SALOME_Actor") ) {
                  SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
                  if ( anActor->hasIO() ) {
                    Handle(SALOME_InteractiveObject) IOS = anActor->getIO();
                    if(IO->isSame(IOS)) {
                      anActor->SetPreSelected( true );
                    }
                  }
                }
                ac = theActors->GetNextActor();
              }
              // MESSAGE ( " NAME PREVIEW " << SActor->getName() );
            }
          }
        }
      }
    } else {
      preview = NULL;
      vtkActorCollection* theActors = this->CurrentRenderer->GetActors();
      theActors->InitTraversal();
      vtkActor *ac = theActors->GetNextActor();
      while( ac ) {
        if ( ac->IsA("SALOME_Actor") ) {
          SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
          anActor->SetPreSelected( false );
        }
        ac = theActors->GetNextActor();
      }
    }
  }
  this->LastPos[0] = x;
  this->LastPos[1] = y;
}

// called on finsh GlobalPan operation 
void VTKViewer_InteractorStyleSALOME::Place(const int theX, const int theY) 
{
  if (this->CurrentRenderer == NULL) {
    return;
  }

  //translate view
  int *aSize = this->CurrentRenderer->GetRenderWindow()->GetSize();
  int centerX = aSize[0]/2;
  int centerY = aSize[1]/2;

  TranslateView(centerX, centerY, theX, theY);

  // restore zoom scale
  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();
  cam->SetParallelScale(myScale);
  this->CurrentRenderer->ResetCameraClippingRange();

  if (this->CurrentLight) {
    this->CurrentLight->SetPosition(cam->GetPosition());
    this->CurrentLight->SetFocalPoint(cam->GetFocalPoint());
  }
  VTKViewer_RenderWindow* aRW = dynamic_cast<VTKViewer_RenderWindow*>(this->Interactor->GetRenderWindow());
  if (aRW) aRW->updateGL();
}



// Translates view from Point to Point
void VTKViewer_InteractorStyleSALOME::TranslateView(int toX, int toY, int fromX, int fromY)
{
  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();
  double viewFocus[4], focalDepth, viewPoint[3];
  float newPickPoint[4], oldPickPoint[4], motionVector[3];
  cam->GetFocalPoint(viewFocus);

  this->ComputeWorldToDisplay(viewFocus[0], viewFocus[1],
			      viewFocus[2], viewFocus);
  focalDepth = viewFocus[2];

  this->ComputeDisplayToWorld(double(toX), double(toY),
			      focalDepth, newPickPoint);
  this->ComputeDisplayToWorld(double(fromX),double(fromY),
			      focalDepth, oldPickPoint);
  
  // camera motion is reversed
  motionVector[0] = oldPickPoint[0] - newPickPoint[0];
  motionVector[1] = oldPickPoint[1] - newPickPoint[1];
  motionVector[2] = oldPickPoint[2] - newPickPoint[2];
  
  cam->GetFocalPoint(viewFocus);
  cam->GetPosition(viewPoint);
  cam->SetFocalPoint(motionVector[0] + viewFocus[0],
		     motionVector[1] + viewFocus[1],
		     motionVector[2] + viewFocus[2]);
  cam->SetPosition(motionVector[0] + viewPoint[0],
		   motionVector[1] + viewPoint[1],
		   motionVector[2] + viewPoint[2]);
}


/// Checks: is the given Actor within display coordinates?
bool VTKViewer_InteractorStyleSALOME::IsInRect(vtkActor* theActor, 
					       const int left, const int top, 
					       const int right, const int bottom)
{
  float* aBounds = theActor->GetBounds();
  float aMin[3], aMax[3];
  ComputeWorldToDisplay(aBounds[0], aBounds[2], aBounds[4], aMin);
  ComputeWorldToDisplay(aBounds[1], aBounds[3], aBounds[5], aMax);
  if (aMin[0] > aMax[0]) {
    float aBuf = aMin[0];
    aMin[0] = aMax[0];
    aMax[0] = aBuf;
  }
  if (aMin[1] > aMax[1]) {
    float aBuf = aMin[1];
    aMin[1] = aMax[1];
    aMax[1] = aBuf;    
  }

  return ((aMin[0]>left) && (aMax[0]<right) && (aMin[1]>bottom) && (aMax[1]<top));
}


/// Checks: is the given Cell within display coordinates?
bool VTKViewer_InteractorStyleSALOME::IsInRect(vtkCell* theCell, 
					       const int left, const int top, 
					       const int right, const int bottom)
{
  float* aBounds = theCell->GetBounds();
  float aMin[3], aMax[3];
  ComputeWorldToDisplay(aBounds[0], aBounds[2], aBounds[4], aMin);
  ComputeWorldToDisplay(aBounds[1], aBounds[3], aBounds[5], aMax);
  if (aMin[0] > aMax[0]) {
    float aBuf = aMin[0];
    aMin[0] = aMax[0];
    aMax[0] = aBuf;
  }
  if (aMin[1] > aMax[1]) {
    float aBuf = aMin[1];
    aMin[1] = aMax[1];
    aMax[1] = aBuf;    
  }

  return ((aMin[0]>left) && (aMax[0]<right) && (aMin[1]>bottom) && (aMax[1]<top));
}


bool VTKViewer_InteractorStyleSALOME::IsInRect(float* thePoint, 
					       const int left, const int top, 
					       const int right, const int bottom)
{
  float aPnt[3];
  ComputeWorldToDisplay(thePoint[0], thePoint[1], thePoint[2], aPnt);

  return ((aPnt[0]>left) && (aPnt[0]<right) && (aPnt[1]>bottom) && (aPnt[1]<top));
}
