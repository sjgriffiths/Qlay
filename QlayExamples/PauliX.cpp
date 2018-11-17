/**
 * @file PauliX.cpp
 *
 * Demonstrates the Pauli X (NOT) gate.
 *
 * @author Sam Griffiths
 */

#include "Examples.h"

using namespace qlay;

void PauliX(unsigned repeats)
{
	init();
	
	int zeroes = 0, ones = 0;

	for (unsigned i = 0; i < repeats; i++)
	{
		QubitSystem qs;
		Qubit q(qs);
		X(q);

		M(q) ? ones++ : zeroes++;
	}

	std::cout << "ZERO: " << zeroes << std::endl << "ONE:  " << ones << std::endl;
}
