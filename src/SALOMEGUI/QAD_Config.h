//  File      : QAD_Config.h
//  Created   : Tue Sep 04 09:54:37 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2001
//  $Header$

#ifndef QAD_CONFIG_H
#define QAD_CONFIG_H

// Used for access to the configuration file e.g.: QAD_CONFIG->getSetting("Language:Language")
#define QAD_CONFIG QAD_Config::getConfig()

#include <qdir.h>
#include <qfont.h>

#include "QAD_Settings.h"

class QAD_Config : public QAD_Settings
{

protected:
  QAD_Config();

  void ini();

public:
  ~QAD_Config();

  static QAD_Config* getConfig();

  /** Sets directory of the local configuration file. */
  void           setConfigDir(QDir _configDir) { configDir=_configDir; }
  /** Gets directory of the local configuration file. */
  QDir           getConfigDir() const { return configDir; }

  /** Sets directory of SALOME_ROOT. */
  void           setPrgDir(QDir _prgDir) { prgDir=_prgDir; }
  /** Gets directory of SALOME_ROOT. */
  QDir           getPrgDir() const { return prgDir; }

  bool           createConfigFile( bool overwrite=false );
  bool           readConfigFile();

private:
  static QAD_Config* theOneAndOnlyConfig;

  QDir   prgDir;
  QDir   configDir;
};


#endif

// EOF
