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
//  File   : VTKViewer_InteractorStyleSALOME.cxx
//  Author : Christophe ATTANASIO
//  Module : SALOME
//  $Header$

#include "VTKViewer_InteractorStyleSALOME.h"
#include "VTKViewer_RenderWindowInteractor.h"
#include "VTKViewer_RenderWindow.h"
#include "VTKViewer_Utilities.h"
#include "VTKViewer_Trihedron.h"
#include "VTKViewer_RectPicker.h"
#include "VTKViewer_CellRectPicker.h"

#include "QAD_Config.h"
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
#include <vtkImageData.h>

#include <qapplication.h>
//VRV: porting on Qt 3.0.5
#if QT_VERSION >= 0x030005
#include <qpainter.h>
#endif
//VRV: porting on Qt 3.0.5
#include <algorithm>

#include "utilities.h"

using namespace std;

extern int SetVisibility(vtkActorCollection* theCollection, int theParam);
extern int GetVisibility(vtkActorCollection* theCollection);

//----------------------------------------------------------------------------
vtkStandardNewMacro(VTKViewer_InteractorStyleSALOME);
//----------------------------------------------------------------------------

VTKViewer_InteractorStyleSALOME::VTKViewer_InteractorStyleSALOME() 
{
  m_Trihedron = 0;
  this->MotionFactor = 10.0;
  this->State = VTK_INTERACTOR_STYLE_CAMERA_NONE;
  this->RadianToDegree = 180.0 / vtkMath::Pi();
  this->ForcedState = VTK_INTERACTOR_STYLE_CAMERA_NONE;
  loadCursors();

  myPActor = vtkActor::New();
  myPActor->PickableOff();
  myPActor->GetProperty()->SetColor(0,1,1);
  myPActor->GetProperty()->SetLineWidth(5);
  myPActor->GetProperty()->SetPointSize(5);
}

