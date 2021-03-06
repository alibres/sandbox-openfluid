/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file SimulationInspectorWare.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/ware/SimulationInspectorWare.hpp>


namespace openfluid { namespace ware {

// =====================================================================
// =====================================================================


bool SimulationInspectorWare::IsUnitIDInPtrList(const openfluid::core::UnitsPtrList_t* UnitsList,
                                          const openfluid::core::UnitID_t& ID)
{
  if (UnitsList == NULL) return false;

  bool Found = false;
  openfluid::core::UnitsPtrList_t::const_iterator UnitsIt = UnitsList->begin();

  while (!Found && UnitsIt != UnitsList->end())
  {
    if ((*UnitsIt)->getID() == ID) Found = true;
    UnitsIt++;
  }

  return Found;
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                            const openfluid::core::InputDataName_t InputName,
                            openfluid::core::StringValue& Val) const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                             "SimulationInspectorWare::OPENFLUID_GetInputData","Inputdata cannot be accessed during INITPARAMS stage")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getInputData()->getValue(InputName,Val))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationInspectorWare::OPENFLUID_GetInputData","StringValue for input data "+ InputName +" does not exist");
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationInspectorWare::OPENFLUID_GetInputData","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::InputDataName_t InputName,
                                    openfluid::core::DoubleValue* Val) const
{
  OPENFLUID_GetInputData(UnitPtr,InputName,*Val);
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::InputDataName_t InputName,
                                    openfluid::core::DoubleValue& Val) const
{
  double TmpVal;
  OPENFLUID_GetInputData(UnitPtr,InputName,TmpVal);
  Val.set(TmpVal);
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::InputDataName_t InputName,
                                    openfluid::core::VectorValue* Val) const
{
  OPENFLUID_GetInputData(UnitPtr,InputName,*Val);
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::InputDataName_t InputName,
                                    openfluid::core::VectorValue& Val) const
{
  openfluid::core::StringValue TmpVal;
  OPENFLUID_GetInputData(UnitPtr,InputName,TmpVal);
  TmpVal.toVectorValue(Val.getStreamSeparator(1),Val);
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                            const openfluid::core::InputDataName_t InputName,
                            openfluid::core::MatrixValue& Val) const
{
  openfluid::core::StringValue TmpVal;
  OPENFLUID_GetInputData(UnitPtr,InputName,TmpVal);
  TmpVal.toMatrixValue(Val.getStreamSeparator(1),Val.getStreamSeparator(2),Val);
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::InputDataName_t InputName,
                                    double *Val) const
{
  OPENFLUID_GetInputData(UnitPtr,InputName,*Val);
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::InputDataName_t InputName,
                                    double& Val) const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                               "SimulationInspectorWare::OPENFLUID_GetInputData","Inputdata cannot be accessed during INITPARAMS stage")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getInputData()->getValueAsDouble(InputName,Val))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationInspectorWare::OPENFLUID_GetInputData","Double value for input data "+ InputName +" does not exist");
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationInspectorWare::OPENFLUID_GetInputData","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::InputDataName_t InputName,
                                    long *Val) const
{
  OPENFLUID_GetInputData(UnitPtr,InputName,*Val);
}



// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::InputDataName_t InputName,
                                    long& Val) const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                                 "SimulationInspectorWare::OPENFLUID_GetInputData","Inputdata cannot be accessed during INITPARAMS stage")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getInputData()->getValueAsLong(InputName,Val))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationInspectorWare::OPENFLUID_GetInputData","Long integer for input data "+ InputName +" does not exist");
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationInspectorWare::OPENFLUID_GetInputData","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::InputDataName_t InputName,
                                    std::string *Val) const
{
  OPENFLUID_GetInputData(UnitPtr,InputName,*Val);
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::InputDataName_t InputName,
                                    std::string& Val) const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::PREPAREDATA,
                                 "SimulationInspectorWare::OPENFLUID_GetInputData","Inputdata cannot be accessed during INITPARAMS stage")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getInputData()->getValue(InputName,Val))
      throw openfluid::base::OFException("OpenFLUID framework","SimulationInspectorWare::OPENFLUID_GetInputData","String value for input data "+ InputName +" does not exist");
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationInspectorWare::OPENFLUID_GetInputData","Unit is NULL");
}


