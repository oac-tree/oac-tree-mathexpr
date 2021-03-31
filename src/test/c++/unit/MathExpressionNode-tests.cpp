/******************************************************************************
 * $HeadURL: $
 * $Id: $
 *
 * Project       : CODAC Core System
 *
 * Description   : MathExpression plugin
 *
 * Author        : G.Ferro (IO)
 *
 * Copyright (c) : 2010-2019 ITER Organization,
 *  CS 90 046
 *  13067 St. Paul-lez-Durance Cedex
 *  France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 ******************************************************************************/

// Global header files
#include <gtest/gtest.h> // Google test framework

#include <common/log-api.h> // Syslog wrapper routines
#include <common/BasicTypes.h>
#include <common/AnyValue.h>
#include <common/SharedReference.h>
#include <common/AnyType.h>

#include <Instruction.h>
#include <InstructionRegistry.h>

#include <Variable.h>
#include <VariableRegistry.h>

#include <Workspace.h>

#include <SequenceParser.h>

// Local header files

#include "LogUI.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type declaration

using namespace sup::sequencer;

// Function definition

static inline bool Initialise(void) {
    // Log to standard output
    (void) ccs::log::SetStdout();
    (void) ccs::log::SetFilter(LOG_DEBUG);
    sup::sequencer::LoadPlugin("../../lib/libMathExpression.so");
    return true;
}

static inline bool Terminate(void) {
    return true;
}

static bool PrintProcedureWorkspace(::sup::sequencer::Procedure *procedure);

static const ccs::types::char8 *expressionTable[][14] =
        { { "c:=a+b", "{\"type\":\"uint8\"}", "8", "c", "{\"type\":\"uint8\"}", "0", "c", "{\"type\":\"uint8\"}", "5", "a", "{\"type\":\"uint8\"}", "3", "b",
                NULL }, { "c:=c+1", "{\"type\":\"uint8\"}", "1", "c", "{\"type\":\"uint8\"}", "0", "c", NULL }, { "c:=a/b", "{\"type\":\"uint8\"}", "2", "c",
                "{\"type\":\"uint8\"}", "0", "c", "{\"type\":\"uint8\"}", "4", "a", "{\"type\":\"uint8\"}", "2", "b", NULL }, { "c:=a/b",
                "{\"type\":\"uint8\"}", "2", "c", "{\"type\":\"uint8\"}", "0", "c", "{\"type\":\"uint8\"}", "5", "a", "{\"type\":\"uint8\"}", "2", "b", NULL },
                { "c:=a/b", "{\"type\":\"float32\"}", "2.5", "c", "{\"type\":\"float32\"}", "0", "c", "{\"type\":\"uint8\"}", "5", "a", "{\"type\":\"uint8\"}",
                        "2", "b", NULL }, { "c:=a.field1+b.field2", "{\"type\":\"uint8\"}", "5", "c", "{\"type\":\"uint8\"}", "0", "c",
                        "{\"type\":\"StructuredData1\", \"attributes\":[{\"field1\":{\"type\":\"uint32\"}}]}", "{\"field1\":2}", "a",
                        "{\"type\":\"StructuredData2\", \"attributes\":[{\"field1\":{\"type\":\"float32\"}}, {\"field2\":{\"type\":\"uint16\"}}]}",
                        "{\"field1\":20.5, \"field2\":3}", "b", NULL },
                { "c:=a[1].field1+b.field2[2].field", "{\"type\":\"uint8\"}", "5", "c", "{\"type\":\"uint8\"}", "0", "c",
                        "{\"type\":\"StructuredData6a\", \"multiplicity\":2, \"element\":{\"type\":\"StructuredData6Base\", \"attributes\":[{\"field1\":{\"type\":\"uint32\"}}]}}",
                        "[{\"field1\":1}, {\"field1\":2}]", "a",
                        "{\"type\":\"StructuredData6b\", \"attributes\":[{\"field1\":{\"type\":\"float32\"}}, {\"field2\":{\"type\":\"StructuredData6c\", \"multiplicity\":3, \"element\":{\"type\":\"internalStruct\", \"attributes\":[{\"field\":{\"type\":\"uint16\"}}]}}}]}",
                        "{\"field1\":20.5, \"field2\":[{\"field\":30}, {\"field\":20}, {\"field\":3}]}", "b", NULL },
                { "c[0].field1:=a[0].field1+2",
                        "{\"type\":\"StructuredData7a\",   \"multiplicity\":2, \"element\":{\"type\":\"StructuredData6Base\", \"attributes\":[{\"field1\":{\"type\":\"uint32\"}}]}}",
                        "[{\"field1\":3}, {\"field1\":2}]", "c",
                        "{\"type\":\"StructuredData7a\",   \"multiplicity\":2, \"element\":{\"type\":\"StructuredData6Base\", \"attributes\":[{\"field1\":{\"type\":\"uint32\"}}]}}",
                        "[{\"field1\":0}, {\"field1\":2}]", "c",
                        "{\"type\":\"StructuredData7b\", \"multiplicity\":2, \"element\":{\"type\":\"StructuredData6Base\", \"attributes\":[{\"field1\":{\"type\":\"uint32\"}}]}}",
                        "[{\"field1\":1}, {\"field1\":2}]", "a", NULL }, { NULL } };

