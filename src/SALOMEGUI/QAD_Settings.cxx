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
//  File   : QAD_Settings.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "QAD_Settings.h"

#include <stdio.h>

// QT Includes
#include <qfile.h>
#include <qregexp.h>
#include <qstring.h>
#include <qtextstream.h>
using namespace std;

/*!
    Class QAD_Settings is a base class for different kind of settings
    (identifying strings and values). 
*/
QAD_Settings::QAD_Settings(QString _sectionName,
			   QChar _nameStarter, QChar _nameStopper,
			   QChar _valueStarter, QChar _valueStopper)
{
  sectionName  = _sectionName;
  nameStarter  = _nameStarter;
  nameStopper  = _nameStopper;
  valueStarter = _valueStarter;
  valueStopper = _valueStopper;
  settingList.setAutoDelete(true);
}

/*!
    Constructor.
*/
QAD_Settings::QAD_Settings(QString _sectionName,
			   QChar _nameStarter,
			   QChar _separator,
			   QChar _valueStopper)
{
  sectionName  = _sectionName;
  nameStarter  = _nameStarter;
  nameStopper  = _separator;
  valueStopper = _valueStopper;
  valueStarter=0;
  settingList.setAutoDelete(true);
}
/*!
    Constructor.
*/
QAD_Settings::QAD_Settings(QChar _nameStopper, QChar _valueStarter,
			   QChar _valueStopper)
{
  sectionName = "";
  nameStarter = 0;
  nameStopper  = _nameStopper;
  valueStarter = _valueStarter;
  valueStopper = _valueStopper;
  settingList.setAutoDelete(true);
}


/*!
    Destructor.
*/
QAD_Settings::~QAD_Settings()
{
}


/*!
    Clears all settings.
*/
void QAD_Settings::clear()
{
  settingList.clear();
}


/*!
    Adds a setting. If a setting with the given name is already defined it
    gets overwritten.
*/
void QAD_Settings::addSetting(QString _name, QString _value, bool _autoValue)
{
  // Remove all settings with the same name:
  removeSettings(_name.simplifyWhiteSpace());

  // Add new setting:
  QAD_Setting* newSetting = new QAD_Setting(_name.simplifyWhiteSpace(),
					    _value.simplifyWhiteSpace(),
					    _autoValue);
  settingList.append(newSetting);
}


/*!
    Adds an int setting. If a setting with the given name is already defined it
    gets overwritten.
*/
void QAD_Settings::addSetting(QString _name, int _value, bool _autoValue)
{
  QString strVal;
  strVal.setNum(_value);

  addSetting(_name, strVal, _autoValue);
}

/*!
    Adds a double setting. If a setting with the given name is already defined it
    gets overwritten.
*/
void QAD_Settings::addSetting(QString _name, double _value, bool _autoValue)
{
  QString strVal;
  strVal.setNum(_value);

  addSetting(_name, strVal, _autoValue);
}


/*!
    Removes all setting with the given name (should be unique anyway).
*/
bool QAD_Settings::removeSettings(QString name)
{
  QAD_Setting* s;
  bool ret=false;

  for(s=settingList.first(); s!=0;) {
    if(s->getName()==name) {
      settingList.remove(s);
      s=settingList.current();
      ret=true;
    }
    else {
      s=settingList.next();
    }
  }

  return ret;
}


/*!
    Removes all setting which are not automatically generated.
*/
void QAD_Settings::removeNonAutoSettings()
{
  QAD_Setting* s;

  for(s=settingList.first(); s!=0;) {
    if(!s->isAutoValue()) {
      settingList.remove(s);
      s=settingList.current();
    }
    else {
      s=settingList.next();
    }
  }
}


/*!
   Gets the setting value of the given setting.
*/
QString QAD_Settings::getSetting(QString name)
{
  QAD_Setting* s;
  
  for(s=settingList.first(); s!=0; s=settingList.next()) {
    if(s->getName()==name) return s->getValue();
  }

  return "";
}

/*!
    Replaces variables in the given string by their values.
*/
QString QAD_Settings::replace(QString str)
{
  if(!str.contains('$')) return str;

  QAD_Setting* t;
  QString result=str.copy();
  int i;
  bool found;

  // Replace until we have no occurences of variables or recursive counter>16:
  do {
    found=false;
    for(t=settingList.first(); t!=0; t=settingList.next()) {
      if(result.contains('$')) {
        i=0;
        do {
          i = result.find(QRegExp("\\$" + t->getName() + "[^a-zA-z0-9_]"), i);
          if(i!=-1) {
            result.replace(i, (int)t->getName().length()+1, t->getValue());
            found=true;
          }
        }while(i!=-1);
      }
      else goto end;
    }
  } while(found);

  end:

  return result;
}

/*!
  Returns true if list contains given setting
*/
bool QAD_Settings::hasSetting(QString name)
{
  QAD_Setting* s;
  
  for(s=settingList.first(); s!=0; s=settingList.next()) {
    if(s->getName()==name) return true;
  }
  return false;
}

// EOF

