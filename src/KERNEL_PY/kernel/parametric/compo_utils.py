# Copyright (C) 2011-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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

"""
This module provides utility functions for the computation codes intended for
use in parametric studies. The computation codes must be implemented as SALOME
components to use these functions. If the computation code is implemented as a
Python script or function, use module
:mod:`salome.kernel.parametric.pyscript_utils` instead. 
"""

import SALOME_TYPES

def create_input_dict(deterministic_dict, parametric_input):
    """
    This function returns a dictionary containing the input values to be used
    in the computation code.
    
    :type  deterministic_dict: dict
    :param deterministic_dict: dictionary containing the fixed values (i.e.
                               non-parametric). This dictionary can be empty
                               if all variables are parametric.

    :type  parametric_input: SALOME_TYPES/ParametricInput
    :param parametric_input: structure containing the description and values
                             of the parametric variables.

    :return: a dictionary containing the input values for the computation code.
    """
    # Simply get the first point from input (no time series, single observation)
    input_point = parametric_input.inputValues[0][0]

    if len(input_point) != len(parametric_input.inputVarList):
        raise Exception("Size mismatch between inputVarList and point to evaluate")

    input_dict = deterministic_dict.copy()
    for i in range(len(input_point)):
        input_dict[parametric_input.inputVarList[i]] = input_point[i][0]

    return input_dict

def create_normal_parametric_output(output_dict, parametric_input):
    """
    This function returns a structure describing the output of the computation
    code in parametric studies.
    
    :type  output_dict: dict
    :param output_dict: dictionary containing the output values of the
                        computation code (the keys are the variable names, the
                        values are the variable values).

    :type  parametric_input: SALOME_TYPES/ParametricInput
    :param parametric_input: structure containing the description and values
                             of the parametric variables.

    :return: a structure of type SALOME_TYPES/ParametricOutput containing the
             output of the computation code.
    """
    output_values = [[[]]]
    for output_var in parametric_input.outputVarList:
        output_values[0][0].append([output_dict[output_var]])
    return SALOME_TYPES.ParametricOutput(outputValues = output_values,
                                         specificOutputInfos = [],
                                         returnCode = 0,
                                         errorMessage = "")

def create_error_parametric_output(error_message):
    """
    This function returns a structure describing the output of the computation
    code in parametric studies in case of error.
    
    :type  error_message: string
    :param error_message: the error message.

    :return: a structure of type SALOME_TYPES/ParametricOutput describing the
             error.
    """
    return SALOME_TYPES.ParametricOutput(outputValues = [],
                                         specificOutputInfos = [],
                                         returnCode = 1,
                                         errorMessage = error_message)
