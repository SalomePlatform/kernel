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

//  SALOME HDFPersist : implementation of HDF persitent ( save/ restore )
//  File   : HDFconvert.cc
//  Module : SALOME
//
#include "HDFconvert.hxx"

#ifdef WIN32
#include <io.h>
#include <windows.h>
#define open _open
#define close _close
#endif

int HDFConvert::FromAscii(const std::string& file, const HDFcontainerObject & hdf_container, const std::string& nomdataset)
{
  
  HDFdataset   * hdf_dataset;
  char         * buffer;
  int          fd;
  struct stat  status;
  size_t       length;
  hdf_size     length_long;
  
  // Ouverture du fichier source
  if ( (fd = open(file.c_str(),O_RDONLY)) <0) { 
    perror("HDFConvert::FromAscii");
    return -1;
  };
  
  // Lit l'‰tat du fichier
  if ( fstat(fd,&status) < 0) {
    perror("HDFConvert::FromAscii");
    return -1;
  };
  
  length = status.st_size; //Calcul la taille du fichier en octets
  length_long = length;
  
#ifdef _POSIX_MAPPED_FILES
  
  // Map le fichier en m‰moire
  if ( (buffer = (char *)  mmap(0,length,PROT_READ,MAP_SHARED,fd,0)) == MAP_FAILED ) {
    perror("HDFConvert::FromAscii");
    return -1;
  };
#elif defined WIN32

#define SHMEMSIZE 4096

  HANDLE hMapObject = CreateFileMapping( 
           INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SHMEMSIZE, "");
  if (hMapObject != NULL) {
  // Get a pointer to the file-mapped shared memory.
  buffer = ( char* ) MapViewOfFile( 
    hMapObject, FILE_MAP_WRITE, 0, 0, 0 );
  if( buffer == NULL )
    CloseHandle(hMapObject);
  };

#else

  // Sort de la compilation
#error Necessite l''utilisation de la primitive mmap
  
#endif
  
  // Creation du Dataset utilisateur 
  hdf_dataset = new HDFdataset( (char *) nomdataset.c_str(),            /*discard const */
                                            (HDFcontainerObject*) &hdf_container,   /*discard const, pas de constructeur par r‰f‰rence */
                                            HDF_STRING,
                                            &length_long,1);
  // Cree le Dataset sur le disk
  hdf_dataset->CreateOnDisk();
  
  // Effectue la copie
  hdf_dataset->WriteOnDisk(buffer);
  
  // Ferme le fichier hdf
  hdf_dataset->CloseOnDisk();
  
  // Memory Clean
  delete hdf_dataset;
  
#ifdef _POSIX_MAPPED_FILES
  
  // Desalloue le mapping
  if (munmap(buffer,length) < 0 ) {
    perror("HDFConvert::FromAscii");
    return -1;
  };
#endif
  
  // Ferme le fichier ASCII
  if (close(fd) <0) {
    perror("HDFConvert::FromAscii");
    return -1;
  };
    

  return length;    
};
