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

#include "Salome_file_i.hxx"
#include <iostream>
#include <fstream>
#include <sstream>
#include "HDFascii.hxx"
#include <stdlib.h>

void print_infos(Engines::file*);
void print_state(Engines::SfState*);

void print_infos(Engines::file * infos) 
{
  std::cerr << "-------------------------------------------------------------------" << std::endl;
  std::cerr << "file_name = " << infos->file_name << std::endl;
  std::cerr << "path = " << infos->path << std::endl;
  std::cerr << "type = " << infos->type << std::endl;
  std::cerr << "source_file_name = " << infos->source_file_name << std::endl;
  std::cerr << "status = " << infos->status << std::endl;
}

void print_state(Engines::SfState * state) 
{
  std::cerr << "-------------------------------------------------------------------" << std::endl;
  std::cerr << "name = " << state->name << std::endl;
  std::cerr << "hdf5_file_name = " <<  state->hdf5_file_name << std::endl;
  std::cerr << "number_of_files = " <<  state->number_of_files << std::endl;
  std::cerr << "files_ok = " <<  state->files_ok << std::endl;
}


int main (int argc, char * argv[])
{
  system("rm toto cat test.hdf test2.hdf");

  Salome_file_i file;
  Salome_file_i file2;
  Salome_file_i file3;
  Salome_file_i file4;
  Salome_file_i file5;
  Engines::file * infos;
  Engines::SfState * state;
  Engines::files * all_infos;
  PortableServer::POA_var root_poa;
  PortableServer::POAManager_var pman;
  CORBA::Object_var obj;

  std::cerr << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cerr << "Test of setLocalFile()" << std::endl;
  file.setLocalFile("/tmp/toto");
  infos = file.getFileInfos("toto");
  print_infos(infos);


  std::cerr << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cerr << "Test of getFilesInfos()" << std::endl;
  all_infos = file.getFilesInfos();
  for (int i = 0; i < all_infos->length(); i++)
  {
    print_infos(&((*all_infos)[i]));
  }

  std::cerr << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cerr << "Test of getSalome_fileState()" << std::endl;
  state = file.getSalome_fileState();
  print_state(state);

  // We start CORBA ...
  CORBA::ORB_ptr orb = CORBA::ORB_init(argc , argv);
  obj = orb->resolve_initial_references("RootPOA");
  root_poa = PortableServer::POA::_narrow(obj);
  pman = root_poa->the_POAManager();
  pman->activate();

  file2.setLocalFile("/tmp/toto_distributed_source");
  Engines::Salome_file_ptr file2_ref = file2._this();

  std::cerr << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cerr << "Test of setDistributedFile()" << std::endl;
  file.setDistributedFile("/tmp/toto_distributed");
  file.connectDistributedFile("toto_distributed", file2_ref);
//  file.setDistributedSourceFile("toto_distributed", "toto_distributed_source");
  infos = file.getFileInfos("toto_distributed");
  print_infos(infos);

  // We create the files ... 
  std::ostringstream oss;
  oss << "/tmp/toto";
  std::ofstream f(oss.str().c_str());
  if (f) 
    f << "blablabla" << std::endl;
 
  std::ostringstream oss2;
  oss2 << "/tmp/toto_distributed_source";
  std::ofstream f2(oss2.str().c_str());
  if (f2) 
    f2 << "bliblibli" << std::endl;

  try 
  {
    file.recvFiles();
  }
  catch (SALOME::SALOME_Exception & e)
  {
    std::cerr << "Exception : " << e.details.text << std::endl;
  }

  std::cerr << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cerr << "Test of getFilesInfos()" << std::endl;
  all_infos = file.getFilesInfos();
  for (int i = 0; i < all_infos->length(); i++)
  {
    print_infos(&((*all_infos)[i]));
  }

  std::cerr << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cerr << "Test of getSalome_fileState()" << std::endl;
  state = file.getSalome_fileState();
  print_state(state);

  orb->destroy();

  file3.setLocalFile("/tmp/toto");
  file3.setLocalFile("/bin/cat");
  state = file3.getSalome_fileState();
  print_state(state);
  file3.save_all("test.hdf");
  file3.setLocalFile("/bin/tutu");
  file3.save("test2.hdf");

  file4.load("test.hdf");
  all_infos = file4.getFilesInfos();
  for (int i = 0; i < all_infos->length(); i++)
  {
    print_infos(&((*all_infos)[i]));
  }
  state = file4.getSalome_fileState();
  print_state(state);
  file5.load("test2.hdf");
  all_infos = file5.getFilesInfos();
  for (int i = 0; i < all_infos->length(); i++)
  {
    print_infos(&((*all_infos)[i]));
  }
  state = file5.getSalome_fileState();
  print_state(state);

  // Test of ConvertFromHDFToASCII
  // and ConvertFromASCIIToHDF
  std::cerr << "Test of ConvertFromASCIIToHDF" << std::endl;
  HDFascii::ConvertFromASCIIToHDF("/tmp/toto"); // RETURN NULL !
  std::cerr << "Test of ConvertFromHDFToASCII" << std::endl;
  std::cerr << HDFascii::ConvertFromHDFToASCII("test2.hdf", false) << std::endl;
  std::cerr << HDFascii::ConvertFromHDFToASCII("test2.hdf", true) << std::endl;

  std::cerr << "End of tests" << std::endl;
}
