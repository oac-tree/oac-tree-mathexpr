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

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MathExpressionEngineProvider.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace sup {
namespace sequencer {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
MathExpressionEngineProvider *MathExpressionEngineProvider::mySelf=NULL;

MathExpressionEngineProvider::MathExpressionEngineProvider() {
    regEngineCreator = NULL;
}

MathExpressionEngineProvider::~MathExpressionEngineProvider() {
    if (mySelf != NULL) {
        delete mySelf;
    }
}

MathExpressionEngineProvider* MathExpressionEngineProvider::Instance() {
    if (mySelf == NULL) {
        mySelf = new MathExpressionEngineProvider;
    }
    return mySelf;
}

void MathExpressionEngineProvider::Register(MathExpressionEngineIBuilderFcn engineCreator) {
    regEngineCreator = engineCreator;
}

MathExpressionEngineI* MathExpressionEngineProvider::CreateNewEngine() {
    if (regEngineCreator != NULL) {
        return regEngineCreator();
    }
    else {
        return NULL;
    }
}

}
}

