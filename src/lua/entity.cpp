#include "entity.h"

void lua::register_entity(sol::state& state) {
	state.new_usertype<entity>("Entity",
		"GetAbsOrigin",
		&entity::get_abs_origin
	);
}

lua::entity::entity(sdk::CBaseEntity* ptr)
	: ptr_(ptr) {
}

std::optional<sdk::vector> lua::entity::get_abs_origin() {
	sdk::CBaseEntity* ent = reinterpret_cast<sdk::CBaseEntity*>(ptr_);
	
	if (!ent)
		return sol::nullopt;

	return ent->GetAbsOrigin();
}
