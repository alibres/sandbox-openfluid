/**
  \file PluginManager.cpp
  \brief implements the plugins management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <wx/dynlib.h>

#include "config.h"

#include "PluginManager.h"
#include "AppTools.h"
#include "base/OFException.h"

#include <iostream>

// =====================================================================
// =====================================================================


PluginManager::PluginManager(openfluid::base::ExecutionMessages* ExecMsgs, RuntimeEnvironment* RunEnv)
{
  mp_RunEnv = RunEnv;
  mp_ExecMsgs = ExecMsgs;
}

// =====================================================================
// =====================================================================

PluginManager::~PluginManager()
{

}



PluginContainer *PluginManager::buildPluginContainer(std::string PluginFilename)
{
  wxDynamicLibrary *PlugLib = new wxDynamicLibrary();
  std::string PluginFile =  mp_RunEnv->getPluginFullPath(PluginFilename);
  PluginContainer* Plug = NULL;

  //if (!wxFileExists(PluginFile)) throw openfluid::base::OFException("Function file "+ std::string(PluginFilename.mb_str(wxConvUTF8)) +"not found");

  // library loading
  if (PluginFile.length()>0 && PlugLib->Load(_U(PluginFile.c_str())))
  {
    // TODO to develop
    /*
    if (PlugLib->HasSymbol(wxT(PLUGSDKVERSION_PROC_NAME)))
    {
      std::cout << ((openfluid::base::GetSignatureProc)PlugLib->GetSymbol(wxT(PLUGSDKVERSION_PROC_NAME))) << std::endl;
    }*/

    // checks if the handle proc exists
    if (PlugLib->HasSymbol(wxT(PLUGFUNCTION_PROC_NAME)) && PlugLib->HasSymbol(wxT(PLUGSIGNATURE_PROC_NAME)))
    {

      // hooks the handle proc
      openfluid::base::GetSignatureProc SignProc = (openfluid::base::GetSignatureProc)PlugLib->GetSymbol(wxT(PLUGSIGNATURE_PROC_NAME));


      if (SignProc != NULL)
      {
        Plug = new PluginContainer();

        Plug->Signature = SignProc();

        openfluid::base::GetPluggableFunctionProc PlugProc = (openfluid::base::GetPluggableFunctionProc)PlugLib->GetSymbol(wxT(PLUGFUNCTION_PROC_NAME));
        if (PlugProc != NULL)
        {
          Plug->Function = PlugProc();
          Plug->Filename = PluginFile;
        }
        else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find function in plugin file " + PluginFilename);
      }
      else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find signature in plugin file " + PluginFilename);

      // unloads the library
      //PlugLib->Unload();

    }
  }
  else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find plugin file " + PluginFilename);

  return Plug;
}



// =============================================================signature========
// =====================================================================
/*
ArrayOfPluginsSignatures PluginManager::getAvailableFunctionsList()
{
  ArrayOfPluginsSignatures Signatures;
  wxArrayString PluginsPaths = mp_RunEnv->getPluginsPaths();
  wxArrayString PluginFiles;
  wxArrayString TmpFiles;
  int i,j;


  for (i=0;i<PluginsPaths.GetCount();i++)
  {
    TmpFiles = GetFilesByExt(PluginsPaths[i],OPENFLUID_PLUGINS_EXT);
    for (j=0;j<TmpFiles.GetCount();j++) PluginFiles.Add(TmpFiles[j]);
  }


  openfluid::base::PluggableFunction* CurrentPlug;

  for (i=0;i<PluginFiles.GetCount();i++)
  {
    CurrentPlug = getPluggableFunction(PluginFiles[i]);
    if (CurrentPlug != NULL) Signatures.Add(CurrentPlug->getSignature());
  }

  return Signatures;
}
*/

ArrayOfPluginsContainers PluginManager::getAvailableFunctions()
{
  ArrayOfPluginsContainers PluginsContainers;
  std::vector<std::string> PluginsPaths = mp_RunEnv->getPluginsPaths();
  std::vector<std::string> PluginFiles;
  std::vector<std::string> TmpFiles;
  int i,j;


  for (i=0;i<PluginsPaths.size();i++)
  {
    TmpFiles = GetFilesByExt(PluginsPaths[i],OPENFLUID_PLUGINS_EXT,false,true);
    for (j=0;j<TmpFiles.size();j++) PluginFiles.push_back(TmpFiles[j]);
  }


  PluginContainer* CurrentPlug;

  for (i=0;i<PluginFiles.size();i++)
  {
    CurrentPlug = buildPluginContainer(PluginFiles[i]);

    if (CurrentPlug != NULL) PluginsContainers.push_back(CurrentPlug);
  }

  return PluginsContainers;
}




// =====================================================================
// =====================================================================

PluginContainer *PluginManager::getPlugin(std::string PluginName,
                                          openfluid::core::CoreRepository* CoreData)
{

  PluginContainer *Plug = buildPluginContainer(PluginName+OPENFLUID_PLUGINS_EXT);



  if (Plug != NULL)
  {
    Plug->Function->setDataRepository(CoreData);
    Plug->Function->setExecutionMessages(mp_ExecMsgs);
    Plug->Function->setFunctionEnvironment(mp_RunEnv->getFunctionEnvironment());
    return Plug;
  }


  return Plug;
}