// =====================================================================
// =====================================================================


bool SimulationInspectorWare::OPENFLUID_IsInputDataExist(const openfluid::core::Unit *UnitPtr,
                               const openfluid::core::InputDataName_t InputName) const
{
  if (UnitPtr != NULL)
    return UnitPtr->getInputData()->isDataExist(InputName);

  throw openfluid::base::OFException("OpenFLUID framework","SimulationInspectorWare::OPENFLUID_IsInputDataExist","Unit is NULL");
  return false;
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetVariable(const openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t VarName,
                                        const openfluid::core::TimeStep_t Step,
                                        openfluid::core::Value* Val) const
{
  OPENFLUID_GetVariable(UnitPtr, VarName,Step,*Val);
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetVariable(const openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t VarName,
                                        const openfluid::core::TimeStep_t Step,
                                        openfluid::core::Value& Val) const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::INITIALIZERUN,
                              "SimulationInspectorWare::OPENFLUID_GetVariable","Variables can be accessed only during INITIALIZERUN, RUNSTEP and FINALIZERUN stages")

  if (UnitPtr != NULL)
  {
    if (!UnitPtr->getVariables()->getValue(VarName,Step,&Val))
    {
      std::string TimeStr;
      openfluid::tools::ConvertValue(Step,&TimeStr);
      throw openfluid::base::OFException("OpenFLUID framework","SimulationInspectorWare::OPENFLUID_GetVariable","Value for variable "+ VarName +" does not exist or is not right type at time step "+ TimeStr);
    }
  }
  else throw openfluid::base::OFException("OpenFLUID framework","SimulationInspectorWare::OPENFLUID_GetVariable","Unit is NULL");
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                                       const openfluid::core::VariableName_t VarName,
                                       const openfluid::core::TimeStep_t Step,
                                       double* Val) const
{
  OPENFLUID_GetVariable(UnitPtr,VarName,Step,*Val);
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                                       const openfluid::core::VariableName_t VarName,
                                       const openfluid::core::TimeStep_t Step,
                                       double& Val) const
{
  openfluid::core::DoubleValue TmpVal(Val);
  OPENFLUID_GetVariable(UnitPtr,VarName,Step,&TmpVal);
  Val = TmpVal.get();
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                                       const openfluid::core::VariableName_t VarName,
                                       const openfluid::core::TimeStep_t Step,
                                       long* Val) const
{
  OPENFLUID_GetVariable(UnitPtr,VarName,Step,*Val);
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                                       const openfluid::core::VariableName_t VarName,
                                       const openfluid::core::TimeStep_t Step,
                                       long& Val) const
{
  openfluid::core::IntegerValue TmpVal(Val);
  OPENFLUID_GetVariable(UnitPtr,VarName,Step,&TmpVal);
  Val = TmpVal.get();
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                                       const openfluid::core::VariableName_t VarName,
                                       const openfluid::core::TimeStep_t Step,
                                       bool* Val) const
{
  OPENFLUID_GetVariable(UnitPtr,VarName,Step,*Val);
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                                       const openfluid::core::VariableName_t VarName,
                                       const openfluid::core::TimeStep_t Step,
                                       bool& Val) const
{
  openfluid::core::BooleanValue TmpVal(Val);
  OPENFLUID_GetVariable(UnitPtr,VarName,Step,&TmpVal);
  Val = TmpVal.get();
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                                       const openfluid::core::VariableName_t VarName,
                                       const openfluid::core::TimeStep_t Step,
                                       std::string* Val) const
{
  OPENFLUID_GetVariable(UnitPtr,VarName,Step,*Val);
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                                       const openfluid::core::VariableName_t VarName,
                                       const openfluid::core::TimeStep_t Step,
                                       std::string& Val) const
{
  openfluid::core::StringValue TmpVal(Val);
  OPENFLUID_GetVariable(UnitPtr,VarName,Step,&TmpVal);
  Val = TmpVal.get();
}


