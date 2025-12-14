/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Analytics/MatrixTypes.h
*     Last Update: 2011/07/1 13:55:14
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2009 - 2011.
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

#ifndef _STRATEGY_STUDIO_ANALYTICS_MATRIX_TYPES_H_
#define _STRATEGY_STUDIO_ANALYTICS_MATRIX_TYPES_H_

#pragma warning(push)
#pragma warning(disable : 4456) // declaration of 'x' hides previous local declaration
#include <eigen/Eigen/Eigen>
#pragma warning(pop)

namespace RCM {
namespace StrategyStudio {
namespace Analytics {

typedef Eigen::MatrixXd MatrixXd;
typedef Eigen::VectorXd VectorXd;
typedef Eigen::VectorXi VectorXi;
typedef Eigen::MatrixXd::Index Index;

} // namespace Analytics
} // namespace StrategyStudio
} // namespace RCM

#endif