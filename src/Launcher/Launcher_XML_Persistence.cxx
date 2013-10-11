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

#include <libxml/parser.h>

#include "Launcher_XML_Persistence.hxx"
#include "Launcher_Job_Command.hxx"
#include "Launcher_Job_YACSFile.hxx"
#include "Launcher_Job_PythonSALOME.hxx"

using namespace std;

namespace Launcher
{

list<Job *>
XML_Persistence::loadJobs(const char* jobs_file)
{
  // Step 1: check jobs_file read access
  FILE* xml_file = fopen(jobs_file, "r");
  if (xml_file == NULL)
  {
    std::string error = "Error opening jobs_file in SALOME_Launcher::loadJobs: " + std::string(jobs_file);
    LAUNCHER_INFOS(error);
    throw LauncherException(error);
  }

  // Step 2: read xml file
  xmlDocPtr doc = xmlReadFile(jobs_file, NULL, 0);
  if (doc == NULL)
  {
    std::string error = "Error in xmlReadFile in SALOME_Launcher::loadJobs, could not parse file: " + std::string(jobs_file);
    LAUNCHER_INFOS(error);
    fclose(xml_file);
    throw LauncherException(error);
  }

  // Step 3: Find jobs
  list<Job *> jobs_list;
  xmlNodePtr root_node = xmlDocGetRootElement(doc);
  xmlNodePtr xmlCurrentNode = root_node->xmlChildrenNode;
  if (!xmlStrcmp(root_node->name, xmlCharStrdup("jobs")))
  {
    while(xmlCurrentNode != NULL)
    {
      if (!xmlStrcmp(xmlCurrentNode->name, xmlCharStrdup("job")))
      {
        LAUNCHER_INFOS("A job is found");
        Launcher::Job * new_job; // It is Launcher_cpp that is going to destroy it
        xmlNodePtr job_node = xmlCurrentNode;

        // Begin Job
        if (!xmlHasProp(job_node, xmlCharStrdup("type"))  ||
            !xmlHasProp(job_node, xmlCharStrdup("name")))
        {
          LAUNCHER_INFOS("A bad job is found, type or name not found");
          break;
        }
        xmlChar* type = xmlGetProp(job_node, xmlCharStrdup("type"));
        xmlChar* name = xmlGetProp(job_node, xmlCharStrdup("name"));
        std::string job_type((const char*) type);
        if (job_type == "command")
          new_job = new Launcher::Job_Command();
        else if (job_type == "yacs_file")
          new_job = new Launcher::Job_YACSFile();
        else if (job_type == "python_salome")
          new_job = new Launcher::Job_PythonSALOME();
        new_job->setJobName(std::string((const char *)name));
        xmlFree(type);
        xmlFree(name);

        xmlNodePtr user_node = xmlFirstElementChild(job_node);
        xmlNodePtr run_node = xmlNextElementSibling(user_node);
        if (user_node == NULL || run_node == NULL)
        {
          LAUNCHER_INFOS("A bad job is found, user_part or run_part not found");
          delete new_job;
          break;
        }
        if (xmlStrcmp(user_node->name, xmlCharStrdup("user_part")) ||
            xmlStrcmp(run_node->name, xmlCharStrdup("run_part")))
        {
          LAUNCHER_INFOS("A bad job is found, cannot get a correct user_part or run_part node");
          delete new_job;
          break;
        }

        // Add user part

        // Get job_file env_file work_directory local_directory result_directory
        xmlNodePtr job_file_node         = xmlFirstElementChild(user_node);
        xmlNodePtr env_file_node         = xmlNextElementSibling(job_file_node);
        xmlNodePtr work_directory_node   = xmlNextElementSibling(env_file_node);
        xmlNodePtr local_directory_node  = xmlNextElementSibling(work_directory_node);
        xmlNodePtr result_directory_node = xmlNextElementSibling(local_directory_node);

                // Parameters for COORM
        xmlNodePtr launcher_file_node = xmlNextElementSibling(result_directory_node);

        if (job_file_node         == NULL ||
            env_file_node         == NULL ||
            work_directory_node   == NULL ||
            local_directory_node  == NULL ||
            result_directory_node == NULL ||
                        // For COORM
            launcher_file_node    == NULL
           )
        {
          LAUNCHER_INFOS("A bad job is found, some user_part are not found");
          delete new_job;
          break;
        }
        if (xmlStrcmp(job_file_node->name,         xmlCharStrdup("job_file"))         ||
            xmlStrcmp(env_file_node->name,         xmlCharStrdup("env_file"))         ||
            xmlStrcmp(work_directory_node->name,   xmlCharStrdup("work_directory"))   ||
            xmlStrcmp(local_directory_node->name,  xmlCharStrdup("local_directory"))  ||
            xmlStrcmp(result_directory_node->name, xmlCharStrdup("result_directory")) ||
                        // For COORM
            xmlStrcmp(launcher_file_node->name, xmlCharStrdup("launcher_file"))
           )
        {
          LAUNCHER_INFOS("A bad job is found, some user part node are not in the rigth or does not have a correct name");
          delete new_job;
          break;
        }
        xmlChar* job_file         = xmlNodeGetContent(job_file_node);
        try
        {
          new_job->setJobFile(std::string((const char *)job_file));
        }
        catch(const LauncherException &ex)
        {
          LAUNCHER_INFOS("Exception receice for job_file, cannot add the job" << ex.msg.c_str());
          delete new_job;
          xmlFree(job_file);
          break;
        }
        xmlChar* env_file         = xmlNodeGetContent(env_file_node);
        xmlChar* work_directory   = xmlNodeGetContent(work_directory_node);
        xmlChar* local_directory  = xmlNodeGetContent(local_directory_node);
        xmlChar* result_directory = xmlNodeGetContent(result_directory_node);

                // Parameters for COORM
        xmlChar* launcher_file = xmlNodeGetContent(launcher_file_node);

        new_job->setEnvFile(std::string((const char *)env_file));
        new_job->setWorkDirectory(std::string((const char *)work_directory));
        new_job->setLocalDirectory(std::string((const char *)local_directory));
        new_job->setResultDirectory(std::string((const char *)result_directory));

                // Parameters for COORM
        new_job->setLauncherFile(std::string((const char *)launcher_file));

        xmlFree(job_file);
        xmlFree(env_file);
        xmlFree(work_directory);
        xmlFree(local_directory);
        xmlFree(result_directory);

                // Parameters for COORM
                xmlFree(launcher_file);

        // Get in and out files
        xmlNodePtr files_node = xmlNextElementSibling(launcher_file_node);
        if (files_node == NULL)
        {
          LAUNCHER_INFOS("A bad job is found, user_part files is not found");
          delete new_job;
          break;
        }
        if (xmlStrcmp(files_node->name, xmlCharStrdup("files")))
        {
          LAUNCHER_INFOS("A bad job is found, files node are not in the rigth place or does not have a correct name or does not exist");
          delete new_job;
          break;
        }
        xmlNodePtr file_node = xmlFirstElementChild(files_node);
        while (file_node != NULL)
        {
          if (!xmlStrcmp(file_node->name, xmlCharStrdup("in_file")))
          {
            xmlChar* in_file = xmlNodeGetContent(file_node);
            new_job->add_in_file(std::string((const char *)in_file));
            xmlFree(in_file);
          }
          else if (!xmlStrcmp(file_node->name, xmlCharStrdup("out_file")))
          {
            xmlChar* out_file = xmlNodeGetContent(file_node);
            new_job->add_out_file(std::string((const char *)out_file));
            xmlFree(out_file);
          }
          file_node = xmlNextElementSibling(file_node);
        }

        // Get resource part
        xmlNodePtr res_node = xmlNextElementSibling(files_node);
        xmlNodePtr maximum_duration_node = xmlNextElementSibling(res_node);
        xmlNodePtr queue_node = xmlNextElementSibling(maximum_duration_node);
        xmlNodePtr exclusive_node = xmlNextElementSibling(queue_node);
        xmlNodePtr mem_per_cpu_node = xmlNextElementSibling(exclusive_node);
        xmlNodePtr launcher_args_node = xmlNextElementSibling(mem_per_cpu_node);
        if (res_node              == NULL ||
            maximum_duration_node == NULL ||
            queue_node            == NULL ||
            exclusive_node        == NULL ||
            mem_per_cpu_node      == NULL ||
                        // For COORM
            launcher_args_node    == NULL
           )
        {
          LAUNCHER_INFOS("A bad job is found, some user_part are not found");
          delete new_job;
          break;
        }
        if (xmlStrcmp(res_node->name,              xmlCharStrdup("resource_params"))  ||
            xmlStrcmp(maximum_duration_node->name, xmlCharStrdup("maximum_duration")) ||
            xmlStrcmp(queue_node->name,            xmlCharStrdup("queue")) ||
            xmlStrcmp(exclusive_node->name,        xmlCharStrdup("exclusive")) ||
            xmlStrcmp(mem_per_cpu_node->name,      xmlCharStrdup("mem_per_cpu")) ||
                        // For COORM
            xmlStrcmp(launcher_args_node->name,    xmlCharStrdup("launcher_args"))
           )
        {
          LAUNCHER_INFOS("A bad job is found, some user part node are not in the rigth or does not have a correct name");
          delete new_job;
          break;
        }
        xmlChar* maximum_duration = xmlNodeGetContent(maximum_duration_node);
        try
        {
          new_job->setMaximumDuration(std::string((const char *)maximum_duration));
        }
        catch(const LauncherException &ex)
        {
          LAUNCHER_INFOS("Exception receice for maximum_duration, cannot add the job" << ex.msg.c_str());
          delete new_job;
          xmlFree(maximum_duration);
          break;
        }
        xmlChar* queue            = xmlNodeGetContent(queue_node);
        new_job->setQueue(std::string((const char *)queue));
        xmlFree(maximum_duration);
        xmlFree(queue);

        xmlChar* exclusive = xmlNodeGetContent(exclusive_node);
        try
        {
          new_job->setExclusiveStr(std::string((const char *)exclusive));
        }
        catch(const LauncherException &ex)
        {
          LAUNCHER_INFOS("Exception received for exclusive, cannot add the job. " << ex.msg.c_str());
          delete new_job;
          xmlFree(exclusive);
          break;
        }
        xmlFree(exclusive);

        xmlChar* mem_per_cpu_str = xmlNodeGetContent(mem_per_cpu_node);
        std::istringstream mem_per_cpu_stream((const char *)mem_per_cpu_str);
        unsigned long mem_per_cpu = 0;
        if (!(mem_per_cpu_stream >> mem_per_cpu))
        {
          LAUNCHER_INFOS("A bad job is found, mem_per_cpu parameter is not correct");
          delete new_job;
          break;
        }
        else
          new_job->setMemPerCpu(mem_per_cpu);

                // For COORM
        xmlChar* launcher_args           = xmlNodeGetContent(launcher_args_node);
        new_job->setLauncherArgs(std::string((const char *)launcher_args));
        xmlFree(launcher_args);

        xmlNodePtr specific_node = xmlNextElementSibling(launcher_args_node);
        if (specific_node == NULL)
        {
          LAUNCHER_INFOS("A bad job is found, specific_parameters part is not found");
          delete new_job;
          break;
        }
        xmlNodePtr parameter_node = xmlFirstElementChild(specific_node);
        while (parameter_node != NULL)
        {
          if (!xmlStrcmp(parameter_node->name, xmlCharStrdup("specific_parameter")))
          {
            xmlNodePtr name_node = xmlFirstElementChild(parameter_node);
            xmlNodePtr value_node = xmlNextElementSibling(name_node);
            if (name_node == NULL ||
                value_node == NULL)
            {
              LAUNCHER_INFOS("A bad job is found, specific_parameter parts are not found");
              delete new_job;
              break;
            }
            if (xmlStrcmp(name_node->name, xmlCharStrdup("name")) ||
                xmlStrcmp(value_node->name, xmlCharStrdup("value")))
            {
              LAUNCHER_INFOS("A bad job is found, specific_parameter bad parts are found");
              delete new_job;
              break;
            }

            xmlChar* name  = xmlNodeGetContent(name_node);
            xmlChar* value = xmlNodeGetContent(value_node);
            try
            {
              new_job->addSpecificParameter(std::string((const char*)name), std::string((const char*)value));
              xmlFree(name);
              xmlFree(value);
            }
            catch(const LauncherException &ex)
            {
              LAUNCHER_INFOS("Exception receice for a specific parameter, cannot add the job" << ex.msg.c_str());
              delete new_job;
              xmlFree(name);
              xmlFree(value);
              break;
            }
          }
          else
          {
            LAUNCHER_INFOS("A bad job is found, specific_parameters part is bad, a node that is not a specific parameter is found");
            delete new_job;
            break;
          }
          parameter_node = xmlNextElementSibling(parameter_node);
        }

        xmlNodePtr res_name_node             = xmlFirstElementChild(res_node);
        xmlNodePtr res_hostname_node         = xmlNextElementSibling(res_name_node);
        xmlNodePtr res_os_node               = xmlNextElementSibling(res_hostname_node);
        xmlNodePtr res_nb_proc_node          = xmlNextElementSibling(res_os_node);
        xmlNodePtr res_nb_node_node          = xmlNextElementSibling(res_nb_proc_node);
        xmlNodePtr res_nb_proc_per_node_node = xmlNextElementSibling(res_nb_node_node);
        xmlNodePtr res_cpu_clock_node        = xmlNextElementSibling(res_nb_proc_per_node_node);
        xmlNodePtr res_mem_mb_node           = xmlNextElementSibling(res_cpu_clock_node);
        if (res_name_node             == NULL ||
            res_hostname_node         == NULL ||
            res_os_node               == NULL ||
            res_nb_proc_node          == NULL ||
            res_nb_node_node          == NULL ||
            res_nb_proc_per_node_node == NULL ||
            res_cpu_clock_node        == NULL ||
            res_mem_mb_node           == NULL
           )
        {
          LAUNCHER_INFOS("A bad job is found, some resource_params user_part are not found");
          delete new_job;
          break;
        }
        if (xmlStrcmp(res_name_node->name,             xmlCharStrdup("name"))             ||
            xmlStrcmp(res_hostname_node->name,         xmlCharStrdup("hostname"))         ||
            xmlStrcmp(res_os_node->name,               xmlCharStrdup("OS"))               ||
            xmlStrcmp(res_nb_proc_node->name,          xmlCharStrdup("nb_proc"))          ||
            xmlStrcmp(res_nb_node_node->name,          xmlCharStrdup("nb_node"))          ||
            xmlStrcmp(res_nb_proc_per_node_node->name, xmlCharStrdup("nb_proc_per_node")) ||
            xmlStrcmp(res_cpu_clock_node->name,        xmlCharStrdup("cpu_clock"))        ||
            xmlStrcmp(res_mem_mb_node->name,           xmlCharStrdup("mem_mb"))
           )
        {
          LAUNCHER_INFOS("A bad job is found, some resource_params user_part node are not in the rigth or does not have a correct name");
          delete new_job;
          break;
        }
        xmlChar* res_name     = xmlNodeGetContent(res_name_node);
        xmlChar* res_hostname = xmlNodeGetContent(res_hostname_node);
        xmlChar* res_os       = xmlNodeGetContent(res_os_node);
        resourceParams p;
        p.name     = std::string((const char*) res_name);
        p.hostname = std::string((const char*) res_hostname);
        p.OS       = std::string((const char*) res_os);
        xmlFree(res_name);
        xmlFree(res_hostname);
        xmlFree(res_os);
        xmlChar* res_nb_proc          = xmlNodeGetContent(res_nb_proc_node);
        xmlChar* res_nb_node          = xmlNodeGetContent(res_nb_node_node);
        xmlChar* res_nb_proc_per_node = xmlNodeGetContent(res_nb_proc_per_node_node);
        xmlChar* res_cpu_clock        = xmlNodeGetContent(res_cpu_clock_node);
        xmlChar* res_mem_mb           = xmlNodeGetContent(res_mem_mb_node);
        bool import_value = true;
        std::istringstream nb_proc_stream((const char *) res_nb_proc);
        if (!(nb_proc_stream >> p.nb_proc))
          import_value = false;
        std::istringstream nb_node_stream((const char *) res_nb_node);
        if (!(nb_node_stream >> p.nb_node))
          import_value = false;
        std::istringstream nb_proc_per_node_stream((const char *) res_nb_proc_per_node);
        if (!(nb_proc_per_node_stream >> p.nb_proc_per_node))
          import_value = false;
        std::istringstream cpu_clock_stream((const char *) res_cpu_clock);
        if (!(cpu_clock_stream >> p.cpu_clock))
          import_value = false;
        std::istringstream mem_mb_stream((const char *) res_mem_mb);
        if (!(mem_mb_stream >> p.mem_mb))
          import_value = false;
        xmlFree(res_nb_proc);
        xmlFree(res_nb_node);
        xmlFree(res_nb_proc_per_node);
        xmlFree(res_cpu_clock);
        xmlFree(res_mem_mb);
        if (!import_value)
        {
          LAUNCHER_INFOS("A bad job is found, some resource_params value are not correct");
          delete new_job;
          break;
        }
        try
        {
          new_job->setResourceRequiredParams(p);
        }
        catch(const LauncherException &ex)
        {
          LAUNCHER_INFOS("A bad job is found, an error when inserting resource_params:" << ex.msg.c_str());
          delete new_job;
          break;
        }

        // We finally get run part to figure out what to do
        xmlNodePtr job_state_node             = xmlFirstElementChild(run_node);
        xmlNodePtr resource_choosed_name_node = xmlNextElementSibling(job_state_node);
        xmlNodePtr job_reference_node         = xmlNextElementSibling(resource_choosed_name_node);
        if (job_state_node             == NULL ||
            resource_choosed_name_node == NULL ||
            job_reference_node         == NULL
           )
        {
          LAUNCHER_INFOS("A bad job is found, some run_part are not found");
          delete new_job;
          break;
        }
        if (xmlStrcmp(job_state_node->name,             xmlCharStrdup("job_state"))             ||
            xmlStrcmp(resource_choosed_name_node->name, xmlCharStrdup("resource_choosed_name")) ||
            xmlStrcmp(job_reference_node->name,         xmlCharStrdup("job_reference"))
           )
        {
          LAUNCHER_INFOS("A bad job is found, some run_part nodes are not in the rigth or does not have a correct name");
          delete new_job;
          break;
        }
        xmlChar* job_state_xml             = xmlNodeGetContent(job_state_node);
        xmlChar* resource_choosed_name_xml = xmlNodeGetContent(resource_choosed_name_node);
        xmlChar* job_reference_xml         = xmlNodeGetContent(job_reference_node);
        std::string job_state((const char *) job_state_xml);
        std::string resource_choosed_name((const char *) resource_choosed_name_xml);
        std::string job_reference((const char *) job_reference_xml);
        xmlFree(job_state_xml);
        xmlFree(resource_choosed_name_xml);
        xmlFree(job_reference_xml);
        new_job->setState(job_state);
        new_job->setReference(job_reference);

        jobs_list.push_back(new_job);
      }
      xmlCurrentNode = xmlCurrentNode->next;
    }
  }
  else
  {
    xmlFreeDoc(doc);
    fclose(xml_file);
    std::string error = "Error in xml file, could not find root_node named jobs: " + std::string(jobs_file);
    LAUNCHER_INFOS(error);
    throw LauncherException(error);
  }

  // Clean
  xmlFreeDoc(doc);
  fclose(xml_file);

  return jobs_list;
}

void
XML_Persistence::saveJobs(const char* jobs_file, const list<const Job *> & jobs_list)
{
  // Step 1: check jobs_file write access
  FILE* xml_file = fopen(jobs_file, "w");
  if (xml_file == NULL)
  {
    std::string error = "Error opening jobs_file in SALOME_Launcher::saveJobs: " + std::string(jobs_file);
    LAUNCHER_INFOS(error);
    throw LauncherException(error);
  }

  // Step 2: First lines
  xmlKeepBlanksDefault(0);
  xmlDocPtr doc = xmlNewDoc(xmlCharStrdup("1.0"));
  xmlNodePtr root_node = xmlNewNode(NULL, xmlCharStrdup("jobs"));
  xmlDocSetRootElement(doc, root_node);
  xmlNodePtr doc_comment = xmlNewDocComment(doc, xmlCharStrdup("SALOME Launcher save jobs file"));
  xmlAddPrevSibling(root_node, doc_comment);

  // Step 3: For each job write it on the xml document
  // We could put a mutex but are not foing to do that currently
  list<const Job *>::const_iterator it_job;
  for (it_job = jobs_list.begin(); it_job != jobs_list.end(); it_job++)
  {
    addJobToXmlDocument(root_node, **it_job);
  }

  // Final step: write file
  int isOk = xmlSaveFormatFile(jobs_file, doc, 1);
  if (!isOk)
  {
    std::string error = "Error during xml file saving in SALOME_Launcher::saveJobs: " + std::string(jobs_file);
    LAUNCHER_INFOS(error);
    xmlFreeDoc(doc);
    fclose(xml_file);
    throw LauncherException(error);
  }

  // Clean
  xmlFreeDoc(doc);
  fclose(xml_file);
  LAUNCHER_MESSAGE("SALOME_Launcher::saveJobs : WRITING DONE!");
}

void
XML_Persistence::addJobToXmlDocument(xmlNodePtr root_node, const Job & job)
{
  // Begin job
  xmlNodePtr job_node = xmlNewChild(root_node, NULL, xmlCharStrdup("job"), NULL);
  xmlNewProp(job_node, xmlCharStrdup("type"), xmlCharStrdup(job.getJobType().c_str()));
  xmlNewProp(job_node, xmlCharStrdup("name"), xmlCharStrdup(job.getJobName().c_str()));

  // Add user part
  xmlNodePtr node = xmlNewChild(job_node, NULL, xmlCharStrdup("user_part"), NULL);

  xmlNewChild(node, NULL, xmlCharStrdup("job_file"),         xmlCharStrdup(job.getJobFile().c_str()));
  xmlNewChild(node, NULL, xmlCharStrdup("env_file"),         xmlCharStrdup(job.getEnvFile().c_str()));
  xmlNewChild(node, NULL, xmlCharStrdup("work_directory"),   xmlCharStrdup(job.getWorkDirectory().c_str()));
  xmlNewChild(node, NULL, xmlCharStrdup("local_directory"),  xmlCharStrdup(job.getLocalDirectory().c_str()));
  xmlNewChild(node, NULL, xmlCharStrdup("result_directory"), xmlCharStrdup(job.getResultDirectory().c_str()));

  // Parameters for COORM
  xmlNewChild(node, NULL, xmlCharStrdup("launcher_file"), xmlCharStrdup(job.getLauncherFile().c_str()));

  // Files
  xmlNodePtr files_node = xmlNewChild(node, NULL, xmlCharStrdup("files"), NULL);
  std::list<std::string> in_files  = job.get_in_files();
  std::list<std::string> out_files = job.get_out_files();
  for(std::list<std::string>::iterator it = in_files.begin(); it != in_files.end(); it++)
    xmlNewChild(files_node, NULL, xmlCharStrdup("in_file"), xmlCharStrdup((*it).c_str()));
  for(std::list<std::string>::iterator it = out_files.begin(); it != out_files.end(); it++)
    xmlNewChild(files_node, NULL, xmlCharStrdup("out_file"), xmlCharStrdup((*it).c_str()));

  // Resource part
  resourceParams resource_params = job.getResourceRequiredParams();
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

  xmlNewChild(node, NULL, xmlCharStrdup("maximum_duration"), xmlCharStrdup(job.getMaximumDuration().c_str()));
  xmlNewChild(node, NULL, xmlCharStrdup("queue"),            xmlCharStrdup(job.getQueue().c_str()));
  xmlNewChild(node, NULL, xmlCharStrdup("exclusive"),        xmlCharStrdup(job.getExclusiveStr().c_str()));
  std::ostringstream mem_per_cpu_stream;
  mem_per_cpu_stream << job.getMemPerCpu();
  xmlNewChild(node, NULL, xmlCharStrdup("mem_per_cpu"),      xmlCharStrdup(mem_per_cpu_stream.str().c_str()));

  // For COORM
  xmlNewChild(node, NULL, xmlCharStrdup("launcher_args"), xmlCharStrdup(job.getLauncherArgs().c_str()));

  // Specific parameters part
  xmlNodePtr specific_parameters_node = xmlNewChild(node, NULL, xmlCharStrdup("specific_parameters"), NULL);
  std::map<std::string, std::string> specific_parameters = job.getSpecificParameters();
  for(std::map<std::string, std::string>::iterator it = specific_parameters.begin(); it != specific_parameters.end(); it++)
  {
    xmlNodePtr specific_parameter_node = xmlNewChild(specific_parameters_node, NULL, xmlCharStrdup("specific_parameter"), NULL);
    xmlNewChild(specific_parameter_node, NULL, xmlCharStrdup("name"), xmlCharStrdup((it->first).c_str()));
    xmlNewChild(specific_parameter_node, NULL, xmlCharStrdup("value"), xmlCharStrdup((it->second).c_str()));
  }

  // Run part
  xmlNodePtr run_node = xmlNewChild(job_node, NULL, xmlCharStrdup("run_part"), NULL);
  xmlNewChild(run_node, NULL, xmlCharStrdup("job_state"), xmlCharStrdup(job.getState().c_str()));
  ParserResourcesType resource_definition = job.getResourceDefinition();
  xmlNewChild(run_node, NULL, xmlCharStrdup("resource_choosed_name"), xmlCharStrdup(resource_definition.Name.c_str()));
  xmlNewChild(run_node, NULL, xmlCharStrdup("job_reference"), xmlCharStrdup(job.getReference().c_str()));
}

}
