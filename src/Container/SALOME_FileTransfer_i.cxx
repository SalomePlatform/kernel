// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

//  File   : SALOME_FileTransfer_i.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$
//
#include "SALOME_FileTransfer_i.hxx"
#include "utilities.h"

/*! \class fileTransfer_i
    \brief A class to manage file transfer in SALOME

*/

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
/*! \brief open the given file
 *
 *  CORBA method: try to open the file. If the file is readable, return
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
/*! \brief close a file
 *
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
  else 
    {
      fclose(fp);
      _fileAccess.erase(fileId);
    }
}

#define FILEBLOCK_SIZE 256*1024

//=============================================================================
/*! \brief get a data block from a file
 * 
 *  CORBA method: get a block of data from the file associated to the fileId
 *  given at open.
 *  \param fileId got in return from open method
 *  \return an octet sequence. Last one is empty.
 */
//=============================================================================

Engines::fileBlock* fileTransfer_i::getBlock(CORBA::Long fileId)
{
  //MESSAGE("fileTransfer_i::getBlock");
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
  //SCRUTE(nbRed);
  aBlock->replace(nbRed, nbRed, buf, 1); // 1 means give ownership
  return aBlock;
}

/*! \brief open the given file in write mode (for copy)
 *
 *  CORBA method: try to open the file. If the file is writable, 
 *  return a positive integer else return 0;
 *  \param  fileName path to the file to be transfered
 *  \return fileId = positive integer > 0 if open OK.
 */
CORBA::Long fileTransfer_i::openW(const char* fileName)
{
  MESSAGE(" fileTransfer_i::openW " << fileName);
  int aKey = _fileKey++;
  _ctr=0;
  FILE* fp;
  if ((fp = fopen(fileName,"wb")) == NULL)
    {
      INFOS("file " << fileName << " is not writable");
      return 0;
    }
  _fileAccess[aKey] = fp;
  return aKey;
}

/*! \brief put a data block for copy into a file
 * 
 *  CORBA method: put a block of data into the file associated to the fileId
 *  given at openW.
 *  \param fileId got in return from openW method
 *  \param block an octet sequence to copy into opened file
 */
void fileTransfer_i::putBlock(CORBA::Long fileId, const Engines::fileBlock& block)
{
  MESSAGE("fileTransfer_i::putBlock");
  FILE* fp;
  if (! (fp = _fileAccess[fileId]) )
    {
      INFOS(" no FILE structure associated to fileId " <<fileId);
      return ;
    }
  int toFollow = block.length();
  SCRUTE(toFollow);
  const CORBA::Octet *buf = block.get_buffer();
  fwrite(buf, sizeof(CORBA::Octet), toFollow, fp);
}


