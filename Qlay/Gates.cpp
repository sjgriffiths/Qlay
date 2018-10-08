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
}
