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
//  File   : QAD_XmlHandler.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include <qxml.h>
#include <qstringlist.h>
#include <qlist.h>
#include <qstack.h>
#include <map>

#include <TColStd_SequenceOfInteger.hxx>

class QAD_Desktop;
class QAD_Menus;

class QAD_XmlHandler : public QXmlDefaultHandler
{
public:
  QAD_XmlHandler();
  virtual ~QAD_XmlHandler();

  bool startDocument();
  bool startElement( const QString& namespaceURI, const QString& localName, 
		     const QString& qName, const QXmlAttributes& atts );
  bool endElement( const QString& namespaceURI, const QString& localName, const QString& qName );
  bool characters( const QString& ch );
  
  QString errorString();
  QString errorProtocol();
  bool fatalError   (const QXmlParseException& exception);
  QString givebib (const int mykey);
  
  void setMainWindow(QAD_Desktop*);
  bool setComponent (const QString& aComponent);
  
  QList<QAD_Menus>            myMenusList;
  TColStd_SequenceOfInteger   myIdList;
  QAD_Menus*                  myMenus;

  static map<int,QString>	      _bibmap;

private:
  QString            myErrorProt;
  QString            myPathResources;

  bool               myBackMenu;
  QStack<QPopupMenu> myBackPopupMenus;

  QAD_Desktop* myDesktop;
};
