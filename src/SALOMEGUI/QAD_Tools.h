//  File      : QAD_Tools.h
//  Created   : 22.09.00
//  Descr     : Helpful functions for QAD

//  Author    : UI team
//  Project   : SALOME
//  Module    : SALOMEGUI 
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_TOOLS_H
#define QAD_TOOLS_H

#include "QAD.h"
#include <qrect.h>
#include <qstring.h>
#include <qfont.h>
#include <qpopupmenu.h>
#include <TCollection_ExtendedString.hxx>
#include <TCollection_AsciiString.hxx>

class QAD_EXPORT QAD_Tools : public Qt
{
public:
  /* min/max functions */
  static int                        getMax(int v1, int v2);
  static int                        getMin(int v1, int v2);

  /* makes rectangle */
  static QRect                      makeRect(int x1, int y1, int x2, int y2); 

  /* trace function */
  static void                       trace (const char* pLog, const char* szFormat, ... );
  
  /* path functions */
  static QString                    getDirFromPath( const QString& path, bool absPath = true );
  static QString                    getFileNameFromPath( const QString& path, bool withExtension = true );
  static QString                    getFileExtensionFromPath( const QString& path );
  static QString                    addSlash( const QString& path ); 
  static QString                    unix2win( const QString& path ); 
  
  /* aligning widget in 'ref' widget */
  static void                       centerWidget( QWidget* src, const QWidget* ref ); 
  static void                       alignWidget ( QWidget* src, const QWidget* ref, int alignFlags = AlignCenter );     

  /* converting strings */
  static QString                    toQString    ( const TCollection_ExtendedString& extString);
  static TCollection_ExtendedString toExtString  ( const QString& qString);
  static QString                    toQString    ( const TCollection_AsciiString& asciiString);
  static TCollection_AsciiString    toAsciiString( const QString& qString);

  static QFont                      stringToFont( const QString& fontDescription );
  static QString                    fontToString( const QFont& font );

  /* popup menu optimization */
  static void                       checkPopup( QPopupMenu* popup );
};

#endif



