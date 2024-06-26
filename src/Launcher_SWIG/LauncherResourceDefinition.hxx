// Copyright (C) 2024  CEA, EDF
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

#pragma once

#include "SALOME_Launcher_SWIG.hxx"
#include "SALOME_ResourcesCatalog_Parser.hxx"

#ifndef SWIG
struct LAUNCHER_SWIG_EXPORT ResourceDefinition_cpp
#else
struct ResourceDefinition_cpp
#endif
{
public:
  std::string name;
  std::string hostname;
  std::string type;
  std::string protocol;
  std::string username;
  std::string applipath;
  std::string OS;
  int  mem_mb;
  int  cpu_clock;
  int  nb_node;
  int  nb_proc_per_node;
  std::string batch;
  std::string mpiImpl;
  std::string iprotocol;
  bool can_launch_batch_jobs;
  bool can_run_containers;
  std::string working_directory;
public:
  void fromPRT(const ParserResourcesType& other);
  ParserResourcesType toPRT() const;
};
