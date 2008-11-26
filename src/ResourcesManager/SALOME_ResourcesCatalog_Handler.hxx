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
//  SALOME ResourcesCatalog : implementation of catalog resources parsing (SALOME_ModuleCatalog.idl)
//  File   : SALOME_ResourcesCatalog_Handler.hxx
//  Author : Estelle Deville
//  Module : SALOME
//$Header$
//
#ifndef SALOME_RESOURCES_CATALOG_HANDLER
#define SALOME_RESOURCES_CATALOG_HANDLER


#include "ResourcesManager_Defs.hxx"

#include "SALOME_ResourcesCatalog_Parser.hxx"

#include <string>
#include <vector>

#include <libxml/parser.h>

class RESOURCESMANAGER_EXPORT SALOME_ResourcesCatalog_Handler
{
  
  public :
    SALOME_ResourcesCatalog_Handler(MapOfParserResourcesType& resources_list,
				    MapOfParserResourcesType& resources_batch_list);

    const MapOfParserResourcesType& GetResourcesAfterParsing() const;

    virtual ~SALOME_ResourcesCatalog_Handler();

    void ProcessXmlDocument(xmlDocPtr theDoc);
  
    void PrepareDocToXmlFile(xmlDocPtr theDoc);

  private :
    std::string previous_module_name;

    ParserResourcesType _resource;
    MapOfParserResourcesType& _resources_list;
    MapOfParserResourcesType& _resources_batch_list;

    const char *test_machine;
    const char *test_resources;

    const char *test_hostname;
    const char *test_alias;
    const char *test_protocol;
    const char *test_mode;
    const char *test_batch;
    const char *test_mpi;
    const char *test_user_name;
    const char *test_appli_path;
    const char *test_modules;
    const char *test_module_name;
    const char *test_os;
    const char *test_mem_in_mb;
    const char *test_cpu_freq_mhz;
    const char *test_nb_of_nodes;
    const char *test_nb_of_proc_per_node;
    const char *test_batch_queue;
    const char *test_user_commands;

  };

#endif // SALOME_RESOURCES_CATALOG_HANDLER
