//=============================================================================
// File      : SALOME_PYQT_GUI.cxx
// Created   : mer jun  4 17:17:20 UTC 2003
// Author    : Paul RASCLE, EDF 
// Project   : SALOME
// Copyright : EDF 2003
// $Header$
//=============================================================================

#include "SALOME_PYQT_GUI.h"

#include "QAD_Desktop.h"

#include "SALOME_Selection.h"
#include "SALOME_InteractiveObject.hxx"
#include "SALOMEGUI_QtCatchCorbaException.hxx"
#include "utilities.h"

#include <sipSalomePyQtDeclSalomePyQt.h>
#include <sipqtQWorkspace.h>
#include <sipqtQPopupMenu.h>

#include <map>
#include <string>

using namespace std;

static PyInterp_PyQt *interp = NULL;
static map<int,PyInterp_PyQt*> mapInterp;

//=============================================================================
/*!
 *  Calls module.setWorkSpace with PyQt QWorkspace object to use with
 *  interpreter.
 */
//=============================================================================

void SALOME_PYQT_GUI::setWorkSpace()
{
  MESSAGE("setWorkSpace");
  PyLockWrapper aLock = interp->GetLockWrapper();

  //   Try to import qt module. If it's not possible don't go on
  PyObjWrapper qtmodule(PyImport_ImportModule("qt"));
  if(!qtmodule){
    PyErr_Print();
    return ;
  }  

  PyObjWrapper pyws(sipMapCppToSelf( QAD_Application::getDesktop()->getMainFrame(),
				     sipClass_QWorkspace));
  PyObjWrapper res(PyObject_CallMethod(_module,"setWorkSpace","O",pyws.get()));
  SCRUTE(pyws->ob_refcnt);
  if(!res){
    PyErr_Print();
    return ;
  }
}

//=============================================================================
/*!
 *  Import Python module (from _moduleName)                   
 */
//=============================================================================
void SALOME_PYQT_GUI::importModule()
{
  PyLockWrapper aLock = interp->GetLockWrapper();

  _module=PyImport_ImportModule((char*)_moduleName.c_str());
  if(!_module){
    MESSAGE ( " Problem... " );
    PyErr_Print();
    return;
  }
}

//=============================================================================
/*!
 *  Initialises python interpreter (only one per study), imports python module
 *  from given module name, sets static reference to module,
 *  sets module workspace.
 */
//=============================================================================

void SALOME_PYQT_GUI::initInterp(int StudyID)
{
  MESSAGE("SALOME_PYQT_GUI::initInterp");
  if(mapInterp.find(StudyID) != mapInterp.end()){
    MESSAGE ( "SALOME_PYQT_GUI::initInterp StudyID is found " << StudyID );
    interp = mapInterp[StudyID];
    return;
  }else{
    MESSAGE ( "SALOME_PYQT_GUI::initInterp StudyID is not found " << StudyID );
    /*
     * The creation of Python interpretor must be protected par a C++ Lock because of C threads
     */
    ThreadLock aPyLock = GetPyThreadLock("SALOME_PYQT_GUI::initInterp");
    interp = new PyInterp_PyQt();
    interp->initialize();
    mapInterp[StudyID] = interp;
  }
  // imports Python GUI module and puts it in _module attribute
  importModule();
  // calls _module.setWorkspace and passes the SIP object main workspace
  setWorkSpace();

}

//=============================================================================
/*!
 * constructor : only calls SALOMEGUI constructor
 */
//=============================================================================
SALOME_PYQT_GUI::SALOME_PYQT_GUI( const QString& theName, QObject* theParent ) :
    SALOMEGUI( theName, theParent ),_module(0)
{
    MESSAGE("SALOME_PYQT_GUI::SALOME_PYQT_GUI");
}

//=============================================================================
/*!
 * destructor : do nothing
 */
//=============================================================================
SALOME_PYQT_GUI::~SALOME_PYQT_GUI()
{
}

//=============================================================================
/*!
 *  Calls python module.OnGUIEvent(theCommandID)
 */
//=============================================================================

bool SALOME_PYQT_GUI::OnGUIEvent (int theCommandID,
				  QAD_Desktop* parent)
{
  MESSAGE("SALOME_PYQT_GUI::OnGUIEvent");
  ThreadLock aPyLock = GetPyThreadLock("SALOME_PYQT_GUI::OnGUIEvent");
  PyLockWrapper aLock = interp->GetLockWrapper();

  PyObjWrapper res(PyObject_CallMethod(_module,"OnGUIEvent","i",theCommandID));
  if(!res){
    PyErr_Print();
    return false;
  }
  return true;
}

//=============================================================================
/*!
 *  no call to python module.OnKeyPress()
 */
//=============================================================================

bool SALOME_PYQT_GUI::OnKeyPress (QKeyEvent* pe,
				  QAD_Desktop* parent,
				  QAD_StudyFrame* studyFrame)
{
  MESSAGE("SALOME_PYQT_GUI::OnKeyPress");
  return true;
}

//=============================================================================
/*!
 *  no call to python module.OnMousePress()
 */
//=============================================================================

bool SALOME_PYQT_GUI::OnMousePress (QMouseEvent* pe ,
				    QAD_Desktop* parent, 
				    QAD_StudyFrame* studyFrame)
{
  MESSAGE("SALOME_PYQT_GUI::OnMousePress");
  return false;
}

//=============================================================================
/*!
 *  no call to python module.OnMouseMove()
 */
//=============================================================================

bool SALOME_PYQT_GUI::OnMouseMove (QMouseEvent* pe ,
				   QAD_Desktop* parent, 
				   QAD_StudyFrame* studyFrame)
{
  // Commented out to avoid multiple traces ...
  // MESSAGE("SALOME_PYQT_GUI::OnMouseMove");
  return true;
}

