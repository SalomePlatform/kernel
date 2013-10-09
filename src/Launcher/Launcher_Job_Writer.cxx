// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#include "Launcher_Job_Writer.hxx"
#include "Launcher_Job.hxx"

void Launcher::addToXmlDocument(xmlNodePtr root_node, Launcher::Job* job)
{
  // Begin job
  xmlNodePtr job_node = xmlNewChild(root_node, NULL, xmlCharStrdup("job"), NULL);
  xmlNewProp(job_node, xmlCharStrdup("type"), xmlCharStrdup(job->getJobType().c_str()));
  xmlNewProp(job_node, xmlCharStrdup("name"), xmlCharStrdup(job->getJobName().c_str()));

  // Add user part
  xmlNodePtr node = xmlNewChild(job_node, NULL, xmlCharStrdup("user_part"), NULL);

  xmlNewChild(node, NULL, xmlCharStrdup("job_file"),         xmlCharStrdup(job->getJobFile().c_str()));
  xmlNewChild(node, NULL, xmlCharStrdup("env_file"),         xmlCharStrdup(job->getEnvFile().c_str()));
  xmlNewChild(node, NULL, xmlCharStrdup("work_directory"),   xmlCharStrdup(job->getWorkDirectory().c_str()));
  xmlNewChild(node, NULL, xmlCharStrdup("local_directory"),  xmlCharStrdup(job->getLocalDirectory().c_str()));
  xmlNewChild(node, NULL, xmlCharStrdup("result_directory"), xmlCharStrdup(job->getResultDirectory().c_str()));

  // Parameters for COORM
  xmlNewChild(node, NULL, xmlCharStrdup("launcher_file"), xmlCharStrdup(job->getLauncherFile().c_str()));

  // Files
  xmlNodePtr files_node = xmlNewChild(node, NULL, xmlCharStrdup("files"), NULL);
  std::list<std::string> in_files  = job->get_in_files();
  std::list<std::string> out_files = job->get_out_files();
  for(std::list<std::string>::iterator it = in_files.begin(); it != in_files.end(); it++)
    xmlNewChild(files_node, NULL, xmlCharStrdup("in_file"), xmlCharStrdup((*it).c_str()));
  for(std::list<std::string>::iterator it = out_files.begin(); it != out_files.end(); it++)
    xmlNewChild(files_node, NULL, xmlCharStrdup("out_file"), xmlCharStrdup((*it).c_str()));

  // Resource part
  resourceParams resource_params = job->getResourceRequiredParams();
  xmlNodePtr res_node = xmlNewChild(node, NULL, xmlCharStrdup("resource_params"), NULL);
  xmlNewChild(res_node, NULL, xmlCharStrdup("name"),   xmlCharStrdup(resource_params.name.c_str()));
  xmlNewChild(res_node, NULL, xmlCharStrdup("hostname"),   xmlCharStrdup(resource_params.hostname.c_str()));
  xmlNewChild(res_node, NULL, xmlCharStrdup("OS"),   xmlCharStrdup(resource_params.OS.c_str()));
  std::ostringstream nb_proc_stream;
  std::ostringstream nb_node_stream;
  std::ostringstream nb_proc_per_node_stream;
  std::ostringstream cpu_clock_stream;
  std::ostringstream mem_mb_stream;
  nb_proc_stream << resource_params.nb_proc;
  nb_node_stream << resource_params.nb_node;
  nb_proc_per_node_stream << resource_params.nb_proc_per_node;
  cpu_clock_stream << resource_params.cpu_clock;
  mem_mb_stream << resource_params.mem_mb;
  xmlNewChild(res_node, NULL, xmlCharStrdup("nb_proc"),            xmlCharStrdup(nb_proc_stream.str().c_str()));
  xmlNewChild(res_node, NULL, xmlCharStrdup("nb_node"),            xmlCharStrdup(nb_node_stream.str().c_str()));
  xmlNewChild(res_node, NULL, xmlCharStrdup("nb_proc_per_node"),   xmlCharStrdup(nb_proc_per_node_stream.str().c_str()));
  xmlNewChild(res_node, NULL, xmlCharStrdup("cpu_clock"),          xmlCharStrdup(cpu_clock_stream.str().c_str()));
  xmlNewChild(res_node, NULL, xmlCharStrdup("mem_mb"),             xmlCharStrdup(mem_mb_stream.str().c_str()));

  xmlNewChild(node, NULL, xmlCharStrdup("maximum_duration"), xmlCharStrdup(job->getMaximumDuration().c_str()));
  xmlNewChild(node, NULL, xmlCharStrdup("queue"),            xmlCharStrdup(job->getQueue().c_str()));
  xmlNewChild(node, NULL, xmlCharStrdup("exclusive"),        xmlCharStrdup(job->getExclusiveStr().c_str()));
  std::ostringstream mem_per_cpu_stream;
  mem_per_cpu_stream << job->getMemPerCpu();
  xmlNewChild(node, NULL, xmlCharStrdup("mem_per_cpu"),      xmlCharStrdup(mem_per_cpu_stream.str().c_str()));

  // For COORM
  xmlNewChild(node, NULL, xmlCharStrdup("launcher_args"), xmlCharStrdup(job->getLauncherArgs().c_str()));

  // Specific parameters part
  xmlNodePtr specific_parameters_node = xmlNewChild(node, NULL, xmlCharStrdup("specific_parameters"), NULL);
  std::map<std::string, std::string> specific_parameters = job->getSpecificParameters();
  for(std::map<std::string, std::string>::iterator it = specific_parameters.begin(); it != specific_parameters.end(); it++)
  {
    xmlNodePtr specific_parameter_node = xmlNewChild(specific_parameters_node, NULL, xmlCharStrdup("specific_parameter"), NULL);
    xmlNewChild(specific_parameter_node, NULL, xmlCharStrdup("name"), xmlCharStrdup((it->first).c_str()));
    xmlNewChild(specific_parameter_node, NULL, xmlCharStrdup("value"), xmlCharStrdup((it->second).c_str()));
  }

  // Run part
  xmlNodePtr run_node = xmlNewChild(job_node, NULL, xmlCharStrdup("run_part"), NULL);
  xmlNewChild(run_node, NULL, xmlCharStrdup("job_state"), xmlCharStrdup(job->getState().c_str()));
  ParserResourcesType resource_definition = job->getResourceDefinition();
  xmlNewChild(run_node, NULL, xmlCharStrdup("resource_choosed_name"), xmlCharStrdup(resource_definition.Name.c_str()));

#ifdef WITH_LIBBATCH
  Batch::JobId job_id = getBatchManagerJobId();
  xmlNewChild(run_node, NULL, xmlCharStrdup("job_reference"), xmlCharStrdup(job_id.getReference().c_str()));
#endif
}

