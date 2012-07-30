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
 \file LineStringEntity.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "LineStringEntity.hpp"

#include <geos/geom/LineString.h>
#include <geos/planargraph/DirectedEdge.h>
#include <geos/planargraph/Node.h>
#include <openfluid/landr/LineStringGraph.hpp>

namespace openfluid {
namespace landr {

// =====================================================================
// =====================================================================

LineStringEntity::LineStringEntity(const geos::geom::LineString* NewLine,
                                   OGRFeature* Feat) :
    LandREntity(Feat), geos::planargraph::Edge(), mp_Line(NewLine), mp_UpNeighbours(
        0), mp_DownNeighbours(0)
{
  mp_Centroide = mp_Line->getCentroid();
  m_Area = mp_Line->getArea();
}

// =====================================================================
// =====================================================================

LineStringEntity::LineStringEntity(const LineStringEntity& Other) :
    LandREntity(), geos::planargraph::Edge(), mp_UpNeighbours(0), mp_DownNeighbours(
        0)
{
  mp_Line = dynamic_cast<geos::geom::LineString*>(Other.getLine()->clone());
  mp_Feature = (const_cast<LineStringEntity&>(Other).getFeature())->Clone();
  mp_Centroide = mp_Line->getCentroid();
  m_Area = mp_Line->getArea();
}

// =====================================================================
// =====================================================================

LineStringEntity::~LineStringEntity()
{
  for (unsigned int i = 0; i < dirEdge.size(); i++)
    delete dirEdge[i];

  delete mp_Line;
  delete mp_UpNeighbours;
  delete mp_DownNeighbours;
}

// =====================================================================
// =====================================================================

const geos::geom::LineString* LineStringEntity::getLine() const
{
  return mp_Line;
}

// =====================================================================
// =====================================================================

geos::planargraph::Node* LineStringEntity::getStartNode()
{
  return getDirEdge(0)->getFromNode();
}

// =====================================================================
// =====================================================================

geos::planargraph::Node* LineStringEntity::getEndNode()
{
  return getDirEdge(0)->getToNode();
}

// =====================================================================
// =====================================================================

std::vector<LineStringEntity*> LineStringEntity::getUpNeighbours()
{
  if (!mp_UpNeighbours)
    computUpNeighbours();

  return *mp_UpNeighbours;
}

// =====================================================================
// =====================================================================

void LineStringEntity::computUpNeighbours()
{
  mp_UpNeighbours = new std::vector<LineStringEntity*>();

  geos::planargraph::DirectedEdgeStar* UpStar = getStartNode()->getOutEdges();

  geos::geom::Coordinate UpNodeCoo = getStartNode()->getCoordinate();

  for (std::vector<geos::planargraph::DirectedEdge*>::iterator it =
      UpStar->iterator(); it != UpStar->end(); ++it)
  {
    LineStringEntity* Unit = dynamic_cast<LineStringEntity*>((*it)->getEdge());

    if (Unit->getEndNode()->getCoordinate().equals(UpNodeCoo))
      mp_UpNeighbours->push_back(Unit);
  }
}

// =====================================================================
// =====================================================================

std::vector<LineStringEntity*> LineStringEntity::getDownNeighbours()
{
  if (!mp_DownNeighbours)
    computDownNeighbours();

  return *mp_DownNeighbours;
}

// =====================================================================
// =====================================================================

void LineStringEntity::computDownNeighbours()
{
  mp_DownNeighbours = new std::vector<LineStringEntity*>();

  geos::planargraph::DirectedEdgeStar* DownStar = getEndNode()->getOutEdges();

  geos::geom::Coordinate DownNodeCoo = getEndNode()->getCoordinate();

  for (std::vector<geos::planargraph::DirectedEdge*>::iterator it =
      DownStar->iterator(); it != DownStar->end(); ++it)
  {
    LineStringEntity* Unit = dynamic_cast<LineStringEntity*>((*it)->getEdge());

    if (Unit->getStartNode()->getCoordinate().equals(DownNodeCoo))
      mp_DownNeighbours->push_back(Unit);
  }
}

// =====================================================================
// =====================================================================

double LineStringEntity::getLength()
{
  return mp_Line->getLength();
}

// =====================================================================
// =====================================================================

}// namespace landr
} /* namespace openfluid */
