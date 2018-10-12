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
	Basis M(Qubit &q)
	{
		Ket &k = q.s->v;
		Basis result = chance(std::norm(k(1)));
		k = result ? ONE : ZERO;
		return result;
	}

	Basis Mx(Qubit &q)
	{
		//Rotate X onto Z
		H(q);

		//Measure in Z
		Basis result = M(q);

		//Rotate back again
		H(q);

		return result;
	}

	void X(Qubit &q)
	{
		Ket &k = q.s->v;

		Mat x(2, 2);
		x << 0, 1,
			 1, 0;

		k = x * k;
	}

	void Y(Qubit &q)
	{
		Ket &k = q.s->v;

		Mat y(2, 2);
		y <<             0, Complex(0, -1),
			 Complex(0, 1),              0;

		k = y * k;
	}

	void Z(Qubit &q)
	{
		Ket &k = q.s->v;

		Mat z(2, 2);
		z << 1,  0,
			 0, -1;

		k = z * k;
	}

	void H(Qubit &q)
	{
		Ket &k = q.s->v;

		Mat h(2, 2);
		h << INV_ROOT_2,  INV_ROOT_2,
			 INV_ROOT_2, -INV_ROOT_2;

		k = h * k;
	}

	void SRNOT(Qubit &q)
	{
		Ket &k = q.s->v;

		Mat srnot(2, 2);
		srnot << Complex(1,  1), Complex(1, -1),
			     Complex(1, -1), Complex(1,  1);
		srnot *= 0.5;

		k = srnot * k;
	}

	void Rx(double angle, Qubit &q)
	{
		Ket &k = q.s->v;

		double hsin = sin(angle / 2.0);
		double hcos = cos(angle / 2.0);

		Mat rx(2, 2);
		rx <<              hcos, Complex(0, -hsin),
			  Complex(0, -hsin),              hcos;

		k = rx * k;
	}

	void Ry(double angle, Qubit &q)
	{
		Ket &k = q.s->v;

		double hsin = sin(angle / 2.0);
		double hcos = cos(angle / 2.0);

		Mat ry(2, 2);
		ry << hcos, -hsin,
			  hsin,  hcos;

		k = ry * k;
	}

	void Rz(double angle, Qubit &q)
	{
		Ket &k = q.s->v;

		Mat rz(2, 2);
		rz << exp(Complex(0, -angle/2.0)),                            0,
			                            0, exp(Complex(0, angle/2.0));

		k = rz * k;
	}

	void Rp(double angle, Qubit &q)
	{
		Ket &k = q.s->v;

		Mat rp(2, 2);
		rp << 1,                      0,
			  0, exp(Complex(0, angle));

		k = rp * k;
	}
}
