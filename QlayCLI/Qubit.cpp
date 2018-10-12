/**
 * @file Qubit.cpp
 *
 * Implements the Qubit class for the
 * C++/CLI wrapper.
 *
 * @author Sam Griffiths
 */

#include "Qlay.h"
#include "../Qlay/Qlay.h"

qlay::cli::Qubit::Qubit() : impl_(new qlay::Qubit())
{
}

qlay::cli::Qubit::~Qubit()
{
	this->!Qubit();
}

qlay::cli::Qubit::!Qubit()
{
	if (impl_)
	{
		delete impl_;
		impl_ = nullptr;
	}
}
