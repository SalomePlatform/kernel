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
#include "VTKViewer_RenderWindow.h"

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
#include "VTKViewer_InteractorStyleSALOME.h"
#include "VTKViewer_VectorText.h"

#include "utilities.h"

//QT Include
#include <qlayout.h>
#include <qcolordialog.h>
#include <qfiledialog.h>
#include <qapplication.h>

// VTK Includes
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h> 

#include <vtkMath.h>
#include <vtkLine.h>
#include <vtkConeSource.h>
#include <vtkFollower.h>

using namespace std;
/*!
    Constructor
*/
VTKViewer_ViewFrame::VTKViewer_ViewFrame(QWidget* parent, const char* name) 
  :  QAD_ViewFrame(parent, name)
{
  m_ViewUp[0] = 0; m_ViewUp[1] = 0; m_ViewUp[2] = -1;
  m_ViewNormal[0] = 0; m_ViewNormal[1] = 0; m_ViewNormal[2] = 1;
  m_Transform = SALOME_Transform::New();

  //  m_InitialSetupDone = false ;
  InitialSetup();
}


vtkFollower* CreateTextActor(char *text, float aSize) {
  VTKViewer_VectorText* aTxt = VTKViewer_VectorText::New();
  aTxt->SetText(text);
  vtkPolyDataMapper* textMapper = vtkPolyDataMapper::New();
  textMapper->SetInput(aTxt->GetOutput());
  vtkFollower* textActor = vtkFollower::New();
  textActor->SetMapper(textMapper);
  float aScale = 17 * aSize/100;
  textActor->SetScale(aScale, aScale, aScale);
  return textActor;
}

void VTKViewer_ViewFrame::AddVector(float* o,float* p,vtkRenderer* renderer, float aSize) {
  vtkPoints* myPoints = vtkPoints::New();
  vtkLine* myLine = vtkLine::New();

  myPoints->InsertNextPoint(o);
  myPoints->InsertNextPoint(p);

  (myLine->GetPointIds())->InsertNextId(0);
  (myLine->GetPointIds())->InsertNextId(1);

  vtkActor* lineActor = vtkActor::New();

  vtkCellArray* cell = vtkCellArray::New();

  cell->InsertNextCell(myLine);

  vtkPolyData* output = vtkPolyData::New();
  
  output->SetPoints(myPoints);
  output->SetLines(cell);
 
  vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();

  mapper->SetInput(output);

  lineActor->SetMapper(mapper);

  // Create CONE

  vtkConeSource* acone =  vtkConeSource::New();

  float dim = aSize;

  acone->SetResolution(2);
  //  acone->SetAngle(70);
  acone->SetRadius(0.02*dim);
  acone->SetHeight(0.08*dim);

  vtkActor* coneActor = vtkActor::New();
 
  vtkPolyDataMapper* coneMapper = vtkPolyDataMapper::New();
  coneMapper->SetInput(acone->GetOutput());

  coneActor->SetMapper(coneMapper);
  float rot[3];
  rot[0]=0; rot[1]=0; rot[2]=0;

  vtkFollower* aTextActor;

  coneActor->AddPosition(p);
  if(p[0]!=0) {
    // x
    aTextActor = CreateTextActor("X", dim);
  } else if(p[1]!=0) {
    // y
    rot[2]=90;
    coneActor->AddOrientation(rot);
    aTextActor = CreateTextActor("Y", dim);
  } else if(p[2]!=0) {
    // z
    rot[1]=-90;
    coneActor->AddOrientation(rot);
    aTextActor = CreateTextActor("Z", dim);
  }
  aTextActor->AddPosition(p);
  aTextActor->SetCamera(renderer->GetActiveCamera());

  coneActor->GetProperty()->SetInterpolation(1);
  coneActor->GetProperty()->SetRepresentationToSurface();
  coneActor->GetProperty()->SetAmbient(1);
  coneActor->GetProperty()->SetAmbientColor(1,1,1);
  coneActor->GetProperty()->SetDiffuseColor(0.7,0.7,0.7);
  coneActor->GetProperty()->SetSpecularColor(0.7,0.7,0.7);

  lineActor->GetProperty()->SetInterpolation(1);
  lineActor->GetProperty()->SetRepresentationToSurface();
  lineActor->GetProperty()->SetAmbient(1);
  lineActor->GetProperty()->SetAmbientColor(1,1,1);
  lineActor->GetProperty()->SetDiffuseColor(0.7,0.7,0.7);
  lineActor->GetProperty()->SetSpecularColor(0.7,0.7,0.7);

  aTextActor->GetProperty()->SetAmbient(1);
  aTextActor->GetProperty()->SetAmbientColor(1,1,1);
  aTextActor->GetProperty()->SetDiffuseColor(0.7,0.7,0.7);
  aTextActor->GetProperty()->SetSpecularColor(0.7,0.7,0.7);
     
  coneActor->PickableOff();
  lineActor->PickableOff();
  aTextActor->PickableOff();
  
  m_Triedron->AddItem(coneActor);
  m_Triedron->AddItem(lineActor);
  m_Triedron->AddItem(aTextActor);

  renderer->AddActor(coneActor);
  renderer->AddActor(lineActor);
  renderer->AddActor(aTextActor);
}  

