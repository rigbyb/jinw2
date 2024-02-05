#pragma once

#include "../sdk/cusercmd.h"

namespace hooks {
	bool __fastcall createmove(void* ecx, void*, float input_sample_frametime, sdk::CUserCmd* usercmd);
}