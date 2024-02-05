#pragma once

#include <sol/sol.hpp>
#include "vector.h"
#include "../sdk/cbaseentity.h"

namespace lua {
	class entity {
	public:
		entity(sdk::CBaseEntity* ptr);

		std::optional<sdk::vector> get_abs_origin();

	private:
		sdk::CBaseEntity* ptr_;
	};

	void register_entity(sol::state& state);
}