bool VTKViewer_ViewFrame::isTrihedronDisplayed() {
  m_Triedron->InitTraversal();
  vtkActor *ac = m_Triedron->GetNextActor();
  while(!(ac==NULL)) {
    if(ac->GetVisibility()) return true;
    ac = m_Triedron->GetNextActor();
  }
  return false;
}

void VTKViewer_ViewFrame::SetTrihedronSize(int size)
{
  m_Triedron->InitTraversal();
  vtkActor* anActor = m_Triedron->GetNextActor();
  while(!(anActor==NULL)) {  
    m_Renderer->RemoveActor( anActor );
    anActor = m_Triedron->GetNextActor();
  }

  m_Triedron->RemoveAllItems();
  AddAxis(m_Renderer);
  m_RW->update();
}


void VTKViewer_ViewFrame::AddAxis(vtkRenderer* renderer) {  
  float origine[3];
  float X[3];
  float Y[3];
  float Z[3];
  float dim;

  QString Size = QAD_CONFIG->getSetting("Viewer:TrihedronSize");
  if( Size.isEmpty() ){
    dim = 100;
  } else {
    dim = Size.toFloat();
  }

  origine[0]=0;        origine[1]=0;        origine[2]=0;
  X[0]=origine[0]+dim; X[1]=origine[0];     X[2]=origine[0];
  Y[0]=origine[0];     Y[1]=origine[0]+dim; Y[2]=origine[0];
  Z[0]=origine[0];     Z[1]=origine[0];     Z[2]=origine[0]+dim;

  AddVector(origine,X,renderer, dim);
  AddVector(origine,Y,renderer, dim);
  AddVector(origine,Z,renderer, dim);
 
}

/*!
  Returns widget containing 3D-Viewer
*/
QWidget* VTKViewer_ViewFrame::getViewWidget() 
{
  return m_RW;
}


void VTKViewer_ViewFrame::InitialSetup() {
  m_Renderer = vtkRenderer::New() ;

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
 
  // CREATE AXIS
  m_Triedron = vtkActorCollection::New();
  AddAxis(m_Renderer);
 
  // Create an interactor.
  m_RWInteractor = VTKViewer_RenderWindowInteractor::New();
  m_RWInteractor->setGUIWindow(m_RW);
  m_RWInteractor->SetRenderWindow(m_RW->getRenderWindow());

  VTKViewer_InteractorStyleSALOME* RWS = VTKViewer_InteractorStyleSALOME::New();
  RWS->setGUIWindow(m_RW);
  m_RWInteractor->SetInteractorStyle(RWS); 

  m_RWInteractor->Initialize();
  RWS->setTriedron( m_Triedron );
  //SRN: additional initialization, to init CurrentRenderer of vtkInteractorStyle 
  RWS->FindPokedRenderer(0, 0);

  setCentralWidget( m_RW );
  onViewReset();
}

