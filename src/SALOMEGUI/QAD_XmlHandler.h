//  File      : QAD_XmlHandler.h
//  Created   : Thu Jun 14 14:02:23 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#include <qxml.h>
#include <qstringlist.h>
#include <qaction.h>
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
