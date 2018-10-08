/**
 * @file Qlay.h
 *
 * Single header file include for the Qlay
 * C++ Quantum Playground API.
 *
 * @author Sam Griffiths
 */

#pragma once

#ifdef QLAY_EXPORTS
#define QLAY_API __declspec(dllexport)
#else
#define QLAY_API __declspec(dllimport)
#endif

#include <memory>

namespace qlay
{
	//Initialises the system with a seed based on current time
	QLAY_API void init();

	//Initialises the system with the given seed
	QLAY_API void init(unsigned seed);

	//Returns true with the given probability (Bernoulli distribution)
	QLAY_API bool chance(double p);
}