// =====================================================================
// =====================================================================


bool SimulationInspectorWare::OPENFLUID_IsVariableExist(const openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t VarName) const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::INITIALIZERUN,
                              "SimulationInspectorWare::OPENFLUID_IsVariableExist","Variables can be accessed only during INITIALIZERUN, RUNSTEP and FINALIZERUN stages")

  return (UnitPtr != NULL && UnitPtr->getVariables()->isVariableExist(VarName));
}


// =====================================================================
// =====================================================================


bool SimulationInspectorWare::OPENFLUID_IsVariableExist(const openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t VarName,
                                        const openfluid::core::TimeStep_t Step) const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::INITIALIZERUN,
                              "SimulationInspectorWare::OPENFLUID_IsVariableExist","Variables can be accessed only during INITIALIZERUN, RUNSTEP and FINALIZERUN stages")

   return (UnitPtr != NULL && UnitPtr->getVariables()->isVariableExist(VarName,Step));
}


// =====================================================================
// =====================================================================


bool SimulationInspectorWare::OPENFLUID_IsVariableExist(const openfluid::core::Unit *UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               const openfluid::core::Value::Type ValueType) const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::INITIALIZERUN,
                              "SimulationInspectorWare::OPENFLUID_IsVariableExist","Variables can be accessed only during INITIALIZERUN, RUNSTEP and FINALIZERUN stages")

  return (UnitPtr != NULL && UnitPtr->getVariables()->isVariableExist(VarName,Step,ValueType));
}


// =====================================================================
// =====================================================================


bool SimulationInspectorWare::OPENFLUID_IsTypedVariableExist(const openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t VarName,
                                        const openfluid::core::Value::Type VarType) const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::INITIALIZERUN,
                              "SimulationInspectorWare::OPENFLUID_IsTypedVariableExist","Variables can be accessed only during INITIALIZERUN, RUNSTEP and FINALIZERUN stages")

  return (UnitPtr != NULL && UnitPtr->getVariables()->isTypedVariableExist(VarName,VarType));
}


// =====================================================================
// =====================================================================