//=============================================================================
/*!
 *  Calls initInterp to initialise python interpreter (only one per study) and
 *  to import python module. Calls module.setSettings() 
 */
//=============================================================================

bool SALOME_PYQT_GUI::SetSettings (QAD_Desktop* parent, char* moduleName)
{
  MESSAGE("SALOME_PYQT_GUI::SetSettings");
  int StudyID = QAD_Application::getDesktop()->getActiveStudy()->getStudyId();
  SCRUTE ( StudyID );
  _moduleName = moduleName + string("GUI");
  SCRUTE(_moduleName);

  // initializes one Python interpreter by study and puts it in interp global variable
  // imports Python GUI module and puts it in _module attribute
  // calls _module.setWorkspace and passes the SIP object main workspace
  initInterp(StudyID);
  
  PyLockWrapper aLock = interp->GetLockWrapper();

  _module = PyImport_ImportModule((char*)_moduleName.c_str());
  if(!_module){
    PyErr_Print();
    return false;
  }

  PyObjWrapper res(PyObject_CallMethod(_module,"setSettings",""));
  if(!res){
    PyErr_Print();
    return false;
  }
  return true;
}

//=============================================================================
/*!
 * Calls module.customPopup with popup menu to custom, and string values of 
 * context, parent and selected object (strings defined by DefinePopup, which
 * is called before).
 */
//=============================================================================

bool SALOME_PYQT_GUI::CustomPopup ( QAD_Desktop* parent,
				    QPopupMenu* popup,
				    const QString & theContext,
				    const QString & theParent,
				    const QString & theObject )
{
  MESSAGE("SALOME_PYQT_GUI::CustomPopup");

  MESSAGE ( " theContext : " << theContext.latin1() );
  MESSAGE ( " theParent : " << theParent.latin1() );
  MESSAGE ( " theObject : " << theObject.latin1() );

  PyLockWrapper aLock = interp->GetLockWrapper();

  PyObjWrapper pypop(sipMapCppToSelf( popup, sipClass_QPopupMenu));

  PyObjWrapper res(PyObject_CallMethod(_module,"customPopup",
				       "Osss",pypop.get(),
				       theContext.latin1(), 
				       theObject.latin1(), 
				       theParent.latin1()));
  if(!res){
    PyErr_Print();
    return false;
  }
  return true;
}


//=============================================================================
/*!
 * Calls module.definePopup to modify the strings that define context parent
 * and selected object. Called before CustomPopup.
 */
//=============================================================================

void SALOME_PYQT_GUI::DefinePopup( QString & theContext,
				   QString & theParent,
				   QString & theObject )
{
  MESSAGE("SALOME_PYQT_GUI::DefinePopup");
  theContext = "";
  theObject = "";
  theParent = "";
  
  PyLockWrapper aLock = interp->GetLockWrapper();

  PyObjWrapper res(PyObject_CallMethod(_module,"definePopup","sss",
				       theContext.latin1(), 
				       theObject.latin1(), 
				       theParent.latin1()));
  if(!res){
    PyErr_Print();
    return;
  }
  char *co, *ob, *pa;
  if(!PyArg_ParseTuple(res, "sss", &co, &ob, &pa))
  {
    // It's not a valid tuple. Do nothing.
    return;
  }

  MESSAGE ("parseOk ");
  MESSAGE (" --- " << co << " " << ob << " " << pa);

  theContext = co;
  theObject = ob;
  theParent = pa;

  MESSAGE ( " theContext : " << theContext.latin1() );
  MESSAGE ( " theParent : " << theParent.latin1() );
  MESSAGE ( " theObject : " << theObject.latin1() );
}

//=============================================================================
/*!
 * Initialize new interpreter (if not exists) with new study ID.
 * Calls module.activeStudyChanged with new study ID. Called twice.
 */
//=============================================================================

bool SALOME_PYQT_GUI::ActiveStudyChanged( QAD_Desktop* parent )
{
  
  int StudyID = parent->getActiveApp()->getActiveStudy()->getStudyId();
  MESSAGE("SALOME_PYQT_GUI::ActiveStudyChanged"<<StudyID<<" - " <<this);
  initInterp(StudyID);
  
  PyLockWrapper aLock = interp->GetLockWrapper();

  PyObjWrapper res(PyObject_CallMethod(_module,"activeStudyChanged","i", StudyID ));
  if(!res){
    PyErr_Print();
    return false;
  }
  return true;
}

//=============================================================================
/*!
 *  no call to python module.BuildPresentation() (not yet ???)
 */
//=============================================================================
void SALOME_PYQT_GUI::BuildPresentation( const Handle(SALOME_InteractiveObject)&,
                                         QAD_ViewFrame* )
{
}

//=============================================================================
/*!
 *  no call to python module.SupportedViewType() (not yet ???)
 */
//=============================================================================
void SALOME_PYQT_GUI::SupportedViewType(int* /*buffer*/, int /*bufferSize*/ )
{
}

//=============================================================================
/*!
 *  no call to python module.Deactivate() (not yet ???)
 */
//=============================================================================
void SALOME_PYQT_GUI::Deactivate()
{
}




//=============================================================================
/*!
 *  Component GUI Factory : returns a new GUI obj at each call
 */
//=============================================================================

extern "C"
{
  Standard_EXPORT SALOMEGUI* GetComponentGUI() {
    MESSAGE("SALOME_PYQT_GUI::GetComponentGUI");
    SALOMEGUI* aGUI = new SALOME_PYQT_GUI("");
    return aGUI;
  }
}
