#! /bin/bash
# Copyright (C) 2018-2024  CEA, EDF
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

# This test launches in parallel a great number of instances of a usual use case
WORKDIR=`mktemp -d`
echo WORKDIR: $WORKDIR
cat > $WORKDIR/command.sh <<< 'echo "OK" > result.txt'
chmod 755 $WORKDIR/command.sh
pid_launcher=''
for i in {1..20}
do
  ./launcher_use_case.py $WORKDIR 2> $WORKDIR/log$i.err &
  pid_launcher=$pid_launcher" "$!
done
exit_code=0
for i in "$pid_launcher"
do
  wait $i
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
