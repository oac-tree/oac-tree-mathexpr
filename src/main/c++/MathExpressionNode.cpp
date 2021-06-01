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
#include <common/log-api.h>
#include <common/AnyValue.h>
#include <InstructionRegistry.h>
#include <Workspace.h>
#include <Procedure.h>

// Local header files

#include "MathExpressionNode.h"

// Constants

#undef LOG_ALTERN_SRC
#define LOG_ALTERN_SRC "sup::sequencer"

// Type definition

namespace sup {

namespace sequencer {

// Global variables

const std::string MathExpressionNode::Type = "MathExpressionNode";

static const bool global_mathexpressionnode_registered = RegisterGlobalInstruction<MathExpressionNode>();

// Function definition

MathExpressionNode::MathExpressionNode() :
        Instruction(Type) {
    //todo when I read an expression
}

MathExpressionNode::~MathExpressionNode() {
//Auto-generated destructor stub for MathExpressionNode

//TODO Verify if manual additions are needed here
}

bool MathExpressionNode::SetupImpl(const Procedure &proc) {
    log_info("MathExpressionNode::SetupImpl ..");

    std::string expression = GetAttribute("expression");

    //copy from sequencer workspace to mathexprtk workspace
    auto varNamesWs = proc.VariableNames();

    for (auto name : varNamesWs) {
        ::ccs::types::AnyValue *valT = new ::ccs::types::AnyValue;
        proc.GetVariableValue(name, *valT);

        workspace.Insert(name, *valT, true);
    }

    bool ret = mathEngine.Compile(expression.c_str(), workspace);
    if (ret) {
        varNames = mathEngine.GetVarNames();
    }
    else {
        log_error("MathExpressionNode::Setup Failed Compilation of %s", expression.c_str());
    }

    return ret;

}

ExecutionStatus MathExpressionNode::ExecuteSingleImpl(UserInterface *ui,
                                                      Workspace *ws) {
    log_info("MathExpressionNode::ExecuteSingleImpl ..");

    ExecutionStatus status = ExecutionStatus::SUCCESS;

    //must update the values in the workspace...
    for (auto varName : varNames) {
        ::ccs::types::AnyValue valT;

        ws->GetValue(varName, valT);
        ::ccs::types::AnyValue *val = NULL;
        workspace.Get(varName, val);
        *val = valT;
    }

    bool ret = mathEngine.Execute();

    if (ret) {
        //must update the values in the workspace...
        for (auto varName : varNames) {
            ::ccs::types::AnyValue *val = NULL;
            workspace.Get(varName, val);
            ws->SetValue(varName, *val);
        }
    }
    else {
        log_error("MathExpressionNode::ExecuteSingleImpl Failed Execution");
        status = ExecutionStatus::FAILURE;
    }
    return status;

}

} // namespace sequencer

} // namespace sup

extern "C" {

// C API function definitions

}// extern C

#undef LOG_ALTERN_SRC
