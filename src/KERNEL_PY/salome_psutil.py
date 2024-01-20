# -*- coding: utf-8 -*-
# Copyright (C) 2007-2024  CEA, EDF, OPEN CASCADE
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

'''
SALOME utilities for process management.
'''

import os
import psutil


def getNumberOfCPUCores(): # pragma pylint: disable=invalid-name
    '''
    Get number of CPU cores in the calculation node.
    :return Number of cores
    '''
    return psutil.cpu_count(logical=True)


def loadOfCPUCores(script=None): # pragma pylint: disable=invalid-name
    '''
    Get a load of each CPU core in the calculation node.

    A script to compute loads can be customized via `script` parameter.
    In that case, the script must either set `cpu_loads` variable (which
    should be of list type), or specify `getCPULoads()` function returning
    list as result. In both cases, the list's size must be equal to the value
    returning by method `getNumberOfCPUCores()`, and each value in this list
    must be in range [0, 1], otherwise exception is raised.

    If `script` is not specified, default implementation is used.

    :param script Custom script to calculate loads
    :return List that contains loads of each CPU core.
    '''
    if not script:
        return [x/100 for x in psutil.cpu_percent(interval=None, percpu=True)]
    cpu_loads, loc = None, {}
    exec(script, globals(), loc) # pragma pylint: disable=exec-used
    cpu_loads = loc['getCPULoads']() if 'getCPULoads' in loc else loc.get('cpu_loads')
    if cpu_loads is None:
        raise ValueError('Bad script. Specify `getCPULoads` function or `cpu_loads` variable')
    if not isinstance(cpu_loads, (list, tuple)):
        raise TypeError('Bad script. Result must be list or tuple.')
    size = getNumberOfCPUCores()
    if len(cpu_loads) != size:
        raise ValueError('Bad script. Result is of incorrect length (must be {})'.format(size))
    values = [i for i in cpu_loads if 0 <= i <= 1]
    if len(values) != size:
        raise ValueError('Bad script. Values are not in [0, 1] range')
    return [i for i in cpu_loads]


def getTotalPhysicalMemory(): # pragma pylint: disable=invalid-name
    '''
    Get total physical memory of the calculation node.
    :return Size of physical memory, in megabytes.
    '''
    return int(psutil.virtual_memory().total/1024/1024)


def getTotalPhysicalMemoryInUse(): # pragma pylint: disable=invalid-name
    '''
    Get used physical memory of the calculation node.
    :return Size of used physical memory, in megabytes.
    '''
    return int(psutil.virtual_memory().used/1024/1024)


def getTotalPhysicalMemoryInUseByMe(): # pragma pylint: disable=invalid-name
    '''
    Get physical memory occupied by the current process.
    :return Size of physical memory, used by current process, in megabytes.
    '''
    process = psutil.Process(os.getpid())
    memory_in_mb = int(process.memory_info().rss/1024/1024)
    return memory_in_mb
