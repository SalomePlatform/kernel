#include "SALOME_ResourcesCatalog_Parser.hxx"
#include <iostream>

#define NULL_VALUE 0

using namespace std;

unsigned int ResourceDataToSort::_nbOfNodesWanted=NULL_VALUE;
unsigned int ResourceDataToSort::_nbOfProcPerNodeWanted=NULL_VALUE;
unsigned int ResourceDataToSort::_CPUFreqMHzWanted=NULL_VALUE;
unsigned int ResourceDataToSort::_memInMBWanted=NULL_VALUE;

ResourceDataToSort::ResourceDataToSort()
{
}

ResourceDataToSort::ResourceDataToSort(const string& hostname,unsigned int nbOfNodes,unsigned int nbOfProcPerNode,unsigned int CPUFreqMHz,unsigned int memInMB):_hostName(hostname),_nbOfNodes(nbOfNodes),_nbOfProcPerNode(nbOfProcPerNode),_CPUFreqMHz(CPUFreqMHz),_memInMB(memInMB)
{
}

//! Method used by list::sort to sort the resources used in SALOME_ResourcesManager::GetResourcesFitting
bool ResourceDataToSort::operator< (const ResourceDataToSort& other) const
{
  unsigned int nbPts=GetNumberOfPoints();
  return nbPts<other.GetNumberOfPoints();
}

unsigned int ResourceDataToSort::GetNumberOfPoints() const
{
  unsigned int ret=0;
  //priority 1 : Nb of nodes
  if(_nbOfNodesWanted!=NULL_VALUE)
    {
      if(_nbOfNodes==_nbOfNodesWanted)
	ret+=3000;
      else if(_nbOfNodes>_nbOfNodesWanted)
	ret+=2000;
      else
	ret+=1000;
    }
  //priority 2 : Nb of proc by node
  if(_nbOfProcPerNodeWanted!=NULL_VALUE)
    {
      if(_nbOfProcPerNode==_nbOfProcPerNodeWanted)
	ret+=300;
      else if(_nbOfProcPerNode > _nbOfProcPerNodeWanted)
	ret+=200;
      else
	ret+=100;
    }
  //priority 3 : Cpu freq
  if(_CPUFreqMHzWanted!=NULL_VALUE)
    {
      if(_CPUFreqMHz==_CPUFreqMHzWanted)
	ret+=30;
      else if(_CPUFreqMHz > _CPUFreqMHzWanted)
	ret+=20;
      else
	ret+=10;
    }
  //priority 4 : memory
  if(_memInMBWanted!=NULL_VALUE)
    {
      if(_memInMB==_memInMBWanted)
	ret+=3;
      else if(_memInMB > _memInMBWanted)
	ret+=2;
      else
	ret+=1;
    }
  return ret;
}

//! Method used for debug
void ResourceDataToSort::Print() const
{
  cout << "Nb of nodes : " << _nbOfNodes << endl;
  cout << "Nb of proc per node : " <<  _nbOfProcPerNode << endl;
  cout << "CPU : " << _CPUFreqMHz << endl;
  cout << "Mem : " << _memInMB << endl;
}

