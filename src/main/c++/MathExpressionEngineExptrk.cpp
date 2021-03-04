/******************************************************************************
* $HeadURL: $
* $Id: $
*
* Project       : CODAC Core System
*
* Description   : EPICS ChannelAccess helper
*
* Author        : G.Ferro (IO)
*
* Copyright (c) : 2010-2020 ITER Organization,
*                 CS 90 046
*                 13067 St. Paul-lez-Durance Cedex
*                 France
*
* This file is part of ITER CODAC software.
* For the terms and conditions of redistribution or use of this software
* refer to the file ITER-LICENSE.TXT located in the top level directory
* of the distribution package.
******************************************************************************/

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MathExpressionEngineExptrk.h"
#include "MathExpressionEngineProvider.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace sup {

namespace sequencer {

static MathExpressionEngineI* MathExpressionEngineExptrkCreator() {
    return new MathExpressionEngineExptrk<::ccs::types::float64> ;
}

class MathExpressionEngineExptrkRegistrator {
public:
    MathExpressionEngineExptrkRegistrator();
};

MathExpressionEngineExptrkRegistrator::MathExpressionEngineExptrkRegistrator() {
    MathExpressionEngineProvider::Instance()->Register(MathExpressionEngineExptrkCreator);
}

static MathExpressionEngineExptrkRegistrator mathExpressionEngineExptrkRegistrator;
}
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

