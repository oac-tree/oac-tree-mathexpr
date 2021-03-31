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

#ifndef MATHEXPRESSIONENGINEEXPTRK_H_
#define MATHEXPRESSIONENGINEEXPTRK_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "exprtk.hpp"
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include <common/BasicTypes.h>
#include <common/AnyValue.h>
#include <common/SharedReference.h>
#include <common/AnyType.h>
#include <common/log-api.h>

#include "Procedure.h"
#include "Workspace.h"
#include "MathExpressionEngineI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace sup {

namespace sequencer {

#define SymbolListType std::deque<typename exprtk::parser<T>::dependent_entity_collector::symbol_t>

class BasicCastI {
public:
    BasicCastI();
    virtual ~BasicCastI();
    virtual void Do(void *in,
                    void *out,
                    ccs::types::uint32 idx)=0;
};

template<typename Tin, typename Tout>
class BasicCast: public BasicCastI {
public:
    BasicCast();
    virtual ~BasicCast();
    virtual void Do(void *in,
                    void *out,
                    ccs::types::uint32 idx);
};

template<typename T>
class MathExpressionEngineExptrk: public MathExpressionEngineI {

private:
    std::vector<std::string> varNames;
    std::vector<std::vector<T> > varMem;
    std::vector<BasicCastI*> varInCast;
    std::vector<BasicCastI*> varOutCast;
    ::ccs::types::uint64 numOfSignalVariables;

    exprtk::symbol_table<T> symbolTable; //<! Stores variables.
    exprtk::parser<T> expressionParser; //<! Parses the expressions.
    exprtk::expression<T> expression; //<! Stores one expression for each output signal declared in the configuration file.

    BasicCast<T, ccs::types::boolean> toBoolCast;
    BasicCast<T, ccs::types::char8> toChar8Cast;
    BasicCast<T, ccs::types::char8> toStringCast;
    BasicCast<T, ccs::types::int8> toInt8Cast;
    BasicCast<T, ccs::types::uint8> toUint8Cast;
    BasicCast<T, ccs::types::int16> toInt16Cast;
    BasicCast<T, ccs::types::uint16> toUint16Cast;
    BasicCast<T, ccs::types::int32> toInt32Cast;
    BasicCast<T, ccs::types::uint32> toUint32Cast;
    BasicCast<T, ccs::types::int64> toInt64Cast;
    BasicCast<T, ccs::types::uint64> toUint64Cast;
    BasicCast<T, ccs::types::float32> toFloat32Cast;
    BasicCast<T, ccs::types::float64> toFloat64Cast;

    BasicCast<ccs::types::boolean, T> fromBoolCast;
    BasicCast<ccs::types::char8, T> fromChar8Cast;
    BasicCast<ccs::types::char8, T> fromStringCast;
    BasicCast<ccs::types::int8, T> fromInt8Cast;
    BasicCast<ccs::types::uint8, T> fromUint8Cast;
    BasicCast<ccs::types::int16, T> fromInt16Cast;
    BasicCast<ccs::types::uint16, T> fromUint16Cast;
    BasicCast<ccs::types::int32, T> fromInt32Cast;
    BasicCast<ccs::types::uint32, T> fromUint32Cast;
    BasicCast<ccs::types::int64, T> fromInt64Cast;
    BasicCast<ccs::types::uint64, T> fromUint64Cast;
    BasicCast<ccs::types::float32, T> fromFloat32Cast;
    BasicCast<ccs::types::float64, T> fromFloat64Cast;

    BasicCastI *inputCasts[14];
    BasicCastI *outputCasts[14];

    const Procedure *myProc;

    void CheckForArray(std::string &varName);

    bool CompileInputs(const std::vector<std::string> &origin_varnames);

    bool CompileOutputs(const std::vector<std::string> &origin_varnames,
                        SymbolListType &symbol_list);

public:
    MathExpressionEngineExptrk();

    virtual ~MathExpressionEngineExptrk();

    //Just don use vars with name including I1234I (a number between two I´)
    virtual bool Compile(const ccs::types::char8 *expressionIn,
                         const Procedure &proc);

