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

#include "DF_definitions.hxx"
#include "DF_Application.hxx"

//Constructor
DF_Application::DF_Application()
{
  _currentID = 0;
}
  
DF_Application::~DF_Application()
{
  _documents.clear();
}

//Creates a new document with given type, returns a smart pointer to
//newly created document.
DF_Document* DF_Application::NewDocument(const std::string& theDocumentType) 
{
  DF_Document* aDoc = new DF_Document(theDocumentType);
  aDoc->_id = ++_currentID;
  _documents[aDoc->_id] = aDoc;
  aDoc->_appli = this; 
  return aDoc;
}

//Closes and removes the given Document
void DF_Application::Close(const DF_Document* theDocument)
{
  int id = -1;
  if(theDocument) id = theDocument->GetDocumentID();

  if(_documents.find(id) != _documents.end()) {
    _documents[id]->Clear();
    _documents.erase(id);
    delete theDocument;
  }
}

//Returns a Document by Document's ID
DF_Document* DF_Application::GetDocument(int theDocumentID)
{
  if(_documents.find(theDocumentID) == _documents.end()) return NULL;

  return _documents[theDocumentID];
}

//Returns a list of IDs of all currently opened documents
std::vector<int> DF_Application::GetDocumentIDs()
{
  std::vector<int> ids;
  typedef std::map<int, DF_Document*>::const_iterator DI;
  for(DI p = _documents.begin(); p!=_documents.end(); p++)
    ids.push_back(p->first);
  return ids;
}

//Returns a number of existent documents
int DF_Application::NbDocuments()
{
  return _documents.size();
}


//Restores a Document from the given file, returns a smart 
//pointer to opened document.
DF_Document* DF_Application::Open(const std::string& theFileName)
{
  //Not implemented
  return NULL;
}


//Saves a Document in a given file with name theFileName
void DF_Application::SaveAs(const DF_Document* theDocument, const std::string& theFileName)
{
  //Not implemented
}
