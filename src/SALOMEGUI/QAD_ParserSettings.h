//  File      : QAD_ParserSettings.h
//  Created   : Sat Sep 08 21:58:39 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_PARSERSETTINGS_H
#define QAD_PARSERSETTINGS_H

#include <qstring.h>

class QAD_ParserSettings
{
public:
  QAD_ParserSettings();
  ~QAD_ParserSettings();

  static QString getContents(QString fileName, bool comments=true, 
			     bool sections=true, bool whitespace=true, 
			     bool htmlComments=true);
  static QString getSection(QString s, QString sectionName);
  static QString getHtmlComment(QString s);
  static QString getNextStringBetween(QString s, int& startIndex, QChar starter, QChar stopper);
  static QString getNextStringUntil(QString s, int& startIndex, QChar stopper);
  static QString removeComments(QString s);
  static QString removeHtmlComments(QString s);
  static QString removeSections(QString s);
  static QString plainTextToHtml(QString s, int autoBreak);
  static QString charToHtml(QChar c);

};

#endif
