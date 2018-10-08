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

namespace qlay
{
	//Global RNG used to simulate nondeterminism
	extern std::default_random_engine rng;
}
