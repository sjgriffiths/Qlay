/**
 * @file Core.h
 *
 * Core internal header defining types, constants and utilities.
 *
 * @author Sam Griffiths
 */

#pragma once

#include "Qlay.h"

#include <random>
#include <chrono>
#include <complex>

#include <Eigen/Dense>

namespace qlay
{
	//Global RNG used to simulate nondeterminism
	extern std::default_random_engine rng;

	//Complex number
	using Complex = std::complex<double>;

	//Bra (row) vector
	using Bra = Eigen::Matrix<Complex, 1, Eigen::Dynamic>;

	//Ket (column) vector
	using Ket = Eigen::Matrix<Complex, Eigen::Dynamic, 1>;

	//Generic matrix
	using Mat = Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic>;

	//State vector wrapper class
	class State
	{
	private:
		Ket v;

	public:
		//References the Eigen state vector
		inline Ket &get() { return v; }
	};

	// |0> basis vector
	const Ket ZERO ((Ket(2) << 1, 0).finished());

	// |1> basis vector
	const Ket ONE  ((Ket(2) << 0, 1).finished());

	//Computes the Kronecker product of the given matrices
	Mat kronecker_product(const Mat &a, const Mat &b);
}
