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


		//Represents a qubit (quantum bit), a linear combination of |0> and |1>
		public ref class Qubit
		{
		internal:
			qlay::Qubit *impl_;

		public:
			Qubit() : impl_(new qlay::Qubit())
			{
			}

			Qubit(bool b) : impl_(new qlay::Qubit(b))
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
		};


		//Contains all quantum logic gates
		public ref class Gates abstract sealed
		{
		public:
			static bool M(Qubit ^q)
			{
				qlay::Qubit *qq = q->impl_;
				return qlay::M(*qq);
			}

			static bool Mx(Qubit ^q)
			{
				qlay::Qubit *qq = q->impl_;
				return qlay::Mx(*qq);
			}

			static void X(Qubit ^q)
			{
				qlay::Qubit *qq = q->impl_;
				return qlay::X(*qq);
			}

			static void Y(Qubit ^q)
			{
				qlay::Qubit *qq = q->impl_;
				return qlay::Y(*qq);
			}

			static void Z(Qubit ^q)
			{
				qlay::Qubit *qq = q->impl_;
				return qlay::Z(*qq);
			}

			static void H(Qubit ^q)
			{
				qlay::Qubit *qq = q->impl_;
				return qlay::H(*qq);
			}

			static void SRNOT(Qubit ^q)
			{
				qlay::Qubit *qq = q->impl_;
				return qlay::SRNOT(*qq);
			}

			static void Rx(double angle, Qubit ^q)
			{
				qlay::Qubit *qq = q->impl_;
				return qlay::Rx(angle, *qq);
			}

			static void Ry(double angle, Qubit ^q)
			{
				qlay::Qubit *qq = q->impl_;
				return qlay::Ry(angle, *qq);
			}

			static void Rz(double angle, Qubit ^q)
			{
				qlay::Qubit *qq = q->impl_;
				return qlay::Rz(angle, *qq);
			}

			static void Rp(double angle, Qubit ^q)
			{
				qlay::Qubit *qq = q->impl_;
				return qlay::Rp(angle, *qq);
			}
		};
	}
}
