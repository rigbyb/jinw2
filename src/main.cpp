#include <Windows.h>
#include <string>
#include <stdexcept>
#include <filesystem>

#include "hooks.h"
#include "lua.h"

#include "sdk/chlclient.h"
#include "sdk/isurface.h"
#include "sdk/ienginevgui.h"
#include "sdk/ivengineclient.h"
#include "sdk/icliententitylist.h"
#include "sdk/tier0.h"
#include "sdk/clientmodeshared.h"

#include "lua/color.h"
#include "lua/draw.h"
#include "lua/entities.h"
#include "lua/entity.h"
#include "lua/globals.h"
#include "lua/vector.h"

#include "hooks/paint.h"
#include "hooks/createmove.h"

// These globals are defined to allow access through hooks
sdk::CHLClient* g_chlclient;
sdk::ISurface* g_isurface;
sdk::IEngineVGui* g_ienginevgui;
sdk::IVEngineClient* g_ivengineclient;
sdk::IClientEntityList* g_icliententitylist;
sdk::Msg g_msg;
sdk::Warning g_warning;
sdk::ClientModeShared* g_clientmodeshared;

// hooks.h definitions
vmt::vmthook* ivengineclient_hook;
vmt::vmthook* clientmodeshared_hook;

// lua.h definitions
std::vector<sol::state> g_lua_instances;

// From Source SDK
using CreateInterface = void*(*)(const char* pName, int* pReturnCode);

static void* get_public_export(const std::string& module, const std::string& name) {
	// Sorry wide chars
	const HMODULE handle = GetModuleHandleA(module.c_str());

	if (!handle)
		throw std::runtime_error("Couldn't find module " + module);

	void* func = GetProcAddress(handle, name.c_str());

	if (!func)
		throw std::runtime_error("Couldn't get export " + name + " address from module " + module);

	return func;
}

static void* create_interface(const std::string& module, const std::string& version) {
	const CreateInterface func = reinterpret_cast<CreateInterface>(get_public_export(module, "CreateInterface"));

	void* interf = func(version.c_str(), NULL);

	if (!interf)
		throw std::runtime_error("Couldn't get interface to \"" + version + "\"");

	return interf;
}

static sdk::ClientModeShared* get_clientmodeshared(sdk::CHLClient* client) {
	/* 
		Global ClientModeShared is referenced in HudProccessInput (virtual method 10)
		mov rcx, cs:g_pClientMode
	*/
	void** table = vmt::get_vtable(client);
	uintptr_t hudprocessinput_ptr = reinterpret_cast<uintptr_t>(table[10]);
	return **reinterpret_cast<sdk::ClientModeShared***>(hudprocessinput_ptr + 0x5);
}

static std::optional<std::filesystem::path> get_appdata_path() {
	char* buffer = nullptr;
	size_t size = 0;

	if (_dupenv_s(&buffer, &size, "APPDATA") || buffer == nullptr)
		return std::nullopt;
		
	std::filesystem::path path(buffer);
	
	free(buffer);

	return path;
}

static DWORD WINAPI init_thread(LPVOID lpParam) {
	// This should always be true
	const HMODULE module = reinterpret_cast<HMODULE>(lpParam);

	// Will force a catch, even though it cannot be dealt with, to avoid exiting the game process
	try {
		// As this is a one-time call, regrabbing a CreateInterface ptr is not the end of the world
		g_chlclient = reinterpret_cast<sdk::CHLClient*>(create_interface("client.dll", "VClient017"));
		g_isurface = reinterpret_cast<sdk::ISurface*>(create_interface("vguimatsurface.dll", "VGUI_Surface030"));
		g_ienginevgui = reinterpret_cast<sdk::IEngineVGui*>(create_interface("engine.dll", "VEngineVGui002"));
		g_ivengineclient = reinterpret_cast<sdk::IVEngineClient*>(create_interface("engine.dll", "VEngineClient013"));
		g_icliententitylist = reinterpret_cast<sdk::IClientEntityList*>(create_interface("client.dll", "VClientEntityList003"));
		g_msg = reinterpret_cast<sdk::Msg>(get_public_export("tier0.dll", "Msg"));
		g_warning = reinterpret_cast<sdk::Warning>(get_public_export("tier0.dll", "Warning"));
		g_clientmodeshared = get_clientmodeshared(g_chlclient);

		ivengineclient_hook = new vmt::vmthook(g_ienginevgui);
		ivengineclient_hook->set_method(PAINT_HOOK_INDEX, hooks::paint);
		clientmodeshared_hook = new vmt::vmthook(g_clientmodeshared);
		clientmodeshared_hook->set_method(CLIENTMODESHARED_HOOK_INDEX, hooks::createmove);

		std::optional<std::filesystem::path> maybe_appdata = get_appdata_path();

		if (!maybe_appdata)
			throw std::runtime_error("Error locating AppData path");

		const std::filesystem::path path = *maybe_appdata / "jinw2";

		if (std::filesystem::is_directory(path)) {
			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
				sol::state state;

				state.open_libraries(
					sol::lib::base,
					sol::lib::math,
					sol::lib::string
				);

				lua::register_globals(state);
				lua::register_vector(state);
				lua::register_color(state);
				lua::register_draw(state);
				lua::register_entity(state);
				lua::register_entities(state);

				sol::protected_function_result result = state.safe_script_file(
					entry.path().string(),
					sol::script_pass_on_error
				);

				if (!result.valid()) {
					sol::error error = result;
					std::string msg = std::string(error.what()) + '\n';
					g_warning(msg.c_str());
				}

				g_lua_instances.push_back(std::move(state));
			}
		}

		g_msg("[jinw2] Initialized\n");
	}
	catch (const std::exception& ex) {
		// Assuming messages are ASCII for now
		MessageBoxA(NULL, ex.what(), "jinw2", MB_ICONERROR);

		FreeLibraryAndExitThread(module, 0);
	}

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(
			NULL,                   // Security attributes
			0,                      // Stack size  
			init_thread,
			hinstDLL,               // Args to pass
			0,                      // Creation flags 
			NULL
		);

		break;
	case DLL_PROCESS_DETACH:
		// Only cleanup hooks if unloaded by self, and not by process exit
		// Should be NULL if called via FreeLibrary
		if (!lpvReserved) {
			delete ivengineclient_hook;
			delete clientmodeshared_hook;
		}

		break;
	default:
		break;
	}

	return TRUE;
}