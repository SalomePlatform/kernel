// Copyright (C) 2021  CEA/DEN, EDF R&D
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

#include "KernelBasis.hxx"

static bool DEFAULT_SSL_MODE = true;
static bool GUI_MODE = false;

// IOR of SALOME_Embedded_NamingService servant
static std::string IOR_OF_EMBEDDED_NS;

bool getSSLMode()
{
  return DEFAULT_SSL_MODE;
}

void setSSLMode(bool sslMode)
{
  DEFAULT_SSL_MODE = sslMode;
}

bool getGUIMode()
{
  return GUI_MODE;
}

void setGUIMode(bool guiMode)
{
  GUI_MODE = guiMode;
}

std::string BASICS_EXPORT getIOROfEmbeddedNS()
{
  return IOR_OF_EMBEDDED_NS;
}

void BASICS_EXPORT setIOROfEmbeddedNS(const std::string& ior)
{
  IOR_OF_EMBEDDED_NS = ior;
}
