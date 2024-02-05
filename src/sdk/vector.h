#pragma once

#include <utility>
#include "vmatrix.h"

namespace sdk {
	class vector {
	public:
		vector(float x, float y, float z);
		vector();

		vector& operator/=(const float mul);
		std::pair<vector, float> operator*(const sdk::VMatrix& mat) const;

		float x_, y_, z_;
	};
}