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
 \file DomainIDataRemoveDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainIDataRemoveDialog.hpp"

#include <gtkmm/stock.h>

#include <glibmm/i18n.h>

#include <boost/foreach.hpp>

// =====================================================================
// =====================================================================


DomainIDataRemoveDialog::DomainIDataRemoveDialog() :
  mp_CoreRepos(0), m_ClassName("")
{
  mp_Dialog = new Gtk::Dialog(_("Removing Inputdata"));

  //  Gtk::Label* MessageLabel = Gtk::manage(new Gtk::Label(
  //      _("All values of this data will be destroyed")));

  Gtk::Label* NameLabel = Gtk::manage(new Gtk::Label(_("Inputdata name:"),
      Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));

  mp_Combo = Gtk::manage(new Gtk::ComboBoxText());

  Gtk::HBox* HBox = Gtk::manage(new Gtk::HBox());
  HBox->pack_start(*NameLabel, Gtk::PACK_SHRINK, 3);
  HBox->pack_start(*mp_Combo);

  mp_Dialog->get_vbox()->pack_start(*HBox, Gtk::PACK_SHRINK, 5);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================


void DomainIDataRemoveDialog::setEngineRequirements(
    openfluid::core::CoreRepository& CoreRepos)
{
  mp_CoreRepos = &CoreRepos;
}

// =====================================================================
// =====================================================================


void DomainIDataRemoveDialog::setClass(std::string ClassName)
{
  m_ClassName = ClassName;

  update();
}

// =====================================================================
// =====================================================================


void DomainIDataRemoveDialog::update()
{
  mp_Combo->clear_items();

  if (mp_CoreRepos->getUnits(m_ClassName))
  {
    // get the first IData only, supposed to be the same on all the class
    BOOST_FOREACH(std::string DataName,mp_CoreRepos->getUnits(m_ClassName)->getList()->begin()->getInputData()->getInputDataNames())
{    mp_Combo->append_text(DataName);
  }
  mp_Combo->set_active(0);
}

}

// =====================================================================
// =====================================================================


std::string DomainIDataRemoveDialog::show()
{
  std::string IDataName = "";

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    IDataName = mp_Combo->get_active_text();
  }

  mp_Dialog->hide();

  return IDataName;
}
