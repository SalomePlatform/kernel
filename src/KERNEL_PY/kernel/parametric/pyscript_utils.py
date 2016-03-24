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

## \defgroup pyscript_utils pyscript_utils
#  \{ 
#  \details
#  This module provides utility functions for the computation codes intended for
#  use in parametric studies. The computation codes must be implemented as a
#  Python script or function to use these functions. If the computation code is
#  implemented as a %SALOME component, use module 
#  <a href="group__compo__utils.html">salome.kernel.parametric.compo_utils</a> instead. 
#  \}

"""
This module provides utility functions for the computation codes intended for
use in parametric studies. The computation codes must be implemented as a
Python script or function to use these functions. If the computation code is
implemented as a SALOME component, use module
:mod:`salome.kernel.parametric.compo_utils` instead. 
"""

## This function returns a dictionary containing the input values to be used
#  in the computation code.
#
#  \param deterministic_dict (dict) dictionary containing the fixed values (i.e.
#  non-parametric). This dictionary can be empty if all variables are parametric.
#
#  \param parametric_input (dict) dictionary containing the description and values
#  of the parametric variables.
#
#  \return a dictionary containing the input values for the computation code.
#  \ingroup pyscript_utils
def create_input_dict(deterministic_dict, parametric_input):
    """
    This function returns a dictionary containing the input values to be used
    in the computation code.
    
    :type  deterministic_dict: dict
    :param deterministic_dict: dictionary containing the fixed values (i.e.
                               non-parametric). This dictionary can be empty
                               if all variables are parametric.

    :type  parametric_input: dict
    :param parametric_input: dictionary containing the description and values
                             of the parametric variables.

    :return: a dictionary containing the input values for the computation code.
    """
    # Simply get the first point from input (no time series, single observation)
    input_point = parametric_input["inputValues"][0][0]

    if len(input_point) != len(parametric_input["inputVarList"]):
        raise Exception("Size mismatch between inputVarList and point to evaluate")

    input_dict = deterministic_dict.copy()
    for i in range(len(input_point)):
        input_dict[parametric_input["inputVarList"][i]] = input_point[i][0]

    return input_dict

## This function returns a dictionary describing the output of the
#  computation code in parametric studies.
#
#  \param output_dict (dict) dictionary containing the output values of the
#  computation code (the keys are the variable names, the
#  values are the variable values).
#
#  \param parametric_input (dict) dictionary containing the description and values
#  of the parametric variables.
#
#  \return a dictionary containing the representation of the output of the
#  computation code.
#  \ingroup pyscript_utils
def create_normal_parametric_output(output_dict, parametric_input):
    """
    This function returns a dictionary describing the output of the
    computation code in parametric studies.
    
    :type  output_dict: dict
    :param output_dict: dictionary containing the output values of the
                        computation code (the keys are the variable names, the
                        values are the variable values).

    :type  parametric_input: dict
    :param parametric_input: dictionary containing the description and values
                             of the parametric variables.

    :return: a dictionary containing the representation of the output of the
             computation code.
    """
    output_values = [[[]]]
    for output_var in parametric_input["outputVarList"]:
        output_values[0][0].append([output_dict[output_var]])
    return {"outputValues" : output_values,
            "specificOutputInfos" : [],
            "returnCode" : 0,
            "errorMessage" : ""}

## This function returns a dictionary describing the output of the
#  computation code in parametric studies in case of error.
#
#  \param error_message (string) the error message.
#
#  \return a dictionary describing the error.
#  \ingroup pyscript_utils
def create_error_parametric_output(error_message):
    """
    This function returns a dictionary describing the output of the
    computation code in parametric studies in case of error.
    
    :type  error_message: string
    :param error_message: the error message.

    :return: a dictionary describing the error.
    """
    return {"outputValues" : [],
            "specificOutputInfos" : [],
            "returnCode" : 1,
            "errorMessage" : error_message}
