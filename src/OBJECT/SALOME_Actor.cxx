using namespace std;
//  File      : SALOME_Actor.cxx
//  Created   : Wed Feb 20 17:24:59 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : OBJECT
//  Copyright : Open CASCADE 2002
//  $Header$

/*!
  \class SALOME_Actor SALOME_Actor.h
  \brief Abstract class of SALOME Objects in VTK.
*/

#include "SALOME_Actor.h"

// SALOME Includes
#include "utilities.h"

// VTK Includes
#include <vtkObjectFactory.h>
#include <vtkMergePoints.h>


void SALOME_Actor::ShallowCopy(vtkProp *prop)
{
  // Now do superclass
  this->vtkActor::ShallowCopy(prop);
}

void SALOME_Actor::Render(vtkRenderer *ren, vtkMapper *Mapper )
{
  if (this->Mapper == NULL) {
    MESSAGE ("No mapper for actor.")
    return;
  }
  
  vtkMapper *bestMapper;
  bestMapper = this->Mapper;

  /* render the property */
  if (!this->Property) {
    // force creation of a property
    this->GetProperty();
  }

  this->Property->Render(this, ren);
  if (this->BackfaceProperty) {
    this->BackfaceProperty->BackfaceRender(this, ren);
    this->Device->SetBackfaceProperty(this->BackfaceProperty);
  }
  this->Device->SetProperty(this->Property);
  
  
  /* render the texture */
  if (this->Texture) {
    this->Texture->Render(ren);
  }
  
  
  // Store information on time it takes to render.
  // We might want to estimate time from the number of polygons in mapper.
  this->Device->Render(ren,bestMapper);
  this->EstimatedRenderTime = bestMapper->GetTimeToDraw();
}

int SALOME_Actor::RenderOpaqueGeometry(vtkViewport *vp)
{
  int renderedSomething = 0; 
  vtkRenderer      *ren = (vtkRenderer *)vp;

  if ( ! this->Mapper ) {
    return 0;
  }

  // make sure we have a property
  if (!this->Property) {
    // force creation of a property
    this->GetProperty();
  }

  // is this actor opaque ?
  if (this->GetIsOpaque()) {
    this->Property->Render(this, ren);
    
    // render the backface property
    if (this->BackfaceProperty) {
      this->BackfaceProperty->BackfaceRender(this, ren);
    }
    
    // render the texture 
    if (this->Texture) {
      this->Texture->Render(ren);
    }
    this->Render(ren,this->Mapper);
    
    renderedSomething = 1;
  }
  
  return renderedSomething; 
}

void SALOME_Actor::ReleaseGraphicsResources(vtkWindow *renWin)
{
  vtkActor::ReleaseGraphicsResources(renWin);
  this->Mapper->ReleaseGraphicsResources(renWin);
}

// void SALOME_Actor::ApplyScaling ( vtkMapper*  newMapper, bool recover = false )
// {
//   if ( recover && m_Isometric )
//     {
//       SetMapper ( m_Isometric );
//       m_Isometric = NULL;
//     }
//   else
//     if ( m_Isometric == NULL )
//       {
// 	 m_Isometric = GetMapper();
// 	 SetMapper(newMapper);
//       }
// }

void SALOME_Actor::SetMapper ( vtkMapper*  newMapper )
{
  if ( m_Initial == NULL && Mapper != NULL )
    {
      m_Initial = GetMapper();
    }
  vtkLODActor::SetMapper(newMapper);
}

SALOME_Actor::SALOME_Actor()
{
  m_Initial = NULL;
  PreviewProperty = NULL;
}
SALOME_Actor::~SALOME_Actor()
{
  if ( m_Initial != NULL )
     m_Initial->Delete();
  if ( PreviewProperty != NULL )
     PreviewProperty->Delete();
}
