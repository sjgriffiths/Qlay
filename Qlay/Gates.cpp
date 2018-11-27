/**
 * @file Gates.cpp
 *
 * Implements quantum logic gates.
 *
 * @author Sam Griffiths
 */

#include "Core.h"

#include <set>

namespace qlay
{
	//Defines all base operator matrices
	namespace matrices
	{
		//Identity
		const Mat I = (Mat(2, 2) << 1, 0,
		                            0, 1).finished();

		//Pauli X
		const Mat X = (Mat(2, 2) << 0, 1,
			                        1, 0).finished();

		//Pauli Y
		const Mat Y = (Mat(2, 2) <<             0, Complex(0, -1),
			                        Complex(0, 1),              0).finished();

		//Pauli Z
		const Mat Z = (Mat(2, 2) << 1,  0,
			                        0, -1).finished();

		//Hadamard
		const Mat H = (Mat(2, 2) << INV_ROOT_2,  INV_ROOT_2,
			                        INV_ROOT_2, -INV_ROOT_2).finished();

		//Sqaure root NOT
		const Mat SRNOT = 0.5 *
			(Mat(2, 2) << Complex(1,  1), Complex(1, -1),
			              Complex(1, -1), Complex(1,  1)).finished();

		//X rotation
		Mat Rx(double angle)
		{
			double hsin = std::sin(angle / 2.0);
			double hcos = std::cos(angle / 2.0);
			return (Mat(2, 2) <<              hcos, Complex(0, -hsin),
				                 Complex(0, -hsin),              hcos).finished();
		}

		//Y rotation
		Mat Ry(double angle)
		{
			double hsin = std::sin(angle / 2.0);
			double hcos = std::cos(angle / 2.0);
			return (Mat(2, 2) << hcos, -hsin,
				                 hsin,  hcos).finished();
		}

		//Z rotation
		Mat Rz(double angle)
		{
			return (Mat(2, 2) << std::exp(Complex(0, -angle/2.0)),                               0,
				                                                0, std::exp(Complex(0, angle/2.0))).finished();
		}

		//Phase shift
		Mat Rp(double angle)
		{
			return (Mat(2, 2) << 1,                           0,
				                 0, std::exp(Complex(0, angle))).finished();
		}

		//SWAP
		const Mat SWAP = (Mat(4, 4) << 1, 0, 0, 0,
			                           0, 0, 1, 0,
			                           0, 1, 0, 0,
			                           0, 0, 0, 1).finished();

		//Square root SWAP
		const Mat SRSWAP = (Mat(4, 4) << 1,                  0,                  0, 0,
			                             0, Complex(0.5,  0.5), Complex(0.5, -0.5), 0,
			                             0, Complex(0.5, -0.5), Complex(0.5,  0.5), 0,
			                             0,                  0,                  0, 1).finished();

		//Controlled NOT
		const Mat CNOT = (Mat(4, 4) << 1, 0, 0, 0,
			                           0, 1, 0, 0,
			                           0, 0, 0, 1,
			                           0, 0, 1, 0).finished();
	}

	//Expands the given operator matrix to apply to a given qubit
	Mat expand_operator(const Mat &m, int count, int index)
	{
		Mat result(1, 1);
		result << 1;

		for (int i = 0; i < index; i++)
			result = kronecker_product(matrices::I, result);

		result = kronecker_product(m, result);

		//Reduce expansion if matrix is already two-input
		int limit = m.rows() == 4 ? count - 2 : count - 1;

		for (int i = index; i < limit; i++)
			result = kronecker_product(matrices::I, result);

		return result;
	}


	//Quantum logic gate functor
	class Gate
	{
	private:
		Mat m_;

	public:
		Gate(Mat m) : m_(m)
		{
		}

		void operator()(const Qubit &q) const
		{
			Ket &state = q.system().state_->get();
			Mat op = expand_operator(m_, q.system().count(), q.index());
			state = op * state;
		}
	};

	//Quantum logic gate functor, parametrised with angle
	class AngleGate
	{
	private:
		std::function<Mat(double)> m_;

	public:
		AngleGate(std::function<Mat(double)> m) : m_(m)
		{
		}

		void operator()(double angle, const Qubit &q) const
		{
			Ket &state = q.system().state_->get();
			Mat op = expand_operator(m_(angle), q.system().count(), q.index());
			state = op * state;
		}
	};

	//Quantum logic gate functor, taking two qubit inputs
	class TwoGate
	{
	private:
		Mat m_;

	public:
		TwoGate(Mat m) : m_(m)
		{
		}

