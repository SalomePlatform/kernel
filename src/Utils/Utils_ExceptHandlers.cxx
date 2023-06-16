// Copyright (C) 2007-2023  CEA/DEN, EDF R&D, OPEN CASCADE
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
#ifdef WIN32
#include "DbgHelp.h"
#include <WinBase.h>
#pragma comment(lib, "Dbghelp.lib")
#else
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#endif

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Exception)

//#define NBLINES_BACKTRACE 64
#ifdef WIN32
void printBacktrace(std::stringstream& txt)
{
	typedef USHORT(WINAPI *CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);

	CaptureStackBackTraceType func = (CaptureStackBackTraceType)(GetProcAddress(LoadLibraryA("kernel32.dll"), "RtlCaptureStackBackTrace"));

	if (func == NULL)
		return;
	const int kMaxCallers = 128;

	void         * callers_stack[kMaxCallers];
	unsigned short frames;
	SYMBOL_INFO  * symbol;
	HANDLE         process;
	process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);
	frames = (func)(0, kMaxCallers, callers_stack, NULL);
	symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	const unsigned short  MAX_CALLERS_SHOWN = 64;
	frames = frames < MAX_CALLERS_SHOWN ? frames : MAX_CALLERS_SHOWN;
	for (unsigned int i = 0; i < frames; i++)
	{
		SymFromAddr(process, (DWORD64)(callers_stack[i]), 0, symbol);
		txt << "*** " << i << ": " << callers_stack[i] << " " << symbol->Name << " - 0x" << symbol->Address << std::endl;
	}

	free(symbol);
}
#else
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
#endif

void SalomeException ()
{
  std::stringstream txt;
  txt << "Salome Exception" << std::endl;
#ifdef WIN32
  printBacktrace(txt);
#else
  void *stacklines[64];
  size_t nbLines;
  nbLines = backtrace(stacklines, 64);
  printBacktrace(stacklines, nbLines, txt);
#endif
  throw SALOME_Exception(txt.str().c_str());
}

void SALOME_SalomeException()
{
  std::stringstream txt;
#ifdef WIN32
  txt << "INTERNAL_ERROR, backtrace stack:" << std::endl;
  printBacktrace(txt);
#else
  void *stacklines[64];
  size_t nbLines;
  nbLines = backtrace(stacklines, 64);
  txt << "INTERNAL_ERROR, backtrace stack:" << nbLines << std::endl;
  printBacktrace(stacklines, nbLines, txt);
#endif
  THROW_SALOME_CORBA_EXCEPTION(txt.str().c_str(), SALOME::INTERNAL_ERROR);
}