VTKViewer_ViewFrame::~VTKViewer_ViewFrame() {
  //
  // In order to ensure that the interactor unregisters
  // this RenderWindow, we assign a NULL RenderWindow to 
  // it before deleting it.
  //
  m_Transform->Delete() ;
    
  m_RWInteractor->SetRenderWindow(NULL) ;
  m_RWInteractor->Delete() ;
  
  //m_RW->Delete() ;

  // NRI : BugID 1137:  m_Renderer->Delete() ;
}


/*!
  Display/hide Trihedron
*/
void VTKViewer_ViewFrame::onViewTrihedron()
{
  if (isTrihedronDisplayed()) {
    m_Triedron->InitTraversal();
    vtkActor *ac = m_Triedron->GetNextActor();
    while(!(ac==NULL)) {
      ac->VisibilityOff();
      ac = m_Triedron->GetNextActor();
    }
  }
  else {
    m_Triedron->InitTraversal();
    vtkActor *ac = m_Triedron->GetNextActor();
    while(!(ac==NULL)) {
      ac->VisibilityOn();
      ac = m_Triedron->GetNextActor();
    }
    m_TriedronVisible = true;
  }  
  m_RW->update();
}

/*!
  Provides top projection of the active view
*/
void VTKViewer_ViewFrame::onViewTop() {
  vtkCamera* camera = m_Renderer->GetActiveCamera();
  camera->SetFocalPoint(0,0,0);
  camera->SetPosition(0,0,1);
  camera->SetViewUp(0,1,0);
  m_Renderer->ResetCamera();  
  onViewFitAll();
  m_RW->update();
}

/*!
  Provides bottom projection of the active view
*/
void VTKViewer_ViewFrame::onViewBottom()
{
  vtkCamera* camera = m_Renderer->GetActiveCamera();
  camera->SetFocalPoint(0,0,0);
  camera->SetPosition(0,0,-1);
  camera->SetViewUp(0,1,0);
  m_Renderer->ResetCamera();  
  onViewFitAll();
  m_RW->update();
}

/*!
  Provides left projection of the active view
*/
void VTKViewer_ViewFrame::onViewLeft()    
{
  vtkCamera* camera = m_Renderer->GetActiveCamera(); 
  camera->SetFocalPoint(0,0,0);
  camera->SetPosition(0,1,0);
  camera->SetViewUp(0,0,1);
  m_Renderer->ResetCamera();  
  onViewFitAll();
  m_RW->update(); 
}

/*!
  Provides right projection of the active view
*/
void VTKViewer_ViewFrame::onViewRight()
{
  vtkCamera* camera = m_Renderer->GetActiveCamera();
  camera->SetFocalPoint(0,0,0);
  camera->SetPosition(0,-1,0);
  camera->SetViewUp(0,0,1);
  m_Renderer->ResetCamera();  
  onViewFitAll();
  m_RW->update();
}

/*!
  Provides back projection of the active view
*/
void VTKViewer_ViewFrame::onViewBack()
{
  vtkCamera* camera = m_Renderer->GetActiveCamera();
  camera->SetPosition(-1,0,0);
  camera->SetFocalPoint(0,0,0);
  camera->SetViewUp(0,0,1);
  m_Renderer->ResetCamera();  
  onViewFitAll();
  m_RW->update();
}

/*!
  Provides front projection of the active view
*/
void VTKViewer_ViewFrame::onViewFront()
{
  vtkCamera* camera = m_Renderer->GetActiveCamera();
  camera->SetPosition(1,0,0);
  camera->SetFocalPoint(0,0,0);
  camera->SetViewUp(0,0,1);
  m_Renderer->ResetCamera();  
  onViewFitAll();
  m_RW->update();
}

