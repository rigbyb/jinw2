#include "draw.h"
#include "../sdk/isurface.h"
#include "../sdk/color.h"

static unsigned long create_font(const std::string& name, unsigned int size) {
	unsigned long id = g_isurface->CreateFont();

	g_isurface->SetFontGlyphSet(
		id,
		name.c_str(),
		size,
		500,								// Weight
		0,									// Blur
		0,									// Scanlines
		sdk::EFontFlags::FONTFLAG_OUTLINE	// Flags
	);

	return id;
}

static void draw_filled_rect(int x1, int y1, int x2, int y2, sdk::color col) {
	g_isurface->DrawSetColor(col.r_, col.g_, col.b_, col.a_);
	g_isurface->DrawFilledRect(x1, y1, x2, y2);
}

static void draw_text(const std::string& text, unsigned long font_id, unsigned int x, unsigned int y, sdk::color col) {
	const std::wstring wide_text = std::wstring(text.begin(), text.end());
	g_isurface->DrawSetTextPos(x, y);
	g_isurface->DrawSetTextFont(font_id);
	g_isurface->DrawSetTextColor(col.r_, col.g_, col.b_, col.a_);
	g_isurface->DrawPrintText(wide_text.c_str(), wide_text.length());
}

void lua::register_draw(sol::state& state) {
	sol::table draw = state.create_named_table("draw",
		"CreateFont", &create_font,
		"FilledRect", &draw_filled_rect,
		"Text", &draw_text
	);

	state.set("draw", draw);
}