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
#define QLAY_API extern "C" __declspec(dllexport)
#else
#define QLAY_API extern "C" __declspec(dllimport)
#endif

#include <memory>

namespace qlay
{
	//Initialises the system with a seed based on current time
	QLAY_API void init();
}
