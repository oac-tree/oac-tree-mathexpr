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

#ifndef MATHEXPRESSIONENGINEI_H_
#define MATHEXPRESSIONENGINEI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <common/BasicTypes.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Instruction.h"
#include <string>
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace sup {

namespace sequencer {

/**
 * @brief MathExpressionEngineI interface
 */
class MathExpressionEngineI {
public:

    /**
     * @brief Constructor
     */
    MathExpressionEngineI() = default;

    /**
     * @brief Destructor
     */
    virtual ~MathExpressionEngineI() = default;

    /**
     * @brief Compile the expression
     */
    virtual bool Compile(const ccs::types::char8 *expressionIn,
                         const Procedure &proc)=0;

    /**
     * @brief Execute the computation.
     */
    virtual bool Execute(Workspace *ws)=0;
};

}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MATHEXPRESSIONENGINEI_H_ */

