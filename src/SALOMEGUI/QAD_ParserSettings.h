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
//  File   : QAD_ParserSettings.h
//  Author : Nicolas REJNERI
//  Module : SALOME
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
