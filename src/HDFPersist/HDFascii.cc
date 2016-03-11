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

//  File      : SALOMEDS_Tool.cxx
//  Created   : Mon Oct 21 16:24:34 2002
//  Author    : Sergey RUIN
//  Project   : SALOME
//  Module    : SALOMEDS
//
#include "HDFOI.hxx"

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <string>

#ifdef WIN32
#include <io.h>
#include <time.h>
#include <windows.h>
#define open _open
#define read _read
#define close _close
#define dir_separator '\\'
#else
#define dir_separator '/'
#endif

void Move(const std::string& fName, const std::string& fNameDst);
bool Exists(const std::string thePath); 
bool CreateAttributeFromASCII(HDFinternalObject *father, FILE* fp);
bool CreateDatasetFromASCII(HDFcontainerObject *father, FILE *fp);
bool CreateGroupFromASCII(HDFcontainerObject *father, FILE *fp);

void SaveAttributeInASCIIfile(HDFattribute *hdf_attribute, FILE* fp, int ident);
void SaveGroupInASCIIfile(HDFgroup *hdf_group, FILE* fp, int ident);
void SaveDatasetInASCIIfile(HDFdataset *hdf_dataset, FILE* fp, int ident);

std::string GetTmpDir();
char* makeName(char* name);
char* restoreName(char* name);
void write_float64(FILE* fp, hdf_float64* value);
void read_float64(FILE* fp, hdf_float64* value);

void WriteSimpleData( FILE* fp, HDFdataset *hdf_dataset, hdf_type type, long size );

#define MAX_STRING_SIZE   65535
#define MAX_ID_SIZE       20
#define NB_FLOAT_IN_ROW   3
#define NB_INTEGER_IN_ROW 9

#define ASCIIHDF_ID  "ASCIIHDF"
#define ATTRIBUTE_ID "ATTRIBUTE"
#define DATASET_ID   "DATASET"
#define GROUP_ID     "GROUP"

#define ASCIIHDF_ID_END  "ASCIIHDF_END"
#define ATTRIBUTE_ID_END "ATTRIBUTE_END"
#define DATASET_ID_END   "DATASET_END"
#define GROUP_ID_END     "GROUP_END"


//============================================================================
// function : isASCII
// purpose  : Returns True if the file is a converted to ASCII HDF file
//============================================================================
bool HDFascii::isASCII(const char* thePath) {
  int fd;
  if(!(fd = open(thePath, O_RDONLY))) return false;
  char* aBuffer = new char[9];
  aBuffer[8] = (char)0;
  read(fd, aBuffer, 8); 
  close(fd);

  bool res = (strcmp(aBuffer, ASCIIHDF_ID) == 0);

  delete [] aBuffer;

  return res;
}

//############################## HDF => ASCII ################################

//============================================================================
// function : ConvertFromHDFToASCII
// purpose  : Converts a file pointed by thePath to ASCII format
//            If isReplace is true the newly created file will replace the existent one.
//            If isReplace is false theExtension will be added to a created file name 
//            Returns the file name of ASCII file
//============================================================================
char* HDFascii::ConvertFromHDFToASCII(const char* thePath,
				      bool isReplace,
			              const char* theExtension)
{
  std::string aPath(thePath);
  if(!isReplace) { 
    if(theExtension == NULL) aPath += ".asc";    
    else aPath += (char*)theExtension;
  }

  std::string aFileName(aPath);
  if(isReplace) aFileName=aPath+".ascii_tmp";
 
  HDFfile *hdf_file = new HDFfile((char*)thePath); 
  hdf_file->OpenOnDisk(HDF_RDONLY);

  char name[HDF_NAME_MAX_LEN+1];
  int nbsons = hdf_file->nInternalObjects(), nbAttr = hdf_file->nAttributes(); 

  FILE* fp = fopen(aFileName.c_str(), "wb");
  fprintf(fp, "%s\n", ASCIIHDF_ID);
  fprintf(fp, "%i\n", nbsons+nbAttr);

  for(int j=0; j<nbAttr; j++) {
    char* attr_name = hdf_file->GetAttributeName(j);
    HDFattribute *hdf_attribute = new HDFattribute(attr_name, hdf_file);
    delete attr_name;
    SaveAttributeInASCIIfile(hdf_attribute, fp, 0);
    hdf_attribute = 0;
  }

  for (int i=0; i<nbsons; i++) {
    hdf_file->InternalObjectIndentify(i,name);
    if (strncmp(name, "INTERNAL_COMPLEX",16) == 0) continue;

    hdf_object_type type = hdf_file->InternalObjectType(name);

    if(type == HDF_DATASET) { 
      HDFdataset* hdf_dataset = new HDFdataset(name, hdf_file);
      SaveDatasetInASCIIfile(hdf_dataset, fp, 0);
      hdf_dataset = 0; 
    } else if(type == HDF_GROUP) {
      HDFgroup *hdf_group = new HDFgroup(name, hdf_file); 
      SaveGroupInASCIIfile(hdf_group, fp, 0);
      hdf_group = 0;
    }
  }

  fprintf(fp, "%s", ASCIIHDF_ID_END);

  fclose(fp);

  hdf_file->CloseOnDisk();
  delete hdf_file;

  if(isReplace) {
    if(Exists(aFileName))
      Move(aFileName, aPath);
    else 
      return NULL;
  }

  int length = strlen(aPath.c_str());
  char *new_str = new char[ 1+length ];
  strcpy(new_str , aPath.c_str()) ;

  return new_str;
}


