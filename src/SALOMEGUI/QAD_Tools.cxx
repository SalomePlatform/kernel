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
//  File   : QAD_Tools.cxx
//  Author : UI team
//  Module : SALOME
//  $Header$

/*!
  \class QAD_Tools QAD_Tools.h
  \brief Helpful functions for QAD.
*/

#include "QAD_Tools.h"
#include "utilities.h"

// QT Inlcude
#include <qwidget.h>
#include <qfileinfo.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <qstringlist.h>
using namespace std;

static char* tempName()
{
  static char tempDir[512];
  strcpy ( tempDir, getenv ("tmp") );

#if defined WNT
  if ( tempDir[ strlen(tempDir)-1 ] != '\\' )
    strcat ( tempDir,"\\");
#else
  if ( tempDir[ strlen(tempDir)-1 ] != '/' )
    strcat ( tempDir,"/");
#endif        
  strcat ( tempDir,"Salome_trace");        
  return tempDir;
}

/*!
    Returns max 'int' value [ static ]
*/
inline int QAD_Tools::getMax(int v1, int v2)
{ 
  return v1 >= v2 ? v1 : v2; 
}

/*!
    Returns min 'int' value [ static ]
*/
inline int QAD_Tools::getMin(int v1, int v2)
{ 
  return v1 <= v2 ? v1 : v2;
}

/*! 
    [ static ]
    Creates a rect with TopLeft = ( min(x1,x2), min(y1,y2) )
    and BottomRight = ( TopLeft + (x2-x1)(y2-y1) )    
*/	
QRect QAD_Tools::makeRect(int x1, int y1, int x2, int y2)
{  
  QRect aRect;
  aRect.setRect( getMin(x1, x2), getMin(y1, y2), abs(x2-x1), abs(y2-y1));
  return aRect;
}

/*!
    Traces output to log-file. [ static ] 
    If log is NULL, 'Salome_trace' file is created in temp directory.
    Log file is written in 'append' mode.
*/
void  QAD_Tools::trace (const char* lpszLog, const char* lpszFormat, ... )
{
  static  char* lpszTmp = tempName();

  FILE*	pStream;
  pStream = fopen ( lpszLog ? lpszLog : lpszTmp, "a" );
  if ( pStream ) 
    {	
      va_list argptr;
      va_start(argptr, lpszFormat);				
      //NRI      vfprintf ( pStream, lpszFormat, argptr);	
      fprintf ( pStream, "- Trace %s [%d] : %s", __FILE__, __LINE__, lpszFormat);	
      va_end(argptr);	
      fclose ( pStream );
    }
}

/*!
    Parses the path to select the dir name only [ static ].
    Returns NULL if the path is empty (null).
    NB: Unix-style slashes are assumed in 'path'
*/
QString QAD_Tools::getDirFromPath ( const QString& path, bool absPath )
{
  return QFileInfo( path ).dirPath( absPath );
}


/*!
    Parses the path to select the file name with or without extension [ static ].
*/
QString QAD_Tools::getFileNameFromPath( const QString& path, 
                                        bool withExtension )
{
  if ( withExtension )
    return QFileInfo( path ).fileName();    
  else 
    return QFileInfo( path ).baseName();
}

/*!
    Parses the path to select the file extension [ static ].
*/
QString QAD_Tools::getFileExtensionFromPath( const QString& path )
{    
  return QFileInfo( path ).extension(false);
}

/*!
    Adds a slash to the end of 'path' if it is not already there [ static ]
*/
QString QAD_Tools::addSlash( const QString& path )
{
  if (!path.isNull()) {
#ifdef WNT
    QChar slash ('\\');
#else
    QChar slash ('/');
#endif
    if ( path.at(path.length()-1) != slash )
      return path + slash;
  }
  return path;
}

/*!
    Converts slashes from unix-like to win-like [ static ]
    Returns null string if 'path' is empty or null. 
*/
QString QAD_Tools::unix2win( const QString& path ) 
{
  QString ret = path;
#ifdef WNT
  int pos;
  QChar winSlash ('\\'), unixSlash('/');    
  for ( int start = 0; 
       (pos = path.find( unixSlash, start, false )) >= 0; 
       start = pos + 1 ) 
    {        
      ret.replace( pos, 1, &winSlash, 1 );
    }
#endif
  return ret;
}
/*!
    Centers widget 'w' as refered to widget 'ref' [ static ]
*/
void QAD_Tools::centerWidget( QWidget* src, const QWidget* ref )
{
  QAD_Tools::alignWidget(src, ref, Qt::AlignCenter);
}