/*!
  Reset the active view
*/
void VTKViewer_ViewFrame::onViewReset()    
{
  vtkCamera* camera = m_Renderer->GetActiveCamera();
  camera->SetPosition(1,-1,1);
  camera->SetFocalPoint(0,0,0);
  camera->SetViewUp(0,0,1);
  m_Renderer->ResetCamera();  
  
  double aOldScale = camera->GetParallelScale();
  camera->SetParallelScale(500);
  double aNewScale = camera->GetParallelScale();
  
  //for controlling labels scale after reset
  float dim;
  QString Size = QAD_CONFIG->getSetting("Viewer:TrihedronSize");
  if( Size.isEmpty() ){
    dim = 100;
  } else {
    dim = Size.toFloat();
  }
  float aScale = 17 * dim/100;

  m_Triedron->InitTraversal();
  vtkActor *ac = m_Triedron->GetNextActor();
  bool IsConeActor = true;
  while(!(ac==NULL)) {
    if(ac->IsA("vtkFollower")) {
      ac->SetScale(aScale, aScale, aScale);
      IsConeActor = true;
    }
    else {
      if (IsConeActor) {
	//coneActor is the first in the list (see m_Triedron->AddItem(...) in VTKViewer_ViewFrame::AddVector(...))
	IsConeActor = false;
      } 
    }
    ac = m_Triedron->GetNextActor();
  }

  m_Renderer->ResetCameraClippingRange();
  m_RW->update();
}

/*!
  Rotates the active view
*/
void VTKViewer_ViewFrame::onViewRotate()
{
  VTKViewer_InteractorStyleSALOME* RWS = dynamic_cast<VTKViewer_InteractorStyleSALOME*>(getRWInteractor()->GetInteractorStyle());
  if (RWS)
    RWS->startRotate();
}

/*!
  Sets a new center of the active view
*/
void VTKViewer_ViewFrame::onViewGlobalPan()
{
  VTKViewer_InteractorStyleSALOME* RWS = dynamic_cast<VTKViewer_InteractorStyleSALOME*>(getRWInteractor()->GetInteractorStyle());
  if (RWS)
    RWS->startGlobalPan();
}

/*!
  Zooms the active view
*/
void VTKViewer_ViewFrame::onViewZoom()
{
  VTKViewer_InteractorStyleSALOME* RWS = dynamic_cast<VTKViewer_InteractorStyleSALOME*>(getRWInteractor()->GetInteractorStyle());
  if (RWS)
    RWS->startZoom();
}

/*!
  Moves the active view
*/
void VTKViewer_ViewFrame::onViewPan()
{
  VTKViewer_InteractorStyleSALOME* RWS = dynamic_cast<VTKViewer_InteractorStyleSALOME*>(getRWInteractor()->GetInteractorStyle());
  if (RWS)
    RWS->startPan();
}

/*!
  Fits all obejcts within a rectangular area of the active view
*/
void VTKViewer_ViewFrame::onViewFitArea()
{
  VTKViewer_InteractorStyleSALOME* RWS = dynamic_cast<VTKViewer_InteractorStyleSALOME*>(getRWInteractor()->GetInteractorStyle());
  if (RWS)
    RWS->startFitArea();
}

