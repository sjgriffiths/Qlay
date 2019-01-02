/**
 * @file GHZ.cpp
 *
 * Performs the GHZ game experiment.
 *
 * @author Sam Griffiths
 */

#include "Examples.h"

using namespace qlay;

void GHZ(unsigned repeats)
{
	init();

	bool r, s, t; //Alice's, Bob's and Charlie's questions

	//Set rst from the set {000, 011, 101, 110}
	auto generate_questions = [&]()
	{
		r = chance(0.5);
		s = chance(0.5);
		t = r ^ s;
	};

	//Classical version
	int wins = 0;
	for (unsigned i = 0; i < repeats; i++)
	{
		generate_questions();

		bool a = true; //Alice's answer
		bool b = true; //Bob's answer
		bool c = true; //Charlie's answer

		if ((r || s || t) == (a ^ b ^ c))
			wins++;
	}

	std::cout << "Classical wins: " << wins << " (" << (double)wins / repeats * 100 << "%)" << std::endl;

	//Quantum version
	wins = 0;
	for (unsigned i = 0; i < repeats; i++)
	{
		generate_questions();

		QubitSystem qs;
		Qubit qa(qs);
		Qubit qb(qs);
		Qubit qc(qs);

		//Prepare state 0.5|000>-0.5|011>-0.5|101>-0.5|110>
		H(qc);
		H(qb);
		CNOT(qc, qa);
		CNOT(qb, qa);
		Rp(PI/2, qa);
		Rp(PI/2, qb);
		Rp(PI/2, qc);

		//Alice's answer
		if (r) H(qa);
		bool a = M(qa);

		//Bob's answer
		if (s) H(qb);
		bool b = M(qb);

		//Charlie's answer
		if (t) H(qc);
		bool c = M(qc);

		if ((r || s || t) == (a ^ b ^ c))
			wins++;
	}

	std::cout << "Quantum wins: " << wins << " (" << (double)wins / repeats * 100 << "%)" << std::endl;
}