//============================================================================
// function : SaveGroupInASCIIfile
// purpose  : 
//============================================================================
void SaveGroupInASCIIfile(HDFgroup *hdf_group, FILE* fp, int ident)
{
  hdf_group->OpenOnDisk();

  int nbsons = hdf_group->nInternalObjects(), nbAttr = hdf_group->nAttributes(); 

  fprintf(fp, "%s\n", GROUP_ID);

  char* name = makeName(hdf_group->GetName());

  fprintf(fp, "%s %i\n", name, nbsons+nbAttr);
  delete [] name;

  for(int j=0; j<nbAttr; j++) {
    name = hdf_group->GetAttributeName(j);
    HDFattribute *hdf_attribute = new HDFattribute(name, hdf_group);
    delete [] name;
    SaveAttributeInASCIIfile(hdf_attribute, fp, ident+1);
    hdf_attribute = 0;
  }

  char objName[HDF_NAME_MAX_LEN+1];
 
  for (int i=0; i<nbsons; i++) {
    hdf_group->InternalObjectIndentify(i, objName);

    if (strncmp(objName, "INTERNAL_COMPLEX",16) == 0)  continue;

    hdf_object_type type = hdf_group->InternalObjectType(objName);

    if  (type == HDF_DATASET) {
      HDFdataset* hdf_dataset = new HDFdataset(objName, hdf_group);
      SaveDatasetInASCIIfile(hdf_dataset, fp, ident+1);
      hdf_dataset = 0;
    } else if (type == HDF_GROUP)   {      
      HDFgroup *hdf_subgroup = new HDFgroup(objName, hdf_group);
      SaveGroupInASCIIfile(hdf_subgroup, fp, ident+1);
      hdf_subgroup = 0;
    } 
  }

  fprintf(fp, "%s\n", GROUP_ID_END);

  hdf_group->CloseOnDisk();  
}

