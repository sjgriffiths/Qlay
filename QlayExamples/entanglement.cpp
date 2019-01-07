/**
 * @file entanglement.cpp
 *
 * Demonstrates basic quantum entanglement.
 *
 * @author Sam Griffiths
 */

#include "Examples.h"

using namespace qlay;

void entanglement(unsigned repeats)
{
	init();

	int zeroes = 0, ones = 0;
	int matches = 0;

	for (unsigned i = 0; i < repeats; i++)
	{
		QubitSystem qs;
		Qubit q0(qs); Qubit q1(qs);
		H(q0);
		CNOT(q0, q1);

		Basis result = M(q0);
		result ? ones++ : zeroes++;

		if (M(q1) == result)
			matches++;
	}

	std::cout << "ZERO:  " << zeroes << std::endl << "ONE:   " << ones << std::endl;
	std::cout << "MATCH: " << matches << std::endl;
}