//----------------------------------------------------------------------------
VTKViewer_InteractorStyleSALOME::~VTKViewer_InteractorStyleSALOME() 
{
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::setPreselectionProp(const double& theRed, const double& theGreen, 
							  const double& theBlue, const int& theWidth) 
{
  myPActor->GetProperty()->SetColor(theRed, theGreen, theBlue);
  myPActor->GetProperty()->SetLineWidth(theWidth);
  myPActor->GetProperty()->SetPointSize(theWidth);
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::setTriedron(VTKViewer_Trihedron* theTrihedron){
  m_Trihedron = theTrihedron;
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
  ::ResetCameraClippingRange(this->CurrentRenderer); 
  this->Interactor->Render();
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::PanXY(int x, int y, int oldX, int oldY)
{
  TranslateView(x, y, oldX, oldY);   
  //vtkRenderWindowInteractor *rwi = this->Interactor;
  /* VSV Light follows camera: if (this->CurrentLight)
    {
      vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();
      this->CurrentLight->SetPosition(cam->GetPosition());
      this->CurrentLight->SetFocalPoint(cam->GetFocalPoint());
      }*/
    
  this->Interactor->Render();
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::ControlLblSize(double aOldScale, double aNewScale) {
  return;
}

//----------------------------------------------------------------------------
void VTKViewer_InteractorStyleSALOME::DollyXY(int dx, int dy)
{
  if (this->CurrentRenderer == NULL) return;

  double dxf = this->MotionFactor * (double)(dx) / (double)(this->CurrentRenderer->GetCenter()[1]);
  double dyf = this->MotionFactor * (double)(dy) / (double)(this->CurrentRenderer->GetCenter()[1]);

  double zoomFactor = pow((double)1.1, dxf + dyf);
  
  vtkCamera *aCam = this->CurrentRenderer->GetActiveCamera();
  if (aCam->GetParallelProjection())
    aCam->SetParallelScale(aCam->GetParallelScale()/zoomFactor);
  else{
    aCam->Dolly(zoomFactor);
    ::ResetCameraClippingRange(this->CurrentRenderer);
  }
  
  /* VSV Light follows camera: if (this->CurrentLight)
    {      
      this->CurrentLight->SetPosition(cam->GetPosition());
      this->CurrentLight->SetFocalPoint(cam->GetFocalPoint());
      }*/
  
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

  double newAngle = atan2((double)(y - this->CurrentRenderer->GetCenter()[1]),
			  (double)(x - this->CurrentRenderer->GetCenter()[0]));
  double oldAngle = atan2((double)(oldY -this->CurrentRenderer->GetCenter()[1]),
			  (double)(oldX - this->CurrentRenderer->GetCenter()[0]));
  
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
  if (this->HasObserver(vtkCommand::LeftButtonPressEvent)) {
    this->InvokeEvent(vtkCommand::LeftButtonPressEvent,NULL);
    return;
  }
  this->FindPokedRenderer(x, y);
  if (this->CurrentRenderer == NULL) {
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
  } else {
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
  if (!myGUIWindow) return false;
  if ( (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::KeyPress) && object != myGUIWindow)
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


void  VTKViewer_InteractorStyleSALOME::ViewFitAll() {
  int aTriedronWasVisible = false;
  if(m_Trihedron){
    aTriedronWasVisible = m_Trihedron->GetVisibility() == VTKViewer_Trihedron::eOn;
    if(aTriedronWasVisible) m_Trihedron->VisibilityOff();
  }

  if(m_Trihedron->GetVisibleActorCount(CurrentRenderer)){
    m_Trihedron->VisibilityOff();
    ::ResetCamera(CurrentRenderer);
  }else{
    m_Trihedron->SetVisibility(VTKViewer_Trihedron::eOnlyLineOn);
    ::ResetCamera(CurrentRenderer,true);
  }
  if(aTriedronWasVisible) m_Trihedron->VisibilityOn();
  else m_Trihedron->VisibilityOff();
  ::ResetCameraClippingRange(CurrentRenderer);
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

  ViewFitAll();
  // make fit all
//   int TriedronWasVisible = false;
//   if(m_Trihedron){
//     TriedronWasVisible = 
//       m_Trihedron->GetVisibility() == VTKViewer_Trihedron::eOn;
//     if(TriedronWasVisible) m_Trihedron->VisibilityOff();
//   }

//   ::ResetCamera(this->CurrentRenderer);
  
//   if(m_Trihedron)
//     if(TriedronWasVisible) 
//       m_Trihedron->VisibilityOn();

  //VTKViewer_RenderWindow* aRW = dynamic_cast<VTKViewer_RenderWindow*>(this->Interactor->GetRenderWindow());
  if (myGUIWindow) myGUIWindow->update();
  
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
  if (this->CurrentRenderer == NULL) return;
 
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

  vtkCamera *aCam = this->CurrentRenderer->GetActiveCamera();
  if(aCam->GetParallelProjection())
    aCam->SetParallelScale(aCam->GetParallelScale()/zoomFactor);
  else{
    aCam->Dolly(zoomFactor);
    ::ResetCameraClippingRange(this->CurrentRenderer);
  }
  
  //vtkRenderWindowInteractor *rwi = this->Interactor;
  /* VSV Light follows camera: if (this->CurrentLight) {
    this->CurrentLight->SetPosition(cam->GetPosition());
    this->CurrentLight->SetFocalPoint(cam->GetFocalPoint());
    }*/
  //  rwi->Render();
  //VTKViewer_RenderWindow* aRW = dynamic_cast<VTKViewer_RenderWindow*>(rwi->GetRenderWindow());
  myGUIWindow->update();
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
  if (!myGUIWindow) return;
  switch (operation)
  {
    case VTK_INTERACTOR_STYLE_CAMERA_ZOOM:
      myGUIWindow->setCursor(myZoomCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_PAN:
      myGUIWindow->setCursor(myPanCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_ROTATE:
      myGUIWindow->setCursor(myRotateCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_SPIN:
      myGUIWindow->setCursor(mySpinCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_GLOBAL_PAN:
      myGUIWindow->setCursor(myGlobalPanCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
      myGUIWindow->setCursor(myHandCursor); 
      myCursorState = true;
      break;
    case VTK_INTERACTOR_STYLE_CAMERA_NONE:
    default:
      myGUIWindow->setCursor(myDefCursor); 
      myCursorState = false;
      break;
  }
}

// called when viewer operation started (!put necessary initialization here!)
void VTKViewer_InteractorStyleSALOME::onStartOperation()
{
  if (!myGUIWindow) return;
  // VSV: LOD actor activisation
  //  this->Interactor->GetRenderWindow()->SetDesiredUpdateRate(this->Interactor->GetDesiredUpdateRate());
  switch (State) {
    case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
    case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    {
      QPainter p(myGUIWindow);
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
  if (!myGUIWindow) return;


  QAD_Study* aActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* aSel    = SALOME_Selection::Selection( aActiveStudy->getSelection() );
  vtkRenderWindowInteractor *rwi = this->Interactor;

  // VSV: LOD actor activisation
  //  rwi->GetRenderWindow()->SetDesiredUpdateRate(rwi->GetStillUpdateRate());

  Selection_Mode aSelectionMode = aSel->SelectionMode();
  bool aSelActiveCompOnly = aSel->IsSelectActiveCompOnly();

  switch (State) {
    case VTK_INTERACTOR_STYLE_CAMERA_SELECT:
    case VTK_INTERACTOR_STYLE_CAMERA_FIT:
    {
      QPainter p(myGUIWindow);
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

// 	  vtkPicker* aPicker = vtkPicker::SafeDownCast(rwi->GetPicker());
// 	  if (aPicker)
// 	    aPicker->SetTolerance(0.01);
	  
          rwi->GetPicker()->Pick(x, y, 0.0, this->CurrentRenderer);
    
          if ( rwi->GetPicker()->IsA("vtkCellPicker") ) {
            vtkCellPicker* picker;
            if ( (picker = vtkCellPicker::SafeDownCast(rwi->GetPicker())) ) {
	      int aVtkId = picker->GetCellId();
	      // INFOS ( " CellId : " << aVtkId );
              if ( aVtkId >= 0 ) {
                vtkActor* ac = picker->GetActor();
                if ( ac->IsA("SALOME_Actor") ) {
                  SALOME_Actor* SActor = SALOME_Actor::SafeDownCast( ac );
                  INFOS ( " NAME Actor : " << SActor->getName() << ", CellId : " << aVtkId );

                  //Cell selection /////////////////////////////// NB
                  if ( aSelectionMode == CellSelection ) {
                    if ( SActor->hasIO() && isValid( SActor, aVtkId, aSelectionMode ) ) {
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

		      int anObjId = SActor->GetObjId(aVtkId);
		      std::vector<int> aVtkList;
		      if (anObjId != -1) {
			aVtkList = SActor->GetVtkId(anObjId);
		      }

		      if (IsSelected) {
			// This IO is already in the selection
			if (aVtkList.size() > 0)
			  bool add = aSel->AddOrRemoveIndex( IO, aVtkList, myShiftState, false );
			else
			  bool add = aSel->AddOrRemoveIndex( IO, aVtkId, myShiftState, false );
		      } else {
			if (!myShiftState) {
			  this->HighlightProp( NULL );
			  aSel->ClearIObjects();
			}
			if (aVtkList.size() > 0)
			  bool add = aSel->AddOrRemoveIndex( IO, aVtkList, myShiftState, false );
			else
			  bool add = aSel->AddOrRemoveIndex( IO, aVtkId, myShiftState, false );
			aSel->AddIObject( IO, false );
		      }
                    }
	          }
		  //Edge, Face, Volume and EdgeOfCell selection ////////////////////////// NB
                  else if ( aSelectionMode == EdgeSelection || 
			    aSelectionMode == FaceSelection || 
			    aSelectionMode == VolumeSelection  ||
			    aSelectionMode == EdgeOfCellSelection ) {
		    if ( SActor->hasIO() && isValid( SActor, aVtkId, aSelectionMode ) ) {
		      Handle(SALOME_InteractiveObject) IO = SActor->getIO();
		      if (vtkDataSet* UGrid = SActor->GetMapper()->GetInput()) {
			if (vtkCell* pickedCell = UGrid->GetCell(aVtkId)) {
			  bool result = false;
			  int edgeId = -1;
			  int anObjId = SActor->GetObjId(aVtkId);
			  std::vector<int> aVtkList;
			  if (anObjId > -1)
			    aVtkList = SActor->GetVtkId(anObjId);

			  if (aSelectionMode == EdgeSelection) {
			    if ( SActor->GetObjDimension( anObjId ) == 1 )
			      result = true;
			  }
			  else if ( aSelectionMode == FaceSelection ) {
			    if ( SActor->GetObjDimension( anObjId ) == 2 )
			      result = true;
			  }
			  else if (aSelectionMode == VolumeSelection) {
			    if ( SActor->GetObjDimension( anObjId ) == 3 )
			      result = true;
			  }
			  else {
			    float pickPosition[3];
			    picker->GetPickPosition(pickPosition);
			    float distMin = 1000000.0, dist = 0;
			    for (int i = 0, iEnd = pickedCell->GetNumberOfEdges(); i < iEnd; i++){
			      vtkCell* edge = pickedCell->GetEdge(i);
			      if (vtkLine* line = vtkLine::SafeDownCast(edge)){
				float pcoords[3],closestPoint[3],weights[3];
				int subId;
				line->EvaluatePosition(pickPosition,closestPoint,subId,pcoords,dist,weights);
				if (dist < distMin) {
				  distMin = dist;
				  edgeId = i;
				}
			      }
			    }
			    if (edgeId != -1 || pickedCell->GetCellDimension() == 1) 
			      result = true;
			    INFOS("edgeID transformed = "<<edgeId);
			  }
			  if (result) {
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
			    if (!myShiftState) {
			      this->HighlightProp( NULL );
			      aSel->ClearIObjects();
			    }
			    // aSel->SetSelectionMode(EdgeSelection, true);
			    if (aVtkList.size() > 0)
			      aSel->AddOrRemoveIndex( IO, aVtkList, true, false);
			    else
			      aSel->AddOrRemoveIndex( IO, aVtkId, true, false);
			    if (edgeId >= 0)
			      aSel->AddOrRemoveIndex( IO, -edgeId-1, true, true );
			    aSel->AddIObject( IO, false );
			  }
			}
		      }
		    }
		  }
                }
	      } else {
		this->HighlightProp( NULL );
		aSel->ClearIObjects();
              }
            }
          } else if ( rwi->GetPicker()->IsA("vtkPointPicker") ) {
            vtkPointPicker* picker;
            if ( (picker = vtkPointPicker::SafeDownCast(rwi->GetPicker())) ) {
              INFOS ( " PointId : " << picker->GetPointId() );
              if ( picker->GetPointId() >= 0 ) {
                vtkActor* ac = picker->GetActor();
                if ( ac->IsA("SALOME_Actor") ) {
                  SALOME_Actor* SActor = SALOME_Actor::SafeDownCast( ac );
                  INFOS ( " NAME Actor : " << SActor->getName() );
                  if ( SActor->hasIO() ) {
                    Handle(SALOME_InteractiveObject) IO = SActor->getIO();
		    /*
		    if (IO.IsNull()) 
		      break;
		    if (aSelActiveCompOnly && 
			strcmp(aActiveComponent->ComponentDataType(), 
			       IO->getComponentDataType()) != 0) {
		      break;
		    }
		    */
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
          }
	  rwi->EndPickCallback();
        } else {
          //processing rectangle selection
	  SALOMEDS::SComponent_var aActiveComponent = 
	    SALOMEDS::SComponent::_narrow(aActiveStudy->getStudyDocument()->
					  FindObject(QAD_Application::getDesktop()->getActiveComponent()));
	  if(aSelActiveCompOnly && aActiveComponent->_is_nil()) return;
	  rwi->StartPickCallback();

	  if (!myShiftState) {
	    this->PropPicked = 0;
	    this->HighlightProp( NULL );
	    aSel->ClearIObjects();
	  }

	  // Compute bounds
	  //	  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();
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
	  case NodeSelection: // Nodes selection
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
			  strcmp(aActiveComponent->ComponentDataType(), 
				 IO->getComponentDataType()) != 0) {
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

			std::vector<int> anIndices;
			for (int i=0; i < aDataSet->GetNumberOfPoints(); i++) {
			  float* aPoint;
			  aPoint = aDataSet->GetPoint(i);
			  if (IsInRect(aPoint,  x1, y1, x2, y2)) {
			    float aDisp[3];
			    ComputeWorldToDisplay(aPoint[0],
						  aPoint[1],
						  aPoint[2], aDisp);
			    aPointPicker->Pick(aDisp[0], aDisp[1], 0.0, CurrentRenderer);

			    int pid = aPointPicker->GetPointId();
			    if ( pid >= 0) { // && (!aSel->IsIndexSelected(IO, aPointPicker->GetPointId()))) {
			      std::vector<int>::iterator found = 
				find(anIndices.begin(), anIndices.end(), pid);
			      if (found == anIndices.end())
				anIndices.push_back(pid);
        		    }
			  }
			}
			if (!anIndices.empty()) {
			  aSel->AddOrRemoveIndex(IO, anIndices, true, false);
			  aSel->AddIObject(IO, false);
			  anIndices.clear();
			}
		      }
		    }
		  }
		}
	      }
	    }
	    break;
	  case CellSelection:
	  case EdgeOfCellSelection:
	  case EdgeSelection:
	  case FaceSelection:
	  case VolumeSelection: 
	    {
	      VTKViewer_CellRectPicker* picker = VTKViewer_CellRectPicker::New();
	      picker->SetTolerance(0.001);
	      picker->Pick(x1, y1, 0.0, x2, y2, 0.0, this->CurrentRenderer);

	      vtkActorCollection* aListActors = picker->GetActors();
	      vtkActor* aActor;
	      aListActors->InitTraversal();
	      while(aActor = aListActors->GetNextActor()) {
		if (aActor->IsA("SALOME_Actor")) {
		  SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast(aActor);
		  if (aSActor->hasIO()) {
		    Handle(SALOME_InteractiveObject) aIO = aSActor->getIO();
		    // INFOS ( " NAME Object : " << aIO->getName() << " from " << aIO->getComponentDataType() );
		    if (aSelActiveCompOnly &&
			strcmp(aActiveComponent->ComponentDataType(), 
			       aIO->getComponentDataType()) != 0) {
		      continue;
		    }
		    VTKViewer_CellDataSet cellList = picker->GetCellData(aActor);
		    if ( !cellList.empty() ) {
		      INFOS ( " NAME Actor : " << aSActor->getName() );
		      std::vector<int> anIndexes;
		      if (aSelectionMode == CellSelection) {
			VTKViewer_CellDataSet::iterator it;
			for (it = cellList.begin(); it != cellList.end(); ++it) {
			  int aCellId = (*it).cellId;
			  if ( !isValid( aSActor, aCellId, aSelectionMode ) )
			    continue;
				      
			  int aObjId = aSActor->GetObjId(aCellId);
			  std::vector<int> aVtkList;
			  if ( aObjId != -1 )
			    aVtkList = aSActor->GetVtkId(aObjId);
				      
			  if (aVtkList.size() > 0) {
			    std::vector<int>::iterator it;
			    for (it = aVtkList.begin(); it != aVtkList.end(); ++it) {
			      std::vector<int>::iterator found = 
				find(anIndexes.begin(), anIndexes.end(), *it);
			      if (found == anIndexes.end())
				anIndexes.push_back(*it);
			    }
			  }
			  else
			    anIndexes.push_back(aCellId);
			}
		      }
		      else {
			if ( vtkDataSet* aDataSet = aSActor->GetMapper()->GetInput() ) {
			  VTKViewer_CellDataSet::iterator it;
			  for ( it = cellList.begin(); it != cellList.end(); ++it ) {
			    int aCellId = (*it).cellId;
					  
			    if ( !isValid( aSActor, aCellId, aSelectionMode ) )
			      continue;
					  
			    int aObjId = aSActor->GetObjId(aCellId);
			    std::vector<int> aVtkList;
			    if (aObjId != -1)
			      aVtkList = aSActor->GetVtkId(aObjId);
			    bool result = false;
			    if (vtkCell* aCell = aDataSet->GetCell(aCellId)) {
			      if ( aSelectionMode == EdgeSelection ) {
				if ( aSActor->GetObjDimension( aObjId ) == 1 )
				  result = true;
			      }
			      else if ( aSelectionMode == FaceSelection ) {
				if ( aSActor->GetObjDimension( aObjId ) == 2 )
				  result = true;
			      }
			      else if ( aSelectionMode == VolumeSelection ) {
				if ( aSActor->GetObjDimension( aObjId ) == 3 )
				  result = true;
			      }
			    }
			    if ( result ) {
			      if (aVtkList.size() > 0) {
				std::vector<int>::iterator it;
				for (it = aVtkList.begin(); it != aVtkList.end(); ++it) {
				  std::vector<int>::iterator found =
				    find(anIndexes.begin(), anIndexes.end(), *it);
				  if (found == anIndexes.end())
				    anIndexes.push_back(*it);
				}
			      }
			      else
				anIndexes.push_back(aCellId);
			    }
			  }
			}
		      }
		      aSel->AddOrRemoveIndex(aIO, anIndexes, true, false);
		      aSel->AddIObject(aIO, false);
		    }
		  }
		}
	      }
	    }
	    break;	    
	  case ActorSelection: // objects selection
	    {
	      VTKViewer_RectPicker* picker = VTKViewer_RectPicker::New();
	      picker->SetTolerance(0.001);
	      picker->Pick(x1, y1, 0.0, x2, y2, 0.0, this->CurrentRenderer);

	      vtkActorCollection* aListActors = picker->GetActors();
	      vtkActor* aActor;
	      SALOME_ListIO aListIO;
	      aListActors->InitTraversal();
	      while(aActor = aListActors->GetNextActor()) {
		if (aActor->IsA("SALOME_Actor")) {
		  SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast(aActor);
		  if (aSActor->hasIO()) {
		    Handle(SALOME_InteractiveObject) aIO = aSActor->getIO();
		    bool isStored = false;
		    if (!aListIO.IsEmpty()) {
		      SALOME_ListIteratorOfListIO It(aListIO);
		      for(;It.More();It.Next()) {
			Handle(SALOME_InteractiveObject) IOS = It.Value();
			if( aIO->isSame(IOS) ) {
			  aIO = IOS; //Added by SRN, fix SAL1307
			  isStored = true;
			  break;
			}
		      }
		    }
		    if (!isStored)
		      aListIO.Append(aIO);
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
  if (myGUIWindow) myGUIWindow->update();

}

// called during viewer operation when user moves mouse (!put necessary processing here!)
void VTKViewer_InteractorStyleSALOME::onOperation(QPoint mousePos) 
{
  if (!myGUIWindow) return;
  int w, h;
  GetInteractor()->GetSize(w, h);
  switch (State) {
  case VTK_INTERACTOR_STYLE_CAMERA_PAN: 
    {
      // processing panning
      //this->FindPokedCamera(mousePos.x(), mousePos.y());
      this->PanXY(mousePos.x(), myPoint.y(), myPoint.x(), mousePos.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_ZOOM: 
    {    
      // processing zooming
      //this->FindPokedCamera(mousePos.x(), mousePos.y());
      this->DollyXY(mousePos.x() - myPoint.x(), mousePos.y() - myPoint.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_ROTATE: 
    {
      // processing rotation
      //this->FindPokedCamera(mousePos.x(), mousePos.y());
      this->RotateXY(mousePos.x() - myPoint.x(), myPoint.y() - mousePos.y());
      myPoint = mousePos;
      break;
    }
  case VTK_INTERACTOR_STYLE_CAMERA_SPIN: 
    {
      // processing spinning
      //this->FindPokedCamera(mousePos.x(), mousePos.y());
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
      QPainter p(myGUIWindow);
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

// called when selection mode changed (!put necessary initialization here!)
void VTKViewer_InteractorStyleSALOME::OnSelectionModeChanged()
{
  myPActor->VisibilityOff();
}

// called when user moves mouse inside viewer window and there is no active viewer operation 
// (!put necessary processing here!)
void VTKViewer_InteractorStyleSALOME::onCursorMove(QPoint mousePos) {
  // processing highlighting
  QAD_Study* anActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel = SALOME_Selection::Selection( anActiveStudy->getSelection() );

  Selection_Mode aSelectionMode = Sel->SelectionMode();
  myPActor->VisibilityOff();
      
  vtkRenderWindowInteractor *rwi = this->Interactor;
  int w, h, x, y;
  rwi->GetSize(w, h);
  x = mousePos.x(); y = h - mousePos.y() - 1;

  this->FindPokedRenderer(x,y);
  rwi->StartPickCallback();
  rwi->GetPicker()->Pick(x, y, 0.0, this->CurrentRenderer);
  
  if (rwi->GetPicker()->IsA("vtkCellPicker")) {
    vtkCellPicker* picker = vtkCellPicker::SafeDownCast(rwi->GetPicker());
    int aVtkId = picker->GetCellId();
    // INFOS ( " Preselected CellId : " << picker->GetCellId() );
    if ( aVtkId >= 0 ) {
      vtkActor* ac = picker->GetActor();
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* SActor = SALOME_Actor::SafeDownCast( ac );
	// INFOS ( " NAME Actor : " << SActor->getName() );

	if (rwi->IsA("VTKViewer_RenderWindowInteractor")) {
	  VTKViewer_RenderWindowInteractor* aRWI = 
	    VTKViewer_RenderWindowInteractor::SafeDownCast(rwi);

	  //Cell selection //////////////////////////////////// NB
	  if ( aSelectionMode == CellSelection )
    {
      if ( SActor->hasIO() && isValid( SActor, aVtkId, CellSelection ) )
      {
	      int anObjId = SActor->GetObjId(aVtkId);
	      std::vector<int> aVtkList;
	      if (anObjId != -1)
          aVtkList = SActor->GetVtkId(anObjId);

	      if (aVtkList.size() > 0)
		      aRWI->setCellData(SActor->getIO(), aVtkList, myPActor);
	      else
          aRWI->setCellData(SActor->getIO(), aVtkId, myPActor);

	      this->CurrentRenderer->AddActor(myPActor);
	      myPActor->GetProperty()->SetRepresentationToSurface();
	      myPActor->VisibilityOn();
	    }
	  }
	
	  //Edge, Face, Volume and EdgeOfCell selection ////////////////////////// NB
	  else if ( aSelectionMode == EdgeSelection || 
		    aSelectionMode == FaceSelection || 
		    aSelectionMode == VolumeSelection  ||
		    aSelectionMode == EdgeOfCellSelection ) {
	    if ( SActor->hasIO() && isValid( SActor, aVtkId, aSelectionMode ) )
      {
	      if (vtkDataSet* UGrid = SActor->GetMapper()->GetInput()) {
		if (vtkCell* pickedCell = UGrid->GetCell(aVtkId)) {
		  bool result = false;
		  int edgeId = -1;
		  int anObjId = SActor->GetObjId(aVtkId);
		  std::vector<int> aVtkList;
		  if (anObjId > -1)
		    aVtkList = SActor->GetVtkId(anObjId);

		  if ( aSelectionMode == EdgeSelection ) {
		    if ( SActor->GetObjDimension( anObjId ) == 1 )
		      result = true;
		  }
		  else if ( aSelectionMode == FaceSelection ) {
		    if ( SActor->GetObjDimension( anObjId ) == 2 )
		      result = true;
		  }
		  else if ( aSelectionMode == VolumeSelection ) {
		    if ( SActor->GetObjDimension( anObjId ) == 3 )
		      result = true;
		  }
		  else {
		    float pickPosition[3];
		    picker->GetPickPosition(pickPosition);
		    float distMin = 1000000.0, dist = 0;
		    for (int i = 0, iEnd = pickedCell->GetNumberOfEdges(); i < iEnd; i++){
		      vtkCell* edge = pickedCell->GetEdge(i);
		      if(vtkLine* line = vtkLine::SafeDownCast(edge)){
			float pcoords[3],closestPoint[3],weights[3];
			int subId;
			line->EvaluatePosition(pickPosition,closestPoint,subId,pcoords,dist,weights);
			if (dist < distMin) {
			  distMin = dist;
			  edgeId = i;
			}
		      }
		    }
		    if (edgeId != -1 || pickedCell->GetCellDimension() == 1) 
		      result = true;
		    // INFOS("edgeID transformed = "<<edgeId);
		  }
		  if (result)
      {
		    this->CurrentRenderer->RemoveActor(myPActor);
		    if (edgeId == -1) {
		      if (aVtkList.size() > 0)
      			aRWI->setCellData(SActor->getIO(), aVtkList, myPActor);
		      else
			      aRWI->setCellData(SActor->getIO(), aVtkId, myPActor);
		    }
		    else
		      aRWI->setEdgeData(SActor->getIO(), aVtkId, -edgeId-1, myPActor);
          
		    this->CurrentRenderer->AddActor(myPActor);
		    myPActor->GetProperty()->SetRepresentationToSurface();
		    myPActor->VisibilityOn();
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  else if (rwi->GetPicker()->IsA("vtkPointPicker")) {
    vtkPointPicker* picker = vtkPointPicker::SafeDownCast(rwi->GetPicker());
    // INFOS ( " Preselected PointId : " << picker->GetPointId() );
    if (picker->GetPointId() >= 0) {
      vtkActor* ac = picker->GetActor();
      if ( ac->IsA("SALOME_Actor") ) {
	SALOME_Actor* SActor = SALOME_Actor::SafeDownCast( ac );
	// INFOS ( " NAME Actor : " << SActor->getName() );

	if (rwi->IsA("VTKViewer_RenderWindowInteractor")) {
	  VTKViewer_RenderWindowInteractor* aRWI = 
	    VTKViewer_RenderWindowInteractor::SafeDownCast(rwi);

	  if ( SActor->hasIO() ) {
	    myPActor->VisibilityOff();
	    this->CurrentRenderer->RemoveActor(myPActor);
	    aRWI->setPointData(SActor->getIO(), picker->GetPointId(), myPActor);
	    this->CurrentRenderer->AddActor(myPActor);
	    myPActor->GetProperty()->SetRepresentationToSurface();
	    myPActor->VisibilityOn();
	  }
	}
      }
    }
  }
  else if ( rwi->GetPicker()->IsA("vtkPicker") ) {
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
              // INFOS ( " NAME PREVIEW " << SActor->getName() );
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
  rwi->EndPickCallback();
  rwi->Render();

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
  ::ResetCameraClippingRange(this->CurrentRenderer);

  /* VSV Light follows camera: if (this->CurrentLight) {
    this->CurrentLight->SetPosition(cam->GetPosition());
    this->CurrentLight->SetFocalPoint(cam->GetFocalPoint());
    }*/
  //VTKViewer_RenderWindow* aRW = dynamic_cast<VTKViewer_RenderWindow*>(this->Interactor->GetRenderWindow());
  if (myGUIWindow) myGUIWindow->update();

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

void VTKViewer_InteractorStyleSALOME::SetFaceFilter( const Handle(VTKViewer_Filter)& f )
{
  myFaceFilter = f;
}

void VTKViewer_InteractorStyleSALOME::RemoveFaceFilter()
{
  myFaceFilter.Nullify();
}

void VTKViewer_InteractorStyleSALOME::SetEdgeFilter( const Handle(VTKViewer_Filter)& f )
{
  myEdgeFilter = f;
}

void VTKViewer_InteractorStyleSALOME::RemoveEdgeFilter()
{
  myEdgeFilter.Nullify();
}

bool VTKViewer_InteractorStyleSALOME::isValid( SALOME_Actor*        theActor,
                                               const int            theId,
                                               const Selection_Mode theSelMode )
{
  if ( theSelMode != CellSelection &&
       theSelMode != FaceSelection &&
       theSelMode != EdgeSelection )
    return true;

  if ( theSelMode == CellSelection )
  {
    vtkDataSet* aUGrid = theActor->GetMapper()->GetInput();
    if ( aUGrid != 0 )
    {
      vtkCell* aCell = aUGrid->GetCell( theId );
      if ( aCell != 0 )
      {
        int nbPoints = aCell->GetNumberOfPoints();
        if ( nbPoints == 2 )
          return myEdgeFilter.IsNull() || myEdgeFilter->IsValid( theActor, theId );
        else
          return myFaceFilter.IsNull() || myFaceFilter->IsValid( theActor, theId );

        return true;
      }
    }
    return false;
  }
  else if ( theSelMode == FaceSelection )
    return myFaceFilter.IsNull() || myFaceFilter->IsValid( theActor, theId );
  else if ( theSelMode == EdgeSelection )
    return myEdgeFilter.IsNull() || myEdgeFilter->IsValid( theActor, theId );
    
  return true;
}