//============================================================================
// function : SaveDatasetInASCIIfile
// purpose  : 
//============================================================================
void SaveDatasetInASCIIfile(HDFdataset *hdf_dataset, FILE* fp, int ident)
{
  hdf_dataset->OpenOnDisk();

  long size =  (long) hdf_dataset->GetSize();
  long ndim = hdf_dataset->nDim(); //Get number of dimesions
  hdf_size *dim = new hdf_size[ndim];
  hdf_type type = hdf_dataset->GetType();
  hdf_byte_order order = hdf_dataset->GetOrder();
  int nbAttr = hdf_dataset->nAttributes(); 

  char* name = makeName(hdf_dataset->GetName());

  fprintf(fp, "%s\n", DATASET_ID);
  fprintf(fp, "%s %i %i\n", name, type, nbAttr);
  delete [] name;

  hdf_dataset->GetDim(dim);
  fprintf(fp, " %i\n", ndim);

  for(int i = 0;i < ndim;i++) {
    fprintf(fp, " %i", dim[i]);
  }

  fprintf(fp, "\n");
  delete [] dim;

  fprintf(fp, "%li %i:", size, order);
  if( type == HDF_ARRAY ) {
    HDFarray *array = new HDFarray(hdf_dataset);
    hdf_type data_type = array->GetDataType();
    fprintf(fp, "\n" );
    fprintf(fp, " %i\n", data_type ); //Write array data type

    //Write nDim of the array
    int arr_ndim = array->nDim();
    fprintf(fp, " %i\n", arr_ndim);
    hdf_size *arr_dim = new hdf_size[arr_ndim];
    array->GetDim(arr_dim);

    for( int i = 0;i < arr_ndim; i++ ) {
      fprintf(fp, " %i", arr_dim[i]);
    }
        
    //And write the data array
    WriteSimpleData( fp, hdf_dataset, data_type, size);
  } else {
    WriteSimpleData( fp, hdf_dataset, type, size);
  }
  
  fprintf(fp, "\n");

  for ( int j=0; j<nbAttr; j++ )
  {
    name = hdf_dataset->GetAttributeName(j);
    HDFattribute *hdf_attribute = new HDFattribute(name, hdf_dataset);
    delete [] name;
    SaveAttributeInASCIIfile(hdf_attribute, fp, ident+1);
    hdf_attribute = 0;
  }

  fprintf(fp, "%s\n", DATASET_ID_END);

  hdf_dataset->CloseOnDisk(); 
}


//============================================================================
// function : SaveAttributeInASCIIfile
// purpose  : 
//============================================================================
void SaveAttributeInASCIIfile(HDFattribute *hdf_attribute, FILE* fp, int ident)
{
  hdf_attribute->OpenOnDisk();

  hdf_type type = hdf_attribute->GetType();

  char* name = makeName(hdf_attribute->GetName());
  int size = hdf_attribute->GetSize();

  fprintf(fp, "%s\n", ATTRIBUTE_ID);
  fprintf(fp, "%s %i %i\n", name, type, size);

  delete [] name;

  if (type == HDF_STRING) {    
    char* val = new char[size+1];
    hdf_attribute->ReadFromDisk(val);
    fprintf(fp, ":");
    fwrite(val, 1, size, fp);
    fprintf(fp, "\n");
    delete [] val;
  } else if (type == HDF_FLOAT64) {
    hdf_float64 val;
    hdf_attribute->ReadFromDisk(&val);
    write_float64(fp, &val);
    fprintf(fp, "\n");
  } else if(type == HDF_INT64) {
    hdf_int64 val;
    hdf_attribute->ReadFromDisk(&val);
    fprintf(fp, "%li \n", val);
  } else if(type == HDF_INT32) {
    hdf_int32 val;
    hdf_attribute->ReadFromDisk(&val);
    fprintf(fp, "%i \n", val);
  }

  fprintf(fp, "%s\n", ATTRIBUTE_ID_END);

  hdf_attribute->CloseOnDisk();  
}

//############################## ASCII => HDF ################################

