//=========================================================================
// Nyx Solver - Non Linear Problems Solver
//
// Copyright 2018 Pierre Guilbert
// Author: Pierre Guilbert (spguilbert@gmail.com)
// Data: 02-11-2018
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//=========================================================================

#ifndef NUMERICAL_DIFF_H
#define NUMERICAL_DIFF_H

// STD
#include <numeric>
#include <iostream>
#include <cmath>

// Eigen
#include <Eigen/Dense>

namespace nyx
{
  enum DifferentiationMethod
  {
    NewtonQuotient = 0,
    SymmetricQuotient = 1,
    SecondOrderQuotient = 2
  };
  /**
  * \class NumericalDiff
  * \brief Compute Jacobian using numerical differantiation
  *
  * Compute the Jacobian matrix of a function using numerical
  * differentiation.
  *
  * Let's F be a function F: U -> R, U an open of R.
  * F is said to be differentiable at a in U if:
  * F'(a) = lim h -> 0 (F(x+h)-F(x)) / h exists (i)
  * it exist o(h), F(a+h) = F(a) + F'(a)h + o(h) and lim h -> 0 o(h)/h = 0 (ii)
  *
  * The idea is to compute an approximation of the derivation
  * of a function F using the differentiation definition with
  * h small enought
  *
  * F(x+h) = F(x) + F'(x)h + o(h) (1)
  * Hence,
  * F'(x) = [F(x+h) - F(x)] / h - o(h) (2)
  * Using (2) and removing the o(h) lead to the Newton's
  * difference quotient, i.e:
  * F'(x) ~ [F(x+h) - F(x)]/h
  *
  * But we can also use a more sophisticated formula to have
  * a better error control. Using (2) with +h and -h leads
  * to the symmetric difference quotient
  * F'(X) ~ [F(x+h) - F(x-h)]/(2h)
  *
  * Using this formula the error goes from o(h) to o(h^2)
  * R = -f'''(a) * h^2 / 6
  *
  * From a mathematical point of view, the smaller h the better the
  * accuracy of the derivative approximation. But, since we are using
  * floating point arithmetic we will introduce rounded error in the approx.
  * The choice of h is then very important, it should not be too big or
  * the approximaion will be poor since the error depend on h o(h) or o(h^2)
  * depending on the diff formula. On the other hand, the choice of h should not
  * be too big or the rounded error will decrease the accuracy.
  * In fact, all the difference formulae are ill-conditioned, i.e
  * a small change of the input argument (h) can lead to a big change
  * of the output argument (the derivative approximation).
  *
  * A choice of h that will not lead to huge rounded error is sqrt(eps) * a
  * with eps being the machine accuracy
  *
  * \author $Author: Pierre Guilbert $
  * \version $Revision: 1.0 $
  * \date $Date: 02-11-2018 $
  * Contact: spguilbert@gmail.com
  */
  template <typename F, typename T>
  class NumericalDiff
  {
  public:
    /// default constructor
    NumericalDiff();

    /// Class constructor
    NumericalDiff(F argFunction);

    /// Compute the jacobian of Function at the point X
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> operator()(Eigen::Matrix<T, Eigen::Dynamic, 1> X)
    {
      this->ComputeJacobian(X);
      return this->Jacobian;
    }

    void SetDifferentiationMethod(DifferentiationMethod method);
  protected:
    /// h values used to approximate the coordinate
    /// functions partial derivation using a determined
    /// differenciate formulae (Newton quotient, symmetric, ...)
    Eigen::Matrix<T, Eigen::Dynamic, 1> h;

    /// Computed approximated jacobian of the F function
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> Jacobian;

    /// Function to numerically differentiate
    F Function; /// function from R^inDIm -> R^outDim
    unsigned int inDim; /// input dimension of F
    unsigned int outDim; /// output dimension of F

    /// Differentiation method used to approximate
    /// the jacobian of the function. An higher order
    /// method means a more accurate approximation
    /// but a longer processing time
    DifferentiationMethod Method;

    /// Compute the approximated Jacobian
    void ComputeJacobian(Eigen::Matrix<T, Eigen::Dynamic, 1> X);
  };

// methods implementation
#include "NumericalDiff.txx"

} // namespace nyx
#endif // NUMERICAL_DIFF_H