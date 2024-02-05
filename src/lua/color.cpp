#include "color.h"
#include "../sdk/color.h"

void lua::register_color(sol::state& state) {
	state.new_usertype<sdk::color>("Color",
		sol::factories(
			[](unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
				return sdk::color(r, g, b, a);
			}
		),
		
		"r",
		&sdk::color::r_,
		"g",
		&sdk::color::g_,
		"b",
		&sdk::color::b_,
		"a",
		&sdk::color::a_
	);
}