# Copyright (C) 2019-2024  CEA, EDF
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

import pylauncher
jp = pylauncher.JobParameters_cpp()
jp.job_name = "toto"
jp.job_type = "command"
jp.job_file = "/home/I35256/salome/scripts/job_sh/script.sh"
jp.work_directory = "/tmp/wd"
jp.result_directory = "/tmp/rd"
rp = pylauncher.resourceParams()
rp.name="localhost"
rp.hostname="localhost"
rp.nb_proc = 1
jp.resource_required = rp
launcher = pylauncher.Launcher_cpp()
# no catalog. localhost is defined anyway
res = pylauncher.ResourcesManager_cpp("")
launcher.SetResourcesManager(res)
jobid = launcher.createJob(jp)
launcher.launchJob(jobid)