/*!
  Fits all objects in the active view
*/
// Reset the camera clipping range to include this entire bounding box
static void ResetCameraClippingRange(vtkRenderer* theRenderer, float bounds[6] )
{
  //see vtkRenderer::ResetCameraClippingRange(float bounds[6]) method
  double  vn[3], position[3], a, b, c, d;
  double  range[2], dist;
  int     i, j, k;
  float center[3];
  float distance;
  float width;

  vtkCamera* anActiveCamera = theRenderer->GetActiveCamera();
  if ( anActiveCamera == NULL )
    {
      //vtkErrorMacro(<< "Trying to reset clipping range of non-existant camera");
    return;
    }
  
  // Find the plane equation for the camera view plane
  anActiveCamera->GetViewPlaneNormal(vn);
  anActiveCamera->GetPosition(position);
//  a = -vn[0];
//  b = -vn[1];
//  c = -vn[2];
//  d = -(a*position[0] + b*position[1] + c*position[2]);

  // Set the max near clipping plane and the min far clipping plane
//  range[0] = a*bounds[0] + b*bounds[2] + c*bounds[4] + d;
//  range[1] = 1e-18;

  // Find the closest / farthest bounding box vertex
//  for ( k = 0; k < 2; k++ )
//    {
//    for ( j = 0; j < 2; j++ )
//	  {
//	  for ( i = 0; i < 2; i++ )
//	    {
//	    dist = a*bounds[i] + b*bounds[2+j] + c*bounds[4+k] + d;
//	    range[0] = (dist<range[0])?(dist):(range[0]);
//	    range[1] = (dist>range[1])?(dist):(range[1]);
//	    }
//	  }
//    }
  
  center[0] = (bounds[0] + bounds[1])/2.0;
  center[1] = (bounds[2] + bounds[3])/2.0;
  center[2] = (bounds[4] + bounds[5])/2.0;
  width = sqrt((bounds[1]-bounds[0])*(bounds[1]-bounds[0]) +
	       (bounds[3]-bounds[2])*(bounds[3]-bounds[2]) +
	       (bounds[5]-bounds[4])*(bounds[5]-bounds[4]));
  distance = sqrt((position[0]-center[0])*(position[0]-center[0]) +
		  (position[1]-center[1])*(position[1]-center[1]) +
		  (position[2]-center[2])*(position[2]-center[2]));
  range[0] = distance - width/2.0;
  range[1] = distance + width/2.0;

  // Give ourselves a little breathing room
  range[0] = 0.99*range[0] - (range[1] - range[0])*0.5;
  range[1] = 1.01*range[1] + (range[1] - range[0])*0.5;

  // Make sure near is not bigger than far
  range[0] = (range[0] >= range[1])?(0.01*range[1]):(range[0]);

  // Make sure near is at least some fraction of far - this prevents near
  // from being behind the camera or too close in front. How close is too
  // close depends on the resolution of the depth buffer
  int ZBufferDepth = 16;
  vtkRenderWindow* aRenderWindow = theRenderer->GetRenderWindow();
  if (aRenderWindow)
    {
      ZBufferDepth = aRenderWindow->GetDepthBufferSize();
    }
  //
  if ( ZBufferDepth <= 16 )
    {
    range[0] = (range[0] < 0.01*range[1])?(0.01*range[1]):(range[0]);
    }
  else if ( ZBufferDepth <= 24 )
    {
    range[0] = (range[0] < 0.01*range[1])?(0.01*range[1]):(range[0]);
    }
  else
    {
    range[0] = (range[0] < 0.01*range[1])?(0.01*range[1]):(range[0]);
    }
  anActiveCamera->SetClippingRange( range );
}

static void ResetCamera(vtkRenderer* theRenderer, vtkActorCollection* theTriedron, VTKViewer_RenderWindowInteractor* theRWInteractor){  
  //see vtkRenderer::ResetCamera(float bounds[6]) method
  float      bounds[6];
  if(!theRenderer) return;
  theRenderer->ComputeVisiblePropBounds( bounds );

  float center[3];
  float distance;
  float width;
  double vn[3], *vup;
  int* winsize;
  
  if ( theRenderer->GetActiveCamera() != NULL )
    {
    theRenderer->GetActiveCamera()->GetViewPlaneNormal(vn);
    }
  else
    {
    MESSAGE("Trying to reset non-existant camera");
    return;
    }

  center[0] = (bounds[0] + bounds[1])/2.0;
  center[1] = (bounds[2] + bounds[3])/2.0;
  center[2] = (bounds[4] + bounds[5])/2.0;
  width = sqrt((bounds[1]-bounds[0])*(bounds[1]-bounds[0]) +
	       (bounds[3]-bounds[2])*(bounds[3]-bounds[2]) +
	       (bounds[5]-bounds[4])*(bounds[5]-bounds[4]));
  double ang = theRenderer->GetActiveCamera()->GetViewAngle();
  distance = 2.0*width/tan(ang*vtkMath::Pi()/360.0);
  
  // find size of the window
  winsize = theRenderer->GetSize();
  
  // check view-up vector against view plane normal
  vup = theRenderer->GetActiveCamera()->GetViewUp();
  if ( fabs(vtkMath::Dot(vup,vn)) > 0.999 )
    {
    MESSAGE("Resetting view-up since view plane normal is parallel");
    theRenderer->GetActiveCamera()->SetViewUp(-vup[2], vup[0], vup[1]);
    }

  // update the camera
  theRenderer->GetActiveCamera()->SetFocalPoint(center[0],center[1],center[2]);
  theRenderer->GetActiveCamera()->SetPosition(center[0]+distance*vn[0],
                                  center[1]+distance*vn[1],
                                  center[2]+distance*vn[2]);
  // setup default parallel scale
  double aOldScale = theRenderer->GetActiveCamera()->GetParallelScale();
  
  if(winsize[0]<winsize[1] )
    width=width*(float(winsize[1])/float(winsize[0]));
  
  theRenderer->GetActiveCamera()->SetParallelScale(width/2.0);
  double aNewScale = theRenderer->GetActiveCamera()->GetParallelScale();
  
  // for controlling label size 
  VTKViewer_InteractorStyleSALOME* Style = 0;
  if (theRWInteractor->GetInteractorStyle()->IsA("VTKViewer_InteractorStyleSALOME")) {
    Style = VTKViewer_InteractorStyleSALOME::SafeDownCast(theRWInteractor->GetInteractorStyle());
    Style->ControlLblSize(aOldScale,aNewScale);
  }
 
  //workaround on VTK
  //theRenderer->ResetCameraClippingRange(bounds);
  ResetCameraClippingRange(theRenderer,bounds);
}

