#ifndef DFDOCUMENT_HXX
#define DFDOCUMENT_HXX

#include "DF_definitions.hxx"
#include "DF_Label.hxx"

#include <string>

class DF_Application;

//Class DF_Document is container for user's data stored as a tree of Labels
//with assigned Attributes
class DF_Document {
public:
  //Constructor
  Standard_EXPORT DF_Document(const std::string& theDocumentType);

  Standard_EXPORT ~DF_Document();

  Standard_EXPORT DF_Application* GetApplication();    

  //Returns a Label of this Document with entry "0:1"
  Standard_EXPORT DF_Label Main();

  //Returns a root Label with entry "0:"
  Standard_EXPORT DF_Label Root();

  //Returns an ID of this 
  Standard_EXPORT int GetDocumentID() const;

  //Returns a type of the Document
  Standard_EXPORT std::string GetDocumentType();

  //Clears the content of this Document
  Standard_EXPORT void Clear();

  //Returns true if this document is empty
  Standard_EXPORT bool IsEmpty();

  //Returns true if this document is modified
  Standard_EXPORT bool IsModified();

  //Returns true if this document is modified
  Standard_EXPORT void SetModified(bool isModified); 

  //########### Load/Save virtual methods ##

  //Restores a content of the Document from the std::string theData
  Standard_EXPORT virtual void Load(const std::string& theData);

  //Converts a content of the Document into the std::string
  Standard_EXPORT virtual std::string Save();

  friend class DF_Application;

private:
  DF_Label    _main;
  DF_Label    _root;
  std::string _type;
  int         _id;
  bool        _modified;
  DF_Application* _appli;
};

#endif
