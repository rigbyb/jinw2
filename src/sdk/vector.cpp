#include "vector.h"

sdk::vector::vector(float x, float y, float z)
	: x_(x), y_(y), z_(z) {
}

sdk::vector::vector()
	: vector(0.0f, 0.0f, 0.0f) {
}

sdk::vector& sdk::vector::operator/=(const float mul) {
	x_ /= mul;
	y_ /= mul;
	z_ /= mul;
	return *this;
}

std::pair<sdk::vector, float> sdk::vector::operator*(const sdk::VMatrix& mat) const {
	float x = x_ * mat[0][0] + y_ * mat[0][1] + z_ * mat[0][2] + mat[0][3];
	float y = x_ * mat[1][0] + y_ * mat[1][1] + z_ * mat[1][2] + mat[1][3];
	float z = x_ * mat[2][0] + y_ * mat[2][1] + z_ * mat[2][2] + mat[2][3];
	float w = x_ * mat[3][0] + y_ * mat[3][1] + z_ * mat[3][2] + mat[3][3];

	return std::make_pair(vector(x, y, z), w);
}