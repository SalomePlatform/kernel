//  File      : QAD_Settings.h
//  Created   : Tue Sep 04 09:30:33 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_SETTINGS_H
#define QAD_SETTINGS_H

#include <qlist.h>
#include <qstring.h>
#include <qobject.h>

/*!
   Class Setting wraps a single setting with a name and a value.
*/
class QAD_Setting
{

public:
  /** Constructor. */
  QAD_Setting() { autoValue=true; }
  QAD_Setting( QString _name, QString _value, bool _autoValue=true )
    { name=_name; value=_value; autoValue=_autoValue; }

  ~QAD_Setting() {}
  
  /** Sets the name of this setting. */
  void setName(QString _name)
    { name=_name; }

  /** Gets the name of this setting. */
  QString getName()
    { return name; }

  /** Sets the value of this setting. */
  void setValue(QString _value)
    { value=_value; }

  /** Gets the value of the setting. */
  QString getValue()
    { return value; }

  /** Sets the auto value flag. */
  void setAutoValue(bool _av)
    { autoValue=_av; }

  /** Gets the auto value flag. */
  bool isAutoValue()
    { return autoValue; }
  
private:
  QString name;
  QString value;
  bool autoValue;
};



class QAD_Settings : public QObject
{
  Q_OBJECT

public:
  /** Constructor. */
  QAD_Settings() { settingList.setAutoDelete(true); }
  QAD_Settings(QString _sectionName,
	       QChar _nameStarter, QChar _nameStopper,
	       QChar _valueStarter, QChar _valueStopper);
  QAD_Settings(QString _sectionName,
	       QChar _nameStarter, QChar _separator,
	       QChar _valueStopper);
  QAD_Settings(QChar _nameStopper, QChar _valueStarter,
	       QChar _valueStopper);
  virtual ~QAD_Settings();

  /** Returns the list of settings as a QList. */
  QList<QAD_Setting>* getSettingList() { return &settingList; }

  /** Sets the name of the section where this settings are saved. */
  virtual void    setSectionName(QString name) { sectionName=name; }

  virtual void    clear();
  virtual void    addSetting(QString _name, QString _value, bool _autoValue=true);
  virtual void    addSetting(QString _name, int _value, bool _autoValue=true);
  virtual void    addSetting(QString _name, double _value, bool _autoValue=true);
  virtual bool    removeSettings(QString name);
  virtual void    removeNonAutoSettings();
  virtual QString getSetting(QString name);
  virtual QString replace(QString str);
  virtual bool    hasSetting(QString name);

protected:
  /** The list which contains all stored settings. */
  QList<QAD_Setting> settingList;

private:
  QString sectionName;
  QChar nameStarter;
  QChar nameStopper;
  QChar valueStarter;
  QChar valueStopper;

};

#endif





