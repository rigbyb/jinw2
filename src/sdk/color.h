#pragma once

#include "vmatrix.h"

namespace sdk {
	class color {
	public:
		color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		color();

		unsigned char r_, g_, b_, a_;
	};
}