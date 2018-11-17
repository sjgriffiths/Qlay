/**
 * @file Gates.cpp
 *
 * Implements quantum logic gates.
 *
 * @author Sam Griffiths
 */

#include "Core.h"

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
		const Mat Y = (Mat(2, 2) <<              0, Complex(0, -1),
			                        Complex(0, -1),              0).finished();

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
			double hsin = sin(angle / 2.0);
			double hcos = cos(angle / 2.0);
			return (Mat(2, 2) <<              hcos, Complex(0, -hsin),
				                 Complex(0, -hsin),              hcos).finished();
		}

		//Y rotation
		Mat Ry(double angle)
		{
			double hsin = sin(angle / 2.0);
			double hcos = cos(angle / 2.0);
			return (Mat(2, 2) << hcos, -hsin,
				                 hsin,  hcos).finished();
		}

		//Z rotation
		Mat Rz(double angle)
		{
			return (Mat(2, 2) << exp(Complex(0, -angle/2.0)),                          0,
				                                           0, exp(Complex(0, angle/2.0))).finished();
		}

		//Phase shift
		Mat Rp(double angle)
		{
			return (Mat(2, 2) << 1,                      0,
				                 0, exp(Complex(0, angle))).finished();
		}
	}

	//Expands the given operator matrix to apply to a given qubit
	Mat expand_operator(const Mat &m, int count, int index)
	{
		Mat result(1, 1);
		result << 1;

		for (int i = 0; i < index; i++)
			result = kronecker_product(matrices::I, result);

		result = kronecker_product(m, result);

		for (int i = index; i < count - 1; i++)
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
			Ket &state = q.system().state_->v;
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
			Ket &state = q.system().state_->v;
			Mat op = expand_operator(m_(angle), q.system().count(), q.index());
			state = op * state;
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
		const AngleGate Ry(matrices::Rx);
		const AngleGate Rz(matrices::Rx);
		const AngleGate Rp(matrices::Rx);
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


	//Basis M(Qubit &q)
	//{
	//	Ket &k = q.s->v;
	//	Basis result = chance(std::norm(k(1)));
	//	k = result ? ONE : ZERO;
	//	return result;
	//}

	//Basis Mx(Qubit &q)
	//{
	//	//Rotate X onto Z
	//	H(q);

	//	//Measure in Z
	//	Basis result = M(q);

	//	//Rotate back again
	//	H(q);

	//	return result;
	//}
}
