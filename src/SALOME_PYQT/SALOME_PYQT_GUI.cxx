using namespace std;
//=============================================================================
// File      : SALOME_PYQT_GUI.cxx
// Created   : mer jun  4 17:17:20 UTC 2003
// Author    : Paul RASCLE, EDF 
// Project   : SALOME
// Copyright : EDF 2003
// $Header$
//=============================================================================

#include "SALOME_PYQT_GUI.hxx"

#include "QAD_Desktop.h"

#include "QAD_MessageBox.h"
#include "SALOME_Selection.h"
#include "SALOME_InteractiveObject.hxx"
#include "SALOMEGUI_QtCatchCorbaException.hxx"
#include "utilities.h"
#include "PyInterp_PyQt.h"
//#include <cStringIO.h>

#include <sipSalomePyQtDeclSalomePyQt.h>
#include <sipqtQWorkspace.h>
#include <sipqtQPopupMenu.h>

#include <map>
#include <string>

static PyInterp_PyQt *interp = NULL;
static map<int,PyInterp_PyQt*> mapInterp;
static PyObject *module;
static string _moduleName;

//=============================================================================
/*!
 *  Calls module.setWorkSpace with PyQt QWorkspace object to use with
 *  interpreter.
 */
//=============================================================================

static void setWorkSpace()
{
  MESSAGE("setWorkSpace");
  PyObject *res,*pyws;

  interp->enter();
  pyws=sipMapCppToSelf( QAD_Application::getDesktop()->getMainFrame(),
			sipClass_QWorkspace);
  res=PyObject_CallMethod(module,"setWorkSpace","O",pyws);
  SCRUTE(pyws->ob_refcnt);
  Py_DECREF(pyws);
  if (res == NULL)
    {
      PyErr_Print();
      interp->quit();
      return ;
    }
  Py_DECREF(res);
  interp->quit();
  return ;
}

//=============================================================================
/*!
 *  Initialises python interpreter (only one per study), imports python module
 *  from given module name, sets static reference to module,
 *  sets module workspace.
 */
//=============================================================================

static void initInterp(int StudyID)
{
  MESSAGE("initInterp");
  if (mapInterp.find(StudyID) != mapInterp.end())
    {
      MESSAGE ( " StudyID is found " << StudyID );
      interp = mapInterp[StudyID];
      return;
    }
  else 
    {
      MESSAGE ( " StudyID is not found " << StudyID );
      interp=new PyInterp_PyQt();
      mapInterp[StudyID] = interp;
    }

  interp->enter();

  module=PyImport_ImportModule((char*)_moduleName.c_str());
  if(module == NULL)
    {
      INFOS ( " Problem... " );
      PyErr_Print();
      interp->quit();
      return;
    }
  // PyQt import is OK
  interp->quit();
  setWorkSpace();
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
  PyObject *res;
  
  interp->enter();
  res=PyObject_CallMethod(module,"OnGUIEvent","i",theCommandID);
  if (res == NULL)
    {
      PyErr_Print();
      interp->quit();
      return false;
    }
  Py_DECREF(res);
  interp->quit();
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
  // La ligne suivante est commentée sinon multiple traces ...
  // MESSAGE("SALOME_PYQT_GUI::OnMouseMouve");
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
  PyObject *res;
  int StudyID = QAD_Application::getDesktop()->getActiveStudy()->getStudyId();
  SCRUTE ( StudyID );
  _moduleName = moduleName + string("GUI");
  SCRUTE(_moduleName);
  initInterp(StudyID);
  
  interp->enter();
  res=PyObject_CallMethod(module,"setSettings","");
  if (res == NULL)
    {
      PyErr_Print();
      interp->quit();
      return false;
    }
  Py_DECREF(res);
  interp->quit();
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

  PyObject *res,*pypop;
  interp->enter();
  pypop=sipMapCppToSelf( popup,
			sipClass_QPopupMenu);

  res=PyObject_CallMethod(module,"customPopup","Osss",pypop,
			  theContext.latin1(), theObject.latin1(), theParent.latin1());
  Py_DECREF(pypop);
  if (res == NULL)
    {
      PyErr_Print();
      interp->quit();
      return false;
    }

  Py_DECREF(res);
  interp->quit();


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
  
  PyObject *res;
  interp->enter();
  res=PyObject_CallMethod(module,"definePopup","sss",
			  theContext.latin1(), theObject.latin1(), theParent.latin1());
  if (res == NULL)
    {
      PyErr_Print();
      interp->quit();
      return ;
    }
  char *co, *ob, *pa;
  int parseOk = PyArg_ParseTuple(res, "sss", &co, &ob, &pa);

  MESSAGE ("parseOk " << parseOk);
  MESSAGE (" --- " << co << " " << ob << " " << pa);

  theContext = co;
  theObject = ob;
  theParent = pa;
  Py_DECREF(res);
  interp->quit();

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

void SALOME_PYQT_GUI::ActiveStudyChanged( QAD_Desktop* parent )
{
  MESSAGE("SALOME_PYQT_GUI::ActiveStudyChanged");
  PyObject *res;
  
  int StudyID = parent->getActiveApp()->getActiveStudy()->getStudyId();
  initInterp(StudyID);
  
  interp->enter();
  res=PyObject_CallMethod(module,"activeStudyChanged","i", StudyID );
  if (res == NULL)
    {
      PyErr_Print();
      interp->quit();
      return ;
    }
  Py_DECREF(res);
  interp->quit();
  return;
}


//=============================================================================
/*!
 *  
 */
//=============================================================================


extern "C"
{
  bool OnGUIEvent(int theCommandID, QAD_Desktop* parent)
  {
    return SALOME_PYQT_GUI::OnGUIEvent(theCommandID, parent);
  }
  
  bool OnKeyPress(QKeyEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
  {
    return SALOME_PYQT_GUI::OnKeyPress(pe, parent, studyFrame);
  }
  
  bool OnMousePress(QMouseEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
  {
    return SALOME_PYQT_GUI::OnMousePress(pe, parent, studyFrame);
  }
  
  bool OnMouseMove(QMouseEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
  {
    return SALOME_PYQT_GUI::OnMouseMove(pe, parent, studyFrame);
  }
  
  bool SetSettings(QAD_Desktop* parent, char* moduleName)
  {
    return SALOME_PYQT_GUI::SetSettings(parent, moduleName);
  }
  
  bool customPopup(QAD_Desktop* parent, QPopupMenu* popup, const QString & theContext,
		   const QString & theParent, const QString & theObject)
  {
    return SALOME_PYQT_GUI::CustomPopup( parent, popup, theContext, theParent, theObject );
  }
  
  void definePopup(QString & theContext, QString & theParent, QString & theObject)
  {
    SALOME_PYQT_GUI::DefinePopup(theContext, theParent, theObject);
  }
  
  void activeStudyChanged ( QAD_Desktop* parent )
  {
    SALOME_PYQT_GUI::ActiveStudyChanged(parent);
  }
}

