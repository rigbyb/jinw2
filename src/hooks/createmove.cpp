#include "createmove.h"
#include "../hooks.h"
#include "../lua.h"
#include "../sdk/tier0.h"

bool __fastcall hooks::createmove(void* ecx, void*, float input_sample_frametime, sdk::CUserCmd* usercmd) {
    using func = bool(__thiscall*)(void*, float, sdk::CUserCmd*);
    bool original_value = reinterpret_cast<func>(clientmodeshared_hook->get_original(CLIENTMODESHARED_HOOK_INDEX))(ecx, input_sample_frametime, usercmd);
    
    if (usercmd->command_number == 0)
        return original_value;

    call_lua_hook("Think", usercmd);

    return original_value;
}
