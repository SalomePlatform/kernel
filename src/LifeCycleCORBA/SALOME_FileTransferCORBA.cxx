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

//  File   : SALOME_FileTransferCORBA.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$
//
#include "SALOME_FileTransferCORBA.hxx"
#include "SALOME_LifeCycleCORBA.hxx"
#include "utilities.h"
#include "Basics_Utils.hxx"
#include <cstdio>

/*! \class SALOME_FileTransferCORBA
    \brief A class to manage file transfer in SALOME (CORBA context)

*/

//=============================================================================
/*! 
 *  Default constructor, not for use.
 */
//=============================================================================

SALOME_FileTransferCORBA::SALOME_FileTransferCORBA()
{
  ASSERT(0);
}

//=============================================================================
/*! \brief Constructor to use when we get a fileRef CORBA object from a component
 *
 *  \param aFileRef file reference CORBA object
 */
//=============================================================================

SALOME_FileTransferCORBA::SALOME_FileTransferCORBA(Engines::fileRef_ptr
                                                   aFileRef)
{
  MESSAGE("SALOME_FileTransferCORBA::SALOME_FileTransferCORBA(aFileRef)");
  _theFileRef = aFileRef;
}

//=============================================================================
/*! \brief Constructor to use when we have the file name and machine from which to
 *         copy, plus an optional Container name on the machine.
 *
 *  \param refMachine    the machine on which is the file to transfer
 *  \param origFileName  abolute file path on refMachine
 *  \param containerName default container name used (FactoryServer) if empty
 */
//=============================================================================

SALOME_FileTransferCORBA::SALOME_FileTransferCORBA(std::string refMachine,
                                                   std::string origFileName,
                                                   std::string containerName)
{
  MESSAGE("SALOME_FileTransferCORBA::SALOME_FileTransferCORBA"
          << refMachine << " " << origFileName  << " " << containerName);
  _refMachine = refMachine;
  _origFileName = origFileName;
  _containerName = containerName;
  if (_refMachine.empty() || _origFileName.empty())
    {
      INFOS("bad parameters: machine and file name must be given");
    } 
}

//=============================================================================
/*! 
 *  Destructor
 */
//=============================================================================

SALOME_FileTransferCORBA::~SALOME_FileTransferCORBA()
{
  MESSAGE("SALOME_FileTransferCORBA::~SALOME_FileTransferCORBA");
}

//=============================================================================
/*! \brief Get a local copy of the reference file
 *
 *  CORBA method
 *  \param  localFile optional absolute path to store the copy
 *  \return the file name (absolute path) of the copy, may be different from
 *          localFile parameter if the copy was already done before the call
 */
//=============================================================================

std::string SALOME_FileTransferCORBA::getLocalFile(std::string localFile)
{
  MESSAGE("SALOME_FileTransferCORBA::getLocalFile " << localFile);

  Engines::Container_var container;

  if (CORBA::is_nil(_theFileRef))
    {
      if (_refMachine.empty() || _origFileName.empty())
        {
          INFOS("not enough parameters: machine and file name must be given");
          return "";
        }

      SALOME_LifeCycleCORBA LCC;
      Engines::ContainerManager_var contManager = LCC.getContainerManager();
      Engines::ResourcesManager_var resManager = LCC.getResourcesManager();

      Engines::ContainerParameters params;
      LCC.preSet(params);
      params.container_name = _containerName.c_str();
      params.resource_params.hostname = _refMachine.c_str();
      params.mode = CORBA::string_dup("findorstart");
      container = contManager->GiveContainer(params);
      if (CORBA::is_nil(container))
        {
          INFOS("machine " << _refMachine << " unreachable");
          return "";
        }

      _theFileRef = container->createFileRef(_origFileName.c_str());
      if (CORBA::is_nil(_theFileRef))
        {
          INFOS("impossible to create fileRef on " << _refMachine);
          return "";
        }
    }

  container = _theFileRef->getContainer();
  ASSERT(! CORBA::is_nil(container));

  std::string myMachine = Kernel_Utils::GetHostname();
  std::string localCopy = _theFileRef->getRef(myMachine.c_str());

  if (localCopy.empty()) // no existing copy available
    {
      if (localFile.empty()) // no name provided for local copy
        {
          char bufName[256];
          localCopy = tmpnam(bufName);
          localFile = bufName;
          SCRUTE(localFile);
        }

      FILE* fp;
      if ((fp = fopen(localFile.c_str(),"wb")) == NULL)
        {
          INFOS("file " << localFile << " cannot be open for writing");
          return "";
        }

      Engines::fileTransfer_var fileTransfer = container->getFileTransfer();
      ASSERT(! CORBA::is_nil(fileTransfer));

      CORBA::Long fileId = fileTransfer->open(_origFileName.c_str());
      if (fileId > 0)
        {
          Engines::fileBlock* aBlock;
          int toFollow = 1;
          int ctr=0;
          while (toFollow)
            {
              ctr++;
              SCRUTE(ctr);
              aBlock = fileTransfer->getBlock(fileId);
              toFollow = aBlock->length();
              SCRUTE(toFollow);
              CORBA::Octet *buf = aBlock->get_buffer();
#if defined(_DEBUG_) || defined(_DEBUG)
              int nbWri = fwrite(buf, sizeof(CORBA::Octet), toFollow, fp);
              ASSERT(nbWri == toFollow);
#else
              fwrite(buf, sizeof(CORBA::Octet), toFollow, fp);
#endif
              delete aBlock;
            }
          fclose(fp);
          MESSAGE("end of transfer");
          fileTransfer->close(fileId);
          _theFileRef->addRef(myMachine.c_str(), localFile.c_str());
          localCopy = localFile;
        }
      else
        {
          INFOS("open reference file for copy impossible");
          return "";
        }
      
    }
  SCRUTE(localCopy);
  return localCopy;
}
