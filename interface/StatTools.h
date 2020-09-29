/*! Definition of various statistical tools used to create hh stat models.
This file is part of https://github.com/cms-hh/HHStatAnalysis. 
Copied from: https://github.com/cms-hh/HHStatAnalysis/blob/master/StatModels/interface/StatTools.h
*/

#pragma once

#include <TVectorD.h>
#include <TMatrixD.h>
#include <TMatrixDEigen.h>

namespace stat_tools {

inline TMatrixD DiagonalizeLinearTransformMatrix(const TMatrixD& matrix, TMatrixD* eigen = 0, TMatrixD* eigen_inv = 0)
{
    if(matrix.GetNcols() != matrix.GetNrows())
        throw std::runtime_error("Matrix is not square.");

    TMatrixDEigen eigen_producer(matrix);
    auto _eigen = eigen_producer.GetEigenVectors();
    auto _eigen_inv = _eigen;
    _eigen_inv.Invert();
    if(eigen) *eigen = _eigen;
    if(eigen_inv) *eigen_inv = _eigen_inv;
    return _eigen_inv * matrix * _eigen;
}

template<typename T>
TMatrixD MatrixPower(const TMatrixD& matrix, T power)
{
    TMatrixD eigen(matrix.GetNrows(), matrix.GetNcols()), eigen_inv = eigen;
    auto diag = DiagonalizeLinearTransformMatrix(matrix, &eigen, &eigen_inv);
    for(int n = 0; n < diag.GetNcols(); ++n)
        diag[n][n] = std::pow(diag[n][n], power);
    return eigen * diag * eigen_inv;
}

inline TMatrixD CreateVarianceMatrix(const TVectorD& std_dev_vector)
{
    const int N = std_dev_vector.GetNrows();
    TMatrixD var_matrix(N, N);
    for(int n = 0; n < N; ++n)
        var_matrix[n][n] = std::pow(std_dev_vector[n], 2);
    return var_matrix;
}

inline TMatrixD CreateCovarianceMatrix(const TMatrixD& corr_matrix, const TVectorD& std_dev_vector)
{
    const int N = corr_matrix.GetNrows();
    if(corr_matrix.GetNcols() != N)
        throw std::runtime_error("Correlation matrix is not square.");
    if(std_dev_vector.GetNrows() != N)
        throw std::runtime_error("Size of the std dev vector do not correspond to the size of the correlation matrix.");

    TMatrixD cov_matrix(N, N);
    for(int n = 0; n < N; ++n) {
        if(corr_matrix[n][n] != 1)
            throw std::runtime_error("Self-correlation not equal to one.");
        for(int k = n; k < N; ++k) {
            if(std::abs(corr_matrix[n][k]) > 1)
                throw std::runtime_error("Correlation coefficient is greater than one.");
            if(corr_matrix[n][k] != corr_matrix[k][n])
                throw std::runtime_error("Correlation matrix is not symmetric.");
            cov_matrix[n][k] = corr_matrix[n][k] * std_dev_vector[n] * std_dev_vector[k];
            cov_matrix[k][n] = cov_matrix[n][k];
        }
    }

    return cov_matrix;
}

inline std::pair<TMatrixD, TVectorD> CreateCorrelationMatrixAndStdDevVector(const TMatrixD& cov_matrix)
{
    const int N = cov_matrix.GetNrows();
    if(cov_matrix.GetNcols() != N)
        throw std::runtime_error("Covariance matrix is not square.");
    TMatrixD corr_matrix(N, N);
    TVectorD std_dev_vector(N);

    for(int n = 0; n < N; ++n) {
        if(cov_matrix[n][n] <= 0)
            throw std::runtime_error("Variance is not positive.");
        std_dev_vector[n] = std::sqrt(cov_matrix[n][n]);
    }

    for(int n = 0; n < N; ++n) {
        corr_matrix[n][n] = 1;
        for(int k = n + 1; k < N; ++k) {
            if(cov_matrix[n][k] != cov_matrix[k][n])
                throw std::runtime_error("Covariance matrix is not symmetric.");
            corr_matrix[n][k] = cov_matrix[n][k] / (std_dev_vector[n] * std_dev_vector[k]);
            corr_matrix[k][n] = corr_matrix[n][k];
        }
    }

    return std::make_pair(corr_matrix, std_dev_vector);
}

// Compute matrix W using optimal whitening to decorrelat original variables.
// Variables z=Wx will be decorrelated between each other.
// Used whitening formula defined by A. Kessy, A. Lewin and K. Strimmer in https://arxiv.org/abs/1512.00809.
inline TMatrixD ComputeWhiteningMatrix(const TMatrixD& corr_matrix, const TVectorD& std_dev_vector)
{
    const int N = corr_matrix.GetNrows();
    if(corr_matrix.GetNcols() != N)
        throw std::runtime_error("Correlation matrix is not square.");
    if(std_dev_vector.GetNrows() != N)
        throw std::runtime_error("Size of the std dev vector do not correspond to the size of the correlation matrix.");

    auto corr_inv_sqrt = MatrixPower(corr_matrix, -0.5);
    TMatrixD var_matrix = CreateVarianceMatrix(std_dev_vector);
    auto var_inv_sqrt = MatrixPower(var_matrix, -0.5);
    return corr_inv_sqrt * var_inv_sqrt;
}

inline TMatrixD ComputeWhiteningMatrix(const TMatrixD& cov_matrix)
{
    auto corr_dev = CreateCorrelationMatrixAndStdDevVector(cov_matrix);
    return ComputeWhiteningMatrix(corr_dev.first, corr_dev.second);
}

} // namespace stat_tools