TEST(MathExpressionNode, Default) // Static initialisation
{
    bool status = Initialise();
    std::string file; // Placeholder

    if (::ccs::HelperTools::Exist("../resources/workspaceMath.xml"))
      {
        file = std::string("../resources/workspaceMath.xml");
      }
    else
      {
        file = std::string("./target/test/resources/workspaceMath.xml");
      }

    auto proc = sup::sequencer::ParseProcedureFile(file);

    status = bool(proc);
    if (status)
      { // Setup instructions
        status = proc->Setup();
      }
    if (status)
    {
        status = PrintProcedureWorkspace(proc.get());
    }

    if (status)
    {
        LogUI ui;
        while((proc->GetStatus()!=ExecutionStatus::SUCCESS) && (proc->GetStatus()!=ExecutionStatus::FAILURE)) {
            proc->ExecuteSingle(&ui);
        }
        status = (proc->GetStatus() == ExecutionStatus::SUCCESS);
    }

    if (status)
    {
        status = PrintProcedureWorkspace(proc.get());
    }

    Terminate();

    ASSERT_EQ(true, status);
}

TEST(MathExpressionNode, Table) // Static initialisation
{
    bool status = Initialise();

    ccs::types::uint32 i=0u;
    while((expressionTable[i][0]!=NULL) && status) {

        std::unique_ptr<Procedure> proc(new Procedure);
        auto myMathNode = GlobalInstructionRegistry().Create("MathExpressionNode");
        myMathNode->AddAttribute("expression", expressionTable[i][0]);

        ::ccs::base::SharedReference<::ccs::types::AnyType> resType;
        printf("parse type %s\n", expressionTable[i][1]);
        ::ccs::HelperTools::Parse(resType, expressionTable[i][1]);

        ::ccs::base::SharedReference<const ::ccs::types::AnyType> constResType(resType);

        ccs::types::AnyValue resVal(constResType);
        printf("parse instance %s\n", expressionTable[i][2]);

        resVal.ParseInstance(expressionTable[i][2]);

        printf("parse variable %s\n", expressionTable[i][3]);

        ccs::types::uint32 j=4u;
        while(expressionTable[i][j]!=NULL) {
#if 0
            std::unique_ptr<Variable> varX(new LocalVariable);
#else
	    auto varX = sup::sequencer::GlobalVariableRegistry().Create("Local");
#endif
            varX->AddAttribute("type", expressionTable[i][j]);
            j++;
            varX->AddAttribute("value", expressionTable[i][j]);
            j++;
            printf("parse variable %s\n", expressionTable[i][j]);
            proc->AddVariable(expressionTable[i][j], varX.release());
            j++;
        }

        status = PrintProcedureWorkspace(proc.get());

        if (status) {
            LogUI ui;
            proc->PushInstruction(myMathNode.release());
            proc->Setup();
            proc->ExecuteSingle(&ui);
            status = (proc->GetStatus() == ExecutionStatus::SUCCESS);
        }

        if (status) {
            ::ccs::types::AnyValue result;
            proc->GetVariableValue(expressionTable[i][3], result);

            status = (result.GetSize() == resVal.GetSize());
            if(status) {
                status = (memcmp(result.GetInstance(), resVal.GetInstance(), result.GetSize())==0);
            }

            if(!status) {
                printf("Failed expression %d\n", i);
            }
        }

        if (status) {
            status = PrintProcedureWorkspace(proc.get());
        }

        i++;
    }

    Terminate();

    ASSERT_EQ(true, status);
}

