// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

//  File   : Salome_file_i.cxx
//  Author : André RIBES, EDF
//  Module : SALOME
//  $Header: 
//
#include "Salome_file_i.hxx"
#include "utilities.h"
#include <stdlib.h>
#include "HDFOI.hxx"
#ifndef WIN32
# include <unistd.h>
# define _getcwd getcwd
# define _open   open
#else
# include <direct.h>
# include <io.h>
# include <windows.h>
#endif

/*! \class Salome_file_i
 *  \brief A class to manage file transfer in %SALOME
 *
 */


//=============================================================================
/*! 
 *  Default constructor,
 */
//=============================================================================

Salome_file_i::Salome_file_i()
{
  _fileId = 0;
#ifndef WIN32
  _path_max = 1 + pathconf("/", _PC_PATH_MAX);
#else
  _path_max = 32768;
  //from MSDN:
  //Note The C Runtime supports path lengths up to 32768 characters in length, but it is up to the operating system, specifically the file system, to support these longer paths. The sum of the fields should not exceed _MAX_PATH for full backwards compatibility with Windows 98 FAT32 file systems. Windows NT 4.0, Windows 2000, Windows XP Home Edition, Windows XP Professional, Windows Server 2003, and Windows Server 2003 NTFS file system supports paths up to 32768 characters in length, but only when using the Unicode APIs. When using long path names, prefix the path with the characters \\?\ and use the Unicode versions of the C Runtime functions.
  //currently #define _MAX_PATH   260
#endif
  _state.name = CORBA::string_dup("");
  _state.hdf5_file_name = CORBA::string_dup("");
  _state.number_of_files = 0;
  _state.files_ok = true;
  _container = Engines::Container::_nil();
  _default_source_Salome_file = Engines::Salome_file::_nil();
}

//=============================================================================
/*! 
 *  Destructor
 */
//=============================================================================

Salome_file_i::~Salome_file_i()
{
}

//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::load
 */
