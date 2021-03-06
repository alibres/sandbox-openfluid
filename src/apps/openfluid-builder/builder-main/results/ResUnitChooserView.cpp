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
 \file ResUnitChooserView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ResUnitChooserView.hpp"

#include <glibmm/i18n.h>

#include <gtkmm/table.h>
#include <gtkmm/separator.h>

#include <set>

// =====================================================================
// =====================================================================


void ResUnitChooserViewImpl::onOkButtonClicked()
{
  m_signal_SelectionValidated.emit();
}

// =====================================================================
// =====================================================================


void ResUnitChooserViewImpl::onTreeViewCBToggled(const Glib::ustring& /*Path*/)
{
  onOkButtonClicked();
}

// =====================================================================
// =====================================================================


ResUnitChooserViewImpl::ResUnitChooserViewImpl() :
  mp_MessageLabel(0)
{
  Gtk::Label* ClassNameLabel = Gtk::manage(new Gtk::Label(_("Class: ")));
  Gtk::Label* IDsLabel =
      Gtk::manage(new Gtk::Label(_("ID:") + std::string("")));

  mp_ShowFilesCB = Gtk::manage(new Gtk::CheckButton(_("Show file")));
  mp_ShowFilesCB->set_active(true);
  mp_ShowFilesCB->signal_toggled().connect(sigc::mem_fun(*this,
      &ResUnitChooserViewImpl::onOkButtonClicked));

  //  mp_MessageLabel = Gtk::manage(new Gtk::Label(""));
  //  mp_MessageLabel->set_visible(true);
  //  Gdk::Color c("red");
  //  mp_MessageLabel->modify_text(mp_MessageLabel->get_state(), c);

  mp_ClassNameValue = Gtk::manage(new Gtk::Label());

  mp_IdCombo = Gtk::manage(new Gtk::ComboBoxText());
  mp_IdCombo->signal_changed().connect(sigc::mem_fun(*this,
      &ResUnitChooserViewImpl::onOkButtonClicked));

  mp_OkButton = Gtk::manage(new Gtk::Button(_("Update")));
  mp_OkButton->signal_clicked().connect(sigc::mem_fun(*this,
      &ResUnitChooserViewImpl::onOkButtonClicked));

  mref_VariablesListStore = Gtk::ListStore::create(m_VarsColumns);

  mp_VariablesTreeView = Gtk::manage(new Gtk::TreeView(mref_VariablesListStore));
  mp_VariablesTreeView->set_headers_visible(false);
  mp_VariablesTreeView->set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_NONE);
  mp_VariablesTreeView->append_column_editable("", m_VarsColumns.m_isChecked);
  mp_VariablesTreeView->append_column("", m_VarsColumns.m_VarName);
  (static_cast<Gtk::CellRendererToggle*> (mp_VariablesTreeView->get_column_cell_renderer(
      0)))->signal_toggled().connect(sigc::mem_fun(*this,
      &ResUnitChooserViewImpl::onTreeViewCBToggled));

  Gtk::Box* TopBox = Gtk::manage(new Gtk::HBox());
  TopBox->pack_start(*ClassNameLabel, Gtk::PACK_SHRINK, 5);
  TopBox->pack_start(*mp_ClassNameValue, Gtk::PACK_SHRINK, 5);
  TopBox->pack_start(*IDsLabel, Gtk::PACK_SHRINK, 5);
  TopBox->pack_start(*mp_IdCombo, Gtk::PACK_SHRINK, 5);
  TopBox->pack_start(*mp_OkButton, Gtk::PACK_SHRINK, 0);

  Gtk::Table* VarsTable = Gtk::manage(new Gtk::Table());
  VarsTable->set_spacings(15);
  VarsTable->attach(*Gtk::manage(new Gtk::Label(_("Variables"))), 0, 1, 0, 1,
        Gtk::SHRINK, Gtk::SHRINK, 0, 0);
  VarsTable->attach(*mp_VariablesTreeView, 0, 1, 1, 2, Gtk::SHRINK, Gtk::SHRINK,
        0, 0);

  mp_MainBox = Gtk::manage(new Gtk::VBox());
  mp_MainBox->set_spacing(15);
  //  mp_MainBox->pack_start(*mp_MessageLabel, Gtk::PACK_SHRINK);
  mp_MainBox->pack_start(*TopBox, Gtk::SHRINK, 5);
  mp_MainBox->pack_start(*Gtk::manage(new Gtk::HSeparator()), Gtk::SHRINK, 5);
  mp_MainBox->pack_start(*mp_ShowFilesCB, Gtk::SHRINK, 5);
  mp_MainBox->pack_start(*Gtk::manage(new Gtk::HSeparator()), Gtk::SHRINK, 5);
  mp_MainBox->pack_start(*VarsTable, Gtk::SHRINK, 0);

  mp_MainBox->show_all_children();
}

