/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                       */
/*    This file is part of the HiGHS linear optimization suite           */
/*                                                                       */
/*    Written and engineered 2008-2019 at the University of Edinburgh    */
/*                                                                       */
/*    Available as open-source under the MIT License                     */
/*                                                                       */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**@file io/FilereaderMps.cpp
 * @brief
 * @author Julian Hall, Ivet Galabova, Qi Huangfu and Michael Feldmeier
 */
#include "io/FilereaderMps.h"
#include "io/HMPSIO.h"
#include "lp_data/HighsLp.h"
#include "io/HMpsFF.h"

FilereaderRetcode FilereaderMps::readModelFromFile(const HighsOptions &options,
                                                   HighsLp &model)
{
  int status = 1;
  const char* filename = options.filename.c_str();

  if (options.parser_type == HighsMpsParserType::free)
  {
    HMpsFF parser{};
    FreeFormatParserReturnCode result = parser.loadProblem(filename, model);
    if (result == FreeFormatParserReturnCode::FILENOTFOUND)
      return FilereaderRetcode::FILENOTFOUND;
    else
      status = 0;
  }
  else
  {
    std::vector<int> integerColumn;
    status = readMPS(
        filename, -1, -1, model.numRow_, model.numCol_, model.sense_,
        model.offset_, model.Astart_, model.Aindex_, model.Avalue_,
        model.colCost_, model.colLower_, model.colUpper_, model.rowLower_,
        model.rowUpper_, integerColumn, model.row_names_, model.col_names_);
  }

  if (status)
    return FilereaderRetcode::PARSERERROR;
  return FilereaderRetcode::OKAY;
}

FilereaderRetcode FilereaderMps::writeModelToFile(const char *filename,
                                                  HighsLp &model)
{
  std::vector<int> integerColumn;
  int numint = 0;
  int objsense = 1;
  double objoffset = 0;
  writeMPS(filename, model.numRow_, model.numCol_, numint, objsense, objoffset,
           model.Astart_, model.Aindex_, model.Avalue_, model.colCost_,
           model.colLower_, model.colUpper_, model.rowLower_, model.rowUpper_,
           integerColumn);
  return FilereaderRetcode::OKAY;
}

FilereaderRetcode FilereaderMps::readModelFromFile(const char *filename,
                                                   HighsModelBuilder &model)
{
  return FilereaderRetcode::PARSERERROR;
}
