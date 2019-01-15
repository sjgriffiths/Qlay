/**
 * @file Qlay.cpp
 *
 * Wraps the Qlay API into C++/CLI for use in managed
 * code environments, such as C#.
 *
 * @author Sam Griffiths
 */

#include "../Qlay/Qlay.h"

namespace qlay
{
	namespace cli
	{
		//Contains core library functions
		public ref class Core abstract sealed
		{
		public:
			//Pi constant
			static double PI() { return qlay::PI; }

			//1/sqrt(2) constant
			static double INV_ROOT_2() { return qlay::INV_ROOT_2; }


			//Initialises the system with a seed based on current time
			static void init() { qlay::init(); }

			//Initialises the system with the given seed
			static void init(unsigned seed) { qlay::init(seed); }

			//Returns true with the given probability (Bernoulli distribution)
			static bool chance(double p) { return qlay::chance(p); }

			//Converts the given angle from degrees to radians
			static double deg_to_rad(double angle) { return qlay::deg_to_rad(angle); }
		};


		//Represents a system of potentially entangled qubits
		public ref class QubitSystem
		{
		internal:
			qlay::QubitSystem *impl_;

		public:
			QubitSystem() : impl_(new qlay::QubitSystem())
			{
			}

			~QubitSystem()
			{
				this->!QubitSystem();
			}

			!QubitSystem()
			{
				if (impl_)
				{
					delete impl_;
					impl_ = nullptr;
				}
			}

			int count() { return impl_->count(); }
			void reset() { impl_->reset(); }
		};
		
		//Represents a qubit (quantum bit), a linear combination of |0> and |1>
		public ref class Qubit
		{
		internal:
			qlay::Qubit *impl_;

		public:
			Qubit(QubitSystem ^system) : impl_(new qlay::Qubit(*(system->impl_)))
			{
			}

			Qubit(QubitSystem ^system, int index) : impl_(new qlay::Qubit(*(system->impl_), index))
			{
			}

			~Qubit()
			{
				this->!Qubit();
			}
			
			!Qubit()
			{
				if (impl_)
				{
					delete impl_;
					impl_ = nullptr;
				}
			}

			int index() { return impl_->index(); }
		};


		//Contains all quantum logic gates
		public ref class Gates abstract sealed
		{
		public:
			static bool M(Qubit ^q)
			{
				return qlay::M(*(q->impl_));
			}

			static bool Mx(Qubit ^q)
			{
				return qlay::Mx(*(q->impl_));
			}

			static void X(Qubit ^q)
			{
				qlay::X(*(q->impl_));
			}

			static void Y(Qubit ^q)
			{
				qlay::Y(*(q->impl_));
			}

			static void Z(Qubit ^q)
			{
				qlay::Z(*(q->impl_));
			}

			static void H(Qubit ^q)
			{
				qlay::H(*(q->impl_));
			}

			static void SRNOT(Qubit ^q)
			{
				qlay::SRNOT(*(q->impl_));
			}

			static void Rx(double angle, Qubit ^q)
			{
				qlay::Rx(angle, *(q->impl_));
			}

			static void Ry(double angle, Qubit ^q)
			{
				qlay::Ry(angle, *(q->impl_));
			}

			static void Rz(double angle, Qubit ^q)
			{
				qlay::Rz(angle, *(q->impl_));
			}

			static void Rp(double angle, Qubit ^q)
			{
				qlay::Rp(angle, *(q->impl_));
			}

			static void SWAP(Qubit ^a, Qubit ^b)
			{
				qlay::SWAP(*(a->impl_), *(b->impl_));
			}

			static void SRSWAP(Qubit ^a, Qubit ^b)
			{
				qlay::SRSWAP(*(a->impl_), *(b->impl_));
			}

			static void CNOT(Qubit ^control, Qubit ^target)
			{
				qlay::CNOT(*(control->impl_), *(target->impl_));
			}
		};
	}
}
