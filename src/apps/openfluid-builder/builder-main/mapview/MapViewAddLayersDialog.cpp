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
 \file MapViewAddLayersDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "MapViewAddLayersDialog.hpp"

#include <glibmm/i18n.h>

#include <gtkmm/stock.h>
#include <gtkmm/label.h>
#include <gtkmm/frame.h>

#include <openfluid/core/CoreRepository.hpp>
#include <openfluid/core/Datastore.hpp>
#include <openfluid/core/DatastoreItem.hpp>
#include <openfluid/machine/SimulationBlob.hpp>

#include "Mediator.hpp"

// =====================================================================
// =====================================================================


MapViewAddLayersDialog::MapViewAddLayersDialog()
{
  mp_Dialog = new Gtk::Dialog(_("Displaying resources in MapView"));

  mref_TreeModel = Gtk::ListStore::create(m_Columns);

  mp_TreeView = Gtk::manage(new Gtk::TreeView(mref_TreeModel));

  mp_TreeView->append_column(_("Data type"), m_Columns.m_Type);
  mp_TreeView->get_column(0)->set_sort_column(m_Columns.m_Type);

  mp_TreeView->append_column(_("Data ID"), m_Columns.m_Id);
  mp_TreeView->get_column(1)->set_sort_column(m_Columns.m_Id);

  mp_TreeView->append_column(_("Unit Class"), m_Columns.m_Class);
  mp_TreeView->get_column(2)->set_sort_column(m_Columns.m_Class);

  mp_TreeView->append_column(_("Data source path"), m_Columns.m_Path);
  mp_TreeView->get_column(3)->set_sort_column(m_Columns.m_Path);

  mp_TreeView->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

  mp_TreeView->expand_all();

  mp_ScrolledWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_ScrolledWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_ScrolledWin->add(*mp_TreeView);

  Gtk::Frame* MainFrame = Gtk::manage(new Gtk::Frame());
  MainFrame->set_shadow_type(Gtk::SHADOW_IN);
  MainFrame->set_border_width(5);
  MainFrame->set_visible(true);
  MainFrame->add(*mp_ScrolledWin);

  Gtk::Label* TheLabel = Gtk::manage(new Gtk::Label(
      _("Choose resources to display:"), 0, 0.5));
  TheLabel->set_padding(0,5);

  mp_Dialog->get_vbox()->pack_start(*TheLabel, Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*MainFrame);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->set_default_size(500, 300);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================

MapViewAddLayersDialog::~MapViewAddLayersDialog()
{
  delete mp_Dialog;
}

// =====================================================================
// =====================================================================


void MapViewAddLayersDialog::setEngineRequirements(
    openfluid::machine::SimulationBlob& SimBlob)
{
  mp_CoreRepos = &SimBlob.getCoreRepository();
  mp_Datastore = &SimBlob.getDatastore();
}

// =====================================================================
// =====================================================================


std::set<std::string> MapViewAddLayersDialog::show(
    const std::set<std::string>& AlreadyDisplayedLayersIds)
{
  update(AlreadyDisplayedLayersIds);

  std::set<std::string> SelectedLayersIds;

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    Gtk::TreeSelection::ListHandle_Path Selecteds =
        mp_TreeView->get_selection()->get_selected_rows();

    for (Gtk::TreeSelection::ListHandle_Path::iterator it = Selecteds.begin(); it
        != Selecteds.end(); ++it)
    {
      SelectedLayersIds.insert(mref_TreeModel->get_iter(*it)->get_value(
          m_Columns.m_Id));
    }
  }

  mp_Dialog->hide();

  return SelectedLayersIds;
}

// =====================================================================
// =====================================================================


void MapViewAddLayersDialog::update(
    const std::set<std::string>& AlreadyDisplayedLayersIds)
{
  mp_TreeView->get_selection()->unselect_all();

  mref_TreeModel->clear();

  openfluid::core::Datastore::DataItemsById_t Items = mp_Datastore->getItems();

  for (openfluid::core::Datastore::DataItemsById_t::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    if (AlreadyDisplayedLayersIds.count(it->first))
      continue;

    if (!(Mediator::hasADisplayableVectorValue(*it->second, *mp_CoreRepos)
        || Mediator::hasADisplayableRasterValue(*it->second)))
      continue;

    Gtk::TreeRow Row = *mref_TreeModel->append();
    Row[m_Columns.m_Type]
        = openfluid::core::UnstructuredValue::getStringFromValueType(
            it->second->getValue()->getType());
    Row[m_Columns.m_Id] = it->first;
    Row[m_Columns.m_Class] = it->second->getUnitClass();
    Row[m_Columns.m_Path] = it->second->getRelativePath();
  }

  mp_TreeView->expand_all();
}
