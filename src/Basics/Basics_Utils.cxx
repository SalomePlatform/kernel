// Copyright (C) 2007-2020  CEA/DEN, EDF R&D, OPEN CASCADE
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

//  File   : Basics_Utils.cxx
//  Author  : Alexander A. BORODIN
//  Module : SALOME
//

#include "Basics_Utils.hxx"
#include <string.h>
#include <stdlib.h>

#ifndef WIN32
#include <unistd.h>
#include <sys/stat.h>
#include <execinfo.h>
#endif


namespace Kernel_Utils
{
  // threadsafe
  std::string GetHostname()
  {
    int ls = 100, r = 1;
    char *s;

    while (ls < 10000 && r)
      {
        ls *= 2;
        s = new char[ls];
        r = gethostname(s, ls-1);//threadsafe see man 7 pthread
        switch (r)
          {
          case 0:
            break;
          default:
#ifdef EINVAL
          case EINVAL:
#endif
#ifdef ENAMETOOLONG
          case ENAMETOOLONG:
#endif
#ifdef WIN32
          case WSAEFAULT:
#endif
            delete [] s;
            continue;
          }

      }

    if (r != 0)
      {
        s = new char[50];
        strcpy(s, "localhost");
      }

    // remove all after '.'
    char *aDot = (strchr(s,'.'));
    if (aDot) aDot[0] = '\0';

    std::string p = s;
    delete [] s;
    return p;
  }

  Localizer::Localizer()
  {
    init(LC_NUMERIC, "C");
  }

  Localizer::Localizer(int category, const char* locale)
  {
    init(category, locale);
  }

  void Localizer::init(int category, const char* locale)
  {
    myCategory = category;
    myOriginalLocale = setlocale(category, NULL);
    setlocale(category, locale);
  }

  Localizer::~Localizer()
  {
    setlocale(myCategory, myOriginalLocale.c_str());
  }

  std::string GetGUID( GUIDtype type )
  {
    std::string guid;

    switch ( type ) {
    case DefUserID:
      guid = "FFFFFFFF-D9CD-11d6-945D-1050DA506788"; break;
    case ObjectdID:
      guid = "C08F3C95-F112-4023-8776-78F1427D0B6D"; break;
    }

    return guid;
  }

  const wchar_t* decode(const char* encoded)
  {
    Localizer loc(LC_CTYPE, "");
    size_t length = strlen(encoded) + sizeof(char);
    wchar_t* decoded = new wchar_t[length];
    memset( decoded, '\0', length);
    mbstowcs(decoded, encoded, length);
    return decoded;
  }

  const wchar_t* decode_s(std::string encoded)
  {
    return decode(encoded.c_str());
  }

  const char* encode(const wchar_t* decoded)
  {
    Localizer loc(LC_CTYPE, "");
    size_t length = std::wcslen(decoded) + sizeof(wchar_t);
    char* encoded = new char[length];
    memset( encoded, '\0', length);
    wcstombs(encoded, decoded, length);
    return encoded;
  }

  std::string encode_s(const wchar_t* decoded)
  {
    return std::string(encode(decoded));
  }

#ifndef WIN32
  void print_traceback()
  {
    void *array[50];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace (array, 40);
    strings = backtrace_symbols (array, size);

    for (i = 0; i < size; i++)
      {
        std::cerr << strings[i] << std::endl;
      }

    free (strings);
  }
#else
  #if (_MSC_VER >= 1400) // Visual Studio 2005
  #include <sstream>
  int setenv(const char *name, const char *value, int rewrite)
  {
    std::stringstream sstr;
    sstr<<name<<'='<<value;
    if(rewrite || std::string(getenv(name)).length() == 0)
      return _putenv(sstr.str().c_str());
    else return -1;
  }
  #endif
#endif

#if defined(WIN32)
  // Convert a wide Unicode string to an UTF8 string
  char* utf8_encode(const wchar_t* encoded)
  {
	  if (encoded == NULL) return NULL;
    auto size_needed = WideCharToMultiByte(CP_UTF8, 0, encoded, (int)std::wcslen(encoded), NULL, 0, NULL, NULL);
	  char* strTo = new char[ size_needed + 1 ];
	  WideCharToMultiByte(CP_UTF8, 0, encoded, (int)std::wcslen(encoded), strTo, size_needed, NULL, NULL);
	  strTo[size_needed] = '\0';
	  return strTo;
  }
  // Convert an UTF8 string to a wide Unicode String
  wchar_t* utf8_decode(const char* decoded)
  {
	  if (decoded == NULL) return NULL;
    auto size_needed = MultiByteToWideChar(CP_UTF8, 0, decoded, (int)strlen(decoded), NULL, 0);
	  wchar_t* wstrTo = new wchar_t[ size_needed + 1 ];
	  MultiByteToWideChar(CP_UTF8, 0, decoded, (int)strlen(decoded), wstrTo, size_needed);
	  wstrTo[size_needed] = '\0';
	  return wstrTo;
  }
  // Convert a wide Unicode std::wstring to an UTF8 string
  std::string utf8_encode_s(const std::wstring& encoded) {
	  char* anEncoded = utf8_encode(encoded.c_str());
	  std::string res = std::string(anEncoded);
	  delete [] anEncoded;
	  return res;
  }
  // Convert an UTF8 std::string to a wide Unicode std::wstring
  std::wstring utf8_decode_s(const std::string& decoded) {
	  wchar_t* aDecoded = utf8_decode(decoded.c_str());
	  std::wstring res = std::wstring(aDecoded);
	  delete [] aDecoded;
	  return res;
  }
#endif
}
