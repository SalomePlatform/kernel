# Copyright (C) 2019  CEA/DEN, EDF R&D
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

import pylauncher as pyla
jyf=pyla.Job_YACSFile()
jyf.setJobName("YDFX")
jyf.setWorkDirectory("/scratch/geay/Example")
jyf.setLocalDirectory("/home/geay/Example")
jyf.setResultDirectory("/home/geay/Example")
jyf.setLauncherFile("")
jyf.setLauncherArgs("")
jyf.setJobFile("/tmp/EvalYFX_geay_180119_093600.xml") # schema YACS
jyf.setPreCommand("")
jyf.setEnvFile("")
for elt in []:
    jyf.add_in_file(elt)
for elt in ["EvalYFX_geay_180119_093600"]:
    jyf.add_out_file(elt)
jyf.setMaximumDuration("00:05")
jyf.setQueue("")
jyf.setPartition("")
jyf.setExclusive(False)
jyf.setMemPerCpu(0)
jyf.setWCKey("P11U5:CARBONES")
jyf.setExtraParams("")
#
rp=pyla.resourceParams()
rp.name = "athos"
rp.hostname = ""
rp.OS = "Linux"
rp.nb_proc = 5
rp.nb_node = 0
rp.nb_proc_per_node = 1
rp.cpu_clock = -1
rp.mem_mb = 0
jyf.setResourceRequiredParams(rp)
jyf.checkSpecificParameters()
l = pyla.Launcher_cpp()
res = pyla.ResourcesManager_cpp("/home/geay/salome/V9_DEV/appli_V9_2_0/CatalogResources.xml")
l.SetResourcesManager(res)
l.createJob(jyf)
nb=jyf.getNumber()
l.launchJob(nb)
l.sendJobToSession(nb)
#l.getJobState(nb)
#l.getJobResults(nb,jyf.getLocalDirectory())
