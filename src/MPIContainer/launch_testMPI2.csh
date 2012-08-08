#! /bin/csh -f
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
