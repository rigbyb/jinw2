#pragma once

#include "cbaseentity.h"

namespace sdk {
	class IClientEntityList {
	public:
		CBaseEntity* GetClientEntity(int id) {
			using func = CBaseEntity*(__thiscall*)(void*, int);
			return reinterpret_cast<func>(vmt::get_method(this, 3))(this, id);
		}
		int GetHighestEntityIndex() {
			using func = int(__thiscall*)(void*);
			return reinterpret_cast<func>(vmt::get_method(this, 6))(this);
		}
	};
}

extern sdk::IClientEntityList* g_icliententitylist;
