//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.cxx
//  Author : Alexey PETROV
//  Module : VISU

#include "MED_Utilities.hxx"

using namespace std;

#ifdef _DEBUG_
static int MYDEBUG = 1;
#else
static int MYDEBUG = 0;
#endif


int MED::PrefixPrinter::myCounter = 0;

MED::PrefixPrinter::PrefixPrinter()
{
  myCounter++;
}

MED::PrefixPrinter::~PrefixPrinter()
{
  myCounter--;
}

string MED::PrefixPrinter::GetPrefix()
{
  if(myCounter)
    return string(myCounter*2,' ');
  return "";
}
