#include "color.h"

sdk::color::color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	: r_(r), g_(g), b_(b), a_(a) {
}

sdk::color::color()
	: color(0, 0, 0, 255) {
}