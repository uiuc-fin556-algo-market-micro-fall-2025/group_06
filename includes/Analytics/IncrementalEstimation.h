/*================================================================================
*     Source: ../RCM/StrategyStudio/includes/Analytics/IncrementalEstimation.h
*     Last Update: 2011/07/01 13:55:14
*     Contents:
*     Distribution:
*
*
*     Copyright (c) RCM-X, 2011.
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

#ifndef _STRATEGY_STUDIO_ANALYTICS_INCREMENTAL_ESTIMATION_H_
#define _STRATEGY_STUDIO_ANALYTICS_INCREMENTAL_ESTIMATION_H_

#include "MatrixTypes.h"
#include "Regression.h"
#include "IIndicator.h"

#include <limits>

namespace RCM {
namespace StrategyStudio {
namespace Analytics {

/**
 * RecursiveLeastSquares performs in incremental least squares estimation,
 * where the objective is to minimize sum of squared errors, weighted by
 * an exponentially applied 'forgetting factor' lambda.
 *
 * This implementation allows an initialization period during which full Ordinary Least
 * Squares estimation is used to arrive at sensible initial values for estimate covariance
 */
class RecursiveLeastSquares {
public:
    /**
     * Constructs a Recursive Least Squares Estimation
     *
     * @param coeff_count This is the number of explanatory variables expected in the regression, including the constant term, which should always be input as "1"
     * @param lambda This is the 'forgetting' factor, and should be 0<lambda<=1. A value of 1 will equal weight all observations. Smaller values correspond to quicker decay in the weight of older observations
     * @param initialization_period The number of observations during which full Ordinary Least Squares should be applied
     * @param normalization_constant This factor is used to prevent numeric instability, which will be present in the same circumstances the OLS equations would be non or nearly non-invertable. A small number should be used, and this number should to be tuned to the use case when numeric stability is a concern. Defaults to machine epsilon.
     */
    RecursiveLeastSquares(int coeff_count, double lambda, int initialization_period, double normalization_constant = std::numeric_limits<double>::epsilon());

    /**
     * Resets the Recursive Least squares calculations
     */
    bool Reset();

    /**
     * Updates the Recursive Least Squares estimation with a new observation
     *
     * @param y value of the new observation of the dependent variable we are estimating
     * @param x vector of values for a new observation of the independent explanatory variables
     */
    bool Update(double y, const VectorXd& x);

    /**
     * Returns the current value of the estimated regression coefficients
     */
    const VectorXd& RegressionCoefficients() const {return m_beta;}

    /**
     * Returns the predicted value of the regression given the current regression coefficient estimations and the
     * given input observations.
     *
     * This function will return 0 until the estimation is fully initialized.
     */
    double PredictedY(const VectorXd& x) const {return x.dot(m_beta);}

    /**
     * Returns whether the calculation is fully initialized, aka whether the initial OLS calculation has completed
     */
    bool fully_initialized() const {return (m_observations >= m_initialization_period);}

    double lambda() const {return m_lambda;}

    void set_lambda(double lambda){m_lambda = lambda;}

private:
    MatrixXd m_update_matrix;
    MatrixXd m_xn;

    VectorXd m_beta;
    VectorXd m_yn;

    double m_normalization_constant;
    double m_lambda;

    int m_observations;
    int m_coeff_count;
    int m_initialization_period;
};

/**
 * FlexibleLeastSquaresKalman estimates a least squares regression where time varying beta
 * is likely. The algorithm explicitly penalizes the estimation for changes in the estimated
 * value of beta between observations.
 *
 * This implementation is based on the Kalman filter algorithm suggested in
 * "Flexible least squares for temporal data mining and statistical arbitrage" by
 * Montana, Trantafyllopoulos, and Tsagaris, 2009. Please visit http://www2.econ.iastate.edu/tesfatsi/FLSTemporalDataMining.GMontana2009.pdf
 * for more information.
 *
 * This algorith may not converge well unless the variances of the observation and
 * state change process are not well approximated by 1 and I/mu respectively, so take
 * care with the scale of the input variables, and/or use the class's setter functions
 * to initialize or rescale the variance settings beyond what's allowed by the core algorithm.
 */
class FlexibleLeastSquaresKalman {
public:

    /**
     * Constructs a Flexible Least Square Kalman Estimation
     *
     * @param coeff_count This is the number of explanatory variables expected in the regression, including the constant term, which should always be input as "1"
     * @param mu This controls the assumed covariance matrix of the change in the coefficient vector. Specifically this matrix is set to I/mu. A large value means small coefficient changes between estimates, small values correspond to more rapidly changing betas.
     */
    FlexibleLeastSquaresKalman(int coeff_count, double mu);

    /**
     * Resets the FlexibleLeastSquaresKalman calculations
     */
    bool Reset();

    /**
     * Updates the FlexibleLeastSquaresKalman estimation with a new observation
     *
     * @param y value of the new observation of the dependent variable we are estimating
     * @param x vector of values for a new observation of the independent explanatory variables
     */
    bool Update(double y, const VectorXd& x);

    /**
     * Returns the current value of the estimated regression coefficients
     */
    const VectorXd& RegressionCoefficients() const {return m_beta;}

    /**
     * Returns the predicted value of the regression given the current regression coefficient estimations and the
     * given input observations.
     *
     * This function will return 0 until the estimation is fully initialized.
     */
    double PredictedY(const VectorXd& x) const {return x.dot(m_beta);}

    /**
     * Returns whether the calculation is fully initialized, aka whether the initial OLS calculation has completed
     */
    bool fully_initialized() const {return m_initialized;}

    /**
     * Can be used to set or provide an initial estimate for the regression coefficients
     *
     * @return Returns true if the beta succesfully updates, false if the dimensions of the input vector are incorrect
     */
    bool set_regression_coefficients(const VectorXd& beta);

    /**
     * Can be used to set or provide an initial estimate for the covariance matrix P[t] = Covar(B[t] - Estimated(B[t]))
     *
     * @return Returns true if the covariance succesfully updates, false if the dimensions of the input matrix are incorrect
     */
    bool set_regression_coefficient_variance(const MatrixXd& pt);

    /**
     * Can be used to set or provide an estimate for variance in the 'error' term in the Kalman state space model,
     * specifically Beta[t+1] = Beta[t] + w[t], y[t] = x'[t]*B[t] + error[t]
     *
     * For the normal Flexible Least Squares computation, this variance maps to 1
     *
     * @param variance The new variance to use in the estimation updates. Should be greater than 0
     * @return Returns true if the variance succesfully updates, false if input value is non-positive
     */
    bool set_error_variance(double variance);

    /**
     * Can be used to set mu, which determines how fast Betas adjust between observations.
     * as with FLS, Covar(B[t+1]-B[t]) = I/mu
     *
     * For the normal Flexible Least Squares computation, this variance maps to 1
     *
     * @param variance The new mu to use in the estimation updates. Should be greater than 0
     * @return Returns true if the variance succesfully updates, false if input value is non-positive
     */
    bool set_mu(double mu);

private:
    MatrixXd m_vw;
    MatrixXd m_rt;
    VectorXd m_beta;
    double m_ve;
    int m_coeff_count;
    bool m_initialized;
};

} // namespace Analytics
} // namespace StrategyStudio
} // namespace RCM

#endif