// =====================================================================
// =====================================================================


sigc::signal<void> ResUnitChooserViewImpl::signal_SelectionValidated()
{
  return m_signal_SelectionValidated;
}

// =====================================================================
// =====================================================================


void ResUnitChooserViewImpl::setClassName(std::string ClassName)
{
  mp_ClassNameValue->set_text(ClassName);
}

// =====================================================================
// =====================================================================


void ResUnitChooserViewImpl::setIDs(std::vector<unsigned int> IDs)
{
  Glib::ustring ExistingSelection = mp_IdCombo->get_active_text();

  mp_IdCombo->clear_items();

  for (unsigned int i = 0; i < IDs.size(); i++)
  {
    mp_IdCombo->append_text(Glib::ustring::compose("%1", IDs[i]));
  }

  mp_IdCombo->set_active_text(ExistingSelection);
  if (mp_IdCombo->get_active_text() == "")
    mp_IdCombo->set_active(0);
}

// =====================================================================
// =====================================================================

void ResUnitChooserViewImpl::setVariables(std::vector<std::string> Values)
{
  bool IsFirstTurn = mref_VariablesListStore->children().empty();

    std::set<std::string> ExistingSelectedVars;

    if (!IsFirstTurn)
    {
      for (unsigned int i = 0; i < mref_VariablesListStore->children().size(); i++)
      {
        Gtk::TreeRow Row = mref_VariablesListStore->children()[i];
        if (Row[m_VarsColumns.m_isChecked])
          ExistingSelectedVars.insert(Row[m_VarsColumns.m_VarName]);
      }
    }

    mref_VariablesListStore->clear();

    for (unsigned int i = 0; i < Values.size(); i++)
    {
      Gtk::TreeRow Row = *(mref_VariablesListStore->append());
      Row[m_VarsColumns.m_VarName] = Values[i];
      Row[m_VarsColumns.m_isChecked] = IsFirstTurn ? true
          : ExistingSelectedVars.find(Values[i]) != ExistingSelectedVars.end();
    }

    mp_VariablesTreeView->set_model(mref_VariablesListStore);
}

// =====================================================================
// =====================================================================


void ResUnitChooserViewImpl::initSelection()
{
  onOkButtonClicked();
}

// =====================================================================
// =====================================================================


int ResUnitChooserViewImpl::getSelectedID()
{
  std::istringstream ss(mp_IdCombo->get_active_text());
  int res;
  ss >> res;
  return res;
}

// =====================================================================
// =====================================================================


std::vector<std::string> ResUnitChooserViewImpl::getSelectedVars()
{
  std::vector<std::string> Vars;

  for (unsigned int i = 0; i < mref_VariablesListStore->children().size(); i++)
  {
    Gtk::TreeRow Row = mref_VariablesListStore->children()[i];
    if (Row[m_VarsColumns.m_isChecked])
      Vars.push_back(Row[m_VarsColumns.m_VarName]);
  }

  return Vars;
}

// =====================================================================
// =====================================================================


bool ResUnitChooserViewImpl::getShowFiles()
{
  return mp_ShowFilesCB->get_active();
}

// =====================================================================
// =====================================================================


Gtk::Widget* ResUnitChooserViewImpl::asWidget()
{
  return mp_MainBox;
}

// =====================================================================
// =====================================================================

void ResUnitChooserViewImpl::setMessage(std::string MessageText)
{
  if (mp_MessageLabel)
  {
    mp_MessageLabel->set_text(MessageText);
  }
  else
  {
    mp_MessageLabel = Gtk::manage(new Gtk::Label(MessageText));
    Gdk::Color c("red");
    mp_MessageLabel->modify_fg(mp_MessageLabel->get_state(), c);
    mp_MessageLabel->set_visible(true);
    mp_MainBox->pack_start(*mp_MessageLabel, Gtk::PACK_SHRINK);
  }
}

// =====================================================================
// =====================================================================


void ResUnitChooserViewImpl::clearMessage()
{
  delete mp_MessageLabel;
  mp_MessageLabel = 0;
  //  if (mp_MessageLabel->get_text() != "")
  //    mp_MainBox->remove(*mp_MessageLabel);
}

// =====================================================================
// =====================================================================

