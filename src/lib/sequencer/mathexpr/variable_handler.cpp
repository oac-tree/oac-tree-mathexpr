/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - Sequencer
 *
 * Description   : Sequencer plugin for mathematical expressions
 *
 * Author        : Ricardo Torres (EXT)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#include "variable_handler.h"

#include <sup/dto/anyvalue_helper.h>

namespace
{
bool CanBeUsedAsDoubleType(const sup::dto::AnyType& anytype);
bool CanBeUsedAsDoubleArrayType(const sup::dto::AnyType& anytype);
}  // unnamed namespace

namespace sup
{
namespace sequencer
{

VariableHandler::VariableHandler(sequencer::Workspace& ws)
  : m_ws{ws}
{}

VariableHandler::VarType VariableHandler::GetVariableType(const std::string& varname) const
{
  sup::dto::AnyValue readvalue;
  if (!m_ws.HasVariable(varname) || !m_ws.GetValue(varname, readvalue))
  {
    return kUnknown;
  }
  if (CanBeUsedAsDoubleType(readvalue.GetType()))
  {
    return kScalar;
  }
  if (CanBeUsedAsDoubleArrayType(readvalue.GetType()))
  {
    return kVector;
  }
  return kUnknown;
}

bool VariableHandler::GetScalar(const std::string& varname, double& val) const
{
  sup::dto::AnyValue readvalue;
  if (!m_ws.GetValue(varname, readvalue))
  {
    return false;
  }
  return readvalue.As(val);
}

bool VariableHandler::SetScalar(const std::string& varname, const double& val)
{
  sup::dto::AnyValue writevalue;
  m_ws.GetValue(varname, writevalue);
  if (!sup::dto::TryConvert(writevalue, val))
  {
    return false;
  }
  if (!m_ws.SetValue(varname, writevalue))
  {
    return false;
  }
  return true;
}

bool VariableHandler::GetVector(const std::string& varname, std::vector<double>& val) const
{
  sup::dto::AnyValue readvalue;
  if (!m_ws.GetValue(varname, readvalue))
  {
    return false;
  }
  val.resize(readvalue.NumberOfElements());
  for (size_t i = 0; i < readvalue.NumberOfElements(); ++i)
  {
    if(!readvalue[i].As(val[i]))
    {
      return false;
    }
  }
  return true;
}

bool VariableHandler::SetVector(const std::string& varname, const std::vector<double>& val)
{
  sup::dto::AnyValue writevalue;
  m_ws.GetValue(varname, writevalue);
  if (writevalue.NumberOfElements() != val.size())
  {
    return false;
  }
  for (size_t i = 0; i < writevalue.NumberOfElements(); ++i)
  {
    if (!sup::dto::TryConvert(writevalue[i], val[i]))
    {
      return false;
    }
  }
  if (!m_ws.SetValue(varname, writevalue))
  {
    return false;
  }
  return true;
}

}  // namespace sequencer

}  // namespace sup

namespace
{

bool CanBeUsedAsDoubleType(const sup::dto::AnyType& anytype)
{
  return sup::dto::IsScalarType(anytype) && (anytype != sup::dto::StringType);
}

bool CanBeUsedAsDoubleArrayType(const sup::dto::AnyType& anytype)
{
  return sup::dto::IsArrayType(anytype) && CanBeUsedAsDoubleType(anytype.ElementType());
}

}  // unnamed namespace
