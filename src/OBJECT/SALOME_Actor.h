//  File      : SALOME_Actor.h
//  Created   : Wed Feb 20 17:24:59 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOME
//  Copyright : Open CASCADE 2002
//  $Header$


#ifndef SALOME_ACTOR_H
#define SALOME_ACTOR_H

// SALOME Includes
#ifndef _Handle_SALOME_InteractiveObject_HeaderFile
#include <Handle_SALOME_InteractiveObject.hxx>
#endif
#include "SALOME_InteractiveObject.hxx"

//VTK Includes
#include <vtkLODActor.h>

class SALOME_Actor : public vtkLODActor 
{
 protected:
  //constructor should be protected due to first implementation of this class
  //it was abstract class
  SALOME_Actor();
  ~SALOME_Actor();
 public:
  vtkTypeMacro(SALOME_Actor,vtkActor);

  // Description:
  // This causes the actor to be rendered. It, in turn, will render the actor`s
  // property and then mapper.
  virtual void Render(vtkRenderer *, vtkMapper *);

  // Description:
  // This method is used internally by the rendering process.
  // We overide the superclass method to properly set the estimated render time.
  virtual int RenderOpaqueGeometry(vtkViewport *viewport);

  // Description:
  // Release any graphics resources that are being consumed by this actor.
  // The parameter window could be used to determine which graphic
  // resources to release.
  virtual void ReleaseGraphicsResources(vtkWindow *);

  virtual void ShallowCopy(vtkProp *prop);

  virtual Standard_Boolean hasIO() { return !myIO.IsNull(); }
  virtual Handle_SALOME_InteractiveObject getIO() { return myIO; } 
  void setIO(const Handle(SALOME_InteractiveObject)& io) { myIO = io; }

  virtual Standard_CString getName()     { return myName; }
  virtual void setName(Standard_CString aName)
    {
      myName = aName;
      if ( hasIO() )
	myIO->setName(aName);
    }

  virtual int getDisplayMode() { return myDisplayMode; }
  virtual void setDisplayMode(int mode) { myDisplayMode = mode; }

  virtual bool hasHighlight() { return false; }  
  virtual void highlight(Standard_Boolean highlight) { ishighlighted = highlight; }  
  virtual Standard_Boolean isHighlighted() { return ishighlighted; }

  virtual void SetOpacity(float opa) { m_opacity = opa; }
  virtual float GetOpacity() { return m_opacity; }

  virtual void SetColor(float r,float g,float b) {};
  virtual void GetColor(float& r,float& g,float& b) {};

  virtual void SetPreviewProperty(vtkProperty* Prop) { PreviewProperty = Prop; }

  virtual void SetPreSelected(Standard_Boolean presel = Standard_False)   { ispreselected = presel; }

  // Used to obtain all dependent actors
  virtual void GetChildActors(vtkActorCollection*) {};
  
  //virtual void ApplyScaling ( vtkMapper*  newMapper, bool recover = false ); 
  virtual void SetMapper ( vtkMapper*  newMapper ); 
  virtual vtkMapper* GetInitialMapper() { return m_Initial; } 

 protected:
  vtkProperty         *PreviewProperty;
  Standard_Boolean    ispreselected;

  Handle_SALOME_InteractiveObject myIO;
  Standard_CString myName;

  float   m_opacity;
  Standard_Boolean    ishighlighted;
  int myDisplayMode;

  vtkMapper*   m_Initial;
};
#endif // SALOME_ACTOR_H
