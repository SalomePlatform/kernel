using namespace std;
#include "HDFcontainerObject.hxx"
#include "HDFexception.hxx"
#include "HDFinternalObject.hxx"
#include "HDFexplorer.hxx"

HDFexplorer::HDFexplorer(HDFcontainerObject *container)
{
  _container = container;
  _size = _container->nSons();
  _head = _container->GetFirstSon();
  _queue = _container->GetLastSon();
  _current = _head;
}

HDFexplorer::~HDFexplorer()
{
}

void HDFexplorer::Init()
{
  _current = _head;
}

void HDFexplorer::Next()
{
  _current = _current->GetNextBrother(); 
}

int HDFexplorer::More()
{
  if (_current != NULL)
    return 1;
  else
    return 0;
}

HDFinternalObject *HDFexplorer::Value()
{
  return _current;
}

void HDFexplorer::Reset(HDFcontainerObject *new_container)
{
  _container = new_container;
  _size = _container->nSons();
  _head = _container->GetFirstSon();
  _queue = _container->GetLastSon();
  _current = _head;
}
