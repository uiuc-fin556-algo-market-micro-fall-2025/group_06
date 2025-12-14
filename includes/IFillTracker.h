/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/IFillTracker.h
*     Last Update: 2010/09/30 13:55:14
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2009 - 2010.
*     All rights reserved.
*
*     This software is part of Licensed material, which is the property of RCM-X ("Company"),
*     and constitutes Confidential Information of the Company.
*     Unauthorized use, modification, duplication or distribution is strictly prohibited by Federal law.
*     No title to or ownership of this software is hereby transferred.
*
*     The software is provided "as is", and in no event shall the Company or any of its affiliates or successors be liable for any
*     damages, including any lost profits or other incidental or consequential damages relating to the use of this software.
*     The Company makes no representations or warranties, express or implied, with regards to this software.
==================================================================================*/

#pragma once

#ifndef _STRATEGY_STUDIO_LIB_I_FILL_TRACKER_H_
#define _STRATEGY_STUDIO_LIB_I_FILL_TRACKER_H_

#include "FillInfo.h"
#include <Utilities/GenericRecord.h>

namespace RCM {
namespace StrategyStudio {

/**
 * IFillTracker is an interface for accessing historic fill records
 */
class IFillTracker: public Utilities::GenericRecord<FillInfo> {
public:
    typedef Container FillInformation;
    typedef Container::iterator FillInformationIter;
    typedef Container::const_iterator FillInformationConstIter;

public:
    IFillTracker(){}
    virtual ~IFillTracker(){}
    virtual FillInformationConstIter fill_info_begin() const = 0;
    virtual FillInformationConstIter fill_info_end() const = 0;
};

} // namespace StrategyStudio
} // namespace RCM

#endif
