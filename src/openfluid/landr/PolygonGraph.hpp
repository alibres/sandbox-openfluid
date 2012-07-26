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
 \file PolygonGraph.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef POLYGONGRAPH_HPP_
#define POLYGONGRAPH_HPP_

#include <geos/planargraph/PlanarGraph.h>

#include <openfluid/core/GeoVectorValue.hpp>

namespace geos {
namespace geom {
class LineString;
class Polygon;
class Coordinate;
class Geometry;
class GeometryFactory;
}

namespace planargraph {
class Node;
class Edge;
class DirectedEdge;
}
}

namespace openfluid {

namespace core {
class GeoRasterValue;
}

namespace landr {

class PolygonEntity;
class PolygonEdge;

class PolygonGraph: public geos::planargraph::PlanarGraph
{
  public:

    typedef std::map<geos::geom::Polygon*, double> RastValByRastPoly_t;

  private:

    std::vector<geos::planargraph::Node*> m_NewNodes;

    std::vector<geos::planargraph::DirectedEdge*> m_NewDirEdges;

    const geos::geom::GeometryFactory* mp_Factory;

    std::map<int, openfluid::landr::PolygonEntity*> m_EntitiesBySelfId;

    std::vector<openfluid::landr::PolygonEntity*> m_Entities;

    openfluid::core::GeoRasterValue* mp_Raster;

    openfluid::core::GeoVectorValue* mp_RasterPolygonized;

    std::vector<geos::geom::Polygon*>* mp_RasterPolygonizedPolys;

    static int FileNum;

    void deleteAll();

    /**
     * @brief Creates a new PolygonEdge, with its two DirectedEdges and add them to this graph.
     *
     * @param LineString The LineString representing the Edge to create.
     * @return The newly created Edge, or 0 if fails.
     */
    PolygonEdge* createEdge(geos::geom::LineString& LineString);

    geos::planargraph::Node* getNode(const geos::geom::Coordinate& Coordinate);

    /**
     * @brief Removes a segment of the exterior boundary of the input Entity.
     * @param Entity The entity to removes the segment to.
     * @param Segment The LineString to remove.
     */
    void removeSegment(PolygonEntity* Entity, geos::geom::LineString* Segment);

  public:

    PolygonGraph();

    PolygonGraph(const openfluid::core::GeoVectorValue& Val);

    /**
     * Do not copy associated raster.
     */
    PolygonGraph(openfluid::landr::PolygonGraph& Other);

    PolygonGraph(const std::vector<openfluid::landr::PolygonEntity*>& Entities);

    /**
     * Delete also associated RasterPolygonized if present.
     */
    virtual ~PolygonGraph();

    /**
     * @brief Returns a LineString representing the linearized input Geometry.
     *
     * @param Geom The Geometry to linearize.
     * @return A new allocated LineString representing the linearized input Geometry,
     * or 0 if the Geometry cannot be linearized into a single LineString.
     */
    static geos::geom::LineString* getMergedLineStringFromGeometry(
        geos::geom::Geometry* Geom);

    /**
     * @brief Returns a vector of LineStrings representing the linearized input Geometry.
     *
     * @param Geom The Geometry to linearize.
     * @return A new allocated vector of LineStrings representing the maximal linearized input Geometry,
     * or 0 if the Geometry is not \"Line\" typed.
     */
    static std::vector<geos::geom::LineString*>* getMergedLineStringsFromGeometry(
        geos::geom::Geometry* Geom);

    /**
     * Takes ownership of Polygon and Feature
     */
    openfluid::landr::PolygonEntity* addPolygon(
        const geos::geom::Polygon* Polygon, OGRFeature* Feat);

    unsigned int getSize();

    openfluid::landr::PolygonEntity* getEntity(int SelfId);

    std::vector<openfluid::landr::PolygonEntity*> getEntities();

    std::vector<openfluid::landr::PolygonEntity*> getSelfIdOrderedEntities();

    std::map<int, openfluid::landr::PolygonEntity*> getEntitiesBySelfId();

    /**
     * Doesn't reset if the AttributeName already exists.
     */
    void addAttribute(std::string AttributeName);

    /**
     * Does nothing if AttributeName doesn't exist.
     */
    void removeAttribute(std::string AttributeName);

    bool isComplete();

    void removeUnusedNodes();

    /**
     * Replace associated raster if exists.
     */
    void addAGeoRasterValue(openfluid::core::GeoRasterValue& Raster);

    float* getRasterValueForEntityCentroid(PolygonEntity& Entity);

    /**
     * Get a map of Polygons and its area intersecting Entity, from associated polygonized Raster.
     *
     * @param Entity The Entity to compare with the associated Raster.
     *
     * @return A map of Polygons from associated polygonized raster, with for each one the relevant intersecting area.
     */
    RastValByRastPoly_t getRasterPolyOverlapping(
        PolygonEntity& Entity);

    openfluid::core::GeoVectorValue* getRasterPolygonized();

    std::vector<geos::geom::Polygon*>* getRasterPolygonizedPolys();
};

} // namespace landr
} /* namespace openfluid */
#endif /* POLYGONGRAPH_HPP_ */