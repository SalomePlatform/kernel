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

//  File   : Parallel_Salome_file_i.cxx
//  Author : André RIBES, EDF
//  Module : SALOME
//  $Header: 
//
#include "Parallel_Salome_file_i.hxx"
#include "utilities.h"

Parallel_Salome_file_i::Parallel_Salome_file_i(CORBA::ORB_ptr orb, 
                                               const char * ior,
                                               int rank) :
  InterfaceParallel_impl(orb,ior,rank), 
  Engines::Salome_file_serv(orb,ior,rank),
  Engines::Salome_file_base_serv(orb,ior,rank),
  Engines::fileTransfer_serv(orb,ior,rank),
  Engines::Parallel_Salome_file_serv(orb,ior,rank),
  Engines::fileTransfer_base_serv(orb,ior,rank),
  SALOME::GenericObj_serv(orb,ior,rank),
  SALOME::GenericObj_base_serv(orb,ior,rank),
  Engines::Parallel_Salome_file_base_serv(orb,ior,rank)
{
  CORBA::Object_ptr obj = _orb->string_to_object(ior);
  proxy = Engines::Parallel_Salome_file::_narrow(obj);
  parallel_file = NULL;
}

Parallel_Salome_file_i::~Parallel_Salome_file_i() {}

void 
Parallel_Salome_file_i::load(const char* hdf5_file) {
  MESSAGE("Parallel_Salome_file_i::load : NOT YET IMPLEMENTED");
  SALOME::ExceptionStruct es;
  es.type = SALOME::INTERNAL_ERROR;
  es.text = "Parallel_Salome_file_i::load : NOT YET IMPLEMENTED";
  throw SALOME::SALOME_Exception(es);
}

void 
Parallel_Salome_file_i::save(const char* hdf5_file) {
  MESSAGE("Parallel_Salome_file_i::save : NOT YET IMPLEMENTED");
  SALOME::ExceptionStruct es;
  es.type = SALOME::INTERNAL_ERROR;
  es.text = "Parallel_Salome_file_i::save : NOT YET IMPLEMENTED";
  throw SALOME::SALOME_Exception(es);
}

void 
Parallel_Salome_file_i::save_all(const char* hdf5_file) {
  MESSAGE("Parallel_Salome_file_i::save_all : NOT YET IMPLEMENTED");
  SALOME::ExceptionStruct es;
  es.type = SALOME::INTERNAL_ERROR;
  es.text = "Parallel_Salome_file_i::save_all : NOT YET IMPLEMENTED";
  throw SALOME::SALOME_Exception(es);
}

void 
Parallel_Salome_file_i::connect(Engines::Salome_file_ptr source_Salome_file) {
  // only one file managed case 
  Salome_file_i::connect(source_Salome_file);

  // Test if the file is managed in an another node
  // If yes, node is updated
  _t_fileManaged::iterator begin = _fileManaged.begin();
  _t_fileManaged::iterator end = _fileManaged.end();
  for(;begin!=end;begin++) {
    std::string file_name = begin->first;
    if (_fileManaged[file_name].node > 0 && getMyRank() == 0) {
      if (parallel_file == NULL)
        parallel_file = Engines::PaCO_Parallel_Salome_file::PaCO_narrow(proxy, _orb);
      parallel_file->connect(source_Salome_file, _fileManaged[file_name].node);
    }
  }
}

void 
Parallel_Salome_file_i::connectDistributedFile(const char * file_name,
                                               Engines::Salome_file_ptr source_Salome_file) {
  Salome_file_i::connectDistributedFile(file_name, source_Salome_file);

  // Test if the file is managed in an another node
  // If yes, node is updated
  std::string fname(file_name);
  if (_fileManaged[fname].node > 0 && getMyRank() == 0) {
    if (parallel_file == NULL)
      parallel_file = Engines::PaCO_Parallel_Salome_file::PaCO_narrow(proxy, _orb);
    parallel_file->connectDistributedFile(file_name, source_Salome_file, _fileManaged[fname].node);
  }
}

