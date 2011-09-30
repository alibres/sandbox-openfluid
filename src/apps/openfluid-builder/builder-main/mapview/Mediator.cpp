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
 \file Mediator.cpp
 \brief Implements ...

 \author Damien CHABBERT <dams.vivien@gmail.com>
 */

//#include <set>

#include "LayerType.hpp"
#include "Mediator.hpp"

#include "EngineHelper.hpp"

#include <glibmm/i18n.h>
#include <openfluid/guicommon/DialogBoxFactory.hpp>

Mediator::Mediator(DrawingArea& DrawingArea, StatusBar& StatusBar,
    ToolBar& ToolBar) :
  mref_DrawingArea(DrawingArea), mref_StatusBar(StatusBar),
      mref_ToolBar(ToolBar), mp_CoreRepos(0)
{
  m_SelectedClassName = "";
  m_addDialogCreate = false;
  m_infoDialogCreate = false;
  mp_MainVBoxMediator = Gtk::manage(new Gtk::VBox());

  mref_DrawingArea.signal_ExposeEventChanged().connect(
      sigc::mem_fun(*this, &Mediator::whenDrawingAreaChanged));
  //***************Signal ToolBar*********************
  mref_ToolBar.signalAddLayerToolButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnAddLayerToolButtonClicked));
  mref_ToolBar.signalInfoToolButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnInfoToolButtonClicked));
  mref_ToolBar.signalMoveLayerToggleToolButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnMoveLayerToggleToolButtonClicked));
  mref_ToolBar.signalSelectAllPreferenceMenuClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnSelectAllPreferenceMenuClicked));
  mref_ToolBar.signalSelectObjectLayerToggleToolButtonClicked().connect(
      sigc::mem_fun(*this,
          &Mediator::whenOnSelectObjectLayerToggleToolButtonClicked));
  mref_ToolBar.signalShow100FocusButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnShow100FocusButtonClicked));
  mref_ToolBar.signalToggleSelectedPreferenceMenuClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnToggleSelectedPreferenceMenuClicked));
  mref_ToolBar.signalUnzoomCursorToggleToolButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnUnzoomCursorToggleToolButtonClicked));
  mref_ToolBar.signalZoomCursorZoomTypeButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnZoomCursorZoomTypeButtonClicked));
  mref_ToolBar.signalZoomFrameZoomTypeButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnZoomFrameZoomTypeButtonClicked));
  mref_ToolBar.signalZoomLayerFocusButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnZoomLayerFocusButtonClicked));
  mref_ToolBar.signalZoomSelectionFocusButtonClicked().connect(
      sigc::mem_fun(*this, &Mediator::whenOnZoomSelectionFocusButtonClicked));
  //***************Signal DrawingArea*****************
  mref_DrawingArea.signal_CoordinateChanged().connect(
      sigc::mem_fun(*this, &Mediator::whenOnMotionNotifyChanged));
  mref_DrawingArea.signal_CoordinateSelected().connect(
      sigc::mem_fun(*this, &Mediator::whenOnSelectObjectChanged));

  mp_MainVBoxMediator->set_visible(true);
}

// =====================================================================
// =====================================================================

Gtk::Widget* Mediator::asWidget()
{
  return mp_MainVBoxMediator;
}

// =====================================================================
// =====================================================================

void Mediator::setEngineRequirements(openfluid::core::CoreRepository& CoreRepos)
{
  mp_CoreRepos = &CoreRepos;
}

// =====================================================================
// =====================================================================

sigc::signal<void> Mediator::signal_DrawingAreaExposeEventChanged()
{
  return m_signal_DrawingAreaExposeEventChanged;
}

// =====================================================================
// =====================================================================

