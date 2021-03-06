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
 \file ProjectExplorerModel.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __PROJECTEXPLORERMODEL_HPP__
#define __PROJECTEXPLORERMODEL_HPP__

#include <sigc++/sigc++.h>
#include <string>

#include "ProjectExplorerCategories.hpp"

namespace openfluid {
namespace machine {
class ModelInstance;
class SimulationBlob;
}
}


// =====================================================================
// =====================================================================


class ProjectExplorerModel
{
  public:

    virtual sigc::signal<void> signal_Initialized() = 0;

    virtual sigc::signal<void> signal_ActivationChanged() = 0;

    virtual sigc::signal<void> signal_UpdateModelAsked() = 0;

    virtual sigc::signal<void> signal_UpdateDomainAsked() = 0;

    virtual sigc::signal<void> signal_UpdateSimulationAsked() = 0;

    virtual sigc::signal<void,bool> signal_UpdateResultsAsked() = 0;

    virtual void setEngineRequirements(
        openfluid::machine::ModelInstance& ModelInstance,
        openfluid::machine::SimulationBlob& SimBlob) = 0;

    virtual void updateModelAsked() = 0;

    virtual void updateDomainAsked() = 0;

    virtual void updateSimulationAsked() = 0;

    virtual void updateResultsAsked(bool WithWarningState) = 0;

    virtual openfluid::machine::ModelInstance* getModelInstance() = 0;

    virtual openfluid::machine::SimulationBlob* getSimulationBlob() = 0;

    virtual void setActivatedElements(
        std::pair<ProjectExplorerCategories::ProjectExplorerCategory,
            std::string> ElementsPair) = 0;

    virtual std::pair<ProjectExplorerCategories::ProjectExplorerCategory,
        std::string> getActivatedElement() = 0;

};

// =====================================================================
// =====================================================================


class ProjectExplorerModelImpl: public ProjectExplorerModel
{
  private:

    sigc::signal<void> m_signal_Initialized;

    sigc::signal<void> m_signal_ActivationChanged;

    sigc::signal<void> m_signal_UpdateModelAsked;

    sigc::signal<void> m_signal_UpdateDomainAsked;

    sigc::signal<void> m_signal_UpdateSimulationAsked;

    sigc::signal<void,bool> m_signal_UpdateResultsAsked;

    openfluid::machine::ModelInstance* mp_ModelInstance;

    openfluid::machine::SimulationBlob* mp_SimBlob;

    std::pair<ProjectExplorerCategories::ProjectExplorerCategory, std::string>
        m_ActivatedElementsPair;

  public:

    ProjectExplorerModelImpl();

    sigc::signal<void> signal_Initialized();

    sigc::signal<void> signal_ActivationChanged();

    sigc::signal<void> signal_UpdateModelAsked();

    sigc::signal<void> signal_UpdateDomainAsked();

    sigc::signal<void> signal_UpdateSimulationAsked();

    sigc::signal<void,bool> signal_UpdateResultsAsked();

    void setEngineRequirements(
        openfluid::machine::ModelInstance& ModelInstance,
        openfluid::machine::SimulationBlob& SimBlob);

    void updateModelAsked();

    void updateDomainAsked();

    void updateSimulationAsked();

    void updateResultsAsked(bool WithWarningState);

    openfluid::machine::ModelInstance* getModelInstance();

    openfluid::machine::SimulationBlob* getSimulationBlob();

    void setActivatedElements(
        std::pair<ProjectExplorerCategories::ProjectExplorerCategory,
            std::string> ElementsPair);

    std::pair<ProjectExplorerCategories::ProjectExplorerCategory, std::string>
    getActivatedElement();

};

#endif /* __PROJECTEXPLORERMODEL_HPP__ */