TEST(MathExpressionNode, Error_NoValueInWorkspace) // Static initialisation
{
    bool status = Initialise();

    std::unique_ptr<Procedure> proc(new Procedure);
    auto myMathNode = GlobalInstructionRegistry().Create("MathExpressionNode");
    myMathNode->AddAttribute("expression", "a:=1");

    LogUI ui;
    proc->PushInstruction(myMathNode.release());
    status=(!proc->Setup());

    Terminate();

    ASSERT_EQ(true, status);
}

TEST(MathExpressionNode, Error_NoTypeInWorkspace) // Static initialisation
{
    bool status = Initialise();

    std::unique_ptr<Procedure> proc(new Procedure);
    auto myMathNode = GlobalInstructionRegistry().Create("MathExpressionNode");
    myMathNode->AddAttribute("expression", "a:=1");

    //std::unique_ptr<Variable> varX(new LocalVariable);
    auto varX = sup::sequencer::GlobalVariableRegistry().Create("Local");
    proc->AddVariable("a", varX.release());

    LogUI ui;
    proc->PushInstruction(myMathNode.release());
    status=(!proc->Setup());

    Terminate();

    ASSERT_EQ(true, status);
}

TEST(MathExpressionNode, Error_InvalidTypeInWorkspace) // Static initialisation
{
    bool status = Initialise();

    std::unique_ptr<Procedure> proc(new Procedure);
    auto myMathNode = GlobalInstructionRegistry().Create("MathExpressionNode");
    myMathNode->AddAttribute("expression", "a:=1");

    //std::unique_ptr<Variable> varX(new LocalVariable);
    auto varX = sup::sequencer::GlobalVariableRegistry().Create("Local");
    varX->AddAttribute("type", "{\"type\":\"StructuredData6a\", \"multiplicity\":2, \"element\":{\"type\":\"StructuredData6Base\", \"attributes\":[{\"field1\":{\"type\":\"uint32\"}}]}}");
    varX->AddAttribute("value", "[{\"field1\":1}, {\"field1\":2}]");

    proc->AddVariable("a", varX.release());

    LogUI ui;
    proc->PushInstruction(myMathNode.release());
    status=(!proc->Setup());

    Terminate();

    ASSERT_EQ(true, status);
}

TEST(MathExpressionNode, Error_CollectVar) // Static initialisation
{
    bool status = Initialise();

    std::unique_ptr<Procedure> proc(new Procedure);
    auto myMathNode = GlobalInstructionRegistry().Create("MathExpressionNode");
    myMathNode->AddAttribute("expression", "++a-");

    //std::unique_ptr<Variable> varX(new LocalVariable);
    auto varX = sup::sequencer::GlobalVariableRegistry().Create("Local");
    varX->AddAttribute("type", "{\"type\":\"uint8\"}");
    varX->AddAttribute("value", "0");

    proc->AddVariable("a", varX.release());

    LogUI ui;
    proc->PushInstruction(myMathNode.release());
    status=(!proc->Setup());

    Terminate();

    ASSERT_EQ(true, status);
}

TEST(MathExpressionNode, Error_Expression) // Static initialisation
{
    bool status = Initialise();

    std::unique_ptr<Procedure> proc(new Procedure);
    auto myMathNode = GlobalInstructionRegistry().Create("MathExpressionNode");
    myMathNode->AddAttribute("expression", "a:=2^a");

    //std::unique_ptr<Variable> varX(new LocalVariable);
    auto varX = sup::sequencer::GlobalVariableRegistry().Create("Local");
    varX->AddAttribute("type", "{\"type\":\"uint8\"}");
    varX->AddAttribute("value", "0");

    proc->AddVariable("a", varX.release());

    LogUI ui;
    proc->PushInstruction(myMathNode.release());
    status=(!proc->Setup());

    Terminate();

    ASSERT_EQ(true, status);
}

static bool PrintProcedureWorkspace(::sup::sequencer::Procedure *procedure) {
    auto var_names = procedure->VariableNames();
    ::ccs::types::char8 val_string[1024];
    for (const auto &var_name : var_names) {
        ::ccs::types::AnyValue val;
        log_info("Variable '%s'", var_name.c_str());

        bool var_initialized = procedure->GetVariableValue(var_name, val);
        if (var_initialized) {
            val.SerialiseInstance(val_string, 1024);
            log_info("Variable '%s', with value\n  %s", var_name.c_str(), val_string);
        }
        else {
            log_info("Variable '%s' uninitialized", var_name.c_str());
        }
    }
    return true;
}

#undef LOG_ALTERN_SRC
