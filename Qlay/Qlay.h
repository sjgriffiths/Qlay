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

	//Pi constant
	constexpr double PI = 3.14159265358979323846;

	//1/sqrt(2) constant
	const double INV_ROOT_2 = 1.0 / sqrt(2.0);


	//State vector (forward declaration used internally)
	struct State;

	template class QLAY_API std::shared_ptr<State>;

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

	//Converts the given angle from degrees to radians
	QLAY_API double deg_to_rad(double angle);


	//Measures the given qubit in the Z (computational) basis
	QLAY_API Basis M(Qubit &q);

	//Measures the given qubit in the X (sign) basis
	QLAY_API Basis Mx(Qubit &q);


	//Pauli X gate (NOT)
	QLAY_API void X(Qubit &q);

	//Pauli Y gate
	QLAY_API void Y(Qubit &q);

	//Pauli Z gate
	QLAY_API void Z(Qubit &q);

	//Hadamard gate
	QLAY_API void H(Qubit &q);

	//Square root NOT gate
	QLAY_API void SRNOT(Qubit &q);


	//Rotation around the X axis
	QLAY_API void Rx(double angle, Qubit &q);

	//Rotation around the Y axis
	QLAY_API void Ry(double angle, Qubit &q);

	//Rotation around the Z axis
	QLAY_API void Rz(double angle, Qubit &q);

	//Phase shift gate
	QLAY_API void Rp(double angle, Qubit &q);
}
