// File: PathPrefix.hxx
// Created: Thu Jul 05 2001
// Author: Estelle Deville
// Project: SALOME
// Copyright : CEA/DEN/DMSS/LGLS
// $Header$

#ifndef PATH_PREFIX_H
#define PATH_PREFIX_H

#include <string>
#include <vector>

struct PathPrefix
{
  string path ;
  vector<string> ListOfComputer ;
} ;

typedef vector<PathPrefix> ListOfPathPrefix ;
	
#endif // PATH_PREFIX_H
