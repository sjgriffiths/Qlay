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

	void init(unsigned seed)
	{
		rng.seed(seed);
	}

	bool chance(double p)
	{
		std::bernoulli_distribution dist(p);
		return dist(rng);
	}

	double deg_to_rad(double angle)
	{
		return angle * PI / 180.0;
	}

	Mat kronecker_product(const Mat &a, const Mat &b)
	{
		Mat k(a.rows() * b.rows(), a.cols() * b.cols());

		for (Eigen::Index i = 0; i < a.rows(); i++)
			for (Eigen::Index j = 0; j < a.cols(); j++)
				k.block(i * b.rows(), j * b.cols(), b.rows(), b.cols())
					= a(i, j) * b;

		return k;
	}
}
