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

//File:    testDF.cxx
//Author:  Sergey RUIN
//
#include <stdio.h>
#include <iostream> 
#include <vector>
#include <string>
#include <string.h>

#include "DF_definitions.hxx"
#include "DF_Application.hxx"
#include "DF_Document.hxx"
#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include "DF_Container.hxx"
#include "DF_ChildIterator.hxx"

#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#else
#include <time.h>
#include <windows.h>
#include <lmcons.h>
#endif


void printStr(const std::string& theValue)
{
  std::cout << "printStr: " << theValue   << std::endl;
}

void GetSystemDate(int& year, int& month, int& day, int& hours, int& minutes, int& seconds)
{
#ifdef WIN32
  SYSTEMTIME    st;

  GetLocalTime ( &st );

  month = st.wMonth;
  day = st.wDay;
  year = st.wYear;
  hours = st.wHour;
  minutes = st.wMinute;
  seconds = st.wSecond;
#else
  struct tm transfert;
  struct timeval tval;
  struct timezone tzone;
  int status;

  status = gettimeofday( &tval, &tzone );
  memcpy(&transfert, localtime((time_t *)&tval.tv_sec), sizeof(tm));
  
  month    = transfert.tm_mon + 1;
  day      = transfert.tm_mday;
  year     = transfert.tm_year + 1900;
  hours    = transfert.tm_hour;
  minutes  = transfert.tm_min ;
  seconds  = transfert.tm_sec ;
#endif
}

std::string GetUserName()
{
#ifdef WIN32
  char*  pBuff = new char[UNLEN + 1];
  DWORD  dwSize = UNLEN + 1;
  std::string retVal;
  GetUserName ( pBuff, &dwSize );
  std::string theTmpUserName(pBuff,(int)dwSize -1 );
  retVal = theTmpUserName;
  delete [] pBuff;
  return retVal;
#else
 struct passwd *infos;
 infos = getpwuid(getuid()); 
 return std::string(infos->pw_name);
#endif
}

std::string GetNameFromPath(const std::string& thePath) {
  if (thePath.empty()) return "";
  int pos1 = thePath.rfind('/');
  int pos2 = thePath.rfind('\\');
  if(pos1 > 0) return thePath.substr(pos1+1, thePath.size()); 
  if(pos2 > 0) return thePath.substr(pos2+1, thePath.size()); 
  return thePath;
}

std::string GetDirFromPath(const std::string& thePath) {
  if (thePath.empty()) return "";

  int pos = thePath.rfind('/');
  std::string path;
  if(pos > 0) {
    path = thePath.substr(0, pos+1);
  }
  if(path.empty()) {
    pos = thePath.rfind('\\');
    if(pos > 0) path = thePath.substr(0, pos+1); 
  }
  if(path.empty()) {
    pos = thePath.rfind('|');
    if(pos > 0) path = thePath.substr(0, pos+1); 
  }
  if(path.empty()) {
    path = thePath+"/";
  }
  
#ifdef WIN32  //Check if the only disk letter is given as path
  if(path.size() == 2 && path[1] == ':') path +='\\';
#endif

  for(int i = 0, len = path.size(); i<len; i++) 
    if(path[i] == '|') path[i] = '/';
  return path;
}


bool Exists(const std::string thePath) 
{
#ifdef WIN32 
  if (  GetFileAttributes (  thePath.c_str()  ) == 0xFFFFFFFF  ) { 
    if (  GetLastError () != ERROR_FILE_NOT_FOUND  ) {
      return false;
    }
  }
#else 
  int status = access ( thePath.c_str() , F_OK ); 
  if (status != 0) return false;
#endif
  return true;
}


std::string divideString(const std::string& theValue, int nbChars)
{
  return theValue.substr(nbChars, theValue.size());
}

std::vector<std::string> splitString(const std::string& theValue, char separator)
{
  std::vector<std::string> vs;
  if(theValue[0] == separator && theValue.size() == 1) return vs;
  int pos = theValue.find(separator);
  if(pos < 0) {
    vs.push_back(theValue);
    return vs;
  }
 
  std::string s = theValue;
  if(s[0] == separator) s = s.substr(1, s.size());
  while((pos = s.find(separator)) >= 0) {
    vs.push_back(s.substr(0, pos));
    s = s.substr(pos+1, s.size());
  }
               
  if(!s.empty() && s[0] != separator) vs.push_back(s);
  return vs;
}


