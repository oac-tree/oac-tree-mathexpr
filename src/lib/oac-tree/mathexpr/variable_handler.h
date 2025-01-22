/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : SUP - oac-tree
 *
 * Description   : oac-tree plugin for mathematical expressions
 *
 * Author        : Ricardo Torres (EXT)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

#ifndef SUP_OAC_TREE_PLUGIN_MATH_VARIABLE_HANDLER_H_
#define SUP_OAC_TREE_PLUGIN_MATH_VARIABLE_HANDLER_H_

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/mathexpr/i_variable_store.h>
#include <sup/oac-tree/workspace.h>

#include <map>
#include <string>
#include <vector>

namespace sup
{
namespace oac_tree
{

class VariableHandler : public mathexpr::IVariableStore
{
public:
  VariableHandler(oac_tree::Workspace& ws);
  ~VariableHandler() override = default;
  VarType GetVariableType(const std::string& varname) const override;
  bool GetScalar(const std::string& varname, double& val) const override;
  bool SetScalar(const std::string& varname, const double& val) override;
  bool GetVector(const std::string& varname, std::vector<double>& val) const override;
  bool SetVector(const std::string& varname, const std::vector<double>& val) override;

private:
  oac_tree::Workspace& m_ws;
};

}  // namespace oac_tree

}  // namespace sup

#endif  // SUP_OAC_TREE_PLUGIN_MATH_VARIABLE_HANDLER_H_
