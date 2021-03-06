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
 \file SimulOutSetsView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutSetsView.hpp"

#include <glibmm/i18n.h>

#include <openfluid/guicommon/DialogBoxFactory.hpp>

// =====================================================================
// =====================================================================


void SimulOutSetsViewImpl::onSelectionChanged()
{
  m_signal_SetSelectionChanged.emit();
}

// =====================================================================
// =====================================================================


void SimulOutSetsViewImpl::onRowActivated(const Gtk::TreeModel::Path& /*Path*/,
    Gtk::TreeViewColumn* /*Column*/)
{
  m_signal_Activated.emit();
}

// =====================================================================
// =====================================================================


SimulOutSetsViewImpl::SimulOutSetsViewImpl()
{
  mp_TreeView = Gtk::manage(new Gtk::TreeView());

  mp_TreeView->append_column(_("Set name"), m_Columns.m_Name);
  mp_TreeView->append_column(_("Unit class"), m_Columns.m_UnitsClass);
  mp_TreeView->append_column(_("Unit IDs"), m_Columns.m_UnitsIDs);
  mp_TreeView->append_column(_("Variables"), m_Columns.m_Vars);
  mp_TreeView->append_column(_("Format name"), m_Columns.m_FormatName);
  mp_TreeView->append_column(_("Precision"), m_Columns.m_Precision);

  mp_TreeView->get_selection()->signal_changed().connect(sigc::mem_fun(*this,
      &SimulOutSetsViewImpl::onSelectionChanged));
  mp_TreeView->signal_row_activated().connect(sigc::mem_fun(*this,
      &SimulOutSetsViewImpl::onRowActivated));

  mp_TreeView->set_visible(true);

  mp_MainWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_MainWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_MainWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
  mp_MainWin->set_visible(true);
  mp_MainWin->add(*mp_TreeView);
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetsViewImpl::signal_SetSelectionChanged()
{
  return m_signal_SetSelectionChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutSetsViewImpl::signal_Activated()
{
  return m_signal_Activated;
}

// =====================================================================
// =====================================================================


void SimulOutSetsViewImpl::setModel(Glib::RefPtr<Gtk::TreeModel> Model)
{
  mp_TreeView->set_model(Model);
}

// =====================================================================
// =====================================================================


void SimulOutSetsViewImpl::setSelectedRow(Gtk::TreeRow Row)
{
  mp_TreeView->get_selection()->select(Row);
}

// =====================================================================
// =====================================================================


Gtk::TreeIter SimulOutSetsViewImpl::getSelectedIter()
{
  return mp_TreeView->get_selection()->get_selected();
}

// =====================================================================
// =====================================================================


Gtk::Widget* SimulOutSetsViewImpl::asWidget()
{
  return mp_MainWin;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


Gtk::TreeView* SimulOutSetsViewSub::getTreeView()
{
  return mp_TreeView;
}
