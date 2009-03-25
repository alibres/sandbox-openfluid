/**
  \file PrimitivesUseFunc.h
  \brief Header of ...
*/

#ifndef __PRIMITIVESUSEFUNC_H__
#define __PRIMITIVESUSEFUNC_H__

#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS;


// =====================================================================
// =====================================================================


/**

*/
class PrimitivesUseFunction : public openfluid::base::PluggableFunction
{
  private:

  public:
    /**
      Constructor
    */
    PrimitivesUseFunction();

    /**
      Destructor
    */
    ~PrimitivesUseFunction();

    bool initParams(openfluid::core::ParamsMap Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif  // __PRIMITIVESUSEFUNC_H__
