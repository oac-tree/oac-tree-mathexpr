/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project       : Supervision and Automation - Sequencer
*
* Description   : SUP sequencer math plugin
*
* Author        : Ricardo Torres (Ext)
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

#include "test_user_interface.h"
#include "unit_test_helper.h"

#include <sup/sequencer/instruction_registry.h>
#include <sup/sequencer/sequence_parser.h>

#include <gtest/gtest.h>

using namespace sup::sequencer;

class MathTest : public ::testing::Test
{
protected:
  MathTest() = default;
  virtual ~MathTest() = default;
};

TEST_F(MathTest, Success)
{
  const std::string body{
    R"(
    <Sequence>
        <MathExpression expression="z:=x+y*y-2"/>
        <Equals leftVar="z" rightVar="a"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"uint64"}' value='5' />
        <Local name="y" type='{"type":"uint64"}' value='3' />
        <Local name="z" type='{"type":"uint64"}' value='0'/>
        <Local name="a" type='{"type":"uint64"}' value='12' />
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui, ExecutionStatus::SUCCESS));
}

TEST_F(MathTest, TypeFailure)
{
  const std::string body{
    R"(
    <Sequence>
        <MathExpression expression="z:=x+y-3"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"uint8"}' value='1' />
        <Local name="y" type='{"type":"uint8"}' value='1' />
        <Local name="z" type='{"type":"uint8"}' value='0' />
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(MathTest, ConditionSuccess)
{
  const std::string body{
    R"(
    <Sequence>
        <MathExpression expression="x > y^2"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"int8"}' value='10' />
        <Local name="y" type='{"type":"int8"}' value='1' />
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui));
}

TEST_F(MathTest, ConditionFailure)
{
  const std::string body{
    R"(
    <Sequence>
        <MathExpression expression="x > y + 1"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"int8"}' value='1' />
        <Local name="y" type='{"type":"int8"}' value='1' />
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(MathTest, SuccessArray)
{
  const std::string body{
    R"(
    <Sequence>
        <MathExpression expression="z:=x+y*y-2"/>
        <Equals leftVar="z" rightVar="a"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[2,4,6]"/>
        <Local name="y" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[1,1,1]"/>
        <Local name="z" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[0,0,0]"/>
        <Local name="a" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[1,3,5]"/>
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui, ExecutionStatus::SUCCESS));
}

TEST_F(MathTest, Increment)
{
  const std::string body{
    R"(
    <Sequence>
        <MathExpression expression="x+=1"/>
        <Equals leftVar="x" rightVar="y"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"int8"}' value='1'/>
        <Local name="y" type='{"type":"int8"}' value='2'/>
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui, ExecutionStatus::SUCCESS));
}

TEST_F(MathTest, TrigIdentity)
{
  const std::string body{
    R"(
    <Sequence>
        <MathExpression expression='z:=sin(x)^2+cos(x)^2'/>
        <Equals leftVar="z" rightVar="y"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"float64"}' value='158'/>
        <Local name="y" type='{"type":"float64"}' value='1'/>
        <Local name="z" type='{"type":"float64"}' value='0'/>
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui, ExecutionStatus::SUCCESS));
}

TEST_F(MathTest, DoubleAssign)
{
  const std::string body{
    R"(
    <Sequence>
        <MathExpression expression="y:=x+1; z:=y+2"/>
        <Equals leftVar="y" rightVar="a"/>
        <Equals leftVar="z" rightVar="b"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"float32"}' value='158'/>
        <Local name="y" type='{"type":"float32"}' value='0'/>
        <Local name="z" type='{"type":"float32"}' value='0'/>
        <Local name="a" type='{"type":"float32"}' value='159'/>
        <Local name="b" type='{"type":"float32"}' value='161'/>
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui, ExecutionStatus::SUCCESS));
}

TEST_F(MathTest, MultiConst)
{
  const std::string body{
    R"(
    <Sequence>
        <MathExpression expression="z:=y*c+2"/>
        <Equals leftVar="z" rightVar="a"/>
    </Sequence>
    <Workspace>
        <Local name="y" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[1,1,1]"/>
        <Local name="c" type='{"type":"uint32"}' value='2'/>
        <Local name="z" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[0,0,0]"/>
        <Local name="a" type='{"type":"uint32_arr","element":{"type":"uint32"}}' value="[4,4,4]"/>
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui, ExecutionStatus::SUCCESS));
}

TEST_F(MathTest, StringFailure)
{
  const std::string body{
    R"(
    <Sequence>
        <MathExpression expression="x > 0"/>
    </Sequence>
    <Workspace>
        <Local name="x" type='{"type":"string"}' value='"oops"' />
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(MathTest, VariableConditionSuccess)
{
  const std::string body{
    R"(
    <MathExpression expression="@cond"/>
    <Workspace>
        <Local name="cond" type='{"type":"string"}' value='"x > y^2"' />
        <Local name="x" type='{"type":"int8"}' value='10' />
        <Local name="y" type='{"type":"int8"}' value='1' />
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui));
}

TEST_F(MathTest, VariableConditionFailure)
{
  const std::string body{
    R"(
    <MathExpression expression="@cond"/>
    <Workspace>
        <Local name="cond" type='{"type":"string"}' value='"x = y^2"' />
        <Local name="x" type='{"type":"int8"}' value='10' />
        <Local name="y" type='{"type":"int8"}' value='1' />
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(MathTest, VariableConditionWrongType)
{
  const std::string body{
    R"(
    <MathExpression expression="@cond"/>
    <Workspace>
        <Local name="cond" type='{"type":"float32"}' value='4.3' />
        <Local name="x" type='{"type":"int8"}' value='10' />
        <Local name="y" type='{"type":"int8"}' value='1' />
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}

TEST_F(MathTest, VariableConditionNotPresent)
{
  const std::string body{
    R"(
    <MathExpression expression="@cond"/>
    <Workspace>
        <Local name="x" type='{"type":"int8"}' value='10' />
        <Local name="y" type='{"type":"int8"}' value='1' />
    </Workspace>
)"};

  test::NullUserInterface ui;
  auto proc = ParseProcedureString(test::CreateProcedureString(body));
  EXPECT_TRUE(test::TryAndExecute(proc, ui, ExecutionStatus::FAILURE));
}
