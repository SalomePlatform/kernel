// Copyright (C) 2011-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include <iostream>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>
#define TIMEOUT 20
#define EPSILON 0.00000001

#ifndef WIN32
# include <unistd.h>
#endif

int main(int argc, char**argv)
{
  int *indg;
  double *vector, sum=0., norm=1., etalon=0.;
  int rank, size, grank, gsize, rsize;
  int vsize=20, lvsize, rlvsize;
  int i, k1, k2, imin, imax, nb;
  int srv=0;
  MPI_Comm com, icom;
  MPI_Status status;
  MPI_Info info;
  char   port_name     [MPI_MAX_PORT_NAME]; 
  char   port_name_clt [MPI_MAX_PORT_NAME]; 
  std::string service = "SERVICE";
  bool debug=false;

  for(i=1;i<argc;i++){
    std::string sargv = argv[i];
    if(sargv.find("-debug")!=std::string::npos)
      debug = true;
    else if(sargv.find("-vsize")!=std::string::npos)
      vsize = atoi(argv[++i]);
  }

  MPI_Init( &argc, &argv );

  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
#ifdef HAVE_MPI2
  MPI_Info_create(&info);
  MPI_Info_set(info, "ompi_unique", "true");
  if(rank==0){
    MPI_Open_port(MPI_INFO_NULL, port_name); 
    if ( MPI_Lookup_name((char*)service.c_str(), MPI_INFO_NULL, port_name_clt) == MPI_SUCCESS )  {
      if(debug)
        std::cout << "[" << rank << "] I am client: I get the service " << service << " !" << std::endl;
      MPI_Close_port( port_name );
    } 
    else if ( MPI_Publish_name((char*)service.c_str(), info, port_name) == MPI_SUCCESS )  {
      if(debug)
        std::cout << "[" << rank << "] I am server: I've managed to publish the service " << service << " !" << std::endl;
      srv = 1;
    }      
    else if ( MPI_Lookup_name((char*)service.c_str(), MPI_INFO_NULL, port_name_clt) == MPI_SUCCESS )  {
      if(debug)
        std::cout << "[" << rank << "] I am client: I get the service " << service << " !" << std::endl;;
      MPI_Close_port( port_name );
    } 
    else{
      if(debug)
        std::cout << "[" << rank << "] ERROR!!!" << std::endl;
      MPI_Finalize(); 
      exit(1);
    }
  }
  else{
    i = 0;
    while ( i != TIMEOUT  ) { 
      sleep(1);
      if ( MPI_Lookup_name((char*)service.c_str(), MPI_INFO_NULL, port_name_clt) == MPI_SUCCESS )  {
        if(debug)
          std::cout << "[" << rank << "] I am client: I get the service " << service << " !" << std::endl;
        break;
      } 
      i++;
    }
    if ( i == TIMEOUT ) {
      if(debug)
        std::cout << "[" << rank << "] Waiting too long exiting !" << std::endl;
      MPI_Finalize(); 
      exit(1);
    }
  }
  MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_ARE_FATAL);
  MPI_Bcast(&srv,1,MPI_INT,0,MPI_COMM_WORLD);
  if ( srv )
    MPI_Comm_accept( port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &icom );
  else
    MPI_Comm_connect(port_name_clt, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &icom );

  MPI_Intercomm_merge(icom,!srv,&com);

  MPI_Comm_rank( com, &grank );
  MPI_Comm_size( com, &gsize );

  MPI_Barrier(com);
  lvsize = ((rank+1)*vsize) / size - (rank*vsize) / size;
  vector = (double*)malloc(lvsize*sizeof(double));
  indg = (int*)malloc(lvsize*sizeof(int));
  rsize = gsize - size;

  for(i=0;i<lvsize;i++){
    indg[i] = (rank*vsize)/size + i;
    if(srv){
      if(debug)
        vector[i] = indg[i];
      else
        vector[i] = 2. * sin( (rank*vsize)/size + i );
      sum += vector[i]*vector[i];
    }
  }
  MPI_Barrier(com);
  if(srv){
    MPI_Reduce(&sum,&norm,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if(rank==0){
      norm = sqrt(norm);
      if(debug)
        std::cout << "[" << grank << "] norm=" << norm << std::endl;
    }
  }

  for(i=0;i<rsize;i++){
    rlvsize = ((i+1)*vsize) / rsize - (i*vsize) / rsize;
    k1 = (i*vsize)/rsize;
    k2 = ((i+1)*vsize)/rsize -1;

    if( (k1 <= indg[lvsize-1]) && (k2 >= indg[0]) ){
      imin = k1;
      if( indg[0] > imin ) imin = indg[0];
      imax = k2;
      if( indg[lvsize-1] < imax) imax = indg[lvsize-1];
      if(srv){
        nb = imax - imin + 1;
        MPI_Send( &nb, 1, MPI_INT, i+size, 100, com );
        MPI_Send( vector+imin-indg[0], nb, MPI_DOUBLE, i+size, 200, com );
      }
      else{
        MPI_Recv( &nb, 1, MPI_INT, i, 100, com, &status );
        MPI_Recv( vector+imin-indg[0], nb, MPI_DOUBLE, i, 200, com, &status );
      }
    }
  }

  MPI_Barrier(com);
  if(!srv){
    sum = 0.;
    sleep(grank);
    for(i=0;i<lvsize;i++){
      if(debug)
        std::cout << "[" << rank << "] vector[" << i << "]=" << vector[i] << std::endl;
      sum += vector[i]*vector[i];
    }
    MPI_Reduce(&sum,&norm,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if(rank==0){
      norm = sqrt(norm);
      if(debug)
        std::cout << "[" << grank << "] norm=" << norm << std::endl;
    }
  }

  MPI_Barrier(com);
  if(srv){
    if(rank==0){
      MPI_Recv(&etalon, 1, MPI_DOUBLE,size,400,com, &status);
      MPI_Send(&norm,1,MPI_DOUBLE, size, 300, com);
    }
  }
  else if(rank==0){
    MPI_Send(&norm,1,MPI_DOUBLE, 0, 400, com);
    MPI_Recv(&etalon, 1, MPI_DOUBLE,0,300,com, &status);
  }

  MPI_Barrier(com);
  if(rank!=0) srv = 0;

  MPI_Comm_disconnect( &com ); 
  if ( srv ) {
    MPI_Unpublish_name((char*)service.c_str(), MPI_INFO_NULL, port_name); 
    MPI_Close_port( port_name ); 
  }

  free(indg);
  free(vector);
#endif
  MPI_Finalize();

  if(rank==0){
    if(fabs(norm-etalon)/norm < EPSILON ){
      if(debug)
        std::cout << "OK" << std::endl;
      exit(0);
    }
    else{
      if(debug)
        std::cout << "KO" << std::endl;
      exit(1);
    }
  }

}
