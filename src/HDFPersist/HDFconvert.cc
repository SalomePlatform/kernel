using namespace std;
#include "HDFconvert.hxx"

int HDFConvert::FromAscii(const string& file, const HDFcontainerObject & hdf_container, const string& nomdataset)
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
  
  // Lit l'état du fichier
  if ( fstat(fd,&status) < 0) {
    perror("HDFConvert::FromAscii");
    return -1;
  };
  
  length = status.st_size; //Calcul la taille du fichier en octets
  length_long = length;
  
#ifdef _POSIX_MAPPED_FILES
  
  // Map le fichier en mémoire
  if ( (buffer = (char *)  mmap(0,length,PROT_READ,MAP_SHARED,fd,0)) == MAP_FAILED ) {
    perror("HDFConvert::FromAscii");
    return -1;
  };
  
#else

  // Sort de la compilation
#error Necessite l''utilisation de la primitive mmap
  
#endif
  
  // Creation du Dataset utilisateur 
  hdf_dataset = new HDFdataset::HDFdataset( (char *) nomdataset.c_str(),            /*discard const */
                                            (HDFcontainerObject*) &hdf_container,   /*discard const, pas de constructeur par référence */
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
    
};
