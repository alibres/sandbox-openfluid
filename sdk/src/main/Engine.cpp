/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "Engine.h"


Engine::Engine(mhydasdk::core::CoreRepository* CoreData, mhydasdk::base::RuntimeEnvironment* RunEnv,
               PluginManager* PlugMan)
{

  mp_CoreData = CoreData;
  mp_RunEnv = RunEnv;
  mp_PlugMan = PlugMan;


  mp_HydroModule = NULL;
  m_HydroFunctions.Clear();

  mp_IOMan = new IOManager(mp_RunEnv);

}

// =====================================================================
// =====================================================================

Engine::~Engine()
{

}


bool Engine::processConfig()
{
  /** \internal

    uses the PluginManager to
      - load and parametrize functions
      - build the processing list of each module
  */


  FunctionConfigsList::Node *FuncNode = m_Config.HydroModuleConfig.GetFirst();
  FunctionConfig *FConf;

  m_HydroFunctions.clear();


  // start display test
/*
  while (FuncNode)
  {
    FConf = (FunctionConfig*)(FuncNode->GetData());
    std::cerr << "Function " << FConf->Name.mb_str(wxConvUTF8) << ", " << FConf->File.mb_str(wxConvUTF8) << std::endl;


    mhydasdk::core::ParamsMap::iterator it;
    for( it = FConf->Params.begin(); it != FConf->Params.end(); ++it )
    {
      std::cerr << "  - " << it->first.mb_str(wxConvUTF8) << " = " << it->second << std::endl;
    }

    FuncNode = FuncNode->GetNext();
  }
*/
  // end display test

  mhydasdk::base::Function* FuncToAdd;

  while (FuncNode)
  {
    FConf = (FunctionConfig*)(FuncNode->GetData());

    FuncToAdd = mp_PlugMan->getFunctionFromPlugin(FConf->File,mhydasdk::base::MOD_HYDROLOGY,mp_CoreData);

    if (FuncToAdd != NULL)
    {
       if (FuncToAdd->initParams(FConf->Params))
       {
         m_HydroFunctions.Append((mhydasdk::base::Function**)FuncToAdd);
       }
       else
       {
         mhydasdk::base::LastError::Message = wxT("Error initializing params function.");
         return false;
       }

    }
    else
    {
      mhydasdk::base::LastError::Message = wxT("Error loading function from plugin.");
      return false;
    }

    FuncNode = FuncNode->GetNext();
  }


  return true;
}


// =====================================================================
// =====================================================================


bool Engine::plugFunctions()
{

  mp_HydroModule = new Module(mp_CoreData,m_HydroFunctions);

  return true;
}


// =====================================================================
// =====================================================================

bool Engine::buildModel()
{
  /** \internal

    builds the model from the config file, calling:
    - loadModelConfig() from mp_IOMan
    - processConfig()
    - plugFunctions

  */

  return (mp_IOMan->loadModelConfig(&m_Config) && processConfig() && plugFunctions());
}


// =====================================================================
// =====================================================================

bool Engine::loadData()
{
  return (mp_IOMan->loadHydroObjects(mp_CoreData->getSpatialData()) &&
          mp_IOMan->loadRainEvent(mp_CoreData->getRainEvent()) &&
          mp_IOMan->loadRainDistribution(mp_CoreData));
}


// =====================================================================
// =====================================================================

bool Engine::checkConsistencyAndInitialize()
{

  // builds topology by linking objects
  if (!mp_CoreData->getSpatialData()->buildObjectLinkedTopologyFromIDs())
  {
    mhydasdk::base::LastError::Message = wxT("Topology rebuild error.");
    return false;
  }

  if (mp_HydroModule == NULL || !mp_HydroModule->checkConsistency())
  {
    mhydasdk::base::LastError::Message = wxT("Hydrology module consistency error.");
    return false;
  }

  // initialization of functions

  if (!mp_HydroModule->initialize())
  {
    mhydasdk::base::LastError::Message = wxT("Hydro module initialization error.");
    return false;
  }


  return true;
}


// =====================================================================
// =====================================================================


bool Engine::run()
{

  // inits

  mp_SimStatus = new mhydasdk::base::SimulationStatus(mp_CoreData->getRainEvent()->getEventStartingTime(),
                                                      mp_CoreData->getRainEvent()->getEventEndingTime(),
                                                      m_Config.DeltaT);

  // run
  do
  {
    std::cout << "t+";
    std::cout.width(5);
    std::cout << mp_SimStatus->getCurrentStep();
    std::cout.width(20);
    std::cout << mp_SimStatus->getCurrentTime().asString().mb_str(wxConvUTF8);

    mp_HydroModule->runStep(mp_SimStatus);

    std::cout << std::endl;

  } while (mp_SimStatus->switchToNextStep());



  // finalization of functions
  mp_HydroModule->finalize();

  return true;
}

// =====================================================================
// =====================================================================



bool Engine::saveResults()
{
  return true;
}

// =====================================================================
// =====================================================================

