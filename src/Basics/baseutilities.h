// Copyright (C) 2023-2024  CEA/DEN, EDF R&D
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

#include "libSALOMELog.hxx"
#include <sstream>
#include <iostream>

#pragma once

#define SIMPLE_MESS_END std::endl; std::cout << os.str() << std::flush;
#define SIMPLE_MESS_INIT(deb) std::ostringstream os; os<< deb
#define SIMPLE_MESS_BEGIN(deb) SIMPLE_MESS_INIT(deb)<<__FILE__ <<" ["<<__LINE__<<"] : "
#define SIMPLE_MESSAGE(msg) { if (SALOME::VerbosityActivated()) {SIMPLE_MESS_BEGIN("- Trace ") << msg << SIMPLE_MESS_END}}
