/**
 * @file Qlay.h
 *
 * Single header file include for the C++/CLI wrapper
 * of the Qlay C++ Quantum Playground API.
 *
 * @author Sam Griffiths
 */

#pragma once

namespace qlay
{
	class Qubit;

	namespace cli
	{
		public ref class Qubit
		{
		private:
			qlay::Qubit *impl_;

		public:
			Qubit();
			
			//Qubit(Basis b);

			//Qubit &operator=(Basis b);

			~Qubit();
			!Qubit();
		};
	}
}
