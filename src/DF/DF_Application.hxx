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

#ifndef DFAPPLICATION_HXX
#define DFAPPLICATION_HXX

#include "DF_definitions.hxx"
#include "DF_Document.hxx"
#include <string>
#include <map>

//Class DF_Application responsible for creation and manipulation of Documents
class DF_Application {
public:
  //Constructor
  Standard_EXPORT DF_Application();

  Standard_EXPORT ~DF_Application();
  
  //Creates a new document with given type, returns a smart pointer to
  //newly created document.
  Standard_EXPORT DF_Document* NewDocument(const std::string& theDocumentType);

  //Closes and removes the given Document
  Standard_EXPORT void Close(const DF_Document* theDocument);

  //Returns a Document by Document's ID
  Standard_EXPORT DF_Document* GetDocument(int theDocumentID);

  //Returns a list of IDs of all currently opened documents
  Standard_EXPORT std::vector<int> GetDocumentIDs();

  //Returns a number of existent documents
  Standard_EXPORT int NbDocuments();

  //Virtual methods to be redefined if required by specific application

  //Restores a Document from the given file, returns a smart 
  //pointer to opened document.
  Standard_EXPORT virtual DF_Document* Open(const std::string& theFileName);

  //Saves a Document in a given file with name theFileName
  Standard_EXPORT virtual void SaveAs(const DF_Document* theDocument, const std::string& theFileName);

private:
  int                           _currentID;
  std::map<int, DF_Document*> _documents;

};
#endif
