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
//  File   : QAD_ParserSettings.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "QAD_ParserSettings.h"

#include <stdio.h>

// QT Includes
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
using namespace std;


/*!
    Constructor.
*/
QAD_ParserSettings::QAD_ParserSettings()
{
}


/*!
    Destructor.
*/
QAD_ParserSettings::~QAD_ParserSettings()
{
}


/*!
    Gets the contents of a file.
*/
QString QAD_ParserSettings::getContents(QString fileName, bool comments,
					bool sections, bool whiteSpace,
					bool htmlComments)
{
  QFileInfo fi(fileName);
  QFile f(fi.absFilePath());
  QString s="";                   // Buffer for the whole file

  if(f.open(IO_ReadOnly)) {       // file opened successfully
    QTextStream t(&f);            // use a text stream

    s = t.read();                 // The whole file in a string
    f.close();

    if(!comments)     s = QAD_ParserSettings::removeComments(s);
    if(!sections)     s = QAD_ParserSettings::removeSections(s);
    if(!whiteSpace)   s = s.simplifyWhiteSpace();
    if(!htmlComments) s = QAD_ParserSettings::removeHtmlComments(s);
  }
  else {
    // Can't open file
  }

  return s;
}


/*!
    Gets the body of a section from a string.
*/
QString QAD_ParserSettings::getSection(QString s, QString sectionName)
{
  QString result="";

  QChar ch;                     // A single byte of the file
  int bracketCounter=1;         // Bracket counter (increase on '{' and decrese on '}'.
  int i;                        // Current index
  int l=0;                      // Length of current part we must have

  if((i=s.find(sectionName, 0, false)) >= 0 &&       // Jump to section [styles]
     (i=s.find('{', i))                >= 0    ) {

    ++i;

    while(i+l<(int)s.length()) {
      ch = s.at(i+l);           // Single byte

      ++l;

      if(ch=='{') ++bracketCounter;
      if(ch=='}') --bracketCounter;

      if(bracketCounter==0) break;
    }

    result = s.mid(i, l-1);
  }

  return result;
}



/*!
    Gets the HTML comments out of a file.
*/
QString QAD_ParserSettings::getHtmlComment(QString s)
{
  QString result="";
  int length;              // length of the sub string
  int ind=0;

  if((ind =s.find("<!--", ind))>=0 &&
     (ind+=4) &&
     (length=s.find("-->",  ind)-ind)!=0) {

    result=s.mid(ind, length-3);
  }

  return result;
}



/*! 
    Gets the next String between two given characters. The index-parameter gets moved to the character after
    the stopper or to -1 if the starter / stopper were not found.
*/
QString QAD_ParserSettings::getNextStringBetween(QString s, int& startIndex, 
						 QChar starter, QChar stopper)
{
  QString result="";
  int length=0;              // length of the sub string

  if((startIndex =s.find(starter, startIndex))>=0 &&
     (length=s.find(stopper, startIndex+1)-startIndex)!=0) {

    result=s.mid(startIndex+1, length-1);
    startIndex+=2;
  }

  return result;
}


/*!
    Gets the next String between the given index and a given stopper character. The index-parameter gets moved to
    the character after the stopper or to -1 if the stopper was not found.
*/
QString QAD_ParserSettings::getNextStringUntil(QString s, int& startIndex, QChar stopper)
{
  QString result="";
  int length;              // length of the sub string

  if((length=s.find(stopper, startIndex)-startIndex)!=0) {
    result=s.mid(startIndex, length);
    startIndex++;
  }

  return result;
}


/*!
    Removes all comments (between '/ *' and '* /').
*/
QString QAD_ParserSettings::removeComments(QString s)
{
  QString result="";
  int i1=0, i2;

  while((i2=s.find("/*", i1))>=0) {
    result += s.mid(i1, i2-i1);

    i1+=2;
    i1=s.find("*/", i1);
    i1+=2;
  }

  result += s.mid(i1, s.length()-i1);

  return result;
}



/*!
    Removes all HTML comments (between '<!--' and '-->').
*/
QString QAD_ParserSettings::removeHtmlComments(QString s)
{
  QString result="";
  int i1=0, i2;

  while((i2=s.find("<!--", i1))>=0) {
    result += s.mid(i1, i2-i1);

    i1+=4;
    i1=s.find("-->", i1);
    i1+=3;
  }

  result += s.mid(i1, s.length()-i1);

  return result;
}



/*!
    Removes all sections ('[section] { }').
*/
QString QAD_ParserSettings::removeSections(QString s)
{
  QString result="";

  QChar ch;                     // A single byte of the file
  int bracketCounter;           // Bracket counter (increase on '{' and decrese on '}'.
  int i=0;                      // Current index

  while(i<(int)s.length()) {
    ch = s.at(i);           // Single byte

    if(ch=='[') {
      bracketCounter=1;
      while(i<(int)s.length() && ch!=']') { ch = s.at(i); ++i; }
      ++i;
      while(i<(int)s.length() && ch!='{') { ch = s.at(i); ++i; }
      ++i;

      while(i<(int)s.length() && bracketCounter!=0) {
        ch = s.at(i);
        if(ch=='{') ++bracketCounter;
        if(ch=='}') --bracketCounter;
        ++i;
      }
      ++i;
    }
    else {
      result+=ch;
    }

    ++i;
  }

  return result;
}


/*!
    Format plain text into HTML-code with a given maximal width.
    Spaces get replaced with non breaking spaces. Tabulators get filled up
    with non breaking spaces.
*/
QString QAD_ParserSettings::plainTextToHtml(QString s, int autoBreak)
{
  QString result="\n";

  if(!s.isEmpty()) {
    int col=1, i;

    for(i=0; i<(int)s.length(); ++i) {
      // Line feed:
      //
      if(s[i]=='\n') {
        result+="<BR>\n";
        col=1;
      }

      // Auto break:
      //
      else if(col==autoBreak && autoBreak!=0) {
        result+="<BR>\n";
        result+=s[i];
        col=1;
      }

      // Tab:
      //
      else if(s[i]=='\t') {
        while(col%8!=0) { result+="&nbsp;"; ++col; }
        result+="&nbsp;";
        ++col;
      }

      // Space:
      //
      else if(s[i]==' ') {
        result+="&nbsp;";
        ++col;
      }

      // Normal char / special code:
      //
      else {
        if(s[i].isLetter() || s[i].isNumber()) {
          result+=s[i];
        }
        else {
          result+=charToHtml(s[i]);
        }
        ++col;
      }
    }

    result+="\n";
  }

  return result;
}


/*!
    Converts a special character to html code (e.g.: '»' to "&#187;")
*/
QString QAD_ParserSettings::charToHtml(QChar c)
{
  QString s;
  QString uc;
  uc.setNum(c.unicode());
  s = "&#" + uc + ";";
  return s;
}


// EOF
