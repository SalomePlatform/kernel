// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

#include <Python.h>
#include <windows.h>

static PyObject * win32pm_spawnhandle( PyObject *self, PyObject *args )
{
  char *argv;
  char *flags;
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  DWORD dwProcessFlags = CREATE_NEW_CONSOLE;
  /* Default value as in Python sources:
     CREATE_NEW_CONSOLE has been known to
     cause random failures on win9x.  Specifically a
     dialog:
     "Your program accessed mem currently in use at xxx"
     and a hopeful warning about the stability of your
     system.
     Cost is Ctrl+C wont kill children, but anyone
     who cares can have a go!
  */

  if (!PyArg_ParseTuple(args, "s|s", &argv, &flags))
    return NULL;
  if ( flags && !strcmp( flags, "-nc" ) ) // no console
    dwProcessFlags = 0;

  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );

  if ( !CreateProcess( NULL, argv,
                        NULL,             // Process handle not inheritable. 
                        NULL,             // Thread handle not inheritable. 
                        TRUE,
                        dwProcessFlags,   // Creation flags. 
                        NULL,             // Use parent's environment block. 
                        NULL,             // Use parent's starting directory. 
                        &si,              // Pointer to STARTUPINFO structure.
                        &pi ) )             // Pointer to PROCESS_INFORMATION structure.
     return NULL;
  return Py_BuildValue("i", pi.hProcess);
}

static PyObject * win32pm_spawnpid( PyObject *self, PyObject *args )
{
  char* argv;
  char *flags;
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  DWORD dwProcessFlags = CREATE_NEW_CONSOLE;
  /* Default value as in Python sources:
     CREATE_NEW_CONSOLE has been known to
     cause random failures on win9x.  Specifically a
     dialog:
     "Your program accessed mem currently in use at xxx"
     and a hopeful warning about the stability of your
     system.
     Cost is Ctrl+C wont kill children, but anyone
     who cares can have a go!
  */

  if (!PyArg_ParseTuple(args, "s|s", &argv, &flags))
    return NULL;
  if ( flags && !strcmp( flags, "-nc" ) ) // no console
    dwProcessFlags = 0;

  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );

  if ( !CreateProcess( NULL, argv,
                        NULL,             // Process handle not inheritable. 
                        NULL,             // Thread handle not inheritable. 
                        TRUE,
                        dwProcessFlags,   // Creation flags. 
                        NULL,             // Use parent's environment block. 
                        NULL,             // Use parent's starting directory. 
                        &si,              // Pointer to STARTUPINFO structure.
                        &pi ) )             // Pointer to PROCESS_INFORMATION structure.
     return NULL;
  return Py_BuildValue("i", pi.dwProcessId);
}

static PyObject * win32pm_handle( PyObject *self, PyObject *args )
{
  int argv1 = 0;
  int argv2 = 0;
  HANDLE ph = 0;
  int pid = 0;
  if (!PyArg_ParseTuple(args, "i|i", &argv1, &argv2))
    return NULL;
  ph = OpenProcess( 1, (BOOL)argv2, argv1  );
  return Py_BuildValue("i", (int)ph );
}

static PyObject * win32pm_killpid( PyObject *self, PyObject *args )
{
  int pid = 0;
  int exitCode = 0;
  HANDLE ph = 0;
  BOOL stat = 0;
  if (!PyArg_ParseTuple(args, "i|i", &pid, &exitCode))
    return NULL;
  ph = OpenProcess( 1, 0, pid );
  stat = TerminateProcess( ph, exitCode );
  return Py_BuildValue("i", (int)stat );
}

static PyObject * win32pm_killhandle( PyObject *self, PyObject *args )
{
  int phId = 0;
  int exitCode = 0;
  BOOL stat = 0;
  if (!PyArg_ParseTuple(args, "i|i", &phId, &exitCode))
    return NULL;

  stat = TerminateProcess( (void*)phId, exitCode );
  return Py_BuildValue("i", (int)stat );
}

static PyMethodDef win32pmMethods[] = {
    {"spawnhandle",  win32pm_spawnhandle, METH_VARARGS,
     "Creates new process. Returns windows handle of new process."},
    {"spawnpid",  win32pm_spawnpid, METH_VARARGS,
     "Creates new process. Returns PID of new process."},
    {"handle",  win32pm_handle, METH_VARARGS,
     "Returns windows handle of indicated process PID."},
    {"killpid",  win32pm_killpid, METH_VARARGS,
     "Terminate process by PID."},
    {"killhandle",  win32pm_killhandle, METH_VARARGS,
     "Terminate process by windows process handle."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initwin32pm(void)
{
    Py_InitModule("win32pm", win32pmMethods);
}
