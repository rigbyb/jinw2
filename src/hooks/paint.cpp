#include "paint.h"
#include "../hooks.h"
#include "../lua.h"
#include "../sdk/ienginevgui.h"
#include "../sdk/tier0.h"

void __fastcall hooks::paint(void* ecx, void*, int mode) {
    using func = void(__thiscall*)(void*, int);
    reinterpret_cast<func>(ivengineclient_hook->get_original(PAINT_HOOK_INDEX))(ecx, mode);

    if (!(mode & sdk::PaintMode_t::PAINT_UIPANELS))
        return;

    call_lua_hook("Paint");
}