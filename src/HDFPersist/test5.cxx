using namespace std;
#include <iostream.h>
#include "HDFOI.hxx"
#include <stdlib.h>


int main()
{
  HDFfile *hdf_file;
  HDFgroup *hdf_group;

  try
  {
    // A file study.hdf with 2 groups : MESH and GEOM
    hdf_file = new HDFfile("study.hdf");

    hdf_file->CreateOnDisk();

    hdf_group = new HDFgroup("GEOM",hdf_file);

    hdf_group->CreateOnDisk();

    hdf_group->CloseOnDisk();

    delete hdf_group;

    hdf_group = new HDFgroup("MESH",hdf_file); 

    hdf_group->CreateOnDisk();

    hdf_group->CloseOnDisk();

    delete hdf_group;    

    hdf_file->CloseOnDisk();
    
    delete hdf_file;

    // a file mesh.hdf with 2 groups MESH_1 and MESH_2
    hdf_file = new HDFfile("mesh.hdf");

    hdf_file->CreateOnDisk();

    hdf_group = new HDFgroup("MESH_1",hdf_file);

    hdf_group->CreateOnDisk();

    hdf_group->CloseOnDisk();

    delete hdf_group;

    hdf_group = new HDFgroup("MESH_2",hdf_file);

    hdf_group->CreateOnDisk();

    hdf_group->CloseOnDisk();

    delete hdf_group;    

    hdf_file->CloseOnDisk();
    
    delete hdf_file;

    // a file geom.hdf with 2 groups GEOM_1 and GEOM_2
    hdf_file = new HDFfile("geom.hdf");

    hdf_file->CreateOnDisk();

    hdf_group = new HDFgroup("GEOM_1",hdf_file);

    hdf_group->CreateOnDisk();

    hdf_group->CloseOnDisk();

    delete hdf_group;

    hdf_group = new HDFgroup("GEOM_2",hdf_file);

    hdf_group->CreateOnDisk();

    hdf_group->CloseOnDisk();

    delete hdf_group;    

    hdf_file->CloseOnDisk();
    
    delete hdf_file;
  }
  catch (HDFexception)
    {
      MESSAGE( "!!!! HDFexception !!!" )
    }

  return 0;
}
