/**
 * @file Core.cpp
 *
 * Implements basic and global library functionality.
 *
 * @author Sam Griffiths
 */

#include "Core.h"

namespace qlay
{
	std::default_random_engine rng;

	void init()
	{
		rng.seed(static_cast<unsigned>(
			std::chrono::high_resolution_clock::now().time_since_epoch().count()
			));
	}
}
