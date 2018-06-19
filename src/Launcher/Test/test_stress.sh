#! /bin/bash
# This test launches in parallel a greate number of instancies of a usual use case
WORKDIR=`mktemp -d`
echo WORKDIR: $WORKDIR
cat > $WORKDIR/command.sh <<< 'echo "OK" > result.txt'
chmod 755 $WORKDIR/command.sh
for i in {1..500}
do
  python launcher_use_case.py $WORKDIR 2> $WORKDIR/log$i.err &
done
exit_code=0
for i in {1..500}
do
  wait -n
  ret=$?
  if [ $ret -ne "0" ]
  then
     echo "Error detected!"
     exit_code=1
  fi
done
# list of error files not empty
ls -l $WORKDIR/*.err | awk '{if ($5 != "0") print $0}'
exit $exit_code