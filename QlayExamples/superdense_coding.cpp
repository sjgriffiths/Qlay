/**
 * @file superdense_coding.cpp
 *
 * Demonstrates superdense coding.
 *
 * @author Sam Griffiths
 */

#include "Examples.h"

using namespace qlay;

void superdense_coding()
{
	init();

	QubitSystem qs;
	Qubit qa(qs); Qubit qb(qs);

	//Prepare Alice's and Bob's qubits in Bell state |Φ+>
	H(qa);
	CNOT(qa, qb);

	//If sending 00, do nothing

	//If sending 01, transform into |Ψ+>
	//X(qa);

	//If sending 10, transform into |Φ->
	//Z(qa);

	//If sending 11, transform into |Ψ->
	//X(qa);
	//Z(qa);

	//Bob 'undoes' the entanglement
	CNOT(qa, qb);
	H(qa);

	std::cout << "Message received: " << M(qa) << M(qb) << std::endl;
}
