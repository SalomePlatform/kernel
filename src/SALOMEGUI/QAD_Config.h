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
//  File   : QAD_Config.h
//  Author : Nicolas REJNERI
//  Module : SALOME
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

  /** Sets directory of KERNEL_ROOT. */
  void           setPrgDir(QDir _prgDir) { prgDir=_prgDir; }
  /** Gets directory of KERNEL_ROOT. */
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
