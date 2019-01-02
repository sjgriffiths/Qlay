/**
 * @file CHSH.cpp
 *
 * Performs the CHSH game experiment.
 *
 * @author Sam Griffiths
 */

#include "Examples.h"

using namespace qlay;

void CHSH(unsigned repeats)
{
	init();

	//Classical version
	int wins = 0;
	for (unsigned i = 0; i < repeats; i++)
	{
		bool r = chance(0.5); //Alice's question
		bool s = chance(0.5); //Bob's question

		bool a = false; //Alice's answer
		bool b = false; //Bob's answer

		if ((r & s) == (a ^ b))
			wins++;
	}

	std::cout << "Classical wins: " << wins << " (" << (double)wins / repeats * 100 << "%)" << std::endl;

	//Quantum version
	wins = 0;
	for (unsigned i = 0; i < repeats; i++)
	{
		bool r = chance(0.5); //Alice's question
		bool s = chance(0.5); //Bob's question

		bool a, b; //Alice's and Bob's answers

		QubitSystem qs;
		Qubit qa(qs);
		Qubit qb(qs);

		//Prepare equal superposition of |00> and |11>
		H(qa);
		CNOT(qa, qb);

		//Alice's answer
		if (r)
			a = Mx(qa);
		else
			a = M(qa);

		//Bob's answer
		if (s)
		{
			Ry(2.0 * PI / 8.0, qb);
			b = M(qb);
		}
		else
		{
			Ry(-2.0 * PI / 8.0, qb);
			b = M(qb);
		}

		if ((r && s) == (a ^ b))
			wins++;
	}

	std::cout << "Quantum wins: " << wins << " (" << (double)wins / repeats * 100 << "%)" << std::endl;
}
