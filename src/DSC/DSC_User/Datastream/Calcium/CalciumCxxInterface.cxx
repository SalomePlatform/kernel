//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : CalciumCxxInterface.cxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 13:27:58 +0100 (jeu, 01 mar 2007) $
// Id          : $Id$
//

#include "CalciumCxxInterface.hxx"
#define PRG_MAIN
#include "calciumP.h"
#include <sys/time.h>
#include <fstream>
#include <sys/stat.h>

using namespace std; 

namespace CalciumInterface 
{
  
  int traceType=-1; // 0=stderr;1=file;
  int traceLevel=-1; // 0=no trace;1=normal trace;2=detailed trace
  std::ofstream traceFile;
  std::ostream *out;

  void initTrace(const std::string& containerName)
    {
      // if initialization has already been done do nothing
      if(traceLevel >= 0)return;

      std::string typeenv="0";
      std::string levelenv="1";
      char* valenv=0;
      valenv=getenv("DSC_TRACE");
      if(valenv)typeenv=valenv;
      valenv=getenv("DSC_TRACELEVEL");
      if(valenv)levelenv=valenv;

      if(levelenv=="0")
        traceLevel=0; // no trace 
      else if(levelenv=="2")
        traceLevel=2; //detailed trace
      else 
        traceLevel=1; // normal trace (default)

      if(traceLevel==0)
	return;

      if(typeenv=="1")
        {
          //trace in file
          traceType=1;
#ifdef WNT
          std::string logFilename=getenv("TEMP");
          logFilename += "\\";
#else
          std::string logFilename="/tmp";
          char* val = getenv("SALOME_TMP_DIR");
          if(val)
            {
              struct stat file_info;
              stat(val, &file_info);
              bool is_dir = S_ISDIR(file_info.st_mode);
              if (is_dir)logFilename=val;
            }
          logFilename += "/";
#endif

          logFilename=logFilename+containerName+".tce";
          traceFile.open(logFilename.c_str(), std::ios::out | std::ios::app);
          out=&traceFile;
        }
      else
        {
          //trace to stderr (default)
          traceType=0;
          out=&std::cerr;
        }
      //trace heading
      out->width(17);
      *out << "Elapsed time" ;
      *out << " | " ;
      out->width(16);
      *out << "Request" ;
      *out << " | " ;
      out->width(16);
      *out << "Container" ;
      *out << " | " ;
      out->width(16);
      *out << "Instance" ;
      *out << " | " ;
      out->width(16);
      *out << "Port" ;
      *out << " | " ;
      out->width(24);
      *out << "Error";
      *out << " | " ;
      *out << "Infos" ;
      *out << std::endl;
    }

  void writeEvent(const char* request,const std::string& containerName, const char* instance_name, const char* port_name, 
                  int error, const char* message)
  {
    if(traceLevel < 0)
      initTrace(containerName);
    if(traceLevel == 0)return;

    struct timeval tv;
    gettimeofday(&tv,0);
    long tt0=tv.tv_sec/3600; //hours

    if(traceType == 2)
      {
        //notifier (not used: salome notifier is now obsolete)
        std::ostringstream msg;
        msg.width(7);
        msg << tt0 ;
        msg << ":" ;
        long tt1=(tv.tv_sec-3600*tt0)/60;//minutes
        msg.width(2);
        msg << tt1 ;
        msg << ":" ;
        long tt2=tv.tv_sec - 3600*tt0-60*tt1; //seconds
        msg.width(2);
        msg << tt2 ;
        msg << ":" ;
        long tt3=tv.tv_usec/1000; //milliseconds
        msg.width(3);
        msg << tt3 ;
        msg << " | " ;
        msg.width(24);
        msg << CPMESSAGE[error];
        msg << " | " ;
        msg << message ;
        //send event to notifier (containerName.c_str(),instance_name, request, msg.str().c_str())
      }
    else
      {
        //cerr or file
        out->width(7);
        *out << tt0 ;
        *out << ":" ;
        long tt1=(tv.tv_sec-3600*tt0)/60;//minutes
        out->width(2);
        *out << tt1 ;
        *out << ":" ;
        long tt2=tv.tv_sec - 3600*tt0-60*tt1; //seconds
        out->width(2);
        *out << tt2 ;
        *out << ":" ;
        long tt3=tv.tv_usec/1000; //milliseconds
        out->width(3);
        *out << tt3 ;
        *out << " | " ;
        out->width(16);
        *out << request ;
        *out << " | " ;
        out->width(16);
        *out << containerName ;
        *out << " | " ;
        out->width(16);
        *out << instance_name ;
        *out << " | " ;
        out->width(16);
        *out << port_name ;
        *out << " | " ;
        out->width(24);
        *out << CPMESSAGE[error];
        *out << " | " ;
        *out << message ;
        *out << std::endl;
      }
  }
};

