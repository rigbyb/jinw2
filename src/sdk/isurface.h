#pragma once

#include "../vmt.h"

namespace sdk {
    enum EFontFlags {
        FONTFLAG_NONE,
        FONTFLAG_ITALIC = 0x001,
        FONTFLAG_UNDERLINE = 0x002,
        FONTFLAG_STRIKEOUT = 0x004,
        FONTFLAG_SYMBOL = 0x008,
        FONTFLAG_ANTIALIAS = 0x010,
        FONTFLAG_GAUSSIANBLUR = 0x020,
        FONTFLAG_ROTARY = 0x040,
        FONTFLAG_DROPSHADOW = 0x080,
        FONTFLAG_ADDITIVE = 0x100,
        FONTFLAG_OUTLINE = 0x200,
        FONTFLAG_CUSTOM = 0x400
    };

    enum FontDrawType_t {
        FONT_DRAW_DEFAULT = 0,
        FONT_DRAW_NONADDITIVE,
        FONT_DRAW_ADDITIVE,
        FONT_DRAW_TYPE_COUNT = 2,
    };

    class ISurface {
    public:
        void DrawSetColor(int r, int g, int b, int a) {
            using func = void(__thiscall*)(void*, int, int, int, int);
            reinterpret_cast<func>(vmt::get_method(this, 11))(this, r, g, b, a);
        }
        void DrawFilledRect(int x1, int y1, int x2, int y2) {
            using func = void(__thiscall*)(void*, int, int, int, int);
            reinterpret_cast<func>(vmt::get_method(this, 12))(this, x1, y1, x2, y2);
        }
        void DrawSetTextFont(unsigned long font) {
            using func = void(__thiscall*)(void*, unsigned long);
            reinterpret_cast<func>(vmt::get_method(this, 17))(this, font);
        }
        void DrawSetTextColor(int r, int g, int b, int a) {
            using func = void(__thiscall*)(void*, int, int, int, int);
            reinterpret_cast<func>(vmt::get_method(this, 19))(this, r, g, b, a);
        }
        void DrawSetTextPos(int x, int y) {
            using func = void(__thiscall*)(void*, int, int);
            reinterpret_cast<func>(vmt::get_method(this, 20))(this, x, y);
        }
        void DrawPrintText(const wchar_t* text, int length, FontDrawType_t type = FontDrawType_t::FONT_DRAW_DEFAULT) {
            using func = void(__thiscall*)(void*, const wchar_t*, int, FontDrawType_t);
            reinterpret_cast<func>(vmt::get_method(this, 22))(this, text, length, type);
        }
        unsigned long CreateFont() {
            using func = unsigned long(__thiscall*)(void*);
            return reinterpret_cast<func>(vmt::get_method(this, 66))(this);
        }
        void SetFontGlyphSet(unsigned long& id, const char* font_name, int tall, int weight, int blur, int scanlines, int flags) {
            using func = void(__thiscall*)(void*, unsigned long, const char*, int, int, int, int, int, int, int);
            reinterpret_cast<func>(vmt::get_method(this, 67))(this, id, font_name, tall, weight, blur, scanlines, flags, 0, 0);
        }
    };
}

extern sdk::ISurface* g_isurface;