//============================================================================
// function : ConvertFromASCIIToHDF
// purpose  : Converts a file pointed by thePath to HDF format
//            Returns a name of directory where a created HDF file is placed
//            The created file is named "hdf_from_ascii.hdf"
//============================================================================
char* HDFascii::ConvertFromASCIIToHDF(const char* thePath,
				      bool isReplace)
{
  std::string aTmpDir, aFullName;
  if(!isReplace) {
    // Get a temporary directory to store a file
    aTmpDir = GetTmpDir();
    // Build a full file name of temporary file
    aFullName = aTmpDir + "hdf_from_ascii.hdf";
  }
  else {
    aTmpDir = thePath;
    aFullName = std::string(thePath)+".ascii_tmp";
  }

  FILE *fp = fopen(thePath, "rb");
  if(!fp) return NULL;

  HDFfile *hdf_file = new HDFfile((char*)aFullName.c_str()); 
  hdf_file->CreateOnDisk();
  
  char type[9];
  int nbsons, i;
  fscanf(fp, "%s", type);
  fscanf(fp, "%i",&nbsons);

  if(strcmp(type, ASCIIHDF_ID) != 0) return NULL;

  for(i = 0; i < nbsons; i++) {
    char id_of_begin[MAX_ID_SIZE];
    fscanf(fp, "%s\n", id_of_begin);

    if(strcmp(id_of_begin, GROUP_ID) == 0) {
      if(!CreateGroupFromASCII(hdf_file, fp)) {
	std::cout << "ConvertFromASCIIToHDF : Can not create group number " << i << std::endl;
	return NULL;
      }
    }
    else if(strcmp(id_of_begin, DATASET_ID) == 0) {
      if(!CreateDatasetFromASCII(hdf_file, fp)) {
	std::cout << "ConvertFromASCIIToHDF :Can not create dataset number " << i << std::endl;
	return NULL;
      }
    }
    else if(strcmp(id_of_begin, ATTRIBUTE_ID) == 0) {
      if(!CreateAttributeFromASCII(hdf_file, fp)) {
	std::cout << "ConvertFromASCIIToHDF :Can not create attribute number " << i << std::endl;
	return NULL;
      }
    }
    else 
      std::cout << "ConvertFromASCIIToHDF : Unrecognized type " << id_of_begin << std::endl; 
  }

  char id_of_end[MAX_ID_SIZE];
  fscanf(fp, "%s", id_of_end);
  if(strcmp(id_of_end, ASCIIHDF_ID_END) != 0) {
    std::cout << "ConvertFromASCIIToHDF : Can not find the end ASCII token " << std::endl;
    return NULL;  
  }

  hdf_file->CloseOnDisk();
  delete hdf_file;

  if(isReplace) {
    if(Exists(aFullName))
      Move(aFullName, thePath);
    else 
      return NULL;
  }

  int length = strlen(aTmpDir.c_str());
  char *new_str = new char[ 1+length ];
  strcpy(new_str , aTmpDir.c_str()) ;

  fclose(fp);

  return new_str;
}


//============================================================================
// function : CreateGroupFromASCII
// purpose  : Creates a HDF group from a set attributes situated under theLabel
//============================================================================
bool CreateGroupFromASCII(HDFcontainerObject *father, FILE *fp)
{
  char name[HDF_NAME_MAX_LEN+1];
  int nbsons, i;
  fscanf(fp, "%s %i\n", name, &nbsons);  

  char* new_name = restoreName(name);

  HDFgroup* hdf_group = new HDFgroup(new_name, father);

  delete [] new_name;

  hdf_group->CreateOnDisk();

  for(i = 0; i < nbsons; i++) {
    char id_of_begin[MAX_ID_SIZE];
    fscanf(fp, "%s\n", id_of_begin);
    
    if(strcmp(id_of_begin, GROUP_ID) == 0) {
      if(!CreateGroupFromASCII(hdf_group, fp)) {
	std::cout << "Can not create subgroup " << i << " for group " << name << std::endl;
	return false;
      }
    }
    else if(strcmp(id_of_begin, DATASET_ID) == 0) {
      if(!CreateDatasetFromASCII(hdf_group, fp)) {
	std::cout << "Can not create dataset " << i << " for group " << name << std::endl;
	return false;
      }
    }
    else if(strcmp(id_of_begin, ATTRIBUTE_ID) == 0) {
      if(!CreateAttributeFromASCII(hdf_group, fp)) {
	std::cout << "Can not create attribute " << i << " for group " << name << std::endl;
	return false;
      }
    }
    else 
      std::cout << "CreateGroupFromASCII : Unrecognized type " << id_of_begin << std::endl; 
  }
  
  hdf_group->CloseOnDisk();
  hdf_group = 0; //will be deleted by father destructor

  char id_of_end[MAX_ID_SIZE];
  fscanf(fp, "%s\n", id_of_end);
  if(strcmp(id_of_end, GROUP_ID_END) != 0) {
    std::cout << "CreateGroupFromASCII : Invalid end token : " << id_of_end << std::endl;
    return false;
  }

  return true;
}


