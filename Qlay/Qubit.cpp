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
		Ket &k = system.state_->get();

		//Print each coefficient
		for (Eigen::Index i = 0; i < k.size(); i++)
		{
			Complex z = k(i);

			//Format basis vector as binary number
			os << "|";
			for (int j = system.count(); j > 0; j--)
				os << ((i >> (j-1)) & 1);
			os << "> ";

			//Print coefficient in the form a+bi
			if (z.real() == 0 && z.imag() == 0)
				os << 0 << std::endl;
			else
			{
				if (z.real() != 0)
					os << z.real();

				if (z.imag() != 0)
				{
					os << (z.imag() > 0 ? "+" : "-");
					if (std::abs(z.imag()) != 1)
						os << std::abs(z.imag());
					os << "i";
				}

				os << std::endl;
			}
		}

		return os;
	}

	Qubit::Qubit(QubitSystem &system) : system_(system)
	{
		Ket &k = system.state_->get();

		if (system.count() == 0)
			k = ZERO;
		else
			k = kronecker_product(ZERO, k);

		index_ = system.count_++;
	}

	Qubit::Qubit(QubitSystem &system, int index) : system_(system), index_(index)
	{
	}
}
