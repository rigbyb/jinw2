#pragma once

namespace sdk {
	using Msg = void(__cdecl*)(const char* message, ...);
	using Warning = void(__cdecl*)(const char* message, ...);
}

extern sdk::Msg g_msg;
extern sdk::Warning g_warning;