//============================================================================
// function : CreateDatasetFromASCII
// purpose  : Creates a HDF dataset from a set attributes situated under theLabel
//============================================================================
bool CreateDatasetFromASCII(HDFcontainerObject *father, FILE *fp)
{
  char name[HDF_NAME_MAX_LEN+1];
  hdf_type type;
  hdf_byte_order order;
  int nbDim, nbAttr;
  long i, size;

  fscanf(fp, "%s %i %i\n", name, &type, &nbAttr);
  char* new_name = restoreName(name);

  fscanf(fp, "%i\n", &nbDim);

  hdf_size* sizeArray = new hdf_size[nbDim];
  int dim = 0;
  for(i = 0; i<nbDim; i++) {
    fscanf(fp, "%i\n", &dim);
    sizeArray[i] = dim;
  }
 
   // order (2-d member) was not written in earlier versions
  char tmp;
  int nbRead = fscanf(fp, "%li %i%c", &size, &order, &tmp);
  if ( nbRead < 2 ) { // fscanf stops before ":"
    fscanf(fp, "%c", &tmp);
    order = H5T_ORDER_NONE;
  }
  if ( type != HDF_FLOAT64 )  // use order only for FLOAT64
    order = H5T_ORDER_NONE;


  HDFarray* anArray = 0;
  if( type == HDF_ARRAY ){
    //Get array information
    hdf_type arr_data_type;
    int arr_ndim;
    fscanf(fp, "%c", &tmp);
    fscanf(fp, " %i\n", &arr_data_type ); //Get array data type
    fscanf(fp, " %i\n", &arr_ndim ); //Get array nDim
    hdf_size *arr_dim = new hdf_size[arr_ndim];

    int tdim = 0;
    for( int i = 0;i < arr_ndim; i++ ) {
      fscanf(fp, " %i", &tdim);
      arr_dim[i] = tdim;
    }
    anArray = new HDFarray(0, arr_data_type, arr_ndim, arr_dim);
    anArray->CreateOnDisk();

    type = arr_data_type;
    delete [] arr_dim;
  }

  HDFdataset* hdf_dataset = new HDFdataset(new_name, father, anArray ? HDF_ARRAY : type, sizeArray, nbDim, order);
  
  if(anArray)
    hdf_dataset->SetArrayId(anArray->GetId());

  delete [] new_name;
  delete [] sizeArray;

  hdf_dataset->CreateOnDisk();

  if (type == HDF_STRING) {	
    char *val = new char[size+1];
    fread(val, 1, size, fp);
    hdf_dataset->WriteOnDisk(val);
    delete [] val;
  } else if (type == HDF_FLOAT64) {
    hdf_float64* val = new hdf_float64[size];
    for(i=0; i<size; i++) {
      read_float64(fp, &(val[i]));
    }
    hdf_dataset->WriteOnDisk(val);
    delete [] val;
  } else if(type == HDF_INT64) {
    hdf_int64* val = new hdf_int64[size];
    for(i=0; i<size; i++) {
      fscanf(fp, " %li", &(val[i]));
    }
    hdf_dataset->WriteOnDisk(val);
    delete [] val;
  } else if(type == HDF_INT32) {
    hdf_int32* val = new hdf_int32[size];
    for(i=0; i<size; i++) {
      fscanf(fp, " %i", &(val[i]));
    }
    hdf_dataset->WriteOnDisk(val);
    delete [] val;
  } else if(type == HDF_CHAR) {
    hdf_char* val = new hdf_char[size];
    for(i=0; i<size; i++) {
      fscanf(fp, " %i", &(val[i]));
    }
    hdf_dataset->WriteOnDisk(val);
    delete [] val;
  }

  char token[MAX_ID_SIZE];

  for(i = 0; i < nbAttr; i++) {
    fscanf(fp, "%s\n", token);
    
    if(strcmp(token, ATTRIBUTE_ID) == 0) {
      if(!CreateAttributeFromASCII(hdf_dataset, fp)) {
	std::cout << "Can not create attribute " << i << " for dataset " << name << std::endl;
	return false;
      }
    }
    else {
      std::cout << "CreateGroupFromASCII : Unrecognized type " << token << std::endl; 
      return false;
    }
  }
  
  fscanf(fp, "%s\n", token);
  if(strcmp(token, DATASET_ID_END) != 0) {
    std::cout << "CreateDatasetFromASCII : Invalid end token : " << token << std::endl;
    return false;
  }

  hdf_dataset->CloseOnDisk();
  hdf_dataset = 0; //will be deleted by father destructor

  if(anArray) {
    anArray->CloseOnDisk();
    anArray = 0; //will be deleted by father destructor
  }

  return true;
}