		void operator()(const Qubit &a, const Qubit &b) const
		{
			QubitSystem &qs = b.system();
			Ket &state = qs.state_->get();

			//If the qubits are consecutive and correctly ordered, apply normally
			if (a.index() == b.index() + 1)
			{
				Mat op = expand_operator(m_, qs.count(), b.index());
				state = op * state;
			}

			//If the qubits are consecutive but incorrectly ordered, swap before and after
			else if (a.index() == b.index() - 1)
			{
				SWAP(b, a);
				Mat op = expand_operator(m_, qs.count(), a.index());
				state = op * state;
				SWAP(b, a);
			}

			//If the qubits are nonconsecutive, swap into end position
			else
			{
				//Swap b into end (q0 position)
				for (int i = b.index(); i > 0; i--)
					SWAP(Qubit(qs, i), Qubit(qs, i - 1));

				//a may have been displaced
				int a_i = a.index() < b.index() ? a.index() + 1 : a.index();

				//Swap a into end-but-one (q1 position)
				for (int i = a_i; i > 1; i--)
					SWAP(Qubit(qs, i), Qubit(qs, i - 1));

				//Apply operator to end
				Mat op = expand_operator(m_, qs.count(), 0);
				state = op * state;

				//Swap a back
				for (int i = 1; i < a_i; i++)
					SWAP(Qubit(qs, i + 1), Qubit(qs, i));

				//Swap b back
				for (int i = 0; i < b.index(); i++)
					SWAP(Qubit(qs, i + 1), Qubit(qs, i));
			}
		}
	};


	namespace gates
	{
		const Gate X(matrices::X);
		const Gate Y(matrices::Y);
		const Gate Z(matrices::Z);
		const Gate H(matrices::H);
		const Gate SRNOT(matrices::SRNOT);

		const AngleGate Rx(matrices::Rx);
		const AngleGate Ry(matrices::Ry);
		const AngleGate Rz(matrices::Rz);
		const AngleGate Rp(matrices::Rp);

		const TwoGate SWAP(matrices::SWAP);
		const TwoGate SRSWAP(matrices::SRSWAP);
		const TwoGate CNOT(matrices::CNOT);
	}

	inline void X(const Qubit &q) { return gates::X(q); }
	inline void Y(const Qubit &q) { return gates::Y(q); }
	inline void Z(const Qubit &q) { return gates::Z(q); }
	inline void H(const Qubit &q) { return gates::H(q); }
	inline void SRNOT(const Qubit &q) { return gates::SRNOT(q); }

	inline void Rx(double angle, const Qubit &q) { return gates::Rx(angle, q); }
	inline void Ry(double angle, const Qubit &q) { return gates::Ry(angle, q); }
	inline void Rz(double angle, const Qubit &q) { return gates::Rz(angle, q); }
	inline void Rp(double angle, const Qubit &q) { return gates::Rp(angle, q); }

	inline void SWAP(const Qubit &a, const Qubit &b) { return gates::SWAP(a, b); }
	inline void SRSWAP(const Qubit &a, const Qubit &b) { return gates::SRSWAP(a, b); }
	inline void CNOT(const Qubit &control, const Qubit &target) { return gates::CNOT(control, target); }


	//Returns a set of numbers below the upper bound x with bit b as value (default true)
	template <typename T, typename U>
	std::set<T> ints_with_bit(T x, U b, bool value = true)
	{
		static_assert(std::is_integral<T>::value, "Upper bound must be integer");
		static_assert(std::is_integral<U>::value, "Bit index must be integer");

		std::set<T> s;
		T mask = 1 << b;

		for (T i = 0; i < x; i++)
			if (static_cast<bool>(i & mask) == value)
				s.insert(i);

		return s;
	}

	Basis M(const Qubit &q)
	{
		//Set of coefficient indices where qx = |1>
		auto s = ints_with_bit(1 << q.system().count(), q.index());

		//Set complement
		auto sc = ints_with_bit(1 << q.system().count(), q.index(), false);

		Ket &state = q.system().state_->get();

		//Sum individual probabilities
		double p = 0;
		for (auto i : s)
			p += std::norm(state(i));

		bool result = chance(p);

		//Set contradictory states to zero
		for (auto i : (result ? sc : s))
			state(i) = 0;

		//Initialise then sum normalisation constant
		double norm = 0;
		for (auto i : (result ? s : sc))
			norm += std::norm(state(i));

		//Renormalise states
		norm = std::sqrt(norm);
		for (auto i : (result ? s : sc))
			state(i) /= norm;

		return result;
	}

	Basis Mx(const Qubit &q)
	{
		//Rotate X onto Z
		H(q);

		//Measure in Z
		Basis result = M(q);

		//Rotate back again
		H(q);

		return result;
	}
}
