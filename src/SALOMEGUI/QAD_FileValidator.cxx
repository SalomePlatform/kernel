//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : QAD_FileValidator.cxx
//  Author : 
//  Module : SALOME
//  $Header$

#include "QAD_FileValidator.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_MessageBox.h"

#include "utilities.h"

#include <qfile.h>
#include <qfileinfo.h>

using namespace std;

/* constructor */
QAD_FileValidator::QAD_FileValidator(QWidget* parent) :
myParent(parent) 
{
  if (!myParent) {
    myParent = QAD_Application::getDesktop();
  }
}
  
/* returns false if can't open file */
bool QAD_FileValidator::canOpen( const QString& file ) 
{
MESSAGE ( "QAD_FileValidator::canOpen" )
  if ( !QFile::exists( file ) ) {
    QAD_MessageBox::error1( myParent,
			    QObject::tr( "ERR_ERROR" ),
			    QObject::tr( "ERR_DOC_FILENOTEXIST" ).arg( file ),
			    QObject::tr( "BUT_OK" ) );
      return false;
    }
  if ( !QFileInfo( file ).isReadable() ) {
    QAD_MessageBox::error1( myParent,
			    QObject::tr( "ERR_ERROR" ),
			    QObject::tr( "ERR_PERMISSION_DENIED" ).arg( file ),
			    QObject::tr( "BUT_OK" ) );
    return false; 
  }
  return true;
}

/* returns false if can't save file */
bool QAD_FileValidator::canSave( const QString& file ) 
{
MESSAGE ( "QAD_FileValidator::canSave" )
  if ( QFile::exists( file ) ) {
    // if file exists - raise warning...
    if ( QAD_MessageBox::warn2( myParent,
			        QObject::tr( "WRN_WARNING" ),
			        QObject::tr( "QUE_DOC_FILEEXISTS" ).arg( file ),
			        QObject::tr( "BUT_YES" ), 
			        QObject::tr( "BUT_NO" ),
			        QAD_YES, 
			        QAD_NO, 
			        QAD_NO ) == QAD_NO ) {
      return false;
    }
    // ... and if user wants to overwrite file, check it for writeability
    if ( !QFileInfo( file ).isWritable() ) {
      QAD_MessageBox::error1( myParent,
			      QObject::tr( "ERR_ERROR" ),
			      QObject::tr( "ERR_PERMISSION_DENIED" ).arg( file ),
			      QObject::tr( "BUT_OK" ) );
      return false; 
    }
  }
  else {
    // if file doesn't exist - try to create it
    QFile qf( file );
    if ( !qf.open( IO_WriteOnly ) ) {
      QAD_MessageBox::error1( myParent,
			      QObject::tr( "ERR_ERROR" ),
			      QObject::tr( "ERR_PERMISSION_DENIED" ).arg( file ),
			      QObject::tr( "BUT_OK" ) );
      return false;
    }
    else {
      // remove just created file
      qf.close();
      qf.remove();
    }
  }
  return true;
}

