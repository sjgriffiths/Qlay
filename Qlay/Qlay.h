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
	class State;

	template class QLAY_API std::shared_ptr<State>;

	//Represents a system of potentially entangled qubits
	class QLAY_API QubitSystem
	{
		friend class Qubit;
		friend class Gate;
		friend class AngleGate;
		friend class TwoGate;
		friend QLAY_API Basis M(const Qubit &q);

	private:
		std::shared_ptr<State> state_;
		int count_ = 0;

	public:
		//Default constructor prepares empty system
		QubitSystem();

		//Returns the number of qubits in the system
		int count() const { return count_; }

		//Resets the system such that all qubits are in the |0> state
		void reset();

		//QubitSystems cannot be classically copied
		QubitSystem(const QubitSystem&) = delete;
		QubitSystem &operator=(const QubitSystem&) = delete;

		QLAY_API friend std::ostream& operator<<(std::ostream& os, const QubitSystem &system);
	};

	//Represents a qubit (quantum bit), a linear combination of |0> and |1>
	class QLAY_API Qubit
	{
	private:
		QubitSystem &system_;
		int index_;

	public:
		//Construct a new Qubit in the given QubitSystem, initialised to |0>
		Qubit(QubitSystem &system);

		//Construct to refer to a preexisting qubit at an index in the given QubitSystem
		Qubit(QubitSystem &system, int index);

		//Returns a reference to the qubit's owning system
		QubitSystem &system() const { return system_; }

		//Returns this qubit's index in the system
		int index() const { return index_; }

		//Qubits cannot be classically copied
		Qubit(const Qubit&) = delete;
		Qubit &operator=(const Qubit&) = delete;
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
	QLAY_API Basis M(const Qubit &q);

	//Measures the given qubit in the X (sign) basis
	QLAY_API Basis Mx(const Qubit &q);


	//Pauli X gate (NOT)
	QLAY_API void X(const Qubit &q);

	//Pauli Y gate
	QLAY_API void Y(const Qubit &q);

	//Pauli Z gate
	QLAY_API void Z(const Qubit &q);

	//Hadamard gate
	QLAY_API void H(const Qubit &q);

	//Square root NOT gate
	QLAY_API void SRNOT(const Qubit &q);


	//Rotation around the X axis
	QLAY_API void Rx(double angle, const Qubit &q);

	//Rotation around the Y axis
	QLAY_API void Ry(double angle, const Qubit &q);

	//Rotation around the Z axis
	QLAY_API void Rz(double angle, const Qubit &q);

	//Phase shift gate
	QLAY_API void Rp(double angle, const Qubit &q);


	//SWAP gate
	QLAY_API void SWAP(const Qubit &a, const Qubit &b);

	//Square root SWAP gate
	QLAY_API void SRSWAP(const Qubit &a, const Qubit &b);

	//Controlled NOT gate
	QLAY_API void CNOT(const Qubit &control, const Qubit &target);
}
