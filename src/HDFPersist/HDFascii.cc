// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
//  File      : SALOMEDS_Tool.cxx
//  Created   : Mon Oct 21 16:24:34 2002
//  Author    : Sergey RUIN

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE

#include "HDFOI.hxx"

#include <OSD_Path.hxx>
#include <OSD_File.hxx>
#include <OSD_Protection.hxx>
#include <OSD_Directory.hxx>
#include <TCollection_AsciiString.hxx> 

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#ifdef WNT
#include <io.h>
#include <time.h>
#endif

using namespace std;

bool CreateAttributeFromASCII(HDFinternalObject *father, FILE* fp);
bool CreateDatasetFromASCII(HDFcontainerObject *father, FILE *fp);
bool CreateGroupFromASCII(HDFcontainerObject *father, FILE *fp);

void SaveAttributeInASCIIfile(HDFattribute *hdf_attribute, FILE* fp, int ident);
void SaveGroupInASCIIfile(HDFgroup *hdf_group, FILE* fp, int ident);
void SaveDatasetInASCIIfile(HDFdataset *hdf_dataset, FILE* fp, int ident);

char* GetTmpDir();
char* makeName(char* name);
char* restoreName(char* name);
void write_float64(FILE* fp, hdf_float64* value);
void read_float64(FILE* fp, hdf_float64* value);

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

  if(strcmp(aBuffer, ASCIIHDF_ID) == 0) return true;

  return false;
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
  TCollection_AsciiString aPath((char*)thePath);
  if(!isReplace) { 
    if(theExtension == NULL) aPath += ".asc";    
    else aPath += (char*)theExtension;
  }

  TCollection_AsciiString aFileName(aPath);
  if(isReplace) aFileName=aPath+".ascii_tmp";
 
  HDFfile *hdf_file = new HDFfile((char*)thePath); 
  hdf_file->OpenOnDisk(HDF_RDONLY);

  char name[HDF_NAME_MAX_LEN+1];
  int nbsons = hdf_file->nInternalObjects(), nbAttr = hdf_file->nAttributes(); 

  FILE* fp = fopen(aFileName.ToCString(), "w");
  fprintf(fp, "%s\n", ASCIIHDF_ID);
  fprintf(fp, "%i\n", nbsons+nbAttr);

  for(unsigned j=0; j<nbAttr; j++) {
    char* attr_name = hdf_file->GetAttributeName(j);
    HDFattribute *hdf_attribute = new HDFattribute(attr_name, hdf_file);
    delete attr_name;
    SaveAttributeInASCIIfile(hdf_attribute, fp, 0);
    hdf_attribute = 0;
  }

  for (Standard_Integer i=0; i<nbsons; i++) {
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
    OSD_Path anOSDPath(aFileName);
    OSD_File anOSDFile(anOSDPath);
    if(anOSDFile.Exists())
      anOSDFile.Move(aPath);
    else 
      return NULL;
  }

  int length = strlen(aPath.ToCString());
  char *new_str = new char[ 1+length ];
  strcpy(new_str , aPath.ToCString()) ;

  return new_str;
}


