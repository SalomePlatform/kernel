// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
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
  if (xmlStrToString(root_node->name) == "jobs")
  {
    xmlNodePtr xmlCurrentNode = root_node->xmlChildrenNode;
    while(xmlCurrentNode != NULL)
    {
      if (xmlStrToString(xmlCurrentNode->name) == "job")
      {
        LAUNCHER_INFOS("A job is found");
        Job * new_job = createJobFromXmlNode(xmlCurrentNode);
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
  xmlNodePtr job_node = addNode(root_node, "job", "");
  addAttr(job_node, "type", job.getJobType());
  addAttr(job_node, "name", job.getJobName());

  // Add user part
  xmlNodePtr node = addNode(job_node, "user_part", "");

  addNode(node, "job_file", job.getJobFile());
  if (!job.getEnvFile().empty())
    addNode(node, "env_file", job.getEnvFile());
  if (!job.getWorkDirectory().empty())
    addNode(node, "work_directory", job.getWorkDirectory());
  if (!job.getLocalDirectory().empty())
    addNode(node, "local_directory", job.getLocalDirectory());
  if (!job.getResultDirectory().empty())
    addNode(node, "result_directory", job.getResultDirectory());

  // Parameters for COORM
  if (!job.getLauncherFile().empty())
    addNode(node, "launcher_file", job.getLauncherFile());
  if (!job.getLauncherArgs().empty())
    addNode(node, "launcher_args", job.getLauncherArgs());

  // Files
  if ( ! (job.get_in_files().empty() && job.get_out_files().empty()) )
  {
    xmlNodePtr files_node = addNode(node, "files", "");
    list<string> in_files = job.get_in_files();
    list<string> out_files = job.get_out_files();
    for(list<string>::iterator it = in_files.begin(); it != in_files.end(); it++)
      addNode(files_node, "in_file", *it);
    for(list<string>::iterator it = out_files.begin(); it != out_files.end(); it++)
      addNode(files_node, "out_file", *it);
  }

  // Resource part
  resourceParams resource_params = job.getResourceRequiredParams();
  xmlNodePtr res_node = addNode(node, "resource_params", "");
  addNode(res_node, "name", resource_params.name);
  if (!resource_params.hostname.empty())
    addNode(res_node, "hostname", resource_params.hostname);
  if (!resource_params.OS.empty())
    addNode(res_node, "OS", resource_params.OS);
  if (resource_params.nb_proc > 0)
    addNumericalNode(res_node, "nb_proc", resource_params.nb_proc);
  if (resource_params.nb_node > 0)
    addNumericalNode(res_node, "nb_node", resource_params.nb_node);
  if (resource_params.nb_proc_per_node > 0)
    addNumericalNode(res_node, "nb_proc_per_node", resource_params.nb_proc_per_node);
  if (resource_params.cpu_clock > 0)
    addNumericalNode(res_node, "cpu_clock", resource_params.cpu_clock);
  if (resource_params.mem_mb > 0)
    addNumericalNode(res_node, "mem_mb", resource_params.mem_mb);

  if (!job.getMaximumDuration().empty())
    addNode(node, "maximum_duration", job.getMaximumDuration());
  if (!job.getQueue().empty())
    addNode(node, "queue", job.getQueue());
  if (job.getExclusive())
    addNode(node, "exclusive", job.getExclusiveStr());
  if (job.getMemPerCpu() > 0)
    addNumericalNode(res_node, "mem_per_cpu", job.getMemPerCpu());
  if (!job.getWCKey().empty())
    addNode(node, "wckey", job.getWCKey());
  if (!job.getExtraParams().empty())
    addNode(node, "extra_params", job.getExtraParams());

  // Specific parameters part
  map<string, string> specific_parameters = job.getSpecificParameters();
  if (!specific_parameters.empty())
  {
    xmlNodePtr specific_parameters_node = addNode(node, "specific_parameters", "");
    for(map<string, string>::iterator it = specific_parameters.begin();
        it != specific_parameters.end();
        it++)
    {
      xmlNodePtr specific_parameter_node = addNode(specific_parameters_node,
                                                   "specific_parameter", "");
      addNode(specific_parameter_node, "name", it->first);
      addNode(specific_parameter_node, "value", it->second);
    }
  }

  // Run part
  xmlNodePtr run_node = addNode(job_node, "run_part", "");
  addNode(run_node, "job_state", job.getState());
  addNode(run_node, "job_reference", job.getReference());
}

Job *
XML_Persistence::createJobFromXmlNode(xmlNodePtr job_node)
{
  Launcher::Job * new_job;

  // Begin Job
  string job_name = getAttrValue(job_node, "name");
  if (job_name.empty())
    throw LauncherException("Invalid job: name is not defined");
  string job_type = getAttrValue(job_node, "type");
  if (job_type.empty())
    throw LauncherException(string("Invalid job \"") + job_name + "\": type is not defined");
  if (job_type == "command")
    new_job = new Launcher::Job_Command();
  else if (job_type == "yacs_file")
    new_job = new Launcher::Job_YACSFile();
  else if (job_type == "python_salome")
    new_job = new Launcher::Job_PythonSALOME();
  else
  {
    string error = string("Invalid job \"") + job_name + "\": invalid type \"" + job_type + "\"";
    throw LauncherException(error);
  }
  new_job->setJobName(job_name);

  try
  {
    xmlNodePtr current_node = xmlFirstElementChild(job_node);
    bool user_ok = false;
    bool run_ok = false;
    while (current_node != NULL)
    {
      string node_name = xmlStrToString(current_node->name);
      if (node_name == "user_part")
      {
        parseUserNode(new_job, current_node);
        user_ok = true;
      }
      else if (node_name == "run_part")
      {
        parseRunNode(new_job, current_node);
        run_ok = true;
      }
      else
        throw LauncherException(string("invalid node \"") + node_name + "\"");
      current_node = xmlNextElementSibling(current_node);
    }
    if (!user_ok) throw LauncherException("missing user part");
    if (!run_ok) throw LauncherException("missing run part");
  }
  catch (const LauncherException & exc)
  {
    delete new_job;
    string error = string("Invalid job \"") + job_name + "\": " + exc.msg;
    throw LauncherException(error);
  }

  return new_job;
}

void
XML_Persistence::parseUserNode(Job * new_job, xmlNodePtr user_node)
{
  xmlNodePtr current_node = xmlFirstElementChild(user_node);
  bool job_file_ok = false;
  while (current_node != NULL)
  {
    string node_name = xmlStrToString(current_node->name);
    if (node_name == "job_file")
    {
      new_job->setJobFile(getNodeContent(current_node));
      job_file_ok = true;
    }
    else if (node_name == "env_file")
      new_job->setEnvFile(getNodeContent(current_node));
    else if (node_name == "work_directory")
      new_job->setWorkDirectory(getNodeContent(current_node));
    else if (node_name == "local_directory")
      new_job->setLocalDirectory(getNodeContent(current_node));
    else if (node_name == "result_directory")
      new_job->setResultDirectory(getNodeContent(current_node));
    else if (node_name == "launcher_file") // For COORM
      new_job->setLauncherFile(getNodeContent(current_node));
    else if (node_name == "launcher_args") // For COORM
      new_job->setLauncherArgs(getNodeContent(current_node));
    else if (node_name == "files")
    {
      // Get in and out files
      xmlNodePtr file_node = xmlFirstElementChild(current_node);
      while (file_node != NULL)
      {
        string file_node_name = xmlStrToString(file_node->name);
        if (file_node_name == "in_file")
          new_job->add_in_file(getNodeContent(file_node));
        else if (file_node_name == "out_file")
          new_job->add_out_file(getNodeContent(file_node));
        else
          throw LauncherException(string("invalid node \"") + file_node_name + "\"");
        file_node = xmlNextElementSibling(file_node);
      }
    }
    else if (node_name == "resource_params")
      parseResourceNode(new_job, current_node);
    else if (node_name == "maximum_duration")
      new_job->setMaximumDuration(getNodeContent(current_node));
    else if (node_name == "queue")
      new_job->setQueue(getNodeContent(current_node));
    else if (node_name == "exclusive")
      new_job->setExclusiveStr(getNodeContent(current_node));
    else if (node_name == "mem_per_cpu")
      new_job->setMemPerCpu(getNumericalNodeContent<unsigned long>(current_node));
    else if (node_name == "wckey")
      new_job->setWCKey(getNodeContent(current_node));
    else if (node_name == "extra_params")
      new_job->setExtraParams(getNodeContent(current_node));
    else if (node_name == "specific_parameters")
    {
      // Get specific parameters
      xmlNodePtr parameter_node = xmlFirstElementChild(current_node);
      while (parameter_node != NULL)
      {
        string parameter_node_name = xmlStrToString(parameter_node->name);
        if (parameter_node_name == "specific_parameter")
        {
          xmlNodePtr inparam_node = xmlFirstElementChild(parameter_node);
          string name;
          string value;
          while (inparam_node != NULL)
          {
            string inparam_node_name = xmlStrToString(inparam_node->name);
            if (inparam_node_name == "name")
              name = getNodeContent(inparam_node);
            else if (inparam_node_name == "value")
              value = getNodeContent(inparam_node);
            else
              throw LauncherException(string("invalid node \"") + inparam_node_name + "\"");
            inparam_node = xmlNextElementSibling(inparam_node);
          }
          if (name.empty()) throw LauncherException("missing parameter name");
          if (value.empty()) throw LauncherException("missing parameter value");
          new_job->addSpecificParameter(name, value);
        }
        else
          throw LauncherException(string("invalid node \"") + parameter_node_name + "\"");
        parameter_node = xmlNextElementSibling(parameter_node);
      }
    }
    else
      throw LauncherException(string("invalid node \"") + node_name + "\"");
    current_node = xmlNextElementSibling(current_node);
  }
  if (!job_file_ok) throw LauncherException("missing job file");
}

void
XML_Persistence::parseResourceNode(Job * new_job, xmlNodePtr res_node)
{
  resourceParams p;
  xmlNodePtr current_node = xmlFirstElementChild(res_node);
  while (current_node != NULL)
  {
    string node_name = xmlStrToString(current_node->name);
    if (node_name == "name")
      p.name = getNodeContent(current_node);
    else if (node_name == "hostname")
      p.hostname = getNodeContent(current_node);
    else if (node_name == "OS")
      p.OS = getNodeContent(current_node);
    else if (node_name == "nb_proc")
      p.nb_proc = getNumericalNodeContent<long>(current_node);
    else if (node_name == "nb_node")
      p.nb_node = getNumericalNodeContent<long>(current_node);
    else if (node_name == "nb_proc_per_node")
      p.nb_proc_per_node = getNumericalNodeContent<long>(current_node);
    else if (node_name == "cpu_clock")
      p.cpu_clock = getNumericalNodeContent<long>(current_node);
    else if (node_name == "mem_mb")
      p.mem_mb = getNumericalNodeContent<long>(current_node);
    else if (node_name == "mem_per_cpu")
      new_job->setMemPerCpu(getNumericalNodeContent<long>(current_node));
    else
      throw LauncherException(string("invalid node \"") + node_name + "\"");
    current_node = xmlNextElementSibling(current_node);
  }
  new_job->setResourceRequiredParams(p);
}

void
XML_Persistence::parseRunNode(Job * new_job, xmlNodePtr run_node)
{
  xmlNodePtr current_node = xmlFirstElementChild(run_node);
  while (current_node != NULL)
  {
    string node_name = xmlStrToString(current_node->name);
    if (node_name == "job_state")
      new_job->setState(getNodeContent(current_node));
    else if (node_name == "resource_choosed_name")
    {
      // This parameter was present in older versions of Salome. Now we just silently ignore it.
    }
    else if (node_name == "job_reference")
      new_job->setReference(getNodeContent(current_node));
    else
      throw LauncherException(string("invalid node \"") + node_name + "\"");
    current_node = xmlNextElementSibling(current_node);
  }
}

string
XML_Persistence::getAttrValue(xmlNodePtr node, const string & attrName)
{
  string attrValue;
  xmlChar * xmlAttrName = xmlCharStrdup(attrName.c_str());
  xmlChar * xmlAttrValue = xmlGetProp(node, xmlAttrName);
  if (xmlAttrValue != NULL) attrValue = (const char *)xmlAttrValue;
  xmlFree(xmlAttrName);
  xmlFree(xmlAttrValue);
  return attrValue;
}

inline string
XML_Persistence::xmlStrToString(const xmlChar * xmlStr)
{
  return string((const char *)xmlStr);
}

string
XML_Persistence::getNodeContent(xmlNodePtr node)
{
  string nodeContent;
  xmlChar * xmlStrContent = xmlNodeGetContent(node);
  if (xmlStrContent != NULL) nodeContent = (const char *)xmlStrContent;
  xmlFree(xmlStrContent);
  return nodeContent;
}

template<typename T>
T
XML_Persistence::getNumericalNodeContent(xmlNodePtr node)
{
  T result;
  istringstream nodeContentStream(getNodeContent(node));
  if (!(nodeContentStream >> result))
    throw LauncherException(xmlStrToString(node->name) + " parameter is not correct");
  return result;
}

xmlNodePtr
XML_Persistence::addNode(xmlNodePtr father, const string & name, const string & content)
{
  xmlChar * xmlStrName = xmlCharStrdup(name.c_str());
  xmlChar * xmlStrContent = NULL;
  if (!content.empty())
    xmlStrContent = xmlCharStrdup(content.c_str());
  xmlNodePtr node = xmlNewChild(father, NULL, xmlStrName, xmlStrContent);
  xmlFree(xmlStrName);
  xmlFree(xmlStrContent);
  return node;
}

template<typename T>
xmlNodePtr
XML_Persistence::addNumericalNode(xmlNodePtr father, const string & name, T content)
{
  ostringstream nodeContentStream;
  nodeContentStream << content;
  return addNode(father, name, nodeContentStream.str());
}

void
XML_Persistence::addAttr(xmlNodePtr node, const string & name, const string & value)
{
  xmlChar * xmlStrName = xmlCharStrdup(name.c_str());
  xmlChar * xmlStrValue = xmlCharStrdup(value.c_str());
  xmlNewProp(node, xmlStrName, xmlStrValue);
  xmlFree(xmlStrName);
  xmlFree(xmlStrValue);
}

}
