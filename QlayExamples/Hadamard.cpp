/**
 * @file Hadamard.cpp
 *
 * Demonstrates the Hadamard gate.
 *
 * @author Sam Griffiths
 */

#include "Examples.h"

using namespace qlay;

void Hadamard(unsigned repeats)
{
	init();

	int zeroes = 0, ones = 0;
	int matches = 0;

	for (unsigned i = 0; i < repeats; i++)
	{
		QubitSystem qs;
		Qubit q(qs);
		H(q);

		Basis result = M(q);
		result ? ones++ : zeroes++;

		if (M(q) == result)
			matches++;
	}

	std::cout << "ZERO:  " << zeroes << std::endl << "ONE:   " << ones << std::endl;
	std::cout << "MATCH: " << matches << std::endl;
}
