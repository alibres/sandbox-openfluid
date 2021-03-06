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
 \file ModelGlobalParamsModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelGlobalParamsModel.hpp"

#include <boost/foreach.hpp>

#include <openfluid/ware/FunctionSignature.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/machine/ModelInstance.hpp>

// =====================================================================
// =====================================================================


ModelGlobalParamsModelImpl::ModelGlobalParamsModelImpl() :
  mp_ModelInstance(0)
{
}

// =====================================================================
// =====================================================================


sigc::signal<void> ModelGlobalParamsModelImpl::signal_FromAppModelChanged()
{
  return m_signal_FromAppModelChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> ModelGlobalParamsModelImpl::signal_GlobalValueChanged()
{
  return m_signal_GlobalValueChanged;
}

// =====================================================================
// =====================================================================


void ModelGlobalParamsModelImpl::setEngineRequirements(
    openfluid::machine::ModelInstance& ModelInstance)
{
  mp_ModelInstance = &ModelInstance;

  update();
}

// =====================================================================
// =====================================================================


void ModelGlobalParamsModelImpl::update()
{
  if (!mp_ModelInstance)
    return;

  m_GloballyNotUsed.clear();

  for (std::list<openfluid::machine::ModelItemInstance*>::const_iterator it =
      mp_ModelInstance->getItems().begin(); it
      != mp_ModelInstance->getItems().end(); ++it)
  {
    BOOST_FOREACH(openfluid::ware::SignatureHandledDataItem Param, (*it)->Signature->HandledData.FunctionParams)
    {
      if(mp_ModelInstance->getGlobalParameters().find(Param.DataName)
        == mp_ModelInstance->getGlobalParameters().not_found())
          m_GloballyNotUsed.insert(Param.DataName);

      m_ByParamNameParamUnit[Param.DataName] = Param.DataUnit;
    }
  }

  m_signal_FromAppModelChanged.emit();

}

// =====================================================================
// =====================================================================


std::map<std::string, std::pair<std::string, std::string> > ModelGlobalParamsModelImpl::getGloballyUsed()
{
  std::map<std::string, std::pair<std::string, std::string> > GlobalParams;

  for (openfluid::ware::WareParams_t::iterator it =
      mp_ModelInstance->getGlobalParameters().begin(); it
      != mp_ModelInstance->getGlobalParameters().end(); ++it)
    GlobalParams[it->first] = std::make_pair(m_ByParamNameParamUnit[it->first],
        it->second.data());

  return GlobalParams;
}

// =====================================================================
// =====================================================================


std::set<std::string> ModelGlobalParamsModelImpl::getGloballyNotUsed()
{
  return m_GloballyNotUsed;
}

// =====================================================================
// =====================================================================


std::string ModelGlobalParamsModelImpl::fromUserGloballyUsedSet(
    std::string ParamName)
{
  mp_ModelInstance->setGlobalParameter(ParamName, "");
  m_GloballyNotUsed.erase(ParamName);

  m_signal_GlobalValueChanged.emit();

  return m_ByParamNameParamUnit[ParamName];
}

// =====================================================================
// =====================================================================


void ModelGlobalParamsModelImpl::fromUserGloballyUsedUnset(
    std::string ParamName)
{
  m_GloballyNotUsed.insert(ParamName);
  mp_ModelInstance->getGlobalParameters().erase(ParamName);

  m_signal_GlobalValueChanged.emit();
}

// =====================================================================
// =====================================================================


void ModelGlobalParamsModelImpl::setGlobalValue(std::string ParamName,
    std::string ParamValue)
{
  mp_ModelInstance->setGlobalParameter(ParamName, ParamValue);

  m_signal_GlobalValueChanged.emit();
}

// =====================================================================
// =====================================================================


