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
 \file SimulOutFilesModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutFilesModel.hpp"

#include <glibmm/ustring.h>

#include <openfluid/base/OutputDescriptor.hpp>
#include <openfluid/base/OutputFilesDescriptor.hpp>


// =====================================================================
// =====================================================================


SimulOutFilesModelImpl::SimulOutFilesModelImpl() :
  m_SelectedFileFormatName(""), mp_OutDesc(0)
{
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutFilesModelImpl::signal_FromAppDescriptorChanged()
{
  return m_signal_FromAppDescriptorChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutFilesModelImpl::signal_FromUserSelectionChanged()
{
  return m_signal_FromUserSelectionChanged;
}

// =====================================================================
// =====================================================================


sigc::signal<void> SimulOutFilesModelImpl::signal_Activated()
{
  return m_signal_Activated;
}

// =====================================================================
// =====================================================================


void SimulOutFilesModelImpl::setEngineRequirements(
    openfluid::base::OutputDescriptor& OutDesc)
{
  mp_OutDesc = &OutDesc;

  update();
}

// =====================================================================
// =====================================================================


openfluid::base::OutputDescriptor* SimulOutFilesModelImpl::getOutDescriptor()
{
  return mp_OutDesc;
}

// =====================================================================
// =====================================================================


void SimulOutFilesModelImpl::setSelectedFileFormatName(
    std::string FileFormatName)
{
  m_SelectedFileFormatName = FileFormatName;

  m_signal_FromUserSelectionChanged.emit();
}

// =====================================================================
// =====================================================================


openfluid::base::OutputFilesDescriptor* SimulOutFilesModelImpl::getSelectedFileFormat()
{
  if (m_ByNameOutFilesDesc.find(m_SelectedFileFormatName)
      != m_ByNameOutFilesDesc.end())
    return m_ByNameOutFilesDesc[m_SelectedFileFormatName];
  else
    return (openfluid::base::OutputFilesDescriptor*) 0;
}

// =====================================================================
// =====================================================================


std::string SimulOutFilesModelImpl::getSelectedFormatName()
{
  if (!existsFormatName(m_SelectedFileFormatName))
    m_SelectedFileFormatName = "";

  return m_SelectedFileFormatName;
}

// =====================================================================
// =====================================================================


void SimulOutFilesModelImpl::deleteSelectedFileFormat()
{
  for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size(); i++)
  {
    if (mp_OutDesc->getFileSets()[i].getName() == m_SelectedFileFormatName)
    {
      mp_OutDesc->getFileSets().erase(mp_OutDesc->getFileSets().begin() + i);
      m_ByNameOutFilesDesc.erase(m_SelectedFileFormatName);
      m_SelectedFileFormatName = "";
      m_signal_FromAppDescriptorChanged.emit();
      break;
    }
  }
}

// =====================================================================
// =====================================================================


void SimulOutFilesModelImpl::addFileFormat(
    openfluid::base::OutputFilesDescriptor* FileDesc)
{
  if (FileDesc)
  {
    mp_OutDesc->getFileSets().push_back(*FileDesc);

    m_ByNameOutFilesDesc[FileDesc->getName()] = FileDesc;

    m_SelectedFileFormatName = FileDesc->getName();

    m_signal_FromAppDescriptorChanged.emit();
  }
}

// =====================================================================
// =====================================================================


void SimulOutFilesModelImpl::update()
{
  m_ByNameOutFilesDesc.clear();

  int CurrentUnnamedFormatIndex = 1;

  for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size(); i++)
  {
    if (mp_OutDesc->getFileSets()[i].getName() == "")
      mp_OutDesc->getFileSets()[i].setName(Glib::ustring::compose("Format #%1",
          CurrentUnnamedFormatIndex++));

    m_ByNameOutFilesDesc[mp_OutDesc->getFileSets()[i].getName()]
        = &mp_OutDesc->getFileSets()[i];
  }

  m_signal_FromAppDescriptorChanged.emit();
}

// =====================================================================
// =====================================================================


bool SimulOutFilesModelImpl::selectedFileFormatHasSets()
{
  return (!getSelectedFileFormat()->getSets().empty());
}

// =====================================================================
// =====================================================================


bool SimulOutFilesModelImpl::isOutputEmpty()
{
  return mp_OutDesc->getFileSets().empty();
}

// =====================================================================
// =====================================================================


bool SimulOutFilesModelImpl::existsFormatName(std::string FormatName)
{
  for (unsigned int i = 0; i < mp_OutDesc->getFileSets().size(); i++)
  {
    if (mp_OutDesc->getFileSets()[i].getName() == FormatName)
      return true;
  }

  return false;
}

