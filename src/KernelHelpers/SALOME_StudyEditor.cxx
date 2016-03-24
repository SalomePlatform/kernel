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
// Author: Guillaume Boulant (EDF/R&D) 

#include "SALOME_StudyEditor.hxx"
#include "SALOME_KernelServices.hxx"

/** Canonic constructor. The object can't be used without a setStudy() */
SALOME_StudyEditor::SALOME_StudyEditor() {
}

void SALOME_StudyEditor::setStudy(SALOMEDS::Study_ptr study) {
  _study = study;
  _sbuilder = _study->NewBuilder();    
}

void SALOME_StudyEditor::setStudyById(int studyId) {
  this->setStudy(KERNEL::getStudyManager()->GetStudyByID(studyId));
}

int SALOME_StudyEditor::getStudyId() {
  if ( _study->_is_nil() ) return UNDEFINED; 
  return _study->StudyId();
}

SALOME_StudyEditor::SALOME_StudyEditor(int studyId) {
  this->setStudyById(studyId);
}
SALOME_StudyEditor::SALOME_StudyEditor(SALOMEDS::Study_ptr study) {
  this->setStudy(study);
}

SALOMEDS::SObject_ptr SALOME_StudyEditor::newObject(SALOMEDS::SObject_ptr parent) {
  return _sbuilder->NewObject(parent);
}

SALOMEDS::SObject_ptr SALOME_StudyEditor::findObject(const char * entry) {
  SALOMEDS::SObject_var sobject = _study->FindObjectID(entry);
  return sobject._retn();
}

SALOMEDS::SComponent_ptr SALOME_StudyEditor::newRoot(const char * moduleName) {
  SALOMEDS::SComponent_var sroot = findRoot(moduleName);
  if ( sroot->_is_nil() ) {
    sroot = _sbuilder->NewComponent(moduleName);
    _sbuilder->SetName(sroot,moduleName);
  }
  return sroot._retn();
}

bool SALOME_StudyEditor::bindEngine(SALOMEDS::SComponent_var studyRoot,
			     Engines::EngineComponent_var engine)
{
  // WARN: The engine should be associated to the SComponent IF
  // AND ONLY IF it is a SALOMEDS::Driver. Otherwise, there is no
  // need to do that, and it could even lead to exception
  // raising (eh, you work on SALOME isn't it?)
  SALOMEDS::Driver_var driver = SALOMEDS::Driver::_narrow(engine);
  if ( driver->_is_nil() || studyRoot->_is_nil() ) return false;
  _sbuilder->DefineComponentInstance(studyRoot, engine);
  return true;
}

SALOMEDS::SComponent_ptr SALOME_StudyEditor::findRoot(const char * moduleName) {
  return _study->FindComponent(moduleName);
}

void SALOME_StudyEditor::setName(SALOMEDS::SObject_var sobject, const char * value) {
  _sbuilder->SetName(sobject,value);  
}
const char * SALOME_StudyEditor::getName(SALOMEDS::SObject_var sobject) {
  if (sobject->_is_nil()) return NULL;
  return sobject->GetName();
}

/**
 * This function specifies which resource file is to be used to
 * associate an icon to the specified object. Note that even if the
 * icon as no sens outside the GUI context, it can be defined here
 * because only the resource name is provided. The effective rendering
 * is done in the object browser of course, and use this string
 * attribute. WARN: note that the resource name is supposed to be the
 * base name of a file managed by the SALOME resource manager (i.e. a
 * file located in the directory specified in the SalomeApp.xml).
 */
void SALOME_StudyEditor::setIcon(SALOMEDS::SObject_var sobject, const char * resourcename) {
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributePixMap_var aPixmap;
  anAttr = _sbuilder->FindOrCreateAttribute(sobject, "AttributePixMap");
  aPixmap = SALOMEDS::AttributePixMap::_narrow(anAttr);
  aPixmap->SetPixMap(resourcename);
}

//
// MEM: AttributeParameter
// Note that the AttributeParameter is a multitype dictionnary whose
// keys are the names specified when setting the value
//

/**
 * Add a parameter attribute of type integer or simply set its value
 * if it already exists.
 */
void SALOME_StudyEditor::setParameterInt(SALOMEDS::SObject_var sobject, const char * name, int value) { 
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeParameter_var aParam;
  anAttr = _sbuilder->FindOrCreateAttribute(sobject, "AttributeParameter");
  aParam = SALOMEDS::AttributeParameter::_narrow(anAttr);
  aParam->SetInt(name,value);
}

int SALOME_StudyEditor::getParameterInt(SALOMEDS::SObject_var sobject, const char * name) {
  if (sobject->_is_nil()) return UNDEFINED;

  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeParameter_var aParam;
  if ( sobject->FindAttribute(anAttr,"AttributeParameter") ) {
    aParam = SALOMEDS::AttributeParameter::_narrow(anAttr);
    return aParam->GetInt(name);
  }
  return UNDEFINED;
}

/**
 * Add a parameter attribute of type boolean or simply set its value
 * if it already exists.
 */
void SALOME_StudyEditor::setParameterBool(SALOMEDS::SObject_var sobject, const char * name, bool value) {
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeParameter_var aParam;
  anAttr = _sbuilder->FindOrCreateAttribute(sobject, "AttributeParameter");
  aParam = SALOMEDS::AttributeParameter::_narrow(anAttr);
  aParam->SetBool(name,value);
}

bool SALOME_StudyEditor::getParameterBool(SALOMEDS::SObject_var sobject, const char * name) {
  if (sobject->_is_nil()) return false;
  // _GBO_ Q: Maybe it's better in this case to raise an exception?

  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeParameter_var aParam;
  if ( sobject->FindAttribute(anAttr,"AttributeParameter") ) {
    aParam = SALOMEDS::AttributeParameter::_narrow(anAttr);
    return aParam->GetBool(name);
  }
  return false;

}
