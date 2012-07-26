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
 \file LineStringGraph_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_linestringgraph
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <tests-config.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/landr/LineStringGraph.hpp>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_fromGeovectorValue)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      new openfluid::landr::LineStringGraph(*Val);

  BOOST_CHECK_EQUAL(Graph->getSize(), 8);
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), 8);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction_fromLineStringEntityVector)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  std::vector<openfluid::landr::LineStringEntity*> Entities;

  OGRLayer* Layer0 = Val->getLayer0();

  Layer0->ResetReading();

  OGRFeature* Feat;
  while ((Feat = Layer0->GetNextFeature()) != NULL)
  {
    OGRGeometry* OGRGeom = Feat->GetGeometryRef();

    geos::geom::Geometry* GeosGeom =
        (geos::geom::Geometry*) OGRGeom->exportToGEOS();

    openfluid::landr::LineStringEntity* Entity =
        new openfluid::landr::LineStringEntity(
            dynamic_cast<geos::geom::LineString*>(GeosGeom->clone()),
            Feat->Clone());

    Entities.push_back(Entity);

    // destroying the feature destroys also the associated OGRGeom
    OGRFeature::DestroyFeature(Feat);
    delete GeosGeom;
  }

  openfluid::landr::LineStringGraph* Graph2 =
      new openfluid::landr::LineStringGraph(Entities);

  BOOST_CHECK_EQUAL(Graph2->getSize(), 8);
  BOOST_CHECK_EQUAL(Graph2->getEdges()->size(), 8);

  delete Graph2;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_copy)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      new openfluid::landr::LineStringGraph(*Val);

  openfluid::landr::LineStringGraph* Copy =
      new openfluid::landr::LineStringGraph(*Graph);

  BOOST_CHECK_EQUAL(Graph->getSize(), Copy->getSize());
  BOOST_CHECK_EQUAL(Graph->getEdges()->size(), Copy->getEdges()->size());
  BOOST_CHECK_EQUAL(Graph->getEntities().size(), Copy->getEntities().size());

  delete Graph;
  delete Copy;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_StartsEnds)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      new openfluid::landr::LineStringGraph(*Val);

  std::vector<openfluid::landr::LineStringEntity*> Starts =
      Graph->getStartLineStringEntities();
  BOOST_CHECK_EQUAL(Starts.size(), 4);

  std::vector<openfluid::landr::LineStringEntity*> Ends =
      Graph->getEndLineStringEntities();
  BOOST_CHECK_EQUAL(Ends.size(), 1);

  BOOST_CHECK_EQUAL(Graph->getLastLineStringEntity()->getSelfId(), 1);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getEntity)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      new openfluid::landr::LineStringGraph(*Val);

  BOOST_CHECK(Graph->getEntity(1));
  BOOST_CHECK(Graph->getEntity(5));
  BOOST_CHECK(Graph->getEntity(3));
  BOOST_CHECK(!Graph->getEntity(100));

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_addRemoveAttribute)
{
  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      new openfluid::landr::LineStringGraph(*Val);

  openfluid::landr::LineStringEntity* U1 = Graph->getEntity(1);
  openfluid::landr::LineStringEntity* U2 = Graph->getEntity(2);

  boost::any IntValue = 0;
  boost::any StrValue = std::string("");

  BOOST_CHECK(!U1->setAttributeValue("att",123));
  BOOST_CHECK(!U2->setAttributeValue("att",std::string("val")));
  BOOST_CHECK(!U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(!U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(boost::any_cast<int>(IntValue), 0);
  BOOST_CHECK_EQUAL(boost::any_cast<std::string>(StrValue), "");

  Graph->addAttribute("att");

  BOOST_CHECK(U1->setAttributeValue("att",123));
  BOOST_CHECK(U2->setAttributeValue("att",std::string("val")));
  BOOST_CHECK(U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(boost::any_cast<int>(IntValue), 123);
  BOOST_CHECK_EQUAL(boost::any_cast<std::string>(StrValue), "val");

  Graph->addAttribute("att");

  IntValue = 0;
  StrValue = std::string("");
  BOOST_CHECK(U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(boost::any_cast<int>(IntValue), 123);
  BOOST_CHECK_EQUAL(boost::any_cast<std::string>(StrValue), "val");

  Graph->removeAttribute("att");

  IntValue = 0;
  StrValue = std::string("");
  BOOST_CHECK(!U1->setAttributeValue("att",123));
  BOOST_CHECK(!U2->setAttributeValue("att",std::string("val")));
  BOOST_CHECK(!U1->getAttributeValue("att",IntValue));
  BOOST_CHECK(!U2->getAttributeValue("att",StrValue));
  BOOST_CHECK_EQUAL(boost::any_cast<int>(IntValue), 0);
  BOOST_CHECK_EQUAL(boost::any_cast<std::string>(StrValue), "");

  Graph->removeAttribute("att");

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_getSelfIdOrderedEntities)
{

  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      new openfluid::landr::LineStringGraph(*Val);

  std::vector<openfluid::landr::LineStringEntity*> OrderedEntities =
      Graph->getSelfIdOrderedEntities();

  for (unsigned int i = 0; i < OrderedEntities.size(); i++)
    BOOST_CHECK_EQUAL(OrderedEntities.at(i)->getSelfId(), i+1);

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_loopMacros)
{
  openfluid::landr::LineStringEntity* CurrentEntity;
  int i = 0;

  openfluid::landr::LineStringGraph* NullGraph = 0;

  DECLARE_ENTITIES_GRAPH_LOOP(1);
  BEGIN_ENTITIES_GRAPH_LOOP(1,NullGraph,CurrentEntity)
    i++;
  END_LOOP

  BOOST_CHECK_EQUAL(i, 0);

  openfluid::core::GeoVectorValue* Val = new openfluid::core::GeoVectorValue(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/landr", "RS.shp");

  openfluid::landr::LineStringGraph* Graph =
      new openfluid::landr::LineStringGraph(*Val);

  i = 0;
  DECLARE_ENTITIES_GRAPH_LOOP(2);
  BEGIN_ENTITIES_GRAPH_LOOP(2,Graph,CurrentEntity)
    i++;
  END_LOOP;

  BOOST_CHECK_EQUAL(i, Graph->getSize());

  i = 1;
  DECLARE_ENTITIES_ORDERED_LOOP(3);
  BEGIN_ENTITIES_ORDERED_LOOP(3,Graph,CurrentEntity)
    BOOST_CHECK_EQUAL(CurrentEntity->getSelfId(),
                    Graph->getEntity(i)->getSelfId());
    i++;
  END_LOOP;

  delete Graph;
  delete Val;
}

// =====================================================================
// =====================================================================