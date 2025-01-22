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

#include "math_instruction.h"

#include "variable_handler.h"

#include <sup/mathexpr/exceptions.h>
#include <sup/mathexpr/expression_context.h>
#include <sup/dto/basic_scalar_types.h>
#include <sup/oac-tree/generic_utils.h>
#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/user_interface.h>
#include <sup/oac-tree/workspace.h>

#include <iostream>
#include <string>

const std::string EXPR_STRING_ATTR_NAME = "expression";

namespace sup
{
namespace oac_tree
{

const std::string MathExprInstruction::Type = "MathExpression";
static bool _math_initialised_flag = RegisterGlobalInstruction<MathExprInstruction>();

MathExprInstruction::MathExprInstruction()
  : Instruction(MathExprInstruction::Type)
{
  AddAttributeDefinition(EXPR_STRING_ATTR_NAME)
    .SetCategory(AttributeCategory::kBoth).SetMandatory();
}

MathExprInstruction::~MathExprInstruction() = default;

ExecutionStatus MathExprInstruction::ExecuteSingleImpl(UserInterface& ui, Workspace& ws)
{
  VariableHandler handler(ws);
  sup::mathexpr::ExpressionContext expr_ctx(handler);
  std::string expression;
  if (!GetAttributeValueAs(EXPR_STRING_ATTR_NAME, ws, ui, expression))
  {
    return ExecutionStatus::FAILURE;
  }
  try
  {
    if (expr_ctx.EvaluateExpression(expression))
    {
      return ExecutionStatus::SUCCESS;
    }
  }
  catch (sup::mathexpr::ExpressionEvaluateException& ex)
  {
    std::string error_message = InstructionErrorProlog(*this) + ex.what();
    LogError(ui, error_message);
  }
  return ExecutionStatus::FAILURE;
}

}  // namespace oac_tree

}  // namespace sup
