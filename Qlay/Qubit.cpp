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
	QubitSystem::QubitSystem() : state_(std::make_shared<State>())
	{
	}

	std::ostream& operator<<(std::ostream& os, const QubitSystem &system)
	{
		os << system.state_->v;
		return os;
	}

	Qubit::Qubit(QubitSystem &system) : system_(system)
	{
		Ket &k = system.state_->v;

		if (system.count() == 0)
			k = ZERO;
		else
			k = kronecker_product(ZERO, k);

		index_ = system.count_++;
	}
}
