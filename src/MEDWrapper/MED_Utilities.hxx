//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : VISU_DatConvertor.hxx
//  Author : Alexey PETROV
//  Module : VISU

#ifndef MED_Utilities_HeaderFile
#define MED_Utilities_HeaderFile

#include <iostream>	
#include <sstream>	
#include <string>
#include <stdexcept>


namespace MED{
  using namespace std;
  class PrefixPrinter{
    static int myCounter;
  public:
    PrefixPrinter();
    ~PrefixPrinter();

    static string GetPrefix();
  };
};


#ifndef MESSAGE

#define MESSAGE(msg) std::cout<<__FILE__<<"["<<__LINE__<<"]::"<<msg<<endl;

#define BEGMSG(msg) std::cout<<MED::PrefixPrinter::GetPrefix()<<msg

#define ADDMSG(msg) std::cout<<msg

#endif


#ifndef EXCEPTION

#define EXCEPTION(TYPE, MSG) {\
  std::ostringstream aStream;\
  aStream<<__FILE__<<"["<<__LINE__<<"]::"<<MSG;\
  throw TYPE(aStream.str());\
}

#endif

#endif
