#include "vector.h"
#include <unordered_map>
#include "../sdk/vmatrix.h"
#include "../sdk/ivengineclient.h"
#include "../sdk/vector.h"

static std::unordered_map<std::string, int> to_screen(const sdk::vector& vec) {
	const sdk::VMatrix& matrix = g_ivengineclient->WorldToScreenMatrix();

	auto [screen, w] = vec * matrix;

	std::unordered_map<std::string, int> map = {};

	if (w > 0.0f) {
		screen /= w;
		screen.z_ = 0.0f;

		int screen_width, screen_height;
		g_ivengineclient->GetScreenSize(screen_width, screen_height);

		screen.x_ = 0.5f * screen_width * (1.0f + screen.x_);
		screen.y_ = 0.5f * screen_height * (1.0f - screen.y_);
	
		map["visible"] = 1;
	}
	else {
		map["visible"] = 0;
	}

	map["x"] = static_cast<int>(screen.x_);
	map["y"] = static_cast<int>(screen.y_);

	return map;
}

void lua::register_vector(sol::state& state) {
	state.new_usertype<sdk::vector>("Vector",
		sol::factories(
			[](float x, float y, float z) {
				return sdk::vector(x, y, z);
			}
		),

		"ToScreen",
		&to_screen,

		"x",
		&sdk::vector::x_,
		"y",
		&sdk::vector::y_,
		"z",
		&sdk::vector::z_
	);
}