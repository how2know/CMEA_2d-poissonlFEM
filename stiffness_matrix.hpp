#pragma once
#include <Eigen/Core>
#include <Eigen/Dense>
#include "grad_shape.hpp"
#include "coordinate_transform.hpp"
#include "integrate.hpp"
#include "shape.hpp"

//----------------compMatrixBegin----------------

//! Evaluate the stiffness matrix on the triangle spanned by
//! the points (a, b, c).
//!
//! Here, the stiffness matrix A is a 3x3 matrix
//! 
//! $$A_{ij} = \int_{K} ( \nabla \lambda_i^K(x, y) \cdot  \nabla \lambda_j^K(x, y)\; dV$$
//! 
//! where $K$ is the triangle spanned by (a, b, c).
//!
//! @param[out] stiffnessMatrix should be a 3x3 matrix
//!                        At the end, will contain the integrals above.
//!
//! @param[in] a the first corner of the triangle
//! @param[in] b the second corner of the triangle
//! @param[in] c the third corner of the triangle

template<class MatrixType, class Point>

void computeStiffnessMatrix(MatrixType& stiffnessMatrix,
                            const Point& a,
                            const Point& b,
                            const Point& c)
{
    // Jacobian J_K of the mapping function
    Eigen::Matrix2d coordinateTransform = makeCoordinateTransform(b - a, c - a);

    // absolute value of the determinant of J_K
    double volumeFactor = std::abs(coordinateTransform.determinant());

    // tranpose of the inverse of J_K
    Eigen::Matrix2d elementMap = coordinateTransform.inverse().transpose();

// (write your solution here)

    /// Start of my solution ///

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // function to compute the integral
            auto integrand = [&](double x, double y)
            {
                Eigen::Vector2d vector1 = elementMap * gradientLambda(i, x, y);
                Eigen::Vector2d vector2 = elementMap * gradientLambda(j, x, y);

                double vProduct = vector1.dot(vector2);

                return vProduct * volumeFactor;
            };

            // element A_ij of the stiffness matrix
            stiffnessMatrix(i, j) = integrate(integrand);
        }
    }

    /// End of my solution ///

    
}

//----------------compMatrixEnd----------------
