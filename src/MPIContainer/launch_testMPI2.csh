#! /bin/csh -f

# Copyright (C) 2011-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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
# get pid of ompi-server
setenv OMPI_URI_FILE ${HOME}/.urifile_$$
set lpid1=`pidof ompi-server`
ompi-server -r ${OMPI_URI_FILE}
set lpid2=`pidof ompi-server`
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
mpirun -np 2 -ompi-server file:${OMPI_URI_FILE} ${KERNEL_ROOT_DIR}/bin/salome/testMPI2 -vsize 32 $debug &
mpirun -np 3 -ompi-server file:${OMPI_URI_FILE} ${KERNEL_ROOT_DIR}/bin/salome/testMPI2 -vsize 32 $debug
set res=$status
sleep 1
# kill ompi-server
kill -9 $pid
# delete uri file
rm -f  ${OMPI_URI_FILE}
# give result of test
if $res == 0 then
  echo "OK"
else
  echo "KO"
endif
exit $res
