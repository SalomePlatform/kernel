
// Copyright (C) 2006  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SALOME_FileTransfer_i.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#include "SALOME_FileTransfer_i.hxx"
#include "utilities.h"

//=============================================================================
/*! 
 *  Default constructor,
 */
//=============================================================================

fileTransfer_i::fileTransfer_i()
{
  MESSAGE("fileTransfer_i::fileTransfer_i");
  _fileKey=1;
}

//=============================================================================
/*! 
 *  Destructor
 */
//=============================================================================

fileTransfer_i::~fileTransfer_i()
{
  MESSAGE("fileTransfer_i::~fileTransfer_i");
}


//=============================================================================
/*! 
 *  CORBA method: try to open the file given. If the file is readable, return
 *  a positive integer else return 0;
 *  \param  fileName path to the file to be transfered
 *  \return fileId = positive integer > 0 if open OK.
 */
//=============================================================================

CORBA::Long fileTransfer_i::open(const char* fileName)
{
  MESSAGE(" fileTransfer_i::open " << fileName);
  int aKey = _fileKey++;
  _ctr=0;
  FILE* fp;
  if ((fp = fopen(fileName,"rb")) == NULL)
    {
      INFOS("file " << fileName << " is not readable");
      return 0;
    }
  _fileAccess[aKey] = fp;
  return aKey;
}

//=============================================================================
/*! 
 *  CORBA method: close the file associated to the fileId given at open.
 *  \param fileId got in return from open method
 */
//=============================================================================

void fileTransfer_i::close(CORBA::Long fileId)
{
  MESSAGE("fileTransfer_i::close");
  FILE* fp;
  if (! (fp = _fileAccess[fileId]) )
    {
      INFOS(" no FILE structure associated to fileId " <<fileId);
    }
  else fclose(fp);
}

//=============================================================================
/*! 
 *  CORBA method: get a block of data from the file associated to the fileId
 *  given at open.
 *  \param fileId got in return from open method
 *  \return an octet sequence. Last one is empty.
 */
//=============================================================================

#define FILEBLOCK_SIZE 256*1024

Engines::fileBlock* fileTransfer_i::getBlock(CORBA::Long fileId)
{
  MESSAGE("fileTransfer_i::getBlock");
  Engines::fileBlock* aBlock = new Engines::fileBlock;

  FILE* fp;
  if (! (fp = _fileAccess[fileId]) )
    {
      INFOS(" no FILE structure associated to fileId " <<fileId);
      return aBlock;
    }

  // use replace member function for sequence to avoid copy
  // see Advanced CORBA Programming with C++ pp 187-194
  
  CORBA::Octet *buf;
  buf = Engines::fileBlock::allocbuf(FILEBLOCK_SIZE);
  int nbRed = fread(buf, sizeof(CORBA::Octet), FILEBLOCK_SIZE, fp);
  SCRUTE(nbRed);
  aBlock->replace(nbRed, nbRed, buf, 1); // 1 means give ownership
  return aBlock;
}

