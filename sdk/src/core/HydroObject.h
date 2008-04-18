/**
  \file HydroObject.h
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
 */

#ifndef __HYDROOBJECT_H__
#define __HYDROOBJECT_H__

#include <string>
#include <vector>
#include <wx/hashmap.h>

#include "EventCollection.h"
#include "TypeDefs.h"

namespace mhydasdk { namespace core {


/**
  Hash table for calculated variables during simulation.
  Each variable is stored in the hash table as an entry, located by its name.
  (exemple "effrain" for efficient rain calculated by the production function)
  Each variable is stored as a vector of double (one vector item = one step, vector[25] is calculated at the 25th step)
 */
WX_DECLARE_STRING_HASH_MAP(SerieOfMHYDASScalarValue*, SimulatedVarsMap);

WX_DECLARE_STRING_HASH_MAP(SerieOfMHYDASVectorValue*, SimulatedVectorVarsMap);

/**
  Hash table for parameters (distributed properties, distributed initial conditions, ...)
 */
WX_DECLARE_STRING_HASH_MAP(PropertyValue,PropertiesMap);

/**
  Hash table for parameters (distributed properties, distributed initial conditions, ...)
 */
WX_DECLARE_STRING_HASH_MAP(wxString,ParamsMap);


/**
  Generic definition class for Hydrological Objetcs

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
 */
class HydroObject
{
  private:

  protected:
    HOID m_ID;
    int m_ProcessOrder;

    SimulatedVarsMap* mp_SimVars;
    SimulatedVectorVarsMap* mp_SimVectorVars;    

    PropertiesMap* mp_Properties;

    PropertiesMap* mp_IniConditions;

    EventCollection* mp_Events;


  public:
    /**
		  Default constructor
     */
    HydroObject();

    /**
		  Constructor
		  \param[in] Geometry The objects geometry
		  \param[in] ID The object identifier
		  \param[in] Comment The associated comment
		  \param[in] ProcessOrder the process order of the objects during the model execution
     */
    HydroObject(HOID ID, int ProcessOrder);

    /**
		  Virtual destructor
     */
    virtual ~HydroObject();

    HOID getID() const;

    int getProcessOrder() const;

    void setProcessOrder(const int ProcessOrder);

    SimulatedVarsMap* getSimulatedVars() { return mp_SimVars; };
    SimulatedVectorVarsMap* getSimulatedVectorVars() { return mp_SimVectorVars; };    

    PropertiesMap* getProperties() { return mp_Properties; };

    PropertiesMap* getIniConditions() { return mp_IniConditions; };

    EventCollection* getEvents() { return mp_Events; };
};



} } // namespace mhydasdk::core



#endif