/*!
  Aligns widget 'w' as refered to widget 'ref' [ static ] 
*/
void QAD_Tools::alignWidget ( QWidget* src, const QWidget* ref, int alignFlags )
{
  if ( !src || !ref || !alignFlags ) return;

  QPoint srcOri = src->mapToGlobal( QPoint( 1, 1 ) );
  QPoint refOri = ref->mapToGlobal( QPoint( 1, 1 ) );
  
  int x = srcOri.x(), y = srcOri.y();
  int refWidth = ref->width(), refHei = ref->height();
  int srcWidth = src->width(), srcHei = src->height();       
  if ( srcWidth <= 1 ) 
    srcWidth = src->sizeHint().width();
  if ( srcHei <= 1 ) 
    srcHei = src->sizeHint().height();
  
  if ( alignFlags & AlignLeft )         
    x = refOri.x();
  if ( alignFlags & AlignRight )
    x = refOri.x() + refWidth - srcWidth;    
  if ( alignFlags & AlignTop )
    y = refOri.y();
  if ( alignFlags & AlignBottom )
    y = refOri.y() + refHei - srcHei;
  if ( alignFlags & AlignHCenter ) 
    x = refOri.x() + ( refWidth - srcWidth ) / 2;    
  if ( alignFlags & AlignVCenter ) 
    y = refOri.y() + ( refHei - srcHei ) / 2;
  
  if ( src->parentWidget() &&        /* we move a widget inside its parent */
      !src->inherits( "QDialog" ))   /* dialogs use global coordinates  */
    {
      QPoint pos = src->parentWidget()->mapFromGlobal( QPoint(x,y) ); 
      x = pos.x(); y = pos.y();
    }
#ifdef WNT
  x -= 4;                             /* - frame border width ( approx. ) */
  y -= 30;                            /* - caption height ( approx. ) */
#endif

  src->move( x, y );
}

/*!
  Converts TCollection_ExtendedString to QString
*/
QString QAD_Tools::toQString( const TCollection_ExtendedString& extString)
{
  QString result = QString ( (const QChar*) extString.ToExtString(),
			                    extString.Length() );
  return result;
}

/*!
  Converts QString to TCollection_ExtendedString
*/
TCollection_ExtendedString QAD_Tools::toExtString( const QString& qString)
{
  TCollection_ExtendedString result;
  for ( int i = 0; i < (int)qString.length(); i++ )
    result.Insert( i + 1, qString[ i ].unicode() );
  return result;
}

/*!
  Converts TCollection_AsciiString to QString
*/
QString QAD_Tools::toQString( const TCollection_AsciiString& asciiString)
{
  QString result = QString ( asciiString.ToCString() );
  return result;
}

/*!
  Converts QString to TCollection_AsciiString
*/
TCollection_AsciiString QAD_Tools::toAsciiString( const QString& qString)
{
  TCollection_AsciiString result = TCollection_AsciiString((char*)(qString.latin1()));
  return result;
}

/*!
  Creates font from string description
*/
QFont QAD_Tools::stringToFont( const QString& fontDescription )
{
  QFont font;
  if ( fontDescription.stripWhiteSpace().isEmpty() || !font.fromString( fontDescription ) )
    font = QFont( "Courier", 11 );
  return font;
}

/*!
  Creates font's string description
*/
QString QAD_Tools::fontToString( const QFont& font )
{
  return font.toString();
}

/*!
  Checks popup menu recursively for unnecessary separators and removes them
*/
void QAD_Tools::checkPopup( QPopupMenu* popup )
{
  if ( popup->count() > 0 ) {
    QValueList<int> idRemove;
    for ( int i = 1; i < popup->count(); i++ ) {
      if ( popup->findItem( popup->idAt( i ) )->isSeparator() ) {
	if ( popup->findItem( popup->idAt( i-1 ) )->isSeparator() )
	  idRemove.append( popup->idAt( i ) );
      }
      else { 
	QPopupMenu* child = popup->findItem( popup->idAt( i ) )->popup();
	if ( child ) {
	  checkPopup( child );
	}
      }
    }
    for ( int i = 0; i < idRemove.count(); i++ )
      popup->removeItem( idRemove[i] );
    if ( popup->count() > 0 && popup->findItem( popup->idAt( 0 ) )->isSeparator() )
      popup->removeItem( popup->idAt( 0 ) );
    if ( popup->count() > 0 && popup->findItem( popup->idAt( popup->count()-1 ) )->isSeparator() )
      popup->removeItem( popup->idAt( popup->count()-1 ) );
  }
}
