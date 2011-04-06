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
 \file ModelCoordinator.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __MODELCOORDINATOR_HPP__
#define __MODELCOORDINATOR_HPP__

#include <sigc++/sigc++.h>

#include <openfluid/machine.hpp>

class ModelAvailFctModel;
class ModelFctDetailModel;
class ModelStructureModel;
class ModelGlobalParamsModel;
class BuilderListToolBox;
class FunctionSignatureRegistry;

class ModelCoordinator: public sigc::trackable
{
  private:
    FunctionSignatureRegistry* mp_SignatureRegistry;
    ModelAvailFctModel& m_AvailFctModel;
    ModelFctDetailModel& m_FctDetailModel;
    ModelStructureModel& m_StructureModel;
    ModelGlobalParamsModel& m_GlobalParamsModel;
    BuilderListToolBox& m_StructureListToolBox;
    void updateStructureListToolBox();
    void whenAvailFctSelectionChanged();
    void whenStructureFctSelectionChanged();
    void whenAddFctAsked();
    void whenRemoveFctAsked();
    void whenMoveTowardTheBeginAsked();
    void whenMoveTowardTheEndAsked();
  public:
    ModelCoordinator(ModelAvailFctModel& AvailFctModel,
        ModelFctDetailModel& FctDetailModel,
        ModelStructureModel& StructureModel,
        ModelGlobalParamsModel& GlobalParamsModel,
        BuilderListToolBox& StructureListToolBox);
    void setSignatures(FunctionSignatureRegistry& Signatures);
    void setFctToDisplay(openfluid::machine::SignatureItemInstance* Signature);
    void
        setEngineRequirements(openfluid::machine::ModelInstance& ModelInstance);
};

#endif /* __MODELCOORDINATOR_HPP__ */