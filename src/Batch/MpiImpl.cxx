// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
/*
 * BatchManager.cxx : 
 *
 * Auteur : Bernard SECHER - CEA/DEN
 * Date   : Juillet 2007
 * Projet : SALOME
 *
 */

#include <iostream>
#include <sstream>
#include <string>
#include "MpiImpl.hxx"

using namespace std;

// Constructor
MpiImpl::MpiImpl()
{
}

// Destructor
MpiImpl::~MpiImpl()
{
}

// lam implementation
// Constructor
MpiImpl_LAM::MpiImpl_LAM() : MpiImpl()
{
}

// Destructor
MpiImpl_LAM::~MpiImpl_LAM()
{
}

string MpiImpl_LAM::size()
{
  return "${LAMWORLD}";
}

string MpiImpl_LAM::rank()
{
  return "${LAMRANK}";
}

string MpiImpl_LAM::boot(const string machinefile, const unsigned int nbnodes)
{
  ostringstream oss;
  oss << "lamboot " << machinefile << endl;
  return oss.str();
}

string MpiImpl_LAM::run(const string machinefile, const unsigned int nbproc, const string fileNameToExecute)
{
  ostringstream oss;
  oss << "mpirun -np " << nbproc << " " << fileNameToExecute << endl;
  return oss.str();
}

string MpiImpl_LAM::halt()
{
  ostringstream oss;
  oss << "lamhalt" << endl;
  return oss.str();
}

// mpich1 implementation
// Constructor
MpiImpl_MPICH1::MpiImpl_MPICH1() : MpiImpl()
{
}

// Destructor
MpiImpl_MPICH1::~MpiImpl_MPICH1()
{
}

string MpiImpl_MPICH1::size()
{
  return "${MPIRUN_NPROCS}";
}

string MpiImpl_MPICH1::rank()
{
  return "${MPIRUN_RANK}";
}

string MpiImpl_MPICH1::boot(const string machinefile, const unsigned int nbnodes)
{
  return "";
}

string MpiImpl_MPICH1::run(const string machinefile, const unsigned int nbproc, const string fileNameToExecute)
{
  ostringstream oss;
  oss << "mpirun -machinefile " << machinefile << " -np " << nbproc << " " << fileNameToExecute << endl;
  return oss.str();
}

string MpiImpl_MPICH1::halt()
{
  return "";
}

// mpich2 implementation
// Constructor
MpiImpl_MPICH2::MpiImpl_MPICH2() : MpiImpl()
{
}

// Destructor
MpiImpl_MPICH2::~MpiImpl_MPICH2()
{
}

string MpiImpl_MPICH2::size()
{
  return "${PMI_SIZE}";
}

string MpiImpl_MPICH2::rank()
{
  return "${PMI_RANK}";
}

string MpiImpl_MPICH2::boot(const string machinefile, const unsigned int nbnodes)
{
  ostringstream oss;
  oss << "mpdboot" << " -n " << nbnodes;
  if (machinefile!="")
    oss  << " -f " << machinefile;
  oss << endl;
  return oss.str();
}

string MpiImpl_MPICH2::run(const string machinefile, const unsigned int nbproc, const string fileNameToExecute)
{
  ostringstream oss;
  oss << "mpirun -np " << nbproc << " " << fileNameToExecute << endl;
  return oss.str();
}

string MpiImpl_MPICH2::halt()
{
  ostringstream oss;
  oss << "mpdallexit" << endl;
  return oss.str();
}

// openmpi implementation
// Constructor
MpiImpl_OPENMPI::MpiImpl_OPENMPI() : MpiImpl()
{
}

// Destructor
MpiImpl_OPENMPI::~MpiImpl_OPENMPI()
{
}

string MpiImpl_OPENMPI::size()
{
  return "${OMPI_MCA_ns_nds_num_procs}";
}

string MpiImpl_OPENMPI::rank()
{
  return "${OMPI_MCA_ns_nds_vpid}";
}

string MpiImpl_OPENMPI::boot(const string machinefile, const unsigned int nbnodes)
{
  return "";
}

string MpiImpl_OPENMPI::run(const string machinefile, const unsigned int nbproc, const string fileNameToExecute)
{
  ostringstream oss;
  oss << "mpirun -hostfile " << machinefile << " -np " << nbproc << " " << fileNameToExecute << endl;
  return oss.str();
}

string MpiImpl_OPENMPI::halt()
{
  return "";
}

// slurm implementation
// Constructor
MpiImpl_SLURM::MpiImpl_SLURM() : MpiImpl()
{
}

// Destructor
MpiImpl_SLURM::~MpiImpl_SLURM()
{
}

string MpiImpl_SLURM::size()
{
  return "${SLURM_NPROCS}";
}

string MpiImpl_SLURM::rank()
{
  return "${SLURM_PROCID}";
}

string MpiImpl_SLURM::boot(const string machinefile, const unsigned int nbnodes)
{
  return "";
}

string MpiImpl_SLURM::run(const string machinefile, const unsigned int nbproc, const string fileNameToExecute)
{
  ostringstream oss;
  oss << "srun " << fileNameToExecute << endl;
  return oss.str();
}

string MpiImpl_SLURM::halt()
{
  return "";
}

// prun implementation
// Constructor
MpiImpl_PRUN::MpiImpl_PRUN() : MpiImpl()
{
}

// Destructor
MpiImpl_PRUN::~MpiImpl_PRUN()
{
}

string MpiImpl_PRUN::size()
{
  return "${RMS_NPROCS}";
}

string MpiImpl_PRUN::rank()
{
  return "${RMS_RANK}";
}

string MpiImpl_PRUN::boot(const string machinefile, const unsigned int nbnodes)
{
  return "";
}

string MpiImpl_PRUN::run(const string machinefile, const unsigned int nbproc, const string fileNameToExecute)
{
  ostringstream oss;
  oss << "prun -n " << nbproc << " " << "-p mpi " << fileNameToExecute << endl;
  return oss.str();
}

string MpiImpl_PRUN::halt()
{
  return "";
}
