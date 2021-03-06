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
 \file BuilderTreeStore_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_BuilderTreeStore
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "BuilderTreeStore.hpp"

// =====================================================================
// =====================================================================

struct init_Store
{
    Glib::RefPtr<BuilderTreeStore> mref_TreeStore;

    class TreeStoreColumns: public Gtk::TreeModel::ColumnRecord
    {
      public:
        TreeStoreColumns()
        {
          add(m_Val);
        }
        Gtk::TreeModelColumn<Glib::ustring> m_Val;
    };

    TreeStoreColumns m_Columns;

    init_Store()
    {
      BuilderTestHelper::getInstance()->initGtk();
    }

    ~init_Store()
    {
    }
};

BOOST_FIXTURE_TEST_SUITE(BuilderTreeStoreTest, init_Store)

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(test_constructor)
{
  Glib::RefPtr<BuilderTreeStore> TreeStore = BuilderTreeStore::create(m_Columns);

  BOOST_CHECK_EQUAL(TreeStore->children().size(),0);
}

BOOST_AUTO_TEST_CASE(test_RowRefConversions)
{
  Glib::RefPtr<BuilderTreeStore> TreeStore = BuilderTreeStore::create(m_Columns);

  Gtk::TreeRow Row = *(TreeStore->append());
  Row[m_Columns.m_Val] = "Test Value";

  Gtk::TreeRowReference* RowRef = TreeStore->createRowRefFromIter(*Row);

  Gtk::TreeRow ReturnRow = TreeStore->getRowFromRowRef(*RowRef);

  BOOST_CHECK_EQUAL(ReturnRow.get_value(m_Columns.m_Val),"Test Value");

  BOOST_CHECK_EQUAL(TreeStore->children().size(),1);
}

BOOST_AUTO_TEST_CASE(test_appendATitleRow)
{
  Glib::RefPtr<BuilderTreeStore> TreeStore = BuilderTreeStore::create(m_Columns);

  TreeStore->appendATitleRow(m_Columns.m_Val,"The Title");

  BOOST_CHECK_EQUAL(TreeStore->children().size(),1);

  BOOST_CHECK_EQUAL(TreeStore->children()[0][m_Columns.m_Val],"The Title");
}

BOOST_AUTO_TEST_CASE(test_appendToRowRef)
{
  Glib::RefPtr<BuilderTreeStore> TreeStore = BuilderTreeStore::create(m_Columns);

  Gtk::TreeRowReference* TitleRowRef = TreeStore->appendATitleRow(m_Columns.m_Val,"The Title");

  BOOST_CHECK_EQUAL(TreeStore->children().size(),1);

  for(int i=0; i<5; i++)
  {
    Gtk::TreeRow NewRow = TreeStore->appendToRowRef(*TitleRowRef);
    NewRow[m_Columns.m_Val] = "New Row";
  }

  BOOST_CHECK_EQUAL(TreeStore->children().size(),1);
  BOOST_CHECK_EQUAL(TreeStore->children()[0].children().size(),5);
}

BOOST_AUTO_TEST_CASE(test_clearChildren)
{
  Glib::RefPtr<BuilderTreeStore> TreeStore = BuilderTreeStore::create(m_Columns);

  Gtk::TreeRowReference* TitleRowRef = TreeStore->appendATitleRow(m_Columns.m_Val,"The Title");

  for(int i=0; i<5; i++)
  {
    Gtk::TreeRow NewRow = TreeStore->appendToRowRef(*TitleRowRef);
    NewRow[m_Columns.m_Val] = "New Row";
  }

  TreeStore->clearChildrenOfRowRef(*TitleRowRef);

  BOOST_CHECK_EQUAL(TreeStore->children().size(),1);
  BOOST_CHECK_EQUAL(TreeStore->children()[0].children().size(),0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_SUITE_END();
