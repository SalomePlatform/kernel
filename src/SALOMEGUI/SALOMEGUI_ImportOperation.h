//  File      : SALOMEGUI_ImportOperation.h
//  Created   : Thu Jun 14 12:07:26 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#if !defined SALOMEGUI_IMPORTOPERATION
#define SALOMEGUI_IMPORTOPERATION

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QAD_Operation.h"
class SALOMEGUI_ImportOperation : public QAD_Operation  
{
public:
    SALOMEGUI_ImportOperation( QAD_Study* );
};

#endif 
