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
  \file TimePoint_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_timepoint
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/machine/TimePoint.hpp>
#include <openfluid/base/SimStatus.hpp>


// =====================================================================
// =====================================================================


class FuncA : openfluid::base::PluggableFunction
{
  public:

	  FuncA() : openfluid::base::PluggableFunction() {};

	  ~FuncA() {};

	  bool initParams(openfluid::core::FuncParamsMap_t /*Params*/)  { return true; }

	  bool prepareData() { return true; }

    bool checkConsistency() { return true; }

	  bool initializeRun(const openfluid::base::SimulationInfo* /*SimInfo*/) { return true; }

    bool runStep(const openfluid::base::SimulationStatus* /*SimStatus*/) { std::cout << "FuncA" << std::endl; return true; }

    bool finalizeRun(const openfluid::base::SimulationInfo* /*SimInfo*/) { return true; }

};


class FuncB : openfluid::base::PluggableFunction
{
  public:

    FuncB() : openfluid::base::PluggableFunction() {};

    ~FuncB() {};

    bool initParams(openfluid::core::FuncParamsMap_t /*Params*/)  { return true; }

    bool prepareData() { return true; }

    bool checkConsistency() { return true; }

    bool initializeRun(const openfluid::base::SimulationInfo* /*SimInfo*/) { return true; }

    bool runStep(const openfluid::base::SimulationStatus* /*SimStatus*/) { std::cout << "FuncB" << std::endl; return true; }

    bool finalizeRun(const openfluid::base::SimulationInfo* /*SimInfo*/) { return true; }

};


class FuncC : openfluid::base::PluggableFunction
{
  public:

    FuncC() : openfluid::base::PluggableFunction() {};

    ~FuncC() {};

    bool initParams(openfluid::core::FuncParamsMap_t /*Params*/)  { return true; }

    bool prepareData() { return true; }

    bool checkConsistency() { return true; }

    bool initializeRun(const openfluid::base::SimulationInfo* /*SimInfo*/) { return true; }

    bool runStep(const openfluid::base::SimulationStatus* /*SimStatus*/) { std::cout << "FuncC" << std::endl; return true; }

    bool finalizeRun(const openfluid::base::SimulationInfo* /*SimInfo*/) { return true; }

};


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::base::SimulationStatus SimStatus(openfluid::core::DateTime(2012,1,1,0,0,0),openfluid::core::DateTime(2012,1,15,14,46,39),60);
  openfluid::machine::TimePoint TP(17,&SimStatus);

  BOOST_REQUIRE_EQUAL(TP.getRelativeTime(),17);
  BOOST_REQUIRE(!TP.hasSimFuncToProcess());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
	openfluid::base::SimulationStatus SimStatus(openfluid::core::DateTime(2012,1,1,0,0,0),openfluid::core::DateTime(2012,1,15,14,46,39),60);
	openfluid::machine::TimePoint TP(17,&SimStatus);

	BOOST_REQUIRE_EQUAL(TP.getRelativeTime(),17);


	TP.appendSimFunc((openfluid::base::PluggableFunction*)(new FuncA()));
  TP.appendSimFunc((openfluid::base::PluggableFunction*)(new FuncB()));
  TP.appendSimFunc((openfluid::base::PluggableFunction*)(new FuncC()));

	BOOST_REQUIRE(TP.hasSimFuncToProcess());

	while (TP.hasSimFuncToProcess()) TP.processNextSimFunc();

  BOOST_REQUIRE(!TP.hasSimFuncToProcess());

}

// =====================================================================
// =====================================================================