//============================================================================
// function : CreateAttributeFromASCII
// purpose  : Creates a HDF attribute from a set attributes situated under theLabel
//============================================================================
bool CreateAttributeFromASCII(HDFinternalObject *father, FILE* fp)
{
  char name[HDF_NAME_MAX_LEN+1];

  hdf_type type;
  int size;
  fscanf(fp, "%s %i %i\n", name, &type, &size);
  char* new_name = restoreName(name);
  HDFattribute* hdf_attribute = new HDFattribute(new_name, father, type, size);

  hdf_attribute->CreateOnDisk();

  delete [] new_name;
  
  if (type == HDF_STRING) {	
    char tmp;
    fscanf(fp, "%c", &tmp);
    char *val = new char[size+1];
    val[size] = (char)0;
    fread(val, 1, size, fp);
    hdf_attribute->WriteOnDisk(val);
    delete [] val;
  } else if (type == HDF_FLOAT64) {
    hdf_float64 val;
    read_float64(fp, &val);
    hdf_attribute->WriteOnDisk(&val);
  } else if(type == HDF_INT64) {
    hdf_int64 val;
    fscanf(fp, "%li", &val);
    hdf_attribute->WriteOnDisk(&val);
  } else if(type == HDF_INT32) {
    hdf_int32 val;
    fscanf(fp, "%i", &val);
    hdf_attribute->WriteOnDisk(&val);
  }
  
  hdf_attribute->CloseOnDisk();
  hdf_attribute = 0; //will be deleted by father destructor


  char id_of_end[MAX_ID_SIZE];
  fscanf(fp, "%s\n", id_of_end);
  if(strcmp(id_of_end, ATTRIBUTE_ID_END) != 0) {
    std::cout << "CreateAttributeFromASCII : Invalid end token : " << id_of_end << std::endl;
    return false;
  }

  return true;
}


//============================================================================
// function : GetTempDir
// purpose  : Return a temp directory to store created files like "/tmp/sub_dir/" 
//============================================================================ 
std::string GetTmpDir()
{
 //Find a temporary directory to store a file
  std::string aTmpDir;
  char *Tmp_dir = getenv("SALOME_TMP_DIR");
  if(Tmp_dir != NULL) {
    aTmpDir = std::string(Tmp_dir);
    if(aTmpDir[aTmpDir.size()-1] != dir_separator) aTmpDir+=dir_separator;
  }
  else {
#ifdef WIN32
    aTmpDir = std::string("C:\\");
#else
    aTmpDir = std::string("/tmp/");
#endif
  }

  srand((unsigned int)time(NULL));
  int aRND = 999 + (int)(100000.0*rand()/(RAND_MAX+1.0)); //Get a random number to present a name of a sub directory
  char buffer[127];
  sprintf(buffer, "%d", aRND);
 std:: string aSubDir(buffer);
  if(aSubDir.size() <= 1) aSubDir = std::string("123409876");

  aTmpDir += aSubDir; //Get RND sub directory

  if(aTmpDir[aTmpDir.size()-1] != dir_separator) aTmpDir+=dir_separator;

  std::string aDir = aTmpDir;

  for(aRND = 0; Exists(aDir); aRND++) {
    sprintf(buffer, "%d", aRND);
    aDir = aTmpDir+buffer;  //Build a unique directory name
  }

#ifdef WIN32
  //fuction CreateDirectory create only final directory, but not intermediate
  CreateDirectory(aTmpDir.c_str(), NULL);
  CreateDirectory(aDir.c_str(), NULL);
#else
  mkdir(aDir.c_str(), 0x1ff); 
#endif

  return aDir + dir_separator;
}

