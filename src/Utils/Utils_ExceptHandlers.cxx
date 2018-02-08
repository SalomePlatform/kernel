// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

// File:        Utils_ExceptHandler.cxx
// Created:     Mon Mar 15 10:23:41 2004
// Author:      Oksana TCHEBANOVA
//              <ota@localhost.localdomain>
//
#include "Utils_ExceptHandlers.hxx"
#include "Utils_CorbaException.hxx"
#include "Utils_SALOME_Exception.hxx"

#include <sstream>
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Exception)

//#define NBLINES_BACKTRACE 64

void printBacktrace(void **stacklines, int nbLines, std::stringstream& txt)
{
  char **stackSymbols = backtrace_symbols(stacklines, nbLines);
  for (int i = 0; i < nbLines; i++)
    {
      Dl_info infodl;
      if (dladdr(stacklines[i], &infodl))
        {
          txt << i << " " << infodl.dli_fname << " " << infodl.dli_fbase << " ";
          char *demangled = NULL;
          int status = 0;
          demangled = abi::__cxa_demangle(infodl.dli_sname, NULL, 0, &status);
          if (status == 0 && demangled != NULL)
            {
              std::string demangstr = demangled; // copy
              txt << demangstr;
            }
          else
            {
              if (infodl.dli_sname != 0 && infodl.dli_sname[0] != 0)
                {
                  std::string sname = infodl.dli_sname;
                  if (sname.size() > 0)
                    txt << infodl.dli_sname;
                }
            }
          txt << " " << infodl.dli_saddr;
          txt << std::endl;
          free(demangled);
        }
      else
        txt << i << " " << stackSymbols[i] << std::endl;
    }
  free(stackSymbols);
}

void SalomeException ()
{
  void *stacklines[64];
  size_t nbLines;
  nbLines = backtrace(stacklines, 64);
  std::stringstream txt;
  txt << "Salome Exception" << std::endl;
  printBacktrace(stacklines, nbLines, txt);
  throw SALOME_Exception(txt.str().c_str());
}

void SALOME_SalomeException()
{
  void *stacklines[64];
  size_t nbLines;
  nbLines = backtrace(stacklines, 64);
  std::stringstream txt;
  txt << "INTERNAL_ERROR, backtrace stack:" << nbLines << std::endl;
  printBacktrace(stacklines, nbLines, txt);
  THROW_SALOME_CORBA_EXCEPTION(txt.str().c_str(), SALOME::INTERNAL_ERROR);
}

