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
}
