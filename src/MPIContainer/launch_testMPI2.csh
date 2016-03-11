#! /bin/csh -f

# Copyright (C) 2011-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

# debug mode display informations about communication
if $1 == "--debug" then
  set debug="-debug"
else
  set debug=""
endif
# get mpi implementation
${KERNEL_ROOT_DIR}/bin/salome/getMPIImplementation
set res = $?
if $res == 1 then
  set mpi="openmpi"
else if $res == 2 then
  set mpi="mpich"
endif
if $mpi == "openmpi" then
# launch ompi-server
  setenv OMPI_URI_FILE ${HOME}/.urifile_$$
  set lpid1=`pidof ompi-server`
  ompi-server -r ${OMPI_URI_FILE}
  set lpid2=`pidof ompi-server`
else if $mpi == "mpich" then
# launch hydra_nameserver
  set lpid1=`pidof hydra_nameserver`
  if $lpid1 == "" then
    hydra_nameserver &
  endif
  set lpid2=`pidof hydra_nameserver`
endif
# get pid of mpi server
set pid=0
foreach i ($lpid2)
  set flag=0
  foreach j ($lpid1)
    if ($i == $j) then
      set flag=1
    endif
  end
  if ($flag == 0) then
    set pid=$i
  endif
end
sleep 2
# launch two instances of executable to create communication between both
if $mpi == "openmpi" then
  mpirun -np 2 -ompi-server file:${OMPI_URI_FILE} ${KERNEL_ROOT_DIR}/bin/salome/testMPI2 -vsize 32 $debug &
  mpirun -np 3 -ompi-server file:${OMPI_URI_FILE} ${KERNEL_ROOT_DIR}/bin/salome/testMPI2 -vsize 32 $debug
else if $mpi == "mpich" then
  mpirun -np 2 -nameserver $HOSTNAME ${KERNEL_ROOT_DIR}/bin/salome/testMPI2 -vsize 32 $debug &
  mpirun -np 3 -nameserver $HOSTNAME ${KERNEL_ROOT_DIR}/bin/salome/testMPI2 -vsize 32 $debug
endif
set res=$status
sleep 1
# kill mpi server
if $pid != 0 then
  kill -9 $pid
endif
if $mpi == "openmpi" then
# delete uri file
  rm -f  ${OMPI_URI_FILE}
endif
# give result of test
if $res == 0 then
  echo "OK"
else
  echo "KO"
endif
exit $res
