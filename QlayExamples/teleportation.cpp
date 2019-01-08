/**
 * @file teleportation.cpp
 *
 * Demonstrates quantum teleportation.
 *
 * @author Sam Griffiths
 */

#include "Examples.h"

using namespace qlay;

void teleportation(unsigned repeats)
{
	init();

	int zeroes = 0, ones = 0;

	for (unsigned i = 0; i < repeats; i++)
	{
		QubitSystem qs;
		Qubit qa(qs); Qubit qb(qs); Qubit qc(qs);

		//Prepare Alice's qubit with 75% chance of |1>, to teleport
		//r|0>+s|1> (where |s|^2 = 0.75)
		Ry(2 * asin(sqrt(0.75)), qc);

		//Give Alice and Bob the Bell state |Φ+>
		H(qa);
		CNOT(qa, qb);

		//Alice entangles her data qubit with her Bell qubit
		CNOT(qc, qa);

		//Alice measures her data qubit in the sign basis
		Basis correct_phase = Mx(qc);

		//Alice measures her Bell qubit in the computational basis
		Basis correct_flip = M(qa);


		//Bob's qubit may have flipped, so correct if necessary
		if (correct_flip) X(qb);

		//Bob's qubit is now either r|0>+s|1> or r|0>-s|1>
		//In the latter case, correct the phase
		if (correct_phase) Z(qb);

		M(qb) ? ones++ : zeroes++;
		


		/* OLD NON-REMOTE CNOT */
		////Prepare Alice's qubit with 75% chance of |1>, to teleport
		////r|0>+s|1> (where |s|^2 = 0.75)
		//Ry(2 * asin(sqrt(0.75)), qc);

		////Copies Alice's measurement to Bob...
		////Doesn't give Bob the state r|0>+s|1> !
		//CNOT(qc, qb);

		////Alice measures in the sign basis, as either |+> or |->
		//Basis result = Mx(qc);

		////Bob's qubit is now either r|0>+s|1> or r|0>-s|1>
		////In the latter case, correct the phase
		//if (result) Z(qb);

		//M(qb) ? ones++ : zeroes++;
	}

	std::cout << "ZERO:  " << zeroes << std::endl << "ONE:   " << ones << std::endl;
}
