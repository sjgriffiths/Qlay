/**
 * @file Deutsch-Jozsa.cpp
 *
 * Implements and demonstrates the Deutsch-Jozsa algorithm.
 *
 * @author Sam Griffiths
 */

#include "Examples.h"

using namespace qlay;



void DeutschJozsa_output()
{
	init();

	QubitSystem qs;
	Qubit x0(qs), x1(qs), x2(qs), y(qs);

	//Create uniform superposition of function domain
	H(x0);
	H(x1);
	H(x2);

	//Prepare final result qubit
	X(y);
	H(y);

	//Output Oracle: Number is odd (least significant digit is 1)
	//Maps |x>|y> to |x>|y XOR f(x)>
	CNOT(x0, y);

	//Take out of superposition
	H(x0);
	H(x1);
	H(x2);

	//Constant if all M(x) = 0, balanced otherwise
	bool constant = !M(x0) && !M(x1) && !M(x2);
	std::cout << (constant ? "CONSTANT" : "BALANCED") << std::endl;
}

void DeutschJozsa_phase()
{
	init();

	QubitSystem qs;
	Qubit x0(qs), x1(qs), x2(qs);

	//Create uniform superposition of function domain
	H(x0);
	H(x1);
	H(x2);

	//Phase Oracle: Number is odd (least significant digit is 1)
	//Flips phase on states matching predicate
	Z(x0);

	//Take out of superposition
	H(x0);
	H(x1);
	H(x2);
	
	//Constant if all M(x) = 0, balanced otherwise
	bool constant = !M(x0) && !M(x1) && !M(x2);
	std::cout << (constant ? "CONSTANT" : "BALANCED") << std::endl;
}
