/**
 * @file Qubit.cpp
 *
 * Implements the Qubit class.
 *
 * @author Sam Griffiths
 */

#include "Core.h"

namespace qlay
{
	Qubit::Qubit() : Qubit(false)
	{
	}

	Qubit::Qubit(Basis b) : s(std::make_shared<State>())
	{
		s->v = b ? ONE : ZERO;
	}

	Qubit &Qubit::operator=(Basis b)
	{
		if (M(*this) != b)
			X(*this);

		return *this;
	}
}