char* makeName(char* name)
{
  std::string aName(name), aNewName;
  int i, length = aName.size();
  char replace = (char)19;

  for(i=0; i<length; i++) {
    if(aName[i] == ' ') aNewName+=replace;
    else aNewName += aName[i];
  }

  length = strlen(aNewName.c_str());
  char *new_str = new char[ 1+length ];
  strcpy(new_str , aNewName.c_str()) ;
  return new_str;
}

char* restoreName(char* name)
{
  std::string aName(name), aNewName;
  int i, length = aName.size();
  char replace = (char)19;

  for(i=0; i<length; i++) {
    if(aName[i] == replace) aNewName+=' ';
    else aNewName += aName[i];
  }

  length = strlen(aNewName.c_str());
  char *new_str = new char[ 1+length ];
  strcpy(new_str , aNewName.c_str()) ;
  return new_str;
}

void write_float64(FILE* fp, hdf_float64* value)
{
  unsigned char* array = (unsigned char*)value;
  for(int i = 0; i < sizeof(hdf_float64); i++) {
    unsigned tmp = (unsigned short)array[i];
    fprintf(fp, " %2x", tmp);
  }
}

void read_float64(FILE* fp, hdf_float64* value)
{
  unsigned char* array = (unsigned char*)value;
  for(int i = 0; i < sizeof(hdf_float64); i++) {
    unsigned tmp;
    fscanf(fp, " %x", &tmp); 
    array[i] = (unsigned char)tmp;
  }
}

bool Exists(const std::string thePath) 
{
#ifdef WIN32 
 if (  GetFileAttributes (  thePath.c_str()  ) == 0xFFFFFFFF  ) { 
    DWORD errorId = GetLastError ();
    if ( errorId == ERROR_FILE_NOT_FOUND || errorId == ERROR_PATH_NOT_FOUND )
      return false;
  }
#else 
  int status = access ( thePath.c_str() , F_OK ); 
  if (status != 0) return false;
#endif
  return true;
}

void Move(const std::string& fName, const std::string& fNameDst)
{ 
#ifdef WIN32
  MoveFileEx (fName.c_str(), fNameDst.c_str(),MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);
#else
  rename(fName.c_str(), fNameDst.c_str());
#endif
}

void WriteSimpleData( FILE* fp, HDFdataset *hdf_dataset, hdf_type type, long size ) {
  if (type == HDF_STRING) {	
    char* val = new char[size];
    hdf_dataset->ReadFromDisk(val);
    fwrite(val, 1, size, fp);
    delete [] val;
  } else if (type == HDF_FLOAT64) {
    hdf_float64* val = new hdf_float64[size];
    hdf_dataset->ReadFromDisk(val);
    fprintf(fp, "\n");
    for (int i = 0, j = 0; i < size; i++) {
      write_float64(fp, &val[i]);
      if(++j == NB_FLOAT_IN_ROW) {
	fprintf(fp, "\n");
	j = 0;
      }
      else fprintf(fp,"  ");
    }
    delete [] val;
  } else if(type == HDF_INT64) {
    hdf_int64* val = new hdf_int64[size];
    hdf_dataset->ReadFromDisk(val);
    fprintf(fp, "\n");
    for (int i = 0, j = 0; i < size; i++) {
      fprintf(fp, " %li", val[i]);
      if(++j == NB_INTEGER_IN_ROW) {
	fprintf(fp, "\n");
	j = 0;
      }
    }
    delete [] val;
  } else if(type == HDF_INT32) {
    hdf_int32* val = new hdf_int32[size];
    hdf_dataset->ReadFromDisk(val);
    fprintf(fp, "\n");
    for (int i = 0, j = 0; i < size; i++) {
      fprintf(fp, " %i", val[i]);
      if(++j == NB_INTEGER_IN_ROW) {
	fprintf(fp, "\n");
	j = 0;
      }
    }
    delete [] val;
  }else if(type == HDF_CHAR) {
    hdf_char* val = new hdf_char[size];
    hdf_dataset->ReadFromDisk(val);
    fprintf(fp, "\n");
    for (int i = 0, j = 0; i < size; i++) {
      fprintf(fp, " %i", val[i]);
      if(++j == NB_INTEGER_IN_ROW) {
	fprintf(fp, "\n");
	j = 0;
      }
    }
    delete [] val;
  }
}
