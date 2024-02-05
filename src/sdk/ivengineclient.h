#pragma once

#include "vmatrix.h"
#include "../vmt.h"

namespace sdk {
    class IVEngineClient {
    public:
        void GetScreenSize(int& width, int& height) {
            using func = void(__thiscall*)(void*, int&, int&);
            reinterpret_cast<func>(vmt::get_method(this, 5))(this, width, height);
        }
        const VMatrix& WorldToScreenMatrix() {
            using func = const VMatrix&(__thiscall*)(void*);
            return reinterpret_cast<func>(vmt::get_method(this, 36))(this);
        }
    };
}

extern sdk::IVEngineClient* g_ivengineclient;
