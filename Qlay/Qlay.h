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
	//Basis vectors (|0> and |1>) yield binary result
	using Basis = bool;


	//State vector (forward declaration used internally)
	struct State;

	//Represents a qubit (quantum bit), a linear combination of |0> and |1>
	class QLAY_API Qubit
	{
	public:
		//Default constructor initialises to |0>
		Qubit();

		//Prepares to the given basis state
		Qubit(Basis b);

		//Assignment by preparing to the given basis state
		Qubit &operator=(Basis b);

		//Qubits cannot be classically copied
		Qubit(const Qubit&) = delete;
		Qubit &operator=(const Qubit&) = delete;

		//State vector representation
		std::shared_ptr<State> s;
	};


	//Initialises the system with a seed based on current time
	QLAY_API void init();

	//Initialises the system with the given seed
	QLAY_API void init(unsigned seed);

	//Returns true with the given probability (Bernoulli distribution)
	QLAY_API bool chance(double p);


	//Measures the given qubit
	QLAY_API Basis M(Qubit &q);
}
