#! /bin/csh -f
set nbproc1=2
set nbproc2=3
set vsize=32
if $1 == "--debug" then
  set debug="-debug"
else
  set debug=""
endif
if $status == 0 then
  setenv OMPI_URI_FILE ${HOME}/.urifile
  ompi-server -r ${OMPI_URI_FILE}
  sleep 2
  mpirun -np $nbproc1 -ompi-server file:${OMPI_URI_FILE} ${KERNEL_ROOT_DIR}/bin/salome/testMPI2 -vsize $vsize $debug &
  mpirun -np $nbproc2 -ompi-server file:${OMPI_URI_FILE} ${KERNEL_ROOT_DIR}/bin/salome/testMPI2 -vsize $vsize $debug
  set res=$status
  sleep 1
  killall ompi-server
endif
if $res == 0 then
  echo "OK"
else
  echo "KO"
endif
rm -f  ${OMPI_URI_FILE}
