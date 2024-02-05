#pragma once

#include "vector.h"
#include "../vmt.h"

namespace sdk {
	class ClientClass {
	public:
		void* m_pCreateFn;
		void* m_pCreateEventFn;
		char* m_pNetworkName;
		void* m_pRecvTable;
		ClientClass* m_pNext;
		int m_ClassID;
	};

	class CBaseEntity {
	public:
		ClientClass* GetClientClass() {
			void* networkable = this + 0x8;
			using func = ClientClass* (__thiscall*)(void*);
			return reinterpret_cast<func>(vmt::get_method(networkable, 2))(networkable);
		}
		vector& GetAbsOrigin() {
			using func = vector& (__thiscall*)(void*);
			return reinterpret_cast<func>(vmt::get_method(this, 9))(this);
		}
	};
}