void 
Parallel_Salome_file_i::setDistributedSourceFile(const char* file_name,
                                                 const char * source_file_name) {
  Salome_file_i::setDistributedSourceFile(file_name, source_file_name);
  // Test if the file is managed in an another node
  // If yes, node is updated
  std::string fname(file_name);
  if (_fileManaged[fname].node > 0 && getMyRank() == 0) {
    if (parallel_file == NULL)
      parallel_file = Engines::PaCO_Parallel_Salome_file::PaCO_narrow(proxy, _orb);
    parallel_file->setDistributedSourceFile(file_name, source_file_name, _fileManaged[fname].node);
  }
}

void
Parallel_Salome_file_i::recvFiles() {
  if (parallel_file == NULL)
    parallel_file = Engines::PaCO_Parallel_Salome_file::PaCO_narrow(proxy, _orb);

  std::string files_not_ok("");
  int total = getTotalNode();
  for (int i =0; i<total; i++) {
    try {
     parallel_file->recvFiles_node(i);
    }
    catch (SALOME::SALOME_Exception & ex) {
      files_not_ok = files_not_ok + std::string(ex.details.text.in());
    }
  }

  if (files_not_ok != "")
  {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    std::string text = "files not ready : " + files_not_ok;
    es.text = CORBA::string_dup(text.c_str());
    throw SALOME::SALOME_Exception(es);
  }
  else
  {
    // We change the state of the Salome_file
    _state.files_ok = true;
  }
}

void 
Parallel_Salome_file_i::recvFiles_node() {

  std::string files_not_ok("");
  _t_fileManaged::iterator begin = _fileManaged.begin();
  _t_fileManaged::iterator end = _fileManaged.end();
  for(;begin!=end;begin++) 
  {
    bool result = true;
    Engines::file file_infos = begin->second;
    if (file_infos.node == getMyRank()) {
      // Test if the file is local or distributed
      if (std::string(file_infos.type.in()) == "local")
      {
        if (std::string(file_infos.status.in()) == "not_ok")
          result = checkLocalFile(file_infos.file_name.in());
      }
      else
      {
        if (std::string(file_infos.status.in()) == "not_ok") {
          // 2 cases :
          // Source file is a Salome_file
          // Source file is a Parallel_Salome_file
          PaCO::InterfaceManager_var interface_manager = 
            PaCO::InterfaceManager::_narrow(_fileDistributedSource[file_infos.file_name.in()]);
          if (CORBA::is_nil(interface_manager))
            result = getDistributedFile(file_infos.file_name.in());
          else
            result = getParallelDistributedFile(file_infos.file_name.in());
        }
      }
      // if the result is false
      // we add this file to files_not_ok
      if (!result) 
      {
        files_not_ok.append(" ");
        files_not_ok.append(file_infos.file_name.in());
      }
    }
  }
  if (files_not_ok != "")
  {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    std::string text = files_not_ok;
    es.text = CORBA::string_dup(text.c_str());
    throw SALOME::SALOME_Exception(es);
  }
}

