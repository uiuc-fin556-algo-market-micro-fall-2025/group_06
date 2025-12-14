/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Analytics/Regression.h
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

#ifndef _STRATEGY_STUDIO_ANALYTICS_REGRESSION_H_
#define _STRATEGY_STUDIO_ANALYTICS_REGRESSION_H_

#include "MatrixTypes.h"

#include <eigen/Eigen/Eigen>

namespace RCM {
namespace StrategyStudio {
namespace Analytics {

enum DecompositionType {
    DECOMPOSITION_TYPE_SVD,
    DECOMPOSITION_TYPE_QR,
    DECOMPOSITION_TYPE_LU
};

/**
 * Performs an Ordinary Least Squares regression
 *
 * @param y the M*1 column vector of observations for the dependent variable of the regression.
 * @param x the M*N matrix of independent observation variables. For regression with an intercept, use a column of 1s in this matrix.
 * @param calculation_method determines how to solve the OLS equation. The default QR balances speed and numeric stability, SVD is more numerically stable in the presence of near singularity, LU is fastest
 * @return is an N*1 column vector containing the regression coefficient estimates.
 */
VectorXd RegressionOLS(const VectorXd& y, const MatrixXd& x, DecompositionType calculation_method = DECOMPOSITION_TYPE_QR);

/**
 * Performs an Weighted Ordinary Least Squares regression
 *
 * @param y the M*1 column vector of observations for the dependent variable of the regression.
 * @param x the M*N matrix of independent observation variables. For regression with an intercept, use a column of 1s in this matrix.
 * @param w the M*1 column vector of observation weights. The algorithm will minimize sum(w*e^2)
 * @param calculation_method determines how to solve the OLS equation. The default QR balances speed and numeric stability, SVD is more numerically stable in the presence of near singularity, LU is fastest
 * @return is an N*1 column vector containing the regression coefficient estimates.
 */
VectorXd RegressionWeightedOLS(const VectorXd& y, const MatrixXd& x, const VectorXd& w, DecompositionType calculation_method = DECOMPOSITION_TYPE_QR);

/**
 * Calculates the residuals from a regression
 *
 * @param y the M*1 column vector of observations for the dependent variable of the regression.
 * @param x the M*N matrix of independent observation variables.
 * @param regression_coeffs the N*1 column vector output from a regression function
 * @return a M*1 column vector containing the regression residuals
 */
VectorXd Residuals(const VectorXd& y, const MatrixXd& x, const VectorXd& regression_coeffs);

/**
 * Retrieves the standard residuals from a regression
 *
 * @param residuals the M*1 column vector of residuals obtained from a regression
 * @param has_intercept indicate whether the regression was performed with an intercept term
 * @return an M*1 column vector containing the standard residuals
 */
VectorXd StandardResiduals(const VectorXd& residuals, bool has_intercept);

/**
 * Calcualtes the standard error of an OLS regression
 *
 * @param residuals the M*1 column vector of residuals obtained from a regression
 * @param coeff_count the number of coefficients calculated by the regression, should be set to x.cols()
 * @return the standard error of the regression
 */
double RegressionStandardError(const VectorXd& residuals, Index coeff_count);

/**
 * Calculates the standard error of a weighted OLS regression
 *
 * @param residuals the M*1 column vector of residuals obtained from a regression
 * @param w the M*1 column vector of observation weights.
 * @param coeff_count the number of coefficients calculated by the regression, should be set to x.cols()
 * @return the standard error of the regression
 */
double RegressionWeightedStandardError(const VectorXd& residuals, const VectorXd& w, Index coeff_count);

/**
 * Calculates the standard errors for the coefficients of an OLS regression
 *
 * @param y the M*1 column vector of observations for the dependent variable of the regression.
 * @param x the M*N matrix of independent observation variables.
 * @param regression_coeffs the N*1 column vector output from a regression function
 * @return an N*1 column vector containing the standard error of each regression coeff
 */
VectorXd CoefficientsStandardError(const VectorXd& y, const MatrixXd& x, const VectorXd& regression_coeffs);

/**
 * Calculates the standard errors for the coefficients of an OLS regression
 *
 * @param y the M*1 column vector of observations for the dependent variable of the regression.
 * @param residuals the M*1 column vector of residuals obtained from a regression
 * @return an N*1 column vector containing the standard error of each regression coeff
 */
VectorXd CoefficientsStandardError(const MatrixXd& x, const VectorXd& residuals);

/**
 * Calculates the standard errors for the coefficients of a weighted OLS regression
 *
 * @param y the M*1 column vector of observations for the dependent variable of the regression.
 * @param residuals the M*1 column vector of residuals obtained from a regression
 * @param w the M*1 column vector of observation weights.
 * @return an N*1 column vector containing the standard error of each regression coeff
 */
VectorXd WeightedCoefficientsStandardError(const MatrixXd& x, const VectorXd& residuals, const VectorXd& w);

/**
 * Calculates Coefficient of Determination for an OLS regression
 *
 * @param y the M*1 column vector of observations for the dependent variable of the regression.
 * @param residuals the M*1 column vector of residuals obtained from a regression
 * @param has_intercept indicate whether the regression was performed with an intercept term
 * @return the r2 of the regression
 */
double R2(const VectorXd& y, const VectorXd& residuals, bool has_intercept);

/**
 * 'Adjusts' the Coefficient of Determination based on the number of explanatory terms in the OLS regression
 *
 * @param r2 the r2 of a least squares regression
 * @param observation_count the number of observations from the regression
 * @param coeff_count the number of coefficients calculated by the regression, should be set to x.cols()
 * @param has_intercept indicate whether the regression was performed with an intercept term
 * @return the adjusted r2 of the regression
 */
double AdjustedR2(double r2, Index observation_count, Index coeff_count, bool has_intercept);

/**
 * Performs a Least Absolute Deviation regression
 *
 * This function is implemented using a simplex solution method based on the Barrodale-Roberts algorithm
 *
 * @param y is the M*1 column vector of observations for the dependent variable of the regression.
 * @param x is the M*N matrix of independent observation variables. For regression with an intercept, use a column of 1s in this matrix.
 * @return is an N*1 column vector containing the regression coefficient estimates.
 */
VectorXd RegressionLeastAbsoluteDeviation(const VectorXd& y, const MatrixXd& x);

} // namespace Analytics
} // namespace StrategyStudio
} // namespace RCM

#endif
