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

#include "NewtonRaphson.h"

#ifndef NEWTON_RAPHSON_TXX
#define NEWTON_RAPHSON_TXX

//-------------------------------------------------------------------------
template <typename F, typename J, typename T>
NewtonRaphson<F, J, T>::NewtonRaphson(F argFunc, J argJaco)
{
  this->MaxIteration = 25;
  this->NbrIterationMade = 0;
  this->Function = argFunc;
  this->Jacobian = argJaco;
}

//-------------------------------------------------------------------------
template <typename F, typename J, typename T>
Eigen::Matrix<T, Eigen::Dynamic, 1> NewtonRaphson<F, J, T>::SolveEquation(Eigen::Matrix<T, Eigen::Dynamic, 1> Y,
                                                                          Eigen::Matrix<T, Eigen::Dynamic, 1> X0)
{
  Eigen::Matrix<T, Eigen::Dynamic, 1> X = X0;
  Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> J;

  while (this->ShouldIterate())
  {
    // Evaluate the Jacobian at the current point
    // and check if the jacobian is singular
    J = this->Jacobian(X);
    if (std::abs(J.determinant()) < 1e-16)
    {
      //std::cout << "NewtonRaphson: Jacobian singular, iterations stopped" << std::endl;
      break;
    }

    X = X - J.inverse() * (this->Function(X) - Y);
  }

  return X;
}

//-------------------------------------------------------------------------
template <typename F, typename J, typename T>
bool NewtonRaphson<F, J, T>::ShouldIterate()
{
  this->NbrIterationMade++;

  // Stop iterations if:
  // - Maximum number of iteration has been reached
  bool shouldIterate = (this->NbrIterationMade <= this->MaxIteration);

  return shouldIterate;
}

#endif // NEWTON_RAPHSON_TXX