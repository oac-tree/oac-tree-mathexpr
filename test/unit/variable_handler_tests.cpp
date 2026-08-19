/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project       : Supervision and Automation - oac-tree
*
* Description   : SUP oac-tree math plugin
*
* Author        : Marcelo Mariano (External)
*
* Copyright (c) : 2010-2026 ITER Organization,
*                 CS 90 046
*                 13067 St. Paul-lez-Durance Cedex
*                 France
* SPDX-License-Identifier: MIT
*
* This file is part of ITER CODAC software.
* For the terms and conditions of redistribution or use of this software
* refer to the file LICENSE located in the top level directory
* of the distribution package.
******************************************************************************/

#include <oac-tree/mathexpr/variable_handler.h>

#include <sup/oac-tree/variable_registry.h>
#include <sup/oac-tree/workspace.h>

#include <sup/dto/anyvalue.h>

#include <gtest/gtest.h>

#include <string>
#include <vector>

using namespace sup::oac_tree;

class VariableHandlerTest : public ::testing::Test
{
protected:
  VariableHandlerTest()
  {
    auto string_array = GlobalVariableRegistry().Create("Local");
    string_array->AddAttribute(
        "type", R"({"type":"array","multiplicity":1,"element":{"type":"string"}})");
    string_array->AddAttribute("value", R"(["a"])");
    m_ws.AddVariable("strvec", std::move(string_array));
    m_ws.Setup();
  }

  Workspace m_ws;
};

// GetScalar fails when the variable is absent from the workspace.
TEST_F(VariableHandlerTest, GetScalarMissingVariable)
{
  VariableHandler handler{m_ws};
  double val = 0.0;
  EXPECT_FALSE(handler.GetScalar("missing", val));
}

// GetVector fails when the variable is absent from the workspace.
TEST_F(VariableHandlerTest, GetVectorMissingVariable)
{
  VariableHandler handler{m_ws};
  std::vector<double> val;
  EXPECT_FALSE(handler.GetVector("missing", val));
}

// GetVector fails when an element cannot be converted to a double.
TEST_F(VariableHandlerTest, GetVectorElementNotConvertible)
{
  VariableHandler handler{m_ws};
  std::vector<double> val;
  EXPECT_FALSE(handler.GetVector("strvec", val));
}

// SetVector fails when the given vector size differs from the variable's.
TEST_F(VariableHandlerTest, SetVectorSizeMismatch)
{
  VariableHandler handler{m_ws};
  EXPECT_FALSE(handler.SetVector("strvec", std::vector<double>{1.0, 2.0}));
}

// SetVector fails when an element cannot be converted from a double.
TEST_F(VariableHandlerTest, SetVectorElementNotConvertible)
{
  VariableHandler handler{m_ws};
  EXPECT_FALSE(handler.SetVector("strvec", std::vector<double>{1.0}));
}