bool 
Parallel_Salome_file_i::getParallelDistributedFile(std::string file_name) {

  bool result = true;
  const char * source_file_name = _fileManaged[file_name].source_file_name.in();
  int fileId;
  FILE* fp;
  std::string comp_file_name(_fileManaged[file_name].path.in());
  comp_file_name.append("/");
  comp_file_name.append(_fileManaged[file_name].file_name.in());

  // Test if the process can write on disk
  if ((fp = fopen(comp_file_name.c_str(),"wb")) == NULL)
  {
    INFOS("file " << comp_file_name << " cannot be open for writing");
    _fileManaged[file_name].status = CORBA::string_dup("not_ok");
    result = false;
    return result;
  }

  Engines::PaCO_Parallel_Salome_file * parallel_source_file = 
    Engines::PaCO_Parallel_Salome_file::PaCO_narrow(_fileDistributedSource[file_name], _orb);

  int node = parallel_source_file->getFileNode(source_file_name);

  try 
  {
    fileId = parallel_source_file->open(source_file_name, node);
  }
  catch (...) 
  {
    _fileManaged[file_name].status = CORBA::string_dup("not_ok");
    fclose(fp);
    result = false;
    return result;
  }

  if (fileId > 0)
  {
    Engines::fileBlock* aBlock;
    int toFollow = 1;
    int ctr=0;
    MESSAGE("begin of transfer of " << comp_file_name);
    while (toFollow)
    {
      ctr++;
      aBlock = parallel_source_file->getBlock(fileId, node);
      toFollow = aBlock->length();
      CORBA::Octet *buf = aBlock->get_buffer();
      int nbWri = fwrite(buf, sizeof(CORBA::Octet), toFollow, fp);
      delete aBlock;
      ASSERT(nbWri == toFollow);
    }
    fclose(fp);
    MESSAGE("end of transfer of " << comp_file_name);
    parallel_source_file->close(fileId, node);
  }
  else
  {
    INFOS("open reference file for copy impossible");
    result = false;
    fclose(fp);
    _fileManaged[file_name].status = CORBA::string_dup("not_ok");
    return result;
  }

  _fileManaged[file_name].status = CORBA::string_dup("ok");
  return result;
}

void 
Parallel_Salome_file_i::setContainer(Engines::Container_ptr container) {
  _container = Engines::Container::_duplicate(container);

  // Update All the files managed by the node
  _t_fileManaged::iterator begin = _fileManaged.begin();
  _t_fileManaged::iterator end = _fileManaged.end();
  for(;begin!=end;begin++) {
    begin->second.container = Engines::Container::_duplicate(container);
  }
}

void 
Parallel_Salome_file_i::setFileNode(const char* file_name, CORBA::Long node) {
  
  // Test if this file is managed
  std::string fname(file_name);
  _t_fileManaged::iterator it = _fileManaged.find(fname);
  if (it == _fileManaged.end()) 
  {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    es.text = "file is not managed";
    throw SALOME::SALOME_Exception(es);
  }

  // Update file infos into this node (node 0)
  // and into the node that actually managed it
  _fileManaged[fname].node = node;

  if (node > 0) {
    if (parallel_file == NULL)
      parallel_file = Engines::PaCO_Parallel_Salome_file::PaCO_narrow(proxy, _orb);

    Engines::Container_ptr cont = parallel_file->updateFile(_fileManaged[fname], node);
    parallel_file->connectDistributedFile(fname.c_str(),
                                          _fileDistributedSource[fname],
                                          node);

    // Update file infos with the new reference of the container
    _fileManaged[fname].container = Engines::Container::_duplicate(cont);
  }
}

Engines::Container_ptr
Parallel_Salome_file_i::updateFile(const Engines::file& file) {
  // Copy file
  Engines::file new_file_infos(file);

  // Adding it to node list
  new_file_infos.container = Engines::Container::_duplicate(_container);
  std::string fname(new_file_infos.file_name.in());
  _fileManaged[fname] = new_file_infos;

  // Return the new reference of the container associated to the file
  return Engines::Container::_duplicate(_container);
}

CORBA::Long 
Parallel_Salome_file_i::getFileNode(const char* file_name) {
  
  // Test if this file is managed
  std::string fname(file_name);
  if (fname == "") {
    // We enter in the simple case where the user
    // has not used setDistributedSourceFile.
    // In this case we try to see if the Salome_file
    if (_fileManaged.size() == 1) 
    {
      // only one file managed 
      _t_fileManaged::iterator it = _fileManaged.begin();
      fname = it->first;
    }
    else
    {
      SALOME::ExceptionStruct es;
      es.type = SALOME::INTERNAL_ERROR;
      es.text = "Error : there is more than one file that is managed";
      throw SALOME::SALOME_Exception(es);
    }
  }
  _t_fileManaged::iterator it = _fileManaged.find(fname);
  if (it == _fileManaged.end()) 
  {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    es.text = "file is not managed";
    throw SALOME::SALOME_Exception(es);
  }

  return _fileManaged[fname].node;
}