    virtual bool Execute(Workspace *ws);
};

}
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace sup {

namespace sequencer {

template<typename T>
MathExpressionEngineExptrk<T>::MathExpressionEngineExptrk() {
    inputCasts[0] = &fromBoolCast;
    inputCasts[1] = &fromChar8Cast;
    inputCasts[2] = &fromStringCast;
    inputCasts[3] = &fromInt8Cast;
    inputCasts[4] = &fromUint8Cast;
    inputCasts[5] = &fromInt16Cast;
    inputCasts[6] = &fromUint16Cast;
    inputCasts[7] = &fromInt32Cast;
    inputCasts[8] = &fromUint32Cast;
    inputCasts[9] = &fromInt64Cast;
    inputCasts[10] = &fromUint64Cast;
    inputCasts[11] = &fromFloat32Cast;
    inputCasts[12] = &fromFloat64Cast;
    inputCasts[13] = NULL;

    outputCasts[0] = &toBoolCast;
    outputCasts[1] = &toChar8Cast;
    outputCasts[2] = &toStringCast;
    outputCasts[3] = &toInt8Cast;
    outputCasts[4] = &toUint8Cast;
    outputCasts[5] = &toInt16Cast;
    outputCasts[6] = &toUint16Cast;
    outputCasts[7] = &toInt32Cast;
    outputCasts[8] = &toUint32Cast;
    outputCasts[9] = &toInt64Cast;
    outputCasts[10] = &toUint64Cast;
    outputCasts[11] = &toFloat32Cast;
    outputCasts[12] = &toFloat64Cast;
    outputCasts[13] = NULL;

    myProc = NULL;
}
template<typename T>
MathExpressionEngineExptrk<T>::~MathExpressionEngineExptrk() {
}

template<typename T>
void MathExpressionEngineExptrk<T>::CheckForArray(std::string &varName) {

    std::string::size_type prev_pos = 0;
    std::string::size_type pos = 0;
    while ((pos = varName.find('I', pos)) != std::string::npos) {
        pos++;
        prev_pos = pos;
        log_info("MathExpressionEngineExptrk::CheckForArray - Found open array");
        if ((pos = varName.find('I', pos)) != std::string::npos) {
            std::string dimStr(varName.substr(prev_pos, pos - prev_pos));
            ::ccs::types::char8 *p;
            ::ccs::types::uint32 dim = std::strtol(dimStr.c_str(), &p, 10);
            //conversion success
            if (*p == 0) {
                varName[prev_pos - 1] = '[';
                varName[pos] = ']';
                pos++;
                prev_pos = pos;
            }
            log_info("MathExpressionEngineExptrk::CheckForArray - Found dimension %d", dim);
        }
        else {
            break;
        }
    }
}

template<typename T>
bool MathExpressionEngineExptrk<T>::CompileInputs(const std::vector<std::string> &origin_varnames) {
    //get the variable number of variables to preallocate the memory
    ::ccs::types::uint32 numberOfVars = varNames.size();
    varMem.resize(numberOfVars);
    varInCast.resize(numberOfVars);
    varOutCast.resize(numberOfVars);
    for (::ccs::types::uint32 i = 0u; (i < numberOfVars); i++) {
        ::ccs::types::AnyValue var;
        CheckForArray(varNames[i]);
        if (!myProc->GetVariableValue(varNames[i], var)) {
            log_error("MathExpressionEngineExptrk::Compile Failed Procedure->GetVariableValue(%s)", varNames[i].c_str());
            return false;
        }

        //use arrays to be generic
        ::ccs::types::uint32 varSize = (var.GetSize() / var.GetType()->GetSize());
        varMem[i].resize(varSize);
        if (!symbolTable.add_vector(origin_varnames[i], varMem[i]))
            return false;

        varInCast[i] = NULL;
        varOutCast[i] = NULL;

        //register the input cast function
        ::ccs::types::uint8 typeId = static_cast<::ccs::types::uint8>(::ccs::types::GetIdentifier(var.GetType()->GetName()));
        if (typeId >= 13u) {
            log_error("MathExpressionEngineExptrk::Compile Failed GetIdentifier of %s", var.GetType()->GetName());
            return false;
        }
        varInCast[i] = inputCasts[typeId];
    }
    return true;
}

template<typename T>
bool MathExpressionEngineExptrk<T>::CompileOutputs(const std::vector<std::string> &origin_varnames,
                                                   SymbolListType &symbol_list) {
    expressionParser.dec().assignment_symbols(symbol_list);
    for (ccs::types::uint32 i = 0; (i < symbol_list.size()); i++) {

        for (ccs::types::uint32 j = 0; (j < origin_varnames.size()); j++) {
            //when the output matches the variable, save the output cast
            if (origin_varnames[j] == symbol_list[i].first) {
                ::ccs::types::AnyValue var;
                (void) myProc->GetVariableValue(varNames[j], var);

                ::ccs::base::SharedReference<const ::ccs::types::AnyType> varTypeRef = var.GetType();
                ::ccs::types::uint8 typeId = static_cast<::ccs::types::uint8>(::ccs::types::GetIdentifier(varTypeRef->GetName()));
                //for the output register the output cast function
                varOutCast[j] = outputCasts[typeId];
            }
        }
    }

    return true;
}

template<typename T>
bool MathExpressionEngineExptrk<T>::Compile(const ccs::types::char8 *expressionIn,
                                            const Procedure &proc) {

    myProc = &proc;
    //get var names from expression
    std::string exprStr = expressionIn;
    std::replace(exprStr.begin(), exprStr.end(), '[', 'I');
    std::replace(exprStr.begin(), exprStr.end(), ']', 'I');
    std::vector < std::string > origin_varnames;
    ::ccs::types::boolean ret = exprtk::collect_variables(exprStr.c_str(), origin_varnames);
    varNames = origin_varnames;
    if (ret) {
        ret = CompileInputs(origin_varnames);
    }
    else {
        log_error("MathExpressionEngineExptrk::Compile Failed exprtk::collect_variables of %s", expressionIn);
    }

    if (ret) {
        SymbolListType symbol_list;
        expressionParser.dec().collect_assignments() = true;
        expression.register_symbol_table(symbolTable);

        ret = expressionParser.compile(exprStr.c_str(), expression);
        if (ret) {
            //compile and get the assignments (outputs)
            ret = CompileOutputs(origin_varnames, symbol_list);
        }
    }

    if (!ret) {
        // Print a log of each detected error.
        for (::ccs::types::uint32 i = 0; i < expressionParser.error_count(); ++i) {
            exprtk::parser_error::type error = expressionParser.get_error(i);
            exprtk::parser_error::update_error(error, expressionIn);

            log_error("Error[%i]. Position: row %i, col %i. Type: %s. Description: %s\n", i, static_cast<::ccs::types::uint32>(error.line_no),
                      static_cast<::ccs::types::uint32>(error.column_no), exprtk::parser_error::to_str(error.mode).c_str(), error.diagnostic.c_str());

        }
    }

    return ret;
}

template<typename T>
bool MathExpressionEngineExptrk<T>::Execute(Workspace *ws) {

    ::ccs::types::uint32 nVars = varMem.size();

    for (::ccs::types::uint32 i = 0u; i < nVars; i++) {
        //do it for each element
        if (varInCast[i] != NULL) {
            ::ccs::types::uint32 varSize = varMem[i].size();
            ::ccs::types::AnyValue valT;
            (void) ws->GetValue(varNames[i], valT);
            void *inPtr = valT.GetInstance();
            for (::ccs::types::uint32 j = 0u; j < varSize; j++) {
                //todo workspace lock here (or better lock single vars)
                varInCast[i]->Do(inPtr, &varMem[i][0], j);
            }
        }
    }

    expression.value();

    for (::ccs::types::uint32 i = 0u; i < nVars; i++) {
        //do it for each element
        if (varOutCast[i] != NULL) {
            ::ccs::types::uint32 varSize = varMem[i].size();
            ::ccs::types::AnyValue valT;
            (void)ws->GetValue(varNames[i], valT);
            void *outPtr = valT.GetInstance();
            for (::ccs::types::uint32 j = 0u; j < varSize; j++) {
                varOutCast[i]->Do(&varMem[i][0], outPtr, j);
            }
            ws->SetValue(varNames[i], valT);
        }
    }
    return true;
}

BasicCastI::BasicCastI() {

}

BasicCastI::~BasicCastI() {

}

template<typename Tin, typename Tout>
BasicCast<Tin, Tout>::BasicCast() {

}

template<typename Tin, typename Tout>
BasicCast<Tin, Tout>::~BasicCast() {

}

template<typename Tin, typename Tout>
void BasicCast<Tin, Tout>::Do(void *in,
                              void *out,
                              ccs::types::uint32 idx) {
    Tin input = (reinterpret_cast<Tin*>(in))[idx];
    (reinterpret_cast<Tout*>(out))[idx] = static_cast<Tout>(input);

}

}
}

#endif /* MATHEXPRESSIONENGINEEXPTRK_H_ */