void VTKViewer_ViewFrame::onViewFitAll()
{
  Standard_Boolean TriedronWasVisible = false;
  if (isTrihedronDisplayed()) {
    m_Triedron->InitTraversal();
    vtkActor *ac = m_Triedron->GetNextActor();
    while(!(ac==NULL)) {
      ac->VisibilityOff();
      ac = m_Triedron->GetNextActor();
    }
    TriedronWasVisible = true;
  }
  bool hasVisibleActors = m_Renderer->VisibleActorCount() > 0;
  if ( hasVisibleActors ) {   // if there are visible actors, not to take into account Trihedron
    ResetCamera(m_Renderer,m_Triedron,m_RWInteractor);
  } 
  if(TriedronWasVisible) {
    m_Triedron->InitTraversal();
    vtkActor *ac = m_Triedron->GetNextActor();
    while(!(ac==NULL)) {
      ac->VisibilityOn();
      ac = m_Triedron->GetNextActor();
    }
    if ( !hasVisibleActors ) { // if there are NO visible actors, fit view to see only Trihedron
      ResetCamera(m_Renderer,m_Triedron,m_RWInteractor);
    } 
  }
  //m_Renderer->ResetCameraClippingRange();
  m_RW->update();
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


void VTKViewer_ViewFrame::Repaint()
{
  // m_RWInteractor->Render();
  m_RW->update();
}

void VTKViewer_ViewFrame::GetScale(double theScale[3]){
  m_Transform->GetScale(theScale);
}

void VTKViewer_ViewFrame::SetScale(double theScale[3]){
  m_Transform->SetScale(theScale[0], theScale[1], theScale[2]);
  m_Transform->Modified();
  vtkActorCollection* theActors = m_Renderer->GetActors();
  theActors->InitTraversal();
  vtkActor *anActor;
  while(anActor = theActors->GetNextActor())
    anActor->GetMapper()->Update();
  Repaint();
}

void VTKViewer_ViewFrame::AddActor( SALOME_Actor* theActor, bool update /*=false*/ ){
  theActor->SetVisibility(true);
  theActor->AddToRender(m_Renderer);
  theActor->SetTransform(m_Transform);
  if(update){
    m_Renderer->ResetCameraClippingRange();
    m_RWInteractor->Render();
  }
}

void VTKViewer_ViewFrame::RemoveActor( SALOME_Actor* theActor, bool update /*=false*/ ){
  theActor->RemoveFromRender(m_Renderer);
  if(update){
    m_Renderer->ResetCameraClippingRange();
    m_RWInteractor->Render();
  }
}


