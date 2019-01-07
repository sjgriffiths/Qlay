/**
 * @file Examples.h
 *
 * Declares the example quantum logic circuitry experiments 
 * as standalone functions which repeat the given number of
 * times, defaulting to just 1.
 *
 * @author Sam Griffiths
 */

#pragma once

#include "../Qlay/Qlay.h"

#include <iostream>

const unsigned DEFAULT_REPEATS = 1;

//Demonstrates the Pauli X (NOT) gate
void PauliX(unsigned repeats = DEFAULT_REPEATS);

//Demonstrates the Hadamard gate
void Hadamard(unsigned repeats = DEFAULT_REPEATS);

//Demontrates basic quantum entanglement
void entanglement(unsigned repeats = DEFAULT_REPEATS);

//Performs the CHSH game experiment
void CHSH(unsigned repeats = DEFAULT_REPEATS);

//Performs the GHZ game experiment
void GHZ(unsigned repeats = DEFAULT_REPEATS);