void Mediator::whenDrawingAreaChanged()
{
  //  m_signal_DrawingAreaExposeEventChanged.emit();
  redraw();
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void Mediator::whenOnShow100FocusButtonClicked()
{
  if (!m_Layer.empty())
  {
    std::vector<Layer*>::iterator it;
    std::pair<std::pair<double, double>, std::pair<double, double> > MinMax;
    MinMax.first.first = (*m_Layer.begin())->getMinMaxLayer().first.first;
    MinMax.first.second = (*m_Layer.begin())->getMinMaxLayer().first.second;
    MinMax.second.first = (*m_Layer.begin())->getMinMaxLayer().second.first;
    MinMax.second.second = (*m_Layer.begin())->getMinMaxLayer().second.second;
    for (it = m_Layer.begin(); it < m_Layer.end(); it++)
    {
      if (MinMax.first.first > (*it)->getMinMaxLayer().first.first)
        MinMax.first.first = (*it)->getMinMaxLayer().first.first;
      if (MinMax.first.second > (*it)->getMinMaxLayer().first.second)
        MinMax.first.second = (*it)->getMinMaxLayer().first.second;
      if (MinMax.second.first < (*it)->getMinMaxLayer().second.first)
        MinMax.second.first = (*it)->getMinMaxLayer().second.first;
      if (MinMax.second.second < (*it)->getMinMaxLayer().second.second)
        MinMax.second.second = (*it)->getMinMaxLayer().second.second;
    }
    mref_DrawingArea.modifyScaleTranslate(MinMax);
    redraw();
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnZoomSelectionFocusButtonClicked()
{
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitId.clear();
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't zoom in a selection without selection.\n\nPlease select a layer."));
  } else if (m_SelectedUnitId.empty())
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't zoom in a selection without selection.\n\nPlease select objects on a current layer."));
  } else
  {
    std::vector<Layer*>::iterator it;
    for (it = m_Layer.begin(); it < m_Layer.end(); it++)
    {
      if (m_SelectedClassName == (*it)->getClassName())
      {
        mref_DrawingArea.modifyScaleTranslate(
            (*it)->getMinMaxSelection(m_SelectedUnitId));
      }
    }
    redraw();
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnZoomLayerFocusButtonClicked()
{
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitId.clear();
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't zoom in a layer without select the corresponding layer before.\n\nPlease select a layer."));
  } else
  {
    std::vector<Layer*>::iterator it;
    for (it = m_Layer.begin(); it < m_Layer.end(); it++)
    {
      if (m_SelectedClassName == (*it)->getClassName())
      {
        mref_DrawingArea.modifyScaleTranslate((*it)->getMinMaxLayer());
      }
    }
    redraw();
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnZoomCursorZoomTypeButtonClicked()
{
  if (mref_ToolBar.getZoomTypeToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(4);
    mref_DrawingArea.changeToZoomCursorState();
  } else
  {
    mref_DrawingArea.changeToInitialState();
    mref_ToolBar.setSensitivePreferenceMenubar(false);
  }

}

// =====================================================================
// =====================================================================

void Mediator::whenOnZoomFrameZoomTypeButtonClicked()
{
  if (mref_ToolBar.getZoomTypeToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(4);
    mref_DrawingArea.changeToZoomFrameState();
  } else
  {
    mref_DrawingArea.changeToInitialState();
    mref_ToolBar.setSensitivePreferenceMenubar(false);
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnSelectAllPreferenceMenuClicked()
{
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitId.clear();
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't select objects without select the corresponding layer before.\n\nPlease select a layer."));
  } else
  {
    std::vector<Layer*>::iterator it;
    for (it = m_Layer.begin(); it < m_Layer.end(); it++)
    {
      if (m_SelectedClassName == (*it)->getClassName())
      {
        std::set<int> temp = (*it)->ObjectSelected();
        temp.swap(m_SelectedUnitId);
      }
    }
    redraw();
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnToggleSelectedPreferenceMenuClicked()
{
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitId.clear();
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't select objects without select the corresponding layer before.\n\nPlease select a layer."));
  } else
  {
    std::vector<Layer*>::iterator it;
    for (it = m_Layer.begin(); it < m_Layer.end(); it++)
    {
      if (m_SelectedClassName == (*it)->getClassName())
      {
        std::set<int> temp = (*it)->ObjectSelected();
        std::set<int>::iterator itset;
        for (itset = m_SelectedUnitId.begin(); itset != m_SelectedUnitId.end(); itset++)
        {
          std::set<int>::iterator it2;
          it2 = temp.find((*itset));
          temp.erase(it2);
        }
        temp.swap(m_SelectedUnitId);
      }
    }
    redraw();
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnAddLayerToolButtonClicked()
{

  if (!m_addDialogCreate)
  {
    mp_AddDialogFileChooser = new AddDialogFileChooser(
        dynamic_cast<Gtk::Window&> (*asWidget()->get_toplevel()),
        _("Please choose a new layer"));
    m_addDialogCreate = true;
  }

  Layer* pLayer;
  pLayer = new Layer(LayerType::LAYER_BASE);
  m_Layer.push_back(pLayer);

  mref_DrawingArea.setLayerExist(true);

  std::set<std::string> ClassNames = EngineHelper::getClassNames(mp_CoreRepos);
  std::vector<Layer*>::iterator it;
  for (it = m_Layer.begin(); it != m_Layer.end(); it++)
  {
    std::set<std::string>::iterator ite;
    ite = ClassNames.find((*it)->getClassName());
    if (ite != ClassNames.end())
      ClassNames.erase(ite);
  }

  if (!ClassNames.empty())
  {
    std::pair<std::pair<std::string, std::string>, std::string> AddFile =
        mp_AddDialogFileChooser->show(ClassNames);
    while (AddFile.second == "" && AddFile.first.first != ""
        && AddFile.first.second != "")
    {
      openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
          _(
              "You can't added a new layer because you aren't select an unit class.\n\nPlease select an unit class."));
      AddFile = mp_AddDialogFileChooser->show(ClassNames);
    }
    pLayer->addNewLayer(AddFile);

  } else
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't added a new layer because there aren't unit class free.\n\nPlease build new unit class or destruct an existing unit class to add a new layer."));

  }
  if (pLayer->getLoadShapeFile())
  {
    pLayer->update(*mp_CoreRepos);
    if (m_Layer.size() == 1)
    {
      mref_DrawingArea.modifyScaleTranslate(pLayer->getMinMaxLayer());
    }
    removeAllObjectMainVBoxMediator();
    addAllObjectMainVBoxMediator();

    //***************Signal Layer*********************
    pLayer->signalUpLayerButtonClicked().connect(
        sigc::mem_fun(*this, &Mediator::whenOnUpLayerButtonClicked));
    pLayer->signalDownLayerButtonClicked().connect(
        sigc::mem_fun(*this, &Mediator::whenOnDownLayerButtonClicked));
    pLayer->signalRemoveLayerButtonClicked().connect(
        sigc::mem_fun(*this, &Mediator::whenOnRemoveLayerButtonClicked));
    pLayer->signalIsDisplayButtonChecked().connect(
        sigc::mem_fun(*this, &Mediator::whenOnIsDisplayButtonChecked));
    pLayer->signalIsSelectedLayerClicked().connect(
        sigc::mem_fun(*this, &Mediator::whenOnIsSelectedLayerClicked));
    pLayer->signalWidgetExpanderBaseChanged().connect(
        sigc::mem_fun(*this, &Mediator::whenOnWidgetExpanderBaseChanged));
    mref_ToolBar.resetSensitiveToolBar(true);
    redraw();
  } else
    m_Layer.pop_back();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnInfoToolButtonClicked()
{
  if (m_SelectedClassName == "" ||  m_SelectedUnitId.empty())
  {
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't have informations without select the corresponding layer before and the corresponding units.\n\nPlease select a layer and units."));
  } else
  {
    if (!m_infoDialogCreate)
    {
      mp_InfoDialog = new Info(
          dynamic_cast<Gtk::Window&> (*asWidget()->get_toplevel()),
          _("Management"), *mp_CoreRepos);
      m_infoDialogCreate = true;
    }
    mp_InfoDialog->show(m_SelectedClassName, m_SelectedUnitId);
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnSelectObjectLayerToggleToolButtonClicked()
{
  if (mref_ToolBar.getSelectObjectLayerToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(2);
    mref_DrawingArea.changeToSelectState();
    mref_ToolBar.setSensitivePreferenceMenubar(true);
  } else
  {
    mref_DrawingArea.changeToInitialState();
    mref_ToolBar.setSensitivePreferenceMenubar(false);
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnMoveLayerToggleToolButtonClicked()
{
  if (mref_ToolBar.getMoveLayerToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(1);
    mref_DrawingArea.changeToMoveState();
  } else
  {
    mref_DrawingArea.changeToInitialState();
    mref_ToolBar.setSensitivePreferenceMenubar(false);
  }
}

// =====================================================================
// =====================================================================

void Mediator::whenOnUnzoomCursorToggleToolButtonClicked()
{
  if (mref_ToolBar.getUnzoomCursorToggleToolButton()->get_active())
  {
    mref_ToolBar.resetToolBar(3);
    mref_DrawingArea.changeToUnzoomCursorState();
  } else
  {
    mref_DrawingArea.changeToInitialState();
    mref_ToolBar.setSensitivePreferenceMenubar(false);
  }
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void Mediator::whenOnUpLayerButtonClicked(std::string ClassName)
{
  upLayer(ClassName);
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnDownLayerButtonClicked(std::string ClassName)
{
  downLayer(ClassName);
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnRemoveLayerButtonClicked(std::string ClassName)
{
  removeLayer(ClassName);
  if (m_Layer.empty())
    mref_ToolBar.resetSensitiveToolBar(false);
  if (m_SelectedClassName == ClassName)
    m_SelectedUnitId.clear();
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnIsSelectedLayerClicked(std::string ClassName)
{
  m_SelectedClassName = ClassName;
  m_SelectedUnitId.clear();
  std::vector<Layer*>::iterator it;
  for (it = m_Layer.begin(); it < m_Layer.end(); it++)
  {
    if (ClassName != (*it)->getClassName())
      (*it)->setIsSelected(false);
  }
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnIsDisplayButtonChecked()
{
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnWidgetExpanderBaseChanged()
{
  redraw();
}

// =====================================================================
// =====================================================================

void Mediator::whenOnMotionNotifyChanged(double X, double Y)
{
  std::stringstream str1;
  std::stringstream str2;
  std::string Xstr;
  std::string Ystr;
  if (!m_Layer.empty())
  {
    str1 << X;
    str2 << Y;
    Xstr = str1.str();
    Ystr = str2.str();
  } else
  {
    str1 << 0;
    str2 << 0;
    Xstr = str1.str();
    Ystr = str2.str();
  }
  mref_StatusBar.setXLabelCoordinate(Xstr);
  mref_StatusBar.setYLabelCoordinate(Ystr);
}

// =====================================================================
// =====================================================================

void Mediator::whenOnSelectObjectChanged(double X, double Y)
{
  bool isRedraw = false;
  if (m_SelectedClassName == "")
  {
    m_SelectedUnitId.clear();
    openfluid::guicommon::DialogBoxFactory::showSimpleWarningMessage(
        _(
            "You can't select objects without select the corresponding layer before.\n\nPlease select a layer."));
  } else
  {
    std::vector<Layer*>::iterator it;
    for (it = m_Layer.begin(); it < m_Layer.end(); it++)
    {
      if (m_SelectedClassName == (*it)->getClassName()
          && !m_SelectedUnitId.empty())
      {
        int temp = (*it)->isObjectSelected(X, Y, mref_DrawingArea.getScale());
        if (temp != -1)
        {
          std::set<int>::iterator it2;
          it2 = m_SelectedUnitId.find(temp);
          if (it2 == m_SelectedUnitId.end()
              && mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitId.insert(temp);
          } else if (it2 == m_SelectedUnitId.end()
              && !mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitId.clear();
            m_SelectedUnitId.insert(temp);
          } else if (it2 != m_SelectedUnitId.end()
              && mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitId.erase(it2);
          } else if (it2 != m_SelectedUnitId.end()
              && !mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitId.clear();
          }
          isRedraw = true;
        } else
        {
          if (!m_SelectedUnitId.empty()
              && !mref_DrawingArea.getPressMultiSelect())
          {
            m_SelectedUnitId.clear();
            isRedraw = true;
          }
        }
      } else if (m_SelectedClassName == (*it)->getClassName()
          && m_SelectedUnitId.empty())
      {
        int temp = (*it)->isObjectSelected(X, Y, mref_DrawingArea.getScale());
        if (temp != -1)
        {
          m_SelectedUnitId.insert(temp);
          isRedraw = true;
        }
      }
    }
  }

  if (isRedraw)
    redraw();
}

// =====================================================================
// =====================================================================
// =====================================================================
// =====================================================================

void Mediator::redraw()
{
  mref_DrawingArea.get_window()->clear();
  Glib::RefPtr<Gdk::Window> Window = mref_DrawingArea.get_window();
  //
  if (Window)
  {
    if (!m_Layer.empty())
    {
      Cairo::RefPtr<Cairo::Context> Context = Window->create_cairo_context();
      Gtk::Allocation allocation = mref_DrawingArea.get_allocation();
      const int width = allocation.get_width();
      const int height = allocation.get_height();

      Context->rectangle(0, 0, width, height);
      Context->clip();
      Context->set_antialias(Cairo::ANTIALIAS_SUBPIXEL);
      Context->scale(mref_DrawingArea.getScale(), -mref_DrawingArea.getScale());
      Context->translate(-mref_DrawingArea.getXTranslate(),
          -mref_DrawingArea.getYTranslate());
      std::vector<Layer*>::reverse_iterator rit;
      for (rit = m_Layer.rbegin(); rit < m_Layer.rend(); ++rit)
      {
        if ((*rit)->getIsDisplay())
        {
          (*rit)->initialiseLayerContext(Context, mref_DrawingArea.getScale());
          if ((*rit)->getClassName() == m_SelectedClassName)
          {
            (*rit)->draw(Context, mref_DrawingArea.getScale(),
                m_SelectedUnitId, (*rit)->getDisplayID());
          } else
          {
            std::set<int> tempVoidVector;
            (*rit)->draw(Context, mref_DrawingArea.getScale(), tempVoidVector,
                (*rit)->getDisplayID());
          }
        }
      }
    }
  }
}

// =====================================================================
// =====================================================================

void Mediator::upLayer(std::string ClassName)
{
  removeAllObjectMainVBoxMediator();
  Layer* pLayer;
  std::vector<Layer*>::reverse_iterator rit;
  for (rit = m_Layer.rbegin(); rit < m_Layer.rend(); ++rit)
  {
    if ((*rit)->getClassName() == ClassName)
    {
      pLayer = (*rit);
      m_Layer.erase(--rit.base());
      ++rit;
      m_Layer.insert(--rit.base(), pLayer);
    }
  }
  addAllObjectMainVBoxMediator();
}

// =====================================================================
// =====================================================================

void Mediator::downLayer(std::string ClassName)
{
  removeAllObjectMainVBoxMediator();
  Layer* pLayer;
  std::vector<Layer*>::iterator it;
  for (it = m_Layer.begin(); it < m_Layer.end(); it++)
  {
    if ((*it)->getClassName() == ClassName)
    {
      pLayer = (*it);
      m_Layer.erase(it);
      it++;
      m_Layer.insert(it, pLayer);
    }
  }
  addAllObjectMainVBoxMediator();
}

// =====================================================================
// =====================================================================

void Mediator::removeLayer(std::string ClassName)
{
  removeAllObjectMainVBoxMediator();
  std::vector<Layer*>::iterator it;
  for (it = m_Layer.begin(); it < m_Layer.end(); it++)
  {
    if ((*it)->getClassName() == ClassName)
    {
      m_Layer.erase(it);
    }
  }
  addAllObjectMainVBoxMediator();
  if (m_Layer.empty())
    mref_DrawingArea.setLayerExist(false);
}

// =====================================================================
// =====================================================================

void Mediator::removeAllObjectMainVBoxMediator()
{
  while (!mp_MainVBoxMediator->children().empty())
    mp_MainVBoxMediator->remove(**mp_MainVBoxMediator->get_children().begin());
}

// =====================================================================
// =====================================================================

void Mediator::addAllObjectMainVBoxMediator()
{
  if (!m_Layer.empty())
  {
    std::vector<Layer*>::iterator it;
    for (it = m_Layer.begin(); it < m_Layer.end(); it++)
    {
      mp_MainVBoxMediator->pack_start(*(*it)->asWidget(), Gtk::PACK_SHRINK);
      (*it)->setWidgetUpSensitive(true);
      (*it)->setWidgetDownSensitive(true);
    }
    m_Layer.front()->setWidgetUpSensitive(false);
    m_Layer.back()->setWidgetDownSensitive(false);
  }
}

// =====================================================================
// =====================================================================
