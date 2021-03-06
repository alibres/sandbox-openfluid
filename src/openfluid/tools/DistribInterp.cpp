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
  @file
  @brief

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include <openfluid/tools/DistribInterp.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/math/special_functions/fpclassify.hpp>

#include <openfluid/tools/DataSrcFile.hpp>
#include <openfluid/tools/DTSerie.hpp>
#include <openfluid/tools/ColTextParser.hpp>


namespace openfluid { namespace tools {


DistributeInterpolate::DistributeInterpolate():
  m_Configured(false)
{

}

// =====================================================================
// =====================================================================


DistributeInterpolate::~DistributeInterpolate()
{
  // TODO clean delete of m_InterpData
}

// =====================================================================
// =====================================================================

void DistributeInterpolate::setConfig(std::string DataDir, std::string DataSourcesFilename,
                                      std::string DistributionFilename, SeriePreprocess SPpcs,
                                      openfluid::core::DateTime Begin,openfluid::core::DateTime End, int TimeStep)
{
  m_DataDir = DataDir;
  m_DataSourcesFilename = DataSourcesFilename;
  m_DistributionFilename = DistributionFilename;
  m_SPpcs = SPpcs;
  m_Begin = Begin;
  m_End = End;
  m_TimeStep = TimeStep;

  m_Configured = true;
}



// =====================================================================
// =====================================================================


bool DistributeInterpolate::loadAndPrepareData()
{
  openfluid::tools::DataSourcesFile DSFile;
  openfluid::tools::DateTimeSerie *Serie;
  openfluid::tools::DateTimeSerie *InterpolatedSerie;
  openfluid::tools::IndexedSerie *IInterpolatedSerie;


  if (!m_Configured)
  {
    throw openfluid::base::OFException("OpenFLUID framework","DistributeInterpolate::loadAndPrepareData","Distributed interpolator not configured");
    return false;
  }


  // loading of data sources

  boost::filesystem::path DSFilePath(m_DataDir + "/" + m_DataSourcesFilename);

  if (!DSFile.load(DSFilePath.string()) && DSFile.getIDs().size() <= 1)
  {
    throw openfluid::base::OFException("OpenFLUID framework","DistributeInterpolate::loadAndPrepareData","Error loading file " + m_DataSourcesFilename);
    return false;
  }
  else
  {
    std::vector<int> IDs = DSFile.getIDs();


    boost::filesystem::path SourcefilePath;
    for (unsigned int i=0;i<IDs.size();i++)
    {
      SourcefilePath = boost::filesystem::path(m_DataDir + "/" + DSFile.getSource(IDs[i]));
      if (!boost::filesystem::exists(SourcefilePath))
      {
        throw openfluid::base::OFException("OpenFLUID framework","DistributeInterpolate::loadAndPrepareData","Error loading "+ DSFile.getSource(IDs[i]) + " file as data source");
        return false;
      }
      else
      {
        // load file

        InterpolatedSerie = new openfluid::tools::DateTimeSerie();
        Serie = new openfluid::tools::DateTimeSerie();
        IInterpolatedSerie = new openfluid::tools::IndexedSerie();

        if (loadDataAsSerie(SourcefilePath.string(),m_SPpcs, Serie))
        {

          // interpolate in time for simulation
          if (Serie->createInterpolatedSerie(m_Begin-m_TimeStep,m_End+m_TimeStep,m_TimeStep,InterpolatedSerie))
          {
            // store
            m_InterpData[IDs[i]] = InterpolatedSerie;
            InterpolatedSerie->createIndexedSerie(IInterpolatedSerie);
            m_InterpIndexedData[IDs[i]] = IInterpolatedSerie;
            delete Serie;

          }
          else
          {
            delete Serie;
            delete InterpolatedSerie;
            throw openfluid::base::OFException("OpenFLUID framework","DistributeInterpolate::loadAndPrepareData","Error interpolating data from file " + DSFile.getSource(IDs[i]));
            return false;
          }
        }
        else
        {
          delete Serie;
          delete InterpolatedSerie;
          throw openfluid::base::OFException("OpenFLUID framework","DistributeInterpolate::loadAndPrepareData","Error loading data from file " + DSFile.getSource(IDs[i]));
          return false;
        }
      }
    }


    boost::filesystem::path DistribFilePath(m_DataDir + "/" + m_DistributionFilename);

    if (!loadDistributionAndDistribute(DistribFilePath.string()))
    {
      return false;
    }



  }


  return true;

}

// =====================================================================
// =====================================================================


bool DistributeInterpolate::loadDataAsSerie(std::string FilePath, SeriePreprocess SPpcs, DateTimeSerie *Serie)
{
  ColumnTextParser FileParser("%");
  bool IsOK;

  IsOK = true;

  long int Year, Month, Day, Hour, Min, Sec;
  double Value;
  double CumValue, ValueToAdd;
  openfluid::core::DateTime ZeDT;


  Serie->clear();


  if (FileParser.loadFromFile(FilePath) && (FileParser.getLinesCount() > 0) && (FileParser.getColsCount() == 7))
  {
    int i = 0;
    CumValue = 0;

    while (i<FileParser.getLinesCount() && IsOK)
    {
      if (FileParser.getLongValue(i,0,&Year) && FileParser.getLongValue(i,1,&Month) &&
          FileParser.getLongValue(i,2,&Day) && FileParser.getLongValue(i,3,&Hour) &&
          FileParser.getLongValue(i,4,&Min) && FileParser.getLongValue(i,5,&Sec) &&
          FileParser.getDoubleValue(i,6,&Value))
      {

        if (!boost::math::isnan(Value) && !boost::math::isinf(Value))
        {

          ZeDT = openfluid::core::DateTime(Year,Month,Day,Hour,Min,Sec);

          CumValue +=  Value;

          if (SPpcs == SERIEPREPCS_CUMULATE) ValueToAdd = CumValue;
          else ValueToAdd = Value;


          if (!boost::math::isnan(ValueToAdd) && !boost::math::isinf(ValueToAdd))
          {
            if (!Serie->addValue(ZeDT,ValueToAdd))
            {
              IsOK = false;
            }
          }
          else
          {
            IsOK = false;
          }
        }
      }
      else
      {
        IsOK = false;
      }

      i++;
    }
  }
  else
  {
    IsOK = false;
  }


  return IsOK;

}

// =====================================================================
// =====================================================================


bool DistributeInterpolate::loadDistributionAndDistribute(std::string FilePath)
{
  long UnitID;
  long DataSrcID;

  int i;


  ColumnTextParser DistriFileParser("%");


  if (boost::filesystem::exists(boost::filesystem::path(FilePath)))
  {

    if ((DistriFileParser.loadFromFile(FilePath)) && (DistriFileParser.getColsCount() == 2) && (DistriFileParser.getLinesCount() >0))
    {

      for (i=0;i<DistriFileParser.getLinesCount();i++)
      {

        if (DistriFileParser.getLongValue(i,0,&UnitID) && DistriFileParser.getLongValue(i,1,&DataSrcID))
        {

          if (m_InterpData.find(DataSrcID) != m_InterpData.end())
          {
            m_UnitsData[UnitID] = m_InterpData[DataSrcID];
            m_UnitsIndexedData[UnitID] = m_InterpIndexedData[DataSrcID];
          }
          else
          {
            throw openfluid::base::OFException("OpenFLUID framework","DistributeInterpolate::loadDistributionAndDistribute","Error in distribution file " + m_DistributionFilename + ", data source ID not found");
            return false;
          }
        }
        else
        {
          throw openfluid::base::OFException("OpenFLUID framework","DistributeInterpolate::loadDistributionAndDistribute","Error in distribution file " + m_DistributionFilename + ", format error");
          return false;
        }
      }
    }
    else
    {
      throw openfluid::base::OFException("OpenFLUID framework","DistributeInterpolate::loadDistributionAndDistribute","Error in distribution file " + m_DistributionFilename + ", file not found or format error");
      return false;
    }
  }
  else
  {
    throw openfluid::base::OFException("OpenFLUID framework","DistributeInterpolate::loadDistributionAndDistribute","Distribution file " + m_DistributionFilename + " not found");
    return false;
  }
  return true;
}


// =====================================================================
// =====================================================================


bool DistributeInterpolate::getValue(int ID, openfluid::core::DateTime DT, openfluid::core::DoubleValue *Value)
{

  return (m_UnitsData[ID]->getValue(DT,Value));
}


// =====================================================================
// =====================================================================


bool DistributeInterpolate::getValue(int ID, int Index, openfluid::core::DoubleValue *Value)
{
  UnitsIndexedSeriesMap::iterator UISMit = m_UnitsIndexedData.find(ID);

  if (UISMit == m_UnitsIndexedData.end()) return false;

  if (Index>(*UISMit).second->Count) return false;

  *Value = ((*UISMit).second->Values[Index]);
  return true;
}


} }


