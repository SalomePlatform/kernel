#include "DF_definitions.hxx"
#include "DF_Application.hxx"

using namespace std;

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
DF_Document* DF_Application::NewDocument(const string& theDocumentType) 
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
vector<int> DF_Application::GetDocumentIDs()
{
  vector<int> ids;
 typedef map<int, DF_Document*>::const_iterator DI;
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
DF_Document* DF_Application::Open(const string& theFileName)
{
  //Not implemented
  return NULL;
}


//Saves a Document in a given file with name theFileName
void DF_Application::SaveAs(const DF_Document* theDocument, const string& theFileName)
{
  //Not implemented
}