int main (int argc, char * argv[])
{
  std::cout << "Test started " << std::endl;
  
  DF_Application* appli = new DF_Application;
  /*  
  DF_Document* doc1 = appli->NewDocument("doc_1");
  
  DF_Label root1 = doc1->Main();
  DF_Label child = root1.FindChild(3, true); //0:1:3
  
  DF_Container* attr1 = DF_Container::Set(child);
  attr1->SetInt("eighteen", 18);


  DF_Attribute* attr = NULL;
  if(!(attr = child.FindAttribute(DF_Container::GetID()))) {
    std::cout << "Attribute wasn't found" << std::endl;
  }
  else {
    attr1 = dynamic_cast<DF_Container*>(attr);
    std::cout << "Attribute was found " << " HasID " << attr1->HasIntID("eighteen") << " value = " << attr1->GetInt("eighteen")<< std::endl;
  }
  
  DF_Container *attr2 = (DF_Container*)child.FindAttribute(DF_Container::GetID());    

  if(!attr2) cout << "Can't find the attribute" << endl;

  std::cout << "Change find : " << attr2->GetInt("eighteen") << std::endl;


  std::cout << "Forgetting " << child.ForgetAttribute(DF_Container::GetID()) << std::endl;
   if(!child.FindAttribute(DF_Container::GetID())) {
    std::cout << "Attribute wasn't found" << std::endl;
  }


  child = root1.NewChild(); //0:1:4
  
  child.NewChild();//0:1:4:1
    
  child.NewChild();//0:1:4:2

  std::cout << "Is equal " << (child == child)   << std::endl;
  std::cout << "Is equal " << (child == root1)   << std::endl;

  child = root1.NewChild(); //0:1:5

  child.NewChild();//0:1:5:1
  root1.NewChild();//0:1:6
  

  DF_ChildIterator CI(root1.Father(), true);
  //root1.dump();
  for(; CI.More(); CI.Next()) {
    std::cout << CI.Value().Entry() << std::endl;
    //CI.Value().dump();
  }

  DF_Label L = DF_Label::Label(child, "0:1:4:1");
  std::cout << "Found Label " <<  L.Entry()   << std::endl;

  std::string s("012-56");
  
  int pos = s.find('-');
  std::cout << "Fisrt part : " << s.substr(0, pos) << std::endl;
  std::cout << "Last part : " << s.substr(pos+1, s.size()) << std::endl;

  std::vector<std::string> vs = splitString("/dn20/salome/srn/salome2/", '/');
  for(int i = 0; i<vs.size(); i++)
    std::cout << vs[i] << std::endl;

  std::cout << "Diveded str = " << divideString("abcdefg",3) << std::endl;
  
  //mkdir("/dn20/salome/srn/salome2", 0x1ff);

  //cout << "Exists " <<  Exists("/dn20/salome/srn/salome2") << endl;

  //cout << GetDirFromPath("/dn20/salome/srn/salome2/test.hdf") << endl;
  //cout << GetDirFromPath("D:\\salome\\srn\\salome2\\test.hdf") << endl;
  //cout << GetDirFromPath("D:") << endl;
  //cout << GetDirFromPath("/dn20") << endl; 
  //cout << GetDirFromPath("..") << endl;
  //cout << GetDirFromPath("D:\\") << endl;
  //cout << GetDirFromPath("D:\\test.hdf") << endl;

  cout << "User : " << GetUserName() << endl;
  
  int month=0,day=0,year=0,hh=0,mn=0,ss=0;
  GetSystemDate(year, month, day, hh, mn, ss);
  std::cout << "Date: " << year << " " << month << " " << day << " " << hh << " " << mn << " " << ss << std::endl;

  std::string t("absd");
  t.insert(t.begin(), 'b');
  cout << "Result = " << t   << endl;
  char* cstr = (char*)t.c_str();
  printStr(cstr+1);
 
  */

  DF_Document* doc2 = appli->NewDocument("doc_2");

  DF_Label root2 = doc2->Main();
  DF_Label sco = root2.NewChild();              //0:1:1
  DF_Label so1 = sco.FindChild(3, true);        //0:1:1:3
  DF_Label so5 = so1.FindChild(5, true);        //0:1:1:5
  DF_Label so51 = so5.NewChild();               //0:1:1:5:1
  DF_Label so511 = so51.NewChild();             //0:1:1:5:1:1
  DF_Label so513 = so51.FindChild(3, true);     //0:1:1:5:1:3
  DF_Label so5131 = so513.NewChild();           //0:1:1:5:1:3:1
  

  so51.FindChild(2, true);


  DF_ChildIterator CI2(so5, true);
  so5.dump();
  for(; CI2.More(); CI2.Next()) {
    std::cout << " ###### Found child with entry = " << CI2.Value().Entry() << std::endl;
    //CI2.Value().dump();
  }

  delete appli;    

  std::cout << "Test finished " << std::endl;    
  return 0;
}

