using namespace std;
//=============================================================================
// File      : SALOMEGUI_QtCatchCorbaException.cxx
// Created   : mer déc 19 14:52:07 CET 2001
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

#include "SALOMEGUI_QtCatchCorbaException.hxx"
#include "utilities.h"

#include "QAD_Desktop.h"
#include "QAD_MessageBox.h"

void QtCatchCorbaException(const SALOME::SALOME_Exception & S_ex)
{
  switch (S_ex.details.type)
    {
    case SALOME::COMM:
    case SALOME::INTERNAL_ERROR:
      {
	QString str(S_ex.details.text);
	QString source(S_ex.details.sourceFile);
	QString line;
	line.setNum(S_ex.details.lineNumber);
	QString message = str + " \n" + source + " : " + line;
	QAD_MessageBox::error1 
	  ( QAD_Application::getDesktop(),
	    QObject::tr ("Engine Error"),
	    message,
	    QObject::tr ("OK") );		    
	break;
      }

    case SALOME::BAD_PARAM:
      {
	QString message(S_ex.details.text);
#ifdef _DEBUG_
	QString source(S_ex.details.sourceFile);
	QString line;
	line.setNum(S_ex.details.lineNumber);
	message = message + " \n" + source + " : " + line;
#endif
	QAD_MessageBox::warn1 
	  ( QAD_Application::getDesktop(),
	    QObject::tr ("Engine Warning"),
	    message,
	    QObject::tr ("OK") );		    
	break;
      }
    default:
      {
	QString str("SALOME CORBA Exception Type invalid");
	QString source(S_ex.details.sourceFile);
	QString line;
	line.setNum(S_ex.details.lineNumber);
	QString message = str + " \n" + source + " : " + line;
	QAD_MessageBox::error1 
	  ( QAD_Application::getDesktop(),
	    QObject::tr ("Internal SALOME Error"),
	    message,
	    QObject::tr ("OK") );		    
	break;
      }
    }
}