//============================================================================
// function : SaveGroupInASCIIfile
// purpose  : 
//============================================================================
void SaveGroupInASCIIfile(HDFgroup *hdf_group, FILE* fp, int ident)
{
  hdf_group->OpenOnDisk();

  TCollection_AsciiString anIdent(ident, '\t');
  int nbsons = hdf_group->nInternalObjects(), nbAttr = hdf_group->nAttributes(); 

  /*fprintf(fp, "%s%s\n", anIdent.ToCString(), GROUP_ID);*/
  fprintf(fp, "%s\n", GROUP_ID);

  char* name = makeName(hdf_group->GetName());

  /*fprintf(fp, "%s%s %i\n", anIdent.ToCString(), name, nbsons+nbAttr);*/
  fprintf(fp, "%s %i\n", name, nbsons+nbAttr);
  delete name;

  for(unsigned j=0; j<nbAttr; j++) {
    name = hdf_group->GetAttributeName(j);
    HDFattribute *hdf_attribute = new HDFattribute(name, hdf_group);
    delete name;
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

  /*fprintf(fp, "%s%s\n", anIdent.ToCString(), GROUP_ID_END);*/
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

  long size =  hdf_dataset->GetSize();
  long ndim = hdf_dataset->nDim(); //Get number of dimesions
  hdf_size *dim = new hdf_size[ndim];
  hdf_type type = hdf_dataset->GetType();
  hdf_byte_order order = hdf_dataset->GetOrder();
  int nbAttr = hdf_dataset->nAttributes(); 

  TCollection_AsciiString anIdent(ident, '\t');
  TCollection_AsciiString anIdentChild(ident+1, '\t');

  char* name = makeName(hdf_dataset->GetName());

  /*fprintf(fp, "%s%s\n", anIdent.ToCString(), DATASET_ID);*/
  fprintf(fp, "%s\n", DATASET_ID);
  /*fprintf(fp, "%s%s %i %i\n", anIdent.ToCString(), name, type, nbAttr);*/
  fprintf(fp, "%s %i %i\n", name, type, nbAttr);
  delete name;

  hdf_dataset->GetDim(dim);
  /*fprintf(fp, "%s %i\n", anIdentChild.ToCString(), ndim);*/
  fprintf(fp, " %i\n", ndim);

  for(int i = 0;i < ndim;i++) {
    /*fprintf(fp, "%s%i",  anIdentChild.ToCString(), dim[i]);*/
    fprintf(fp, " %i", dim[i]);
  }

  /*fprintf(fp, "%s\n", anIdentChild.ToCString());*/
  fprintf(fp, "\n");
  delete dim;

  /*fprintf(fp, "%s%li:", anIdentChild.ToCString(), size);*/
//   fprintf(fp, "%li:", size);
  fprintf(fp, "%li %i:", size, order);

  if (type == HDF_STRING) {	
    char* val = new char[size];
    hdf_dataset->ReadFromDisk(val);
    fwrite(val, 1, size, fp);
    delete val;
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
    delete val;
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
    delete val;
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
    delete val;
  }
  
  fprintf(fp, "\n");

#ifndef WNT
  for(unsigned j=0; j<nbAttr; j++) {
#else
  for(j=0; j<nbAttr; j++) {
#endif
    name = hdf_dataset->GetAttributeName(j);
    HDFattribute *hdf_attribute = new HDFattribute(name, hdf_dataset);
    delete name;
    SaveAttributeInASCIIfile(hdf_attribute, fp, ident+1);
    hdf_attribute = 0;
  }

  /*fprintf(fp, "%s%s\n", anIdent.ToCString(), DATASET_ID_END); */
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

  TCollection_AsciiString anIdent(ident, '\t');
  TCollection_AsciiString anIdentChild(ident+1, '\t');

  char* name = makeName(hdf_attribute->GetName());
  int size = hdf_attribute->GetSize();

  /*fprintf(fp, "%s%s\n", anIdent.ToCString(), ATTRIBUTE_ID);*/
  fprintf(fp, "%s\n", ATTRIBUTE_ID);
  /*fprintf(fp, "%s%s %i %i\n", anIdent.ToCString(), name, type, size);*/
  fprintf(fp, "%s %i %i\n", name, type, size);

  delete name;

  if (type == HDF_STRING) {    
    char* val = new char[size+1];
    hdf_attribute->ReadFromDisk(val);
    /*fprintf(fp, "%s:", anIdentChild.ToCString());*/
    fprintf(fp, ":");
    fwrite(val, 1, size, fp);
    fprintf(fp, "\n");
    delete val;
  } else if (type == HDF_FLOAT64) {
    hdf_float64 val;
    hdf_attribute->ReadFromDisk(&val);
    /*fprintf(fp, "%s",  anIdentChild.ToCString());*/
    write_float64(fp, &val);
    fprintf(fp, "\n");
  } else if(type == HDF_INT64) {
    hdf_int64 val;
    hdf_attribute->ReadFromDisk(&val);
    /*fprintf(fp, "%s%li \n", anIdentChild.ToCString(), val);*/
    fprintf(fp, "%li \n", val);
  } else if(type == HDF_INT32) {
    hdf_int32 val;
    hdf_attribute->ReadFromDisk(&val);
    /*fprintf(fp, "%s%i \n", anIdentChild.ToCString(), val);*/
    fprintf(fp, "%i \n", val);
  }

  /*fprintf(fp, "%s%s\n", anIdent.ToCString(), ATTRIBUTE_ID_END);*/
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
char* HDFascii::ConvertFromASCIIToHDF(const char* thePath)
{
  // Get a temporary directory to store a file
  TCollection_AsciiString aTmpDir = GetTmpDir(), aFileName("hdf_from_ascii.hdf");
  // Build a full file name of temporary file
  TCollection_AsciiString aFullName = aTmpDir + aFileName;

  HDFfile *hdf_file = new HDFfile(aFullName.ToCString()); 
  hdf_file->CreateOnDisk();
  
  FILE *fp = fopen(thePath, "r");
  if(!fp) return NULL;

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
	cout << "ConvertFromASCIIToHDF : Can not create group number " << i << endl;
	return NULL;
      }
    }
    else if(strcmp(id_of_begin, DATASET_ID) == 0) {
      if(!CreateDatasetFromASCII(hdf_file, fp)) {
	cout << "ConvertFromASCIIToHDF :Can not create dataset number " << i << endl;
	return NULL;
      }
    }
    else if(strcmp(id_of_begin, ATTRIBUTE_ID) == 0) {
      if(!CreateAttributeFromASCII(hdf_file, fp)) {
	cout << "ConvertFromASCIIToHDF :Can not create attribute number " << i << endl;
	return NULL;
      }
    }
    else 
      cout << "ConvertFromASCIIToHDF : Unrecognized type " << id_of_begin << endl; 
  }

  char id_of_end[MAX_ID_SIZE];
  fscanf(fp, "%s", id_of_end);
  if(strcmp(id_of_end, ASCIIHDF_ID_END) != 0) {
    cout << "ConvertFromASCIIToHDF : Can not find the end ASCII token " << endl;
    return false;  
  }

  hdf_file->CloseOnDisk();
  delete hdf_file;

  int length = strlen(aTmpDir.ToCString());
  char *new_str = new char[ 1+length ];
  strcpy(new_str , aTmpDir.ToCString()) ;

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

  delete new_name;

  hdf_group->CreateOnDisk();

  for(i = 0; i < nbsons; i++) {
    char id_of_begin[MAX_ID_SIZE];
    fscanf(fp, "%s\n", id_of_begin);
    
    if(strcmp(id_of_begin, GROUP_ID) == 0) {
      if(!CreateGroupFromASCII(hdf_group, fp)) {
	cout << "Can not create subgroup " << i << " for group " << name << endl;
	return false;
      }
    }
    else if(strcmp(id_of_begin, DATASET_ID) == 0) {
      if(!CreateDatasetFromASCII(hdf_group, fp)) {
	cout << "Can not create dataset " << i << " for group " << name << endl;
	return false;
      }
    }
    else if(strcmp(id_of_begin, ATTRIBUTE_ID) == 0) {
      if(!CreateAttributeFromASCII(hdf_group, fp)) {
	cout << "Can not create attribute " << i << " for group " << name << endl;
	return false;
      }
    }
    else 
      cout << "CreateGroupFromASCII : Unrecognized type " << id_of_begin << endl; 
  }
  
  hdf_group->CloseOnDisk();
  hdf_group = 0; //will be deleted by father destructor

  char id_of_end[MAX_ID_SIZE];
  fscanf(fp, "%s\n", id_of_end);
  if(strcmp(id_of_end, GROUP_ID_END) != 0) {
    cout << "CreateGroupFromASCII : Invalid end token : " << id_of_end << endl;
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

  HDFdataset* hdf_dataset = new HDFdataset(new_name, father,type, sizeArray, nbDim, order);
  delete new_name;
  delete sizeArray;

  hdf_dataset->CreateOnDisk();

  if (type == HDF_STRING) {	
    char *val = new char[size+1];
    fread(val, 1, size, fp);
    hdf_dataset->WriteOnDisk(val);
    delete val;
  } else if (type == HDF_FLOAT64) {
    hdf_float64* val = new hdf_float64[size];
    for(i=0; i<size; i++) {
      read_float64(fp, &(val[i]));
    }
    hdf_dataset->WriteOnDisk(val);
    delete val;
  } else if(type == HDF_INT64) {
    hdf_int64* val = new hdf_int64[size];
    for(i=0; i<size; i++) {
      fscanf(fp, " %li", &(val[i]));
    }
    hdf_dataset->WriteOnDisk(val);
    delete val;
  } else if(type == HDF_INT32) {
    hdf_int32* val = new hdf_int32[size];
    for(i=0; i<size; i++) {
      fscanf(fp, " %i", &(val[i]));
    }
    hdf_dataset->WriteOnDisk(val);
    delete val;
  }

  char token[MAX_ID_SIZE];

  for(i = 0; i < nbAttr; i++) {
    fscanf(fp, "%s\n", token);
    
    if(strcmp(token, ATTRIBUTE_ID) == 0) {
      if(!CreateAttributeFromASCII(hdf_dataset, fp)) {
	cout << "Can not create attribute " << i << " for dataset " << name << endl;
	return false;
      }
    }
    else {
      cout << "CreateGroupFromASCII : Unrecognized type " << token << endl; 
      return false;
    }
  }
  
  fscanf(fp, "%s\n", token);
  if(strcmp(token, DATASET_ID_END) != 0) {
    cout << "CreateDatasetFromASCII : Invalid end token : " << token << endl;
    return false;
  }

  hdf_dataset->CloseOnDisk();
  hdf_dataset = 0; //will be deleted by father destructor

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

  delete new_name;
  
  if (type == HDF_STRING) {	
    char tmp;
    fscanf(fp, "%c", &tmp);
    char *val = new char[size+1];
    val[size] = (char)0;
    fread(val, 1, size, fp);
    hdf_attribute->WriteOnDisk(val);
    delete val;
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
    cout << "CreateAttributeFromASCII : Invalid end token : " << id_of_end << endl;
    return false;
  }

  return true;
}


//============================================================================
// function : GetTempDir
// purpose  : Return a temp directory to store created files like "/tmp/sub_dir/" 
//============================================================================ 
char* GetTmpDir()
{
  //Find a temporary directory to store a file

  TCollection_AsciiString aTmpDir;

#ifdef WNT
  char *aTmp;
  aTmp = getenv("TMP");
  if(aTmp != NULL)
	aTmpDir = TCollection_AsciiString(aTmp);
  else
	aTmpDir = TCollection_AsciiString("C:\\");
#else
  aTmpDir = TCollection_AsciiString("/tmp/");
#endif

  srand((unsigned int)time(NULL));

  int aRND = 999 + (int)(100000.0*rand()/(RAND_MAX+1.0)); //Get a random number to present a name of a sub directory
  TCollection_AsciiString aSubDir(aRND);
  if(aSubDir.Length() <= 1) aSubDir = TCollection_AsciiString("123409876");

  aTmpDir += aSubDir; //Get RND sub directory

#ifdef WIN32
  if(aTmpDir.Value(aTmpDir.Length()) != '\\') aTmpDir+='\\';
#else
  if(aTmpDir.Value(aTmpDir.Length()) != '/') aTmpDir+='/';
#endif

  OSD_Path aPath(aTmpDir);
  OSD_Directory aDir(aPath);

  for(aRND = 0; aDir.Exists(); aRND++) {
    aTmpDir.Insert((aTmpDir.Length() - 1), TCollection_AsciiString(aRND));  //Build a unique directory name
    aPath = OSD_Path(aTmpDir);
    aDir = OSD_Directory(aPath);
  }

  OSD_Protection aProtection(OSD_RW, OSD_RWX, OSD_RX, OSD_RX);
  aDir.Build(aProtection);

  int length = strlen(aTmpDir.ToCString());
  char *new_str = new char[ 1+length ];
  strcpy(new_str , aTmpDir.ToCString());

  return new_str;
}

char* makeName(char* name)
{
  TCollection_AsciiString aName(name), aNewName;
  Standard_Integer i, length = aName.Length();
  char replace = (char)19;

  for(i=1; i<=length; i++) {
    if(aName.Value(i) == ' ') aNewName+=replace;
    else aNewName += aName.Value(i);
  }

  length = strlen(aNewName.ToCString());
  char *new_str = new char[ 1+length ];
  strcpy(new_str , aNewName.ToCString()) ;
  return new_str;
}

char* restoreName(char* name)
{
  TCollection_AsciiString aName(name), aNewName;
  Standard_Integer i, length = aName.Length();
  char replace = (char)19;

  for(i=1; i<=length; i++) {
    if(aName.Value(i) == replace) aNewName+=' ';
    else aNewName += aName.Value(i);
  }

  length = strlen(aNewName.ToCString());
  char *new_str = new char[ 1+length ];
  strcpy(new_str , aNewName.ToCString()) ;
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
