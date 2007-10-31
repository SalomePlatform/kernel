#ifndef DFCONTAINER_HXX
#define DFCONTAINER_HXX

#include "DF_definitions.hxx"
#include "DF_Attribute.hxx"
#include <string>
#include <map>

//Class DF_Container is used to store several types of data 
class DF_Container : public DF_Attribute 
{
public:

  //Static method that returns an ID of the give type of attributes
  Standard_EXPORT static const std::string& GetID();

  //Creates if not exists a Container attribute and places if is not placed it the Label
  Standard_EXPORT static DF_Container* Set(DF_Label& theLabel);

  //Constructor
  Standard_EXPORT DF_Container();

  //Destructor
  Standard_EXPORT ~DF_Container();

  //Sets an integer value of the attribute with given ID
  Standard_EXPORT void SetInt(const std::string& theID, int theValue);

  //Returns an integer value of the attribute with given ID
  Standard_EXPORT int GetInt(const std::string& theID);

  //Returns True if there is an integer with given ID
  Standard_EXPORT bool HasIntID(const std::string& theID);

  //Sets a double value of the attribute with given ID
  Standard_EXPORT void SetDouble(const std::string& theID, const double& theValue);

  //Returns a double value of the attribute with given ID
  Standard_EXPORT double GetDouble(const std::string& theID);

  //Returns True if there is a double with given ID
  Standard_EXPORT bool HasDoubleID(const std::string& theID);
 
  //Sets a string value of the attribute with given ID
  Standard_EXPORT void SetString(const std::string& theID, const std::string& theValue);

  //Returns a string  value of the attribute with given ID
  Standard_EXPORT std::string GetString(const std::string& theID);

  //Returns True if there is a string with given ID
  Standard_EXPORT bool HasStringID(const std::string& theID);

  //Sets a boolean value of the attribute with given ID
  Standard_EXPORT void SetBool(const std::string& theID, bool theValue);

  //Returns a boolean value of the attribute with given ID
  Standard_EXPORT bool GetBool(const std::string& theID);

  //Returns True if there is a boolean value with given ID
  Standard_EXPORT bool HasBoolID(const std::string& theID);

  //Clears a content of the attribute
  Standard_EXPORT void Clear();

  //ID is a std::string that uniquely identify the given type of Attributes within the Application.
  Standard_EXPORT virtual const std::string& ID() const;

  //Restores a content of this Attribute from another Attribute
  Standard_EXPORT virtual  void Restore(DF_Attribute* theAttribute);

  //Creates a new empty copy of this Attribute
  Standard_EXPORT virtual DF_Attribute* NewEmpty() const;

  //Pastes a content of this Attribute into another Attribute 
  Standard_EXPORT virtual  void Paste(DF_Attribute* theIntoAttribute);

  
protected:
  std::map<std::string, int>         _ints;
  std::map<std::string, double>      _doubles;
  std::map<std::string, std::string> _strings;
  std::map<std::string, bool>        _bools;
};

#endif
