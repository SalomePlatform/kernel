//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
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
//  File   : SALOMEGUI_QtCatchCorbaException.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#include "SALOMEGUI_QtCatchCorbaException.hxx"
#include "utilities.h"

#include "QAD_Desktop.h"
#include "QAD_MessageBox.h"
using namespace std;

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