//=============================================================================
void 
Salome_file_i::load(const char* hdf5_file) {
  _state.hdf5_file_name = CORBA::string_dup(hdf5_file);
  try
  {
    HDFfile *hdf_file;
    HDFgroup *hdf_group;
    HDFdataset *hdf_dataset;
    int size;
    int fd;
    char * value;
    char * buffer;

    hdf_file = new HDFfile((char*) hdf5_file);
    hdf_file->OpenOnDisk(HDF_RDONLY);

    hdf_group = new HDFgroup("CONFIG",hdf_file); 
    hdf_group->OpenOnDisk();
    hdf_dataset = new HDFdataset("MODE",hdf_group);
    hdf_dataset->OpenOnDisk();
    size = hdf_dataset->GetSize();
    value = new char[size];
    hdf_dataset->ReadFromDisk(value);
    hdf_dataset->CloseOnDisk();
    std::string mode(value);
    delete value;
    
    hdf_group = new HDFgroup("GROUP_FILES",hdf_file); 
    hdf_group->OpenOnDisk();
    hdf_dataset = new HDFdataset("LIST_OF_FILES",hdf_group);
    hdf_dataset->OpenOnDisk();
    size = hdf_dataset->GetSize();
    value = new char[size];
    hdf_dataset->ReadFromDisk(value);
    hdf_dataset->CloseOnDisk();
    std::string list_of_files(value);
    delete value;

    std::istringstream iss(list_of_files);
    std::string file_name;
    while (std::getline(iss, file_name, ' '))
    {
      std::string dataset_group_name("DATASET");
      dataset_group_name += file_name;

      hdf_group = new HDFgroup(dataset_group_name.c_str(), hdf_file); 
      hdf_group->OpenOnDisk();

      hdf_dataset = new HDFdataset("NAME",hdf_group);
      hdf_dataset->OpenOnDisk();
      size = hdf_dataset->GetSize();
      value = new char[size];
      hdf_dataset->ReadFromDisk(value);
      hdf_dataset->CloseOnDisk();
      std::string name(value);

      hdf_dataset = new HDFdataset("PATH",hdf_group);
      hdf_dataset->OpenOnDisk();
      size = hdf_dataset->GetSize();
      value = new char[size];
      hdf_dataset->ReadFromDisk(value);
      hdf_dataset->CloseOnDisk();
      std::string path(value);

      hdf_dataset = new HDFdataset("TYPE",hdf_group);
      hdf_dataset->OpenOnDisk();
      size = hdf_dataset->GetSize();
      value = new char[size];
      hdf_dataset->ReadFromDisk(value);
      hdf_dataset->CloseOnDisk();
      std::string type(value);

      hdf_dataset = new HDFdataset("SOURCE_FILE_NAME",hdf_group);
      hdf_dataset->OpenOnDisk();
      size = hdf_dataset->GetSize();
      value = new char[size];
      hdf_dataset->ReadFromDisk(value);
      hdf_dataset->CloseOnDisk();
      std::string source_file_name(value);

      hdf_dataset = new HDFdataset("STATUS",hdf_group);
      hdf_dataset->OpenOnDisk();
      size = hdf_dataset->GetSize();
      value = new char[size];
      hdf_dataset->ReadFromDisk(value);
      hdf_dataset->CloseOnDisk();
      std::string status(value);

      if (mode == "all") {

        // Changing path, is now current directory
        path = getcwd(NULL, _path_max);
  
        std::string group_name("GROUP");
        group_name += file_name;
        hdf_group = new HDFgroup(group_name.c_str(),hdf_file); 
        hdf_group->OpenOnDisk();
        hdf_dataset = new HDFdataset("FILE DATASET",hdf_group);
        hdf_dataset->OpenOnDisk();
        size = hdf_dataset->GetSize();
        buffer = new char[size];
      
        if ( (fd = ::open(file_name.c_str(),O_RDWR|O_CREAT,00666)) <0) { 
          SALOME::ExceptionStruct es;
          es.type = SALOME::INTERNAL_ERROR;
          std::string text = "open failed";
          es.text = CORBA::string_dup(text.c_str());
          throw SALOME::SALOME_Exception(es);
        };
        hdf_dataset->ReadFromDisk(buffer);
        if ( write(fd,buffer,size) <0) { 
          SALOME::ExceptionStruct es;
          es.type = SALOME::INTERNAL_ERROR;
          std::string text = "write failed";
          es.text = CORBA::string_dup(text.c_str());
          throw SALOME::SALOME_Exception(es);
        };
        // Close the target file
        ::close(fd);

        Engines::file infos;
        infos.file_name = CORBA::string_dup(file_name.c_str());
        infos.path = CORBA::string_dup(path.c_str());
        infos.type = CORBA::string_dup(type.c_str());
        infos.source_file_name = CORBA::string_dup(source_file_name.c_str());
        infos.status = CORBA::string_dup(status.c_str());

        _fileManaged[file_name] = infos;

        // Update Salome_file state
        _state.number_of_files++;
        _state.files_ok = true;
      }
      else {
        Engines::file infos;
        infos.file_name = CORBA::string_dup(file_name.c_str());
        infos.path = CORBA::string_dup(path.c_str());
        infos.type = CORBA::string_dup(type.c_str());
        infos.source_file_name = CORBA::string_dup(source_file_name.c_str());
        infos.status = CORBA::string_dup(status.c_str());
        // Infos for parallel extensions...
        infos.node = 0;
        infos.container = Engines::Container::_duplicate(_container);

        _fileManaged[file_name] = infos;

        // Update Salome_file state
        _state.number_of_files++;
        if (status != "ok")
          _state.files_ok = false;
      }
    }
  }
  catch (HDFexception)
  {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    std::string text = "!!!! HDFexception";
    es.text = CORBA::string_dup(text.c_str());
    throw SALOME::SALOME_Exception(es);
  }
}