bool SimulationInspectorWare::OPENFLUID_IsTypedVariableExist(const openfluid::core::Unit *UnitPtr,
                                        const openfluid::core::VariableName_t VarName,
                                        const openfluid::core::TimeStep_t Step,
                                        const openfluid::core::Value::Type VarType) const
{
  REQUIRE_SIMULATION_STAGE_GE(openfluid::base::SimulationStatus::INITIALIZERUN,
                              "SimulationInspectorWare::OPENFLUID_IsTypedVariableExist","Variables can be accessed only during INITIALIZERUN, RUNSTEP and FINALIZERUN stages")

  return (UnitPtr != NULL && UnitPtr->getVariables()->isTypedVariableExist(VarName,Step,VarType));
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetEvents(const openfluid::core::Unit *UnitPtr,
                                            const openfluid::core::DateTime BeginDate,
                                            const openfluid::core::DateTime EndDate,
                                            openfluid::core::EventsCollection* Events) const
{
  OPENFLUID_GetEvents(UnitPtr,BeginDate,EndDate,*Events);
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetEvents(const openfluid::core::Unit *UnitPtr,
                                            const openfluid::core::DateTime BeginDate,
                                            const openfluid::core::DateTime EndDate,
                                            openfluid::core::EventsCollection& Events) const
{
  if (UnitPtr != NULL)
    UnitPtr->getEvents()->getEventsBetween(BeginDate,EndDate,Events);

  else throw openfluid::base::OFException("OpenFLUID framework","SimulationInspectorWare::OPENFLUID_GetEvents","Unit is NULL");
}


// =====================================================================
// =====================================================================


bool SimulationInspectorWare::OPENFLUID_IsUnitExist(openfluid::core::UnitClass_t ClassName,
                                              openfluid::core::UnitID_t ID) const
{
  return (mp_CoreData->getUnit(ClassName,ID) != NULL);
}


// =====================================================================
// =====================================================================


bool SimulationInspectorWare::OPENFLUID_IsUnitClassExist(openfluid::core::UnitClass_t ClassName) const
{
  return mp_CoreData->isUnitsClassExist(ClassName);
}


// =====================================================================
// =====================================================================


bool SimulationInspectorWare::OPENFLUID_GetUnitsCount(const openfluid::core::UnitClass_t ClassName,
                                                unsigned int& UnitsCount) const
{
  UnitsCount = 0;
  if (mp_CoreData->isUnitsClassExist(ClassName))
  {
    UnitsCount = mp_CoreData->getUnits(ClassName)->getList()->size();
    return true;
  }
  else return false;
}


// =====================================================================
// =====================================================================


void SimulationInspectorWare::OPENFLUID_GetUnitsCount(unsigned int& UnitsCount) const
{
  UnitsCount = 0;
  openfluid::core::UnitsListByClassMap_t::const_iterator UnitsIt;

  for (UnitsIt = mp_CoreData->getUnitsByClass()->begin(); UnitsIt != mp_CoreData->getUnitsByClass()->end();++UnitsIt )
  {
    UnitsCount = UnitsCount + (*UnitsIt).second.getList()->size();
  }
}


// =====================================================================
// =====================================================================


bool SimulationInspectorWare::OPENFLUID_GetUnit(const openfluid::core::UnitClass_t& ClassName,
                                          const openfluid::core::UnitID_t& ID,
                                          openfluid::core::Unit* aUnit) const
{
  aUnit =  const_cast<openfluid::core::Unit*>(mp_CoreData->getUnit(ClassName,ID));
  return (aUnit != NULL);
}


// =====================================================================
// =====================================================================


openfluid::core::Unit* SimulationInspectorWare::OPENFLUID_GetUnit(const openfluid::core::UnitClass_t& ClassName,
                                                            const openfluid::core::UnitID_t& ID) const
{
  return mp_CoreData->getUnit(ClassName,ID);
}


// =====================================================================
// =====================================================================

bool SimulationInspectorWare::OPENFLUID_IsUnitConnectedTo(openfluid::core::Unit* aUnit,
                                                    const openfluid::core::UnitClass_t& ClassNameTo,
                                                    const openfluid::core::UnitID_t& IDTo) const
{
  if (aUnit == NULL || !mp_CoreData->isUnitsClassExist(ClassNameTo)) return false;
  return IsUnitIDInPtrList(aUnit->getToUnits(ClassNameTo),IDTo);
}


// =====================================================================
// =====================================================================


bool SimulationInspectorWare::OPENFLUID_IsUnitConnectedFrom(openfluid::core::Unit* aUnit,
                                                      const openfluid::core::UnitClass_t& ClassNameFrom,
                                                      const openfluid::core::UnitID_t& IDFrom) const
{
  if (aUnit == NULL || !mp_CoreData->isUnitsClassExist(ClassNameFrom)) return false;
  return IsUnitIDInPtrList(aUnit->getFromUnits(ClassNameFrom),IDFrom);
}


// =====================================================================
// =====================================================================


bool SimulationInspectorWare::OPENFLUID_IsUnitChildOf(openfluid::core::Unit* aUnit,
                                                const openfluid::core::UnitClass_t& ClassNameParent,
                                                const openfluid::core::UnitID_t& IDParent) const
{
  if (aUnit == NULL || !mp_CoreData->isUnitsClassExist(ClassNameParent)) return false;
  return IsUnitIDInPtrList(aUnit->getParentUnits(ClassNameParent),IDParent);
}


// =====================================================================
// =====================================================================


bool SimulationInspectorWare::OPENFLUID_IsUnitParentOf(openfluid::core::Unit* aUnit,
                                                 const openfluid::core::UnitClass_t& ClassNameChild,
                                                 const openfluid::core::UnitID_t& IDChild) const
{
  if (aUnit == NULL || !mp_CoreData->isUnitsClassExist(ClassNameChild)) return false;
  return IsUnitIDInPtrList(aUnit->getChildrenUnits(ClassNameChild),IDChild);
}



} } // openfluid::ware

