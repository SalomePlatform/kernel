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
#include "VTKViewer_RenderWindowInteractor.h"
#include "VTKViewer_InteractorStyleSALOME.h"
#include "VTKViewer_Algorithm.h"
#include "VTKViewer_Functor.h"
#include "VTKViewer_Prs.h"

#include "SALOME_Actor.h"
#include "SALOME_Transform.h"
#include "SALOME_TransformFilter.h"
#include "SALOME_GeometryFilter.h"
#include "SALOMEGUI.h"

#include "QAD_Settings.h"
#include "QAD_Config.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "SALOME_Selection.h"
#include "SALOME_InteractiveObject.hxx"
#include "ToolsGUI.h"
#include "SALOMEDS_Tool.hxx"

#include "utilities.h"

//QT Include
#include <qlayout.h>
#include <qcolordialog.h>
#include <qfiledialog.h>
#include <qapplication.h>

// VTK Includes
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkActorCollection.h>

#include <TColStd_IndexedMapOfInteger.hxx>

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
  m_RWInteractor->SetRenderWindow(m_RW->getRenderWindow());

  VTKViewer_InteractorStyleSALOME* RWS = VTKViewer_InteractorStyleSALOME::New();
  m_RWInteractor->SetInteractorStyle(RWS); 
  RWS->Delete();

  m_RWInteractor->setGUIWindow(m_RW);
  RWS->setGUIWindow(m_RW);

  m_RWInteractor->Initialize();
  m_RWInteractor->setViewFrame(this);
  RWS->setTriedron(m_Triedron);
  RWS->setViewFrame(this);
  //SRN: additional initialization, to init CurrentRenderer of vtkInteractorStyle 
  RWS->FindPokedRenderer(0, 0);

  setCentralWidget( m_RW );
  onViewReset();
}

