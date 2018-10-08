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
}
