#include "globals.h"
#include "../sdk/tier0.h"
#include "../sdk/cusercmd.h"

static void msg(const std::string& msg) {
	g_msg(msg.c_str());
}

static void warning(const std::string& msg) {
	g_warning(msg.c_str());
}

void lua::register_globals(sol::state& state) {
	state.set_function("Msg", &msg);
	state.set_function("Warning", &warning);

	state.new_usertype<sdk::CUserCmd>("Usercmd",
		"buttons",
		&sdk::CUserCmd::buttons
	);
}	