VTKViewer_ViewFrame::~VTKViewer_ViewFrame() {
  // In order to ensure that the interactor unregisters
  // this RenderWindow, we assign a NULL RenderWindow to 
  // it before deleting it.
  m_RWInteractor->SetRenderWindow(NULL) ;
  m_RWInteractor->Delete() ;
  
  m_Transform->Delete() ;
  //m_RW->Delete() ;
  m_Renderer->RemoveAllProps();
  // NRI : BugID 1137:  m_Renderer->Delete() ;
  m_Triedron->Delete();
  INFOS("VTKViewer_ViewFrame::~VTKViewer_ViewFrame()");
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

bool VTKViewer_ViewFrame::ComputeTrihedronSize( double& theNewSize, double& theSize )
{
  // calculating diagonal of visible props of the renderer
  float bnd[ 6 ];
  m_Triedron->VisibilityOff();
  if ( ::ComputeVisiblePropBounds( m_Renderer, bnd ) == 0 )
  {
    bnd[ 1 ] = bnd[ 3 ] = bnd[ 5 ] = 100;
    bnd[ 0 ] = bnd[ 2 ] = bnd[ 100 ] = 0;
  }
  m_Triedron->VisibilityOn();
  float aLength = 0;
  static bool CalcByDiag = false;
  if ( CalcByDiag )
  {
    aLength = sqrt( ( bnd[1]-bnd[0])*(bnd[1]-bnd[0] )+
                    ( bnd[3]-bnd[2])*(bnd[3]-bnd[2] )+
                    ( bnd[5]-bnd[4])*(bnd[5]-bnd[4] ) );
  }
  else
  {
    aLength = bnd[ 1 ]-bnd[ 0 ];
    aLength = max( ( bnd[ 3 ] - bnd[ 2 ] ),aLength );
    aLength = max( ( bnd[ 5 ] - bnd[ 4 ] ),aLength );
  }

  static float aSizeInPercents = 105;
  QString aSetting = QAD_CONFIG->getSetting( "Viewer:TrihedronSize" );
  if ( !aSetting.isEmpty() )
    aSizeInPercents = aSetting.toFloat();

  static float EPS_SIZE = 5.0E-3;
  theSize = m_Triedron->GetSize();
  theNewSize = aLength * aSizeInPercents / 100.0;

  // if the new trihedron size have sufficient difference, then apply the value
  return fabs( theNewSize - theSize) > theSize * EPS_SIZE ||
         fabs( theNewSize-theSize ) > theNewSize * EPS_SIZE;
}

double VTKViewer_ViewFrame::GetTrihedronSize() const
{
  return m_Triedron->GetSize();
}

void VTKViewer_ViewFrame::AdjustTrihedrons( const bool forcedUpdate )
{
  if ( !isTrihedronDisplayed() && !forcedUpdate )
    return;

  int aVisibleNum = m_Triedron->GetVisibleActorCount( m_Renderer );
  if ( aVisibleNum || forcedUpdate )
  {
    // if the new trihedron size have sufficient difference, then apply the value
    double aNewSize = 100, anOldSize;
    if ( ComputeTrihedronSize( aNewSize, anOldSize ) || forcedUpdate )
    {
      m_Triedron->SetSize( aNewSize );
      // itearte throuh displayed objects and set size if necessary

      vtkActorCollection* anActors = getRenderer()->GetActors();
      anActors->InitTraversal();
      while( vtkActor* anActor = anActors->GetNextActor() )
      {
        if( SALOME_Actor* aSActor = SALOME_Actor::SafeDownCast( anActor ) )
        {
          if ( aSActor->IsResizable() )
            aSActor->SetSize( 0.5 * aNewSize );
        }
      }
    }
  }

  ::ResetCameraClippingRange(m_Renderer);
}

void VTKViewer_ViewFrame::onAdjustTrihedron()
{   
  AdjustTrihedrons( false );
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
  camera->SetPosition(0,-1,0);
  camera->SetViewUp(0,0,1);
  camera->SetFocalPoint(0,0,0);
  onViewFitAll();
}

/*!
  Provides right projection of the active view
*/
void VTKViewer_ViewFrame::onViewRight(){
  vtkCamera* camera = m_Renderer->GetActiveCamera();
  camera->SetPosition(0,1,0);
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


void VTKViewer_ViewFrame::SetSelectionMode( Selection_Mode mode )
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
  m_RWInteractor->highlight(IObject, highlight, update);

  if(Sel->HasIndex(IObject) && IObject->hasEntry()){
    TColStd_IndexedMapOfInteger MapIndex;
    Sel->GetIndex(IObject,MapIndex);
    using namespace SALOME::VTK;
    const char* anEntry = IObject->getEntry();
    vtkActorCollection* aCollection = getRenderer()->GetActors();
    if(SALOME_Actor* anActor = Find<SALOME_Actor>(aCollection,TIsSameEntry<SALOME_Actor>(anEntry))){
      switch (Sel->SelectionMode()) {
      case NodeSelection:
	m_RWInteractor->highlightPoint(MapIndex,anActor,highlight,update);
	break;
      case EdgeOfCellSelection:
	m_RWInteractor->highlightEdge(MapIndex,anActor,highlight,update);
	break;
      case CellSelection:
      case EdgeSelection:
      case FaceSelection:
      case VolumeSelection:
	m_RWInteractor->highlightCell(MapIndex,anActor,highlight,update);
	break;
      }
    }
  }else{
    m_RWInteractor->unHighlightSubSelection();
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

/* selection */
Handle(SALOME_InteractiveObject) VTKViewer_ViewFrame::FindIObject(const char* theEntry)
{
  using namespace SALOME::VTK;
  SALOME_Actor* anActor = 
    Find<SALOME_Actor>(getRenderer()->GetActors(),
		       TIsSameEntry<SALOME_Actor>(theEntry));
  if(anActor)
    return anActor->getIO();

  return Handle(SALOME_InteractiveObject)();
}

/* display */		
void VTKViewer_ViewFrame::Display(const Handle(SALOME_InteractiveObject)& theIObject, bool update)
{
  QAD_Study*          aQADStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection*   aSel      = SALOME_Selection::Selection( aQADStudy->getSelection() );
  SALOMEDS::Study_var aStudy    = aQADStudy->getStudyDocument();

  m_RWInteractor->Display(theIObject,false);
  ToolsGUI::SetVisibility( aStudy, theIObject->getEntry(), true, this );
  aSel->AddIObject(theIObject,false);

  if(update)
    Repaint();
}


struct TDisplayAction{
  SALOME_Selection* mySel;
  Handle(SALOME_InteractiveObject) myIO;
  TDisplayAction(SALOME_Selection* theSel,
		 Handle(SALOME_InteractiveObject) theIO): 
    mySel(theSel), myIO(theIO)
  {}
  void operator()(SALOME_Actor* theActor){
    theActor->SetVisibility(true);
    mySel->AddIObject(myIO,false);
  }
};

void VTKViewer_ViewFrame::DisplayOnly(const Handle(SALOME_InteractiveObject)& theIObject)
{
  QAD_Study* aStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* aSel = SALOME_Selection::Selection(aStudy->getSelection());

  aSel->ClearIObjects();
  //m_RWInteractor->EraseAll();
  EraseAll();

  using namespace SALOME::VTK;
  ForEachIf<SALOME_Actor>(getRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TDisplayAction(aSel,theIObject));

  ToolsGUI::SetVisibility(
    aStudy->getStudyDocument(), theIObject->getEntry(), true, this );

  Repaint();
}


struct TEraseAction: TDisplayAction{
  VTKViewer_RenderWindowInteractor* myRWInteractor;
  TEraseAction(SALOME_Selection* theSel,
	       Handle(SALOME_InteractiveObject) theIO,
	       VTKViewer_RenderWindowInteractor* theRWInteractor): 
    TDisplayAction(theSel,theIO),
    myRWInteractor(theRWInteractor)
  {}
  void operator()(SALOME_Actor* theActor){
    myRWInteractor->Erase(myIO,false);
    mySel->RemoveIObject(myIO,false);
  }
};

void VTKViewer_ViewFrame::Erase(const Handle(SALOME_InteractiveObject)& theIObject, bool update)
{
  QAD_Study* aStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* aSel = SALOME_Selection::Selection(aStudy->getSelection());

  using namespace SALOME::VTK;
  ForEachIf<SALOME_Actor>(getRenderer()->GetActors(),
			  TIsSameIObject<SALOME_Actor>(theIObject),
			  TEraseAction(aSel,theIObject,m_RWInteractor));

  ToolsGUI::SetVisibility(
    aStudy->getStudyDocument(), theIObject->getEntry(), false, this );

  if(update)
    Repaint();
}


void VTKViewer_ViewFrame::DisplayAll()
{
  m_RWInteractor->DisplayAll();

  // update flag of visibility
  QAD_Study*               aQADStudy  = QAD_Application::getDesktop()->getActiveStudy();
  SALOMEDS::Study_var      aStudy     = aQADStudy->getStudyDocument();
  QAD_Desktop*             aDesktop   = QAD_Application::getDesktop();
  const QString&           aCompName  = aDesktop->getComponentDataType();
  SALOMEDS::SObject_var    aComponent =
    SALOMEDS::SObject::_narrow( aStudy->FindComponent ( aCompName.latin1() ) );
    
  std::list<SALOMEDS::SObject_var> aList;
  SALOMEDS_Tool::GetAllChildren( aStudy, aComponent, aList );

  std::list<SALOMEDS::SObject_var>::iterator anIter = aList.begin();
  for ( ; anIter != aList.end(); ++anIter )
    ToolsGUI::SetVisibility( aStudy, (*anIter)->GetID(), true, this );
}


void VTKViewer_ViewFrame::EraseAll()
{
  m_RWInteractor->EraseAll();

  // update flag of visibility
  QAD_Study*               aQADStudy  = QAD_Application::getDesktop()->getActiveStudy();
  SALOMEDS::Study_var      aStudy     = aQADStudy->getStudyDocument();
  QAD_Desktop*             aDesktop   = QAD_Application::getDesktop();
  const QString&           aCompName  = aDesktop->getComponentDataType();
  SALOMEDS::SObject_var    aComponent =
    SALOMEDS::SObject::_narrow( aStudy->FindComponent ( aCompName.latin1() ) );

  std::list<SALOMEDS::SObject_var> aList;
  SALOMEDS_Tool::GetAllChildren( aStudy, aComponent, aList );

  std::list<SALOMEDS::SObject_var>::iterator anIter = aList.begin();
  for ( ; anIter != aList.end(); ++anIter )
    ToolsGUI::SetVisibility( aStudy, (*anIter)->GetID(), false, this );
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

void VTKViewer_ViewFrame::InsertActor( SALOME_Actor* theActor, bool theMoveInternalActors ){
  theActor->AddToRender(m_Renderer);
  theActor->SetTransform(m_Transform);
  if(theMoveInternalActors) 
    m_RWInteractor->MoveInternalActors();
}

void VTKViewer_ViewFrame::AddActor( SALOME_Actor* theActor, bool theUpdate /*=false*/ ){
  InsertActor(theActor);
  if(theUpdate) 
    Repaint();
}

void VTKViewer_ViewFrame::RemoveActor( SALOME_Actor* theActor, bool theUpdate /*=false*/ ){
  theActor->RemoveFromRender(m_Renderer);
  if(theUpdate) 
    Repaint();
}

void VTKViewer_ViewFrame::MoveActor(SALOME_Actor* theActor)
{
  RemoveActor(theActor);
  InsertActor(theActor,true);
}

//==========================================================
/*!
 *  VTKViewer_ViewFrame::Display
 *  Display presentation
 */
//==========================================================
void VTKViewer_ViewFrame::Display( const SALOME_VTKPrs* prs )
{
  // try do downcast object
  const VTKViewer_Prs* aVTKPrs = dynamic_cast<const VTKViewer_Prs*>( prs );
  if ( !aVTKPrs || aVTKPrs->IsNull() )
    return;

  vtkActorCollection* actors = aVTKPrs->GetObjects();
  if ( !actors )
    return;

  actors->InitTraversal();
  vtkActor* actor;
  while( ( actor = actors->GetNextActor() ) )
  {
    SALOME_Actor* salomeActor = SALOME_Actor::SafeDownCast( actor );
    if ( salomeActor )                      
    {
      // just display the object
      m_RWInteractor->Display( salomeActor, false );
      
      // Set visibility flag
      Handle(SALOME_InteractiveObject) anObj = salomeActor->getIO();
      if ( !anObj.IsNull() && anObj->hasEntry() )
      {
        SALOMEDS::Study_var aStudy =
          QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();
        ToolsGUI::SetVisibility( aStudy, anObj->getEntry(), true, this );
      }

      if ( salomeActor->IsSetCamera() )
        salomeActor->SetCamera( getRenderer()->GetActiveCamera() );
    }
  }
}

//==========================================================
/*!
 *  VTKViewer_ViewFrame::Erase
 *  Erase presentation
 */
//==========================================================
void VTKViewer_ViewFrame::Erase( const SALOME_VTKPrs* prs, const bool forced )
{
  // try do downcast object
  const VTKViewer_Prs* aVTKPrs = dynamic_cast<const VTKViewer_Prs*>( prs );
  if ( !aVTKPrs || aVTKPrs->IsNull() )
    return;

  vtkActorCollection* actors = aVTKPrs->GetObjects();
  if ( !actors )
    return;

  SALOMEDS::Study_var aStudy =
    QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();

  actors->InitTraversal();
  vtkActor* actor;
  while( ( actor = actors->GetNextActor() ) ) {
    SALOME_Actor* salomeActor = SALOME_Actor::SafeDownCast( actor );
    if ( salomeActor ) {
      // just erase the object
      if ( forced )
        m_RWInteractor->Remove( salomeActor, false );
      else
        m_RWInteractor->Erase( salomeActor, forced );

      // Set visibility flag if necessary
      if ( !forced )
      {
        Handle(SALOME_InteractiveObject) anObj = salomeActor->getIO();
        if ( !anObj.IsNull() && anObj->hasEntry() )
          ToolsGUI::SetVisibility( aStudy, anObj->getEntry(), true, this );
      }
    }
  }
}
  
//==========================================================
/*!
 *  VTKViewer_ViewFrame::CreatePrs
 *  Create presentation by entry
 */
//==========================================================
SALOME_Prs* VTKViewer_ViewFrame::CreatePrs( const char* entry )
{
  VTKViewer_Prs* prs = new VTKViewer_Prs();
  if ( entry ) {
    vtkActorCollection* theActors = m_Renderer->GetActors();
    theActors->InitTraversal();
    vtkActor* ac;
    while( ( ac = theActors->GetNextActor() ) ) {
      SALOME_Actor* anActor = SALOME_Actor::SafeDownCast( ac );
      if ( anActor && anActor->hasIO() && !strcmp( anActor->getIO()->getEntry(), entry ) ) {
	prs->AddObject( ac );
      }
    }
  }
  return prs;
}

//==========================================================
/*!
 *  VTKViewer_ViewFrame::BeforeDisplay
 *  Axiluary method called before displaying of objects
 */
//==========================================================
void  VTKViewer_ViewFrame::BeforeDisplay( SALOME_Displayer* d )
{
  d->BeforeDisplay( this, SALOME_VTKViewType() );
}

//==========================================================
/*!
 *  VTKViewer_ViewFrame::AfterDisplay
 *  Axiluary method called after displaying of objects
 */
//==========================================================
void  VTKViewer_ViewFrame::AfterDisplay( SALOME_Displayer* d )
{
  d->AfterDisplay( this, SALOME_VTKViewType() );
}

//==========================================================
/*!
 *  VTKViewer_ViewFrame::undo
 *  Redisplay all objects in viewer
 */
//==========================================================
void VTKViewer_ViewFrame::undo( QAD_Study* theQADStudy, const char* /*StudyFrameEntry*/ )
{
  redisplayAll( theQADStudy, true );
}

//==========================================================
/*!
 *  VTKViewer_ViewFrame::redo
 *  Redisplay all objects in viewer
 */
//==========================================================
void VTKViewer_ViewFrame::redo( QAD_Study* theQADStudy, const char* /*StudyFrameEntry*/ )
{
  redisplayAll( theQADStudy, true );
}

//==========================================================
/*!
 *  VTKViewer_ViewFrame::redisplayAll
 *  Redisplay all objects in viewer
 */
//==========================================================
void VTKViewer_ViewFrame::redisplayAll( QAD_Study* theQADStudy, const bool theToUpdate )
{
  SALOMEDS::Study_var      aStudy     = theQADStudy->getStudyDocument();
  SALOME_Selection*        aSel       = SALOME_Selection::Selection( theQADStudy->getSelection() );
  QAD_Desktop*             aDesktop   = QAD_Application::getDesktop();
  SALOMEGUI*               aGUI       = aDesktop->getActiveGUI();
  const QString&           aCompName  = aDesktop->getComponentDataType();
  SALOMEDS::SObject_var    aComponent =
    SALOMEDS::SObject::_narrow( aStudy->FindComponent ( aCompName.latin1() ) );

  if ( aComponent->_is_nil() )
    return;

  bool isTrhDisplayed = isTrihedronDisplayed();

  m_RWInteractor->RemoveAll( false );
  //m_RWInteractor->EraseAll();

  aSel->ClearIObjects();
  
  if ( isTrhDisplayed )
    m_Triedron->AddToRender( m_Renderer );

  std::list<SALOMEDS::SObject_var> aList;
  SALOMEDS_Tool::GetAllChildren( aStudy, aComponent, aList );

  std::list<SALOMEDS::SObject_var>::iterator anIter = aList.begin();
  for ( ; anIter != aList.end(); ++anIter )
  {
    SALOMEDS::SObject_var anObj = (*anIter);
    if ( ToolsGUI::GetVisibility( aStudy, anObj, this ) )
    {
      Handle(SALOME_InteractiveObject) anIObj = new SALOME_InteractiveObject();
      anIObj->setEntry( anObj->GetID() );
      aGUI->BuildPresentation( anIObj, this );
    }
    
  }

  if ( theToUpdate )
    Repaint();
}















