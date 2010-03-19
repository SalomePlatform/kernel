//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
#include "SALOME_LoadRateManager.hxx"
#include <iostream>
#include <map>

using namespace std;

string LoadRateManagerFirst::Find(const vector<string>& hosts,
                                  MapOfParserResourcesType& resList)
{
  if (hosts.size() == 0)
    return string("");

  return string(hosts[0]);
}

string LoadRateManagerCycl::Find(const vector<string>& hosts,
                                 MapOfParserResourcesType& resList)
{
  static int imachine = 0;
  static int iproc = 0;

  // if empty list return empty string
  if (hosts.size() == 0)
    return string("");
  else{
    ParserResourcesType resource = resList[string(hosts[imachine])];
    int nbproc = resource.DataForSort._nbOfProcPerNode * resource.DataForSort._nbOfNodes;
    if( nbproc <= 0) nbproc = 1;
    if( iproc < nbproc ){
      iproc++;
      return string(hosts[imachine]);
    }
    else{
      iproc = 1;
      imachine++;
      if(imachine >= (int)hosts.size())
        imachine = 0;
      return string(hosts[imachine]);
    }
  }
}

string LoadRateManagerAltCycl::Find(const vector<string>& hosts,
                                    MapOfParserResourcesType& resList)
{
  if (hosts.size() == 0)
    return string("");

  std::string selected=hosts[0];
  int uses=0;
  if(_numberOfUses.count(selected) != 0)
    uses=_numberOfUses[selected];
  else
    uses=0;

  for (std::vector<std::string>::const_iterator iter = hosts.begin(); iter != hosts.end(); iter++)
    {
      std::string machine=*iter;
      if(_numberOfUses.count(machine) == 0)
        _numberOfUses[machine]=0;
      if(_numberOfUses[machine] < uses)
        {
          selected=machine;
          uses=_numberOfUses[machine];
        }
    }

  _numberOfUses[selected]=_numberOfUses[selected]+1;
  std::cerr << "selected: " << selected << " " << _numberOfUses[selected] << std::endl;
  return selected;
}