//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::save
 */
//=============================================================================
void 
Salome_file_i::save(const char* hdf5_file) {
  _state.hdf5_file_name = CORBA::string_dup(hdf5_file);
  try
  {
    HDFfile *hdf_file;
    HDFgroup *hdf_group;
    HDFdataset *hdf_dataset;
    hdf_size size[1];
    _t_fileManaged::iterator begin = _fileManaged.begin();
    _t_fileManaged::iterator end = _fileManaged.end();

    hdf_file = new HDFfile((char*) _state.hdf5_file_name.in());
    hdf_file->CreateOnDisk();

    // Save mode information
    hdf_group = new HDFgroup("CONFIG", hdf_file);
    hdf_group->CreateOnDisk();
    std::string mode("infos");
    size[0] = strlen(mode.c_str()) + 1;
    hdf_dataset = new HDFdataset("MODE", hdf_group, HDF_STRING, size, 1);
    hdf_dataset->CreateOnDisk();
    hdf_dataset->WriteOnDisk((void *) mode.c_str());
    hdf_dataset->CloseOnDisk();
    hdf_group->CloseOnDisk();

    // List of files that are managed
    std::string list_of_files;
    for(;begin!=end;begin++) 
    {
      Engines::file file_infos = begin->second;
      std::string file_name(file_infos.file_name.in());

      list_of_files = list_of_files + file_name + std::string(" ");
    }
    hdf_group = new HDFgroup("GROUP_FILES", hdf_file);
    hdf_group->CreateOnDisk();
    size[0] = strlen(list_of_files.c_str()) + 1;
    hdf_dataset = new HDFdataset("LIST_OF_FILES", hdf_group, HDF_STRING, size, 1);
    hdf_dataset->CreateOnDisk();
    hdf_dataset->WriteOnDisk((void *) list_of_files.c_str());
    hdf_dataset->CloseOnDisk();
    hdf_group->CloseOnDisk();

    // Insert Files into the hdf5_file
    begin = _fileManaged.begin();
    for(;begin!=end;begin++) 
    {
      Engines::file file_infos = begin->second;
      std::string file_name(file_infos.file_name.in());
      std::string comp_file_name(_fileManaged[file_name].path.in());
      comp_file_name.append(_fileManaged[file_name].file_name.in());
      std::string dataset_group_name("DATASET");
      dataset_group_name += std::string(_fileManaged[file_name].file_name.in());

      hdf_group = new HDFgroup((char *) dataset_group_name.c_str(), hdf_file);
      hdf_group->CreateOnDisk();
      size[0] = strlen(file_infos.file_name.in()) + 1;
      hdf_dataset = new HDFdataset("NAME", hdf_group, HDF_STRING, size, 1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((void *) file_infos.file_name.in());
      hdf_dataset->CloseOnDisk();
      size[0] = strlen(file_infos.path.in()) + 1;
      hdf_dataset = new HDFdataset("PATH", hdf_group, HDF_STRING, size, 1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((void *) file_infos.path.in());
      hdf_dataset->CloseOnDisk();
      size[0] = strlen(file_infos.type.in()) + 1;
      hdf_dataset = new HDFdataset("TYPE", hdf_group, HDF_STRING, size, 1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((void *) file_infos.type.in());
      hdf_dataset->CloseOnDisk();
      size[0] = strlen(file_infos.source_file_name.in()) + 1;
      hdf_dataset = new HDFdataset("SOURCE_FILE_NAME", hdf_group, HDF_STRING, size, 1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((void *) file_infos.source_file_name.in());
      hdf_dataset->CloseOnDisk();
      size[0] = strlen(file_infos.status.in()) + 1;
      hdf_dataset = new HDFdataset("STATUS", hdf_group, HDF_STRING, size, 1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((void *) file_infos.status.in());
      hdf_dataset->CloseOnDisk();
      hdf_group->CloseOnDisk();
    }

    hdf_file->CloseOnDisk();

    //      delete hdf_dataset;
    //      delete hdf_group; ----> SEGFAULT !!!
    //      delete hdf_file; ----> SEGFAULT !!!
  }
  catch (HDFexception)
  {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    std::string text = "!!!! HDFexception";
    es.text = CORBA::string_dup(text.c_str());
    throw SALOME::SALOME_Exception(es);
  }
}

//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::save_all
 */
//=============================================================================
void 
Salome_file_i::save_all(const char* hdf5_file) {

  _state.hdf5_file_name = CORBA::string_dup(hdf5_file);
  // Test Salome_file status
  if (_state.files_ok == false) {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    std::string text = "File Not Ok !";
    es.text = CORBA::string_dup(text.c_str());
    throw SALOME::SALOME_Exception(es);
  }

  // For each file we create two groups
  // First group contains file's informations
  // Second group contains the file
  // At the end we create a group and a dataset containing the names
  // of all the files.
  try
  {
    HDFfile *hdf_file;
    HDFgroup *hdf_group;
    HDFdataset *hdf_dataset;
    hdf_size size[1];
    _t_fileManaged::iterator begin = _fileManaged.begin();
    _t_fileManaged::iterator end = _fileManaged.end();

    hdf_file = new HDFfile((char*) _state.hdf5_file_name.in());
    hdf_file->CreateOnDisk();

    // Save mode information
    hdf_group = new HDFgroup("CONFIG", hdf_file);
    hdf_group->CreateOnDisk();
    std::string mode("all");
    size[0] = strlen(mode.c_str()) + 1;
    hdf_dataset = new HDFdataset("MODE", hdf_group, HDF_STRING, size, 1);
    hdf_dataset->CreateOnDisk();
    hdf_dataset->WriteOnDisk((void *) mode.c_str());
    hdf_dataset->CloseOnDisk();
    hdf_group->CloseOnDisk();


    // List of files that will be inserted
    std::string list_of_files;
    for(;begin!=end;begin++) 
    {
      Engines::file file_infos = begin->second;
      std::string file_name(file_infos.file_name.in());

      list_of_files = list_of_files + file_name + std::string(" ");
    }
    hdf_group = new HDFgroup("GROUP_FILES", hdf_file);
    hdf_group->CreateOnDisk();
    size[0] = strlen(list_of_files.c_str()) + 1;
    hdf_dataset = new HDFdataset("LIST_OF_FILES", hdf_group, HDF_STRING, size, 1);
    hdf_dataset->CreateOnDisk();
    hdf_dataset->WriteOnDisk((void *) list_of_files.c_str());
    hdf_dataset->CloseOnDisk();
    hdf_group->CloseOnDisk();

    // Insert Files into the hdf5_file
    begin = _fileManaged.begin();
    for(;begin!=end;begin++) 
    {
      Engines::file file_infos = begin->second;
      std::string file_name(file_infos.file_name.in());
      std::string comp_file_name(_fileManaged[file_name].path.in());
      comp_file_name.append(_fileManaged[file_name].file_name.in());
      std::string group_name("GROUP");
      group_name += std::string(_fileManaged[file_name].file_name.in());
      std::string dataset_group_name("DATASET");
      dataset_group_name += std::string(_fileManaged[file_name].file_name.in());

      hdf_group = new HDFgroup((char *) group_name.c_str(), hdf_file);
      hdf_group->CreateOnDisk();
      HDFConvert::FromAscii(comp_file_name.c_str(), *hdf_group, "FILE DATASET");
      hdf_group->CloseOnDisk();

      hdf_group = new HDFgroup((char *) dataset_group_name.c_str(), hdf_file);
      hdf_group->CreateOnDisk();
      size[0] = strlen(file_infos.file_name.in()) + 1;
      hdf_dataset = new HDFdataset("NAME", hdf_group, HDF_STRING, size, 1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((void *) file_infos.file_name.in());
      hdf_dataset->CloseOnDisk();
      size[0] = strlen(file_infos.path.in()) + 1;
      hdf_dataset = new HDFdataset("PATH", hdf_group, HDF_STRING, size, 1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((void *) file_infos.path.in());
      hdf_dataset->CloseOnDisk();
      size[0] = strlen(file_infos.type.in()) + 1;
      hdf_dataset = new HDFdataset("TYPE", hdf_group, HDF_STRING, size, 1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((void *) file_infos.type.in());
      hdf_dataset->CloseOnDisk();
      size[0] = strlen(file_infos.source_file_name.in()) + 1;
      hdf_dataset = new HDFdataset("SOURCE_FILE_NAME", hdf_group, HDF_STRING, size, 1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((void *) file_infos.source_file_name.in());
      hdf_dataset->CloseOnDisk();
      size[0] = strlen(file_infos.status.in()) + 1;
      hdf_dataset = new HDFdataset("STATUS", hdf_group, HDF_STRING, size, 1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((void *) file_infos.status.in());
      hdf_dataset->CloseOnDisk();
      hdf_group->CloseOnDisk();

    }

    hdf_file->CloseOnDisk();

    //      delete hdf_dataset;
    //      delete hdf_group; ----> SEGFAULT !!!
    //      delete hdf_file; ----> SEGFAULT !!!
  }
  catch (HDFexception)
  {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    std::string text = "!!!! HDFexception";
    es.text = CORBA::string_dup(text.c_str());
    throw SALOME::SALOME_Exception(es);
  }
}

//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::setLocalFile
 */
//=============================================================================
void 
Salome_file_i::setLocalFile(const char* comp_file_name)
{
  std::string file_name("");
  std::string path("");
  std::string type("local");
  std::string source_file_name("");
  std::string status("not_ok");

  std::string cp_file_name(comp_file_name);
  std::size_t index = cp_file_name.rfind("/");
  if (index != -1)
  {
    file_name = cp_file_name.substr(index+1);
    path =  cp_file_name.substr(0,index+1);
  }
  else
  {
    file_name = comp_file_name;    
    path = getcwd(NULL, _path_max);;
  }

  // Test if this file is already added
  _t_fileManaged::iterator it = _fileManaged.find(file_name);
  if (it != _fileManaged.end()) 
  {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    std::string text = "file already added";
    es.text = CORBA::string_dup(text.c_str());
    throw SALOME::SALOME_Exception(es);
  }

  // Test if the file is ok
  if(fopen(comp_file_name,"rb") != NULL)
    status = "ok";

  // Adding file with is informations
  Engines::file infos;
  infos.file_name = CORBA::string_dup(file_name.c_str());
  infos.path = CORBA::string_dup(path.c_str());
  infos.type = CORBA::string_dup(type.c_str());
  infos.source_file_name = CORBA::string_dup(source_file_name.c_str());
  infos.status = CORBA::string_dup(status.c_str());
  // Infos for parallel extensions...
  infos.node = 0;
  infos.container = Engines::Container::_duplicate(_container);

  _fileManaged[file_name] = infos;

  // Update Salome_file state
  _state.number_of_files++;
  if (status != "ok")
    _state.files_ok = false;
}

//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::setDistributedFile
 */
//=============================================================================
void 
Salome_file_i::setDistributedFile(const char* comp_file_name)
{
  std::string file_name("");
  std::string path("");
  std::string type("distributed");
  std::string source_file_name("");
  std::string status("not_ok");

  std::string cp_file_name(comp_file_name);
  std::size_t index = cp_file_name.rfind("/");
  if (index != -1)
  {
    file_name = cp_file_name.substr(index+1);
    path =  cp_file_name.substr(0,index+1);
  }
  else
  {
    file_name = comp_file_name;
    path = getcwd(NULL, _path_max);;
  }

  // Test if this file is already added
  _t_fileManaged::iterator it = _fileManaged.find(file_name);
  if (it != _fileManaged.end()) 
  {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    std::string text = "file already added";
    es.text = CORBA::string_dup(text.c_str());
    throw SALOME::SALOME_Exception(es);
  }

  // Adding file with his informations
  Engines::file infos;
  infos.file_name = CORBA::string_dup(file_name.c_str());
  infos.path = CORBA::string_dup(path.c_str());
  infos.type = CORBA::string_dup(type.c_str());
  infos.source_file_name = CORBA::string_dup(source_file_name.c_str());
  infos.status = CORBA::string_dup(status.c_str());
  // Infos for parallel extensions...
  infos.node = 0;
  infos.container = Engines::Container::_duplicate(_container);

  _fileManaged[file_name] = infos;

  if(!CORBA::is_nil(_default_source_Salome_file)) 
  {
    _fileDistributedSource[file_name] = 
      Engines::Salome_file::_duplicate(_default_source_Salome_file);
  }

  // Update Salome_file state
  _state.number_of_files++;
  _state.files_ok = false;
}

//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::connect
 */
//=============================================================================
void
Salome_file_i::connect(Engines::Salome_file_ptr source_Salome_file) 
{
  if(CORBA::is_nil(_default_source_Salome_file)) 
  {
    _default_source_Salome_file = Engines::Salome_file::_duplicate(source_Salome_file);
    _t_fileManaged::iterator begin = _fileManaged.begin();
    _t_fileManaged::iterator end = _fileManaged.end();
    for(;begin!=end;begin++) {
      // Get the name of the file
      std::string file_name = begin->first;
      _t_fileDistributedSource::iterator it = _fileDistributedSource.find(file_name);
      if (it == _fileDistributedSource.end()) 
      {
        _fileDistributedSource[file_name] = Engines::Salome_file::_duplicate(source_Salome_file);
      }
    }
  }
  else
  {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    std::string text = "already connected to a default Salome_file";
    es.text = CORBA::string_dup(text.c_str());
    throw SALOME::SALOME_Exception(es);
  }
  // We can connect this Salome_file if there is only one file managed
  // by the Salome_file
  //std::string fname;
  //if (_fileManaged.size() == 1) 
  //{
    // only one file managed 
  //  _t_fileManaged::iterator it = _fileManaged.begin();
  //  fname = it->first;
  //  _fileDistributedSource[fname] = Engines::Salome_file::_duplicate(source_Salome_file);
  //}
  //else 
  //{
  //  SALOME::ExceptionStruct es;
  //  es.type = SALOME::INTERNAL_ERROR;
  //  std::string text = "cannot connect";
   // es.text = CORBA::string_dup(text.c_str());
   // throw SALOME::SALOME_Exception(es);
  //}
}

//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::connectDistributedFile
 */
//=============================================================================
void
Salome_file_i::connectDistributedFile(const char * file_name,
                                      Engines::Salome_file_ptr source_Salome_file) 
{
  // Test if this file is added
  _t_fileManaged::iterator it = _fileManaged.find(file_name);
  if (it == _fileManaged.end()) 
  {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    std::string text = "file is not added";
    es.text = CORBA::string_dup(text.c_str());
    throw SALOME::SALOME_Exception(es);
  }
  else 
  {
    _fileDistributedSource[file_name] = Engines::Salome_file::_duplicate(source_Salome_file);
  }
}

//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::setDistributedSourceFile
 */
//=============================================================================
void 
Salome_file_i::setDistributedSourceFile(const char* file_name,
                                        const char * source_file_name)
{
  std::string fname(file_name);

  // Test if this file is added
  _t_fileManaged::iterator it = _fileManaged.find(fname);
  if (it == _fileManaged.end()) 
  {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    std::string text = "file is not added";
    es.text = CORBA::string_dup(text.c_str());
    throw SALOME::SALOME_Exception(es);
  }
  else 
  {
    _fileManaged[fname].source_file_name = CORBA::string_dup(source_file_name);
  }
}

//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::recvFiles
 */
//=============================================================================
void 
Salome_file_i::recvFiles() {
  
  std::string files_not_ok("");

  _t_fileManaged::iterator begin = _fileManaged.begin();
  _t_fileManaged::iterator end = _fileManaged.end();
  for(;begin!=end;begin++) 
  {
    bool result = true;
    Engines::file file_infos = begin->second;
    // Test if the file is local or distributed
    if (std::string(file_infos.type.in()) == "local")
    {
      if (std::string(file_infos.status.in()) == "not_ok")
        result = checkLocalFile(file_infos.file_name.in());
    }
    else
    {
      if (std::string(file_infos.status.in()) == "not_ok")
        result = getDistributedFile(file_infos.file_name.in());
    }
    // if the result is false
    // we add this file to files_not_ok
    if (!result) 
    {
      files_not_ok.append(" ");
      files_not_ok.append(file_infos.file_name.in());
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

//=============================================================================
/*! 
 *  local C++ method : This method is used by revFiles to check if a local 
 *  managed file is ok.
 *  \param  fileName name of the file
 */
//=============================================================================
bool
Salome_file_i::checkLocalFile(std::string file_name)
{
  bool result = true;

  std::string comp_file_name(_fileManaged[file_name].path.in());
  comp_file_name.append("/");
  comp_file_name.append(_fileManaged[file_name].file_name.in());
  if(fopen(comp_file_name.c_str(),"rb") == NULL)
  {
    INFOS("file " << comp_file_name << " cannot be open for reading");
    _fileManaged[file_name].status = CORBA::string_dup("not_ok");
    result = false;
  }

  if (result)
  {
    _fileManaged[file_name].status = CORBA::string_dup("ok");
  }
  return result;
}

//=============================================================================
/*! 
 *  local C++ method : this method is used by recvFiles to get a 
 *  distributed file from is distributed source.
 *  If there is no source_file_name for the file, it tries to get
 *  the file from the source. In this case, the source distributed file has to managed
 *  only one file to be able to the send the file.
 *
 *  \param  fileName name of the file
 */
//=============================================================================
bool
Salome_file_i::getDistributedFile(std::string file_name)
{
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

  try 
  {
    fileId = _fileDistributedSource[file_name]->open(source_file_name);
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
      aBlock = _fileDistributedSource[file_name]->getBlock(fileId);
      toFollow = aBlock->length();
      CORBA::Octet *buf = aBlock->get_buffer();
#if defined(_DEBUG_) || defined(_DEBUG)
      int nbWri = fwrite(buf, sizeof(CORBA::Octet), toFollow, fp);
      ASSERT(nbWri == toFollow);
#else
      fwrite(buf, sizeof(CORBA::Octet), toFollow, fp);
#endif
      delete aBlock;
    }
    fclose(fp);
    MESSAGE("end of transfer of " << comp_file_name);
    _fileDistributedSource[file_name]->close(fileId);
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

//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::removeFile
 */
//=============================================================================
void 
Salome_file_i::removeFile(const char* file_name) 
{
  MESSAGE("Salome_file_i::removeFile : NOT YET IMPLEMENTED");
}
    
//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::removeFiles
 */
//=============================================================================
void 
Salome_file_i::removeFiles() {
  MESSAGE("Salome_file_i::removeFiles : NOT YET IMPLEMENTED");
}

//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::getFilesInfos
 */
//=============================================================================
Engines::files* 
Salome_file_i::getFilesInfos() {

  Engines::files * infos = new Engines::files();
  infos->length(_fileManaged.size());

  _t_fileManaged::iterator begin = _fileManaged.begin();
  _t_fileManaged::iterator end = _fileManaged.end();
  int i = 0;
  for(;begin!=end;begin++) {
    (*infos)[i] = *(new Engines::file(begin->second));
    i++;
  }
  return infos;
}

//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::getFileInfos
 */
//=============================================================================
Engines::file* 
Salome_file_i::getFileInfos(const char* file_name) {

  std::string fname(file_name);

  // Test if this file is managed
  _t_fileManaged::iterator it = _fileManaged.find(fname);
  if (it == _fileManaged.end()) 
  {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    es.text = "file is not managed";
    throw SALOME::SALOME_Exception(es);
  }

  Engines::file * infos = new Engines::file(_fileManaged[fname]);
  return infos;
}

//=============================================================================
/*! 
 *  CORBA method
 * \see Engines::Salome_file::getSalome_fileState
 */
//=============================================================================
Engines::SfState* 
Salome_file_i::getSalome_fileState() 
{
  return new Engines::SfState(_state);
}

//=============================================================================
/*! 
 *  CORBA method: try to open the file given. If the file is readable, return
 *  a positive integer else return 0;
 *  \param  fileName file name to be transfered
 *  \return fileId = positive integer > 0 if open OK.
 */
//=============================================================================

CORBA::Long 
Salome_file_i::open(const char* file_name)
{
  int aKey = 0;

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
      // we can't choose the file so :
      return aKey;
    }
  }

  _t_fileManaged::iterator it = _fileManaged.find(fname);
  if (it == _fileManaged.end())
  {
    return aKey;
  }
  
  std::string comp_file_name(_fileManaged[fname].path.in());
  comp_file_name.append("/");
  comp_file_name.append(fname);
  MESSAGE("Salome_file_i::open " << comp_file_name);
  FILE* fp;
  if ((fp = fopen(comp_file_name.c_str(),"rb")) == NULL)
    {
      INFOS("file " << comp_file_name << " is not readable");
      return aKey;
    }

  aKey = ++_fileId;
  _fileAccess[aKey] = fp;
  return aKey;
}

//=============================================================================
/*! 
 *  CORBA method: close the file associated to the fileId given at open.
 *  \param fileId got in return from open method
 */
//=============================================================================

void 
Salome_file_i::close(CORBA::Long fileId)
{
  MESSAGE("Salome_file_i::close");
  FILE* fp;
  if (!(fp = _fileAccess[fileId]) )
    {
      INFOS(" no FILE structure associated to fileId " << fileId);
    }
  else fclose(fp);
}

//=============================================================================
/*! 
 *  CORBA method: get a block of data from the file associated to the fileId
 *  given at open.
 *  \param fileId got in return from open method
 *  \return an octet sequence. Last one is empty.
 */
//=============================================================================

#define FILEBLOCK_SIZE 256*1024

Engines::fileBlock* 
Salome_file_i::getBlock(CORBA::Long fileId)
{
  Engines::fileBlock* aBlock = new Engines::fileBlock;

  FILE* fp;
  if (! (fp = _fileAccess[fileId]) )
  {
    INFOS(" no FILE structure associated to fileId " <<fileId);
    return aBlock;
  }

  // use replace member function for sequence to avoid copy
  // see Advanced CORBA Programming with C++ pp 187-194
  CORBA::Octet *buf;
  buf = Engines::fileBlock::allocbuf(FILEBLOCK_SIZE);
  int nbRed = fread(buf, sizeof(CORBA::Octet), FILEBLOCK_SIZE, fp);
  aBlock->replace(nbRed, nbRed, buf, 1); // 1 means give ownership
  return aBlock;
}

void 
Salome_file_i::setContainer(Engines::Container_ptr container)
{
  _container = Engines::Container::_duplicate(container);

  // Update All the files
  _t_fileManaged::iterator begin = _fileManaged.begin();
  _t_fileManaged::iterator end = _fileManaged.end();
  for(;begin!=end;begin++) {
    begin->second.container = Engines::Container::_duplicate(container);
  }
}

