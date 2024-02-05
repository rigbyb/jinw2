#include "entities.h"
#include "entity.h"
#include "../sdk/icliententitylist.h"

static std::vector<lua::entity> get_all_entities() {
	std::vector<lua::entity> entities = {};

	// 1 is world
	for (int i = 1; i < g_icliententitylist->GetHighestEntityIndex(); i++) {
		sdk::CBaseEntity* ptr = g_icliententitylist->GetClientEntity(i);

		entities.push_back(lua::entity(ptr));
	}

	return entities;
}

static std::vector<lua::entity> get_entities_by_class(std::string target_name) {
	std::vector<lua::entity> entities = {};

	// 1 is world
	for (int i = 1; i < g_icliententitylist->GetHighestEntityIndex(); i++) {
		sdk::CBaseEntity* ptr = g_icliententitylist->GetClientEntity(i);
		
		if (!ptr)
			continue;
		
		const std::string class_name = ptr->GetClientClass()->m_pNetworkName;

		if (class_name != target_name)
			continue;

		entities.push_back(lua::entity(ptr));
	}

	return entities;
}

void lua::register_entities(sol::state& state) {
	sol::table entities = state.create_named_table("Entites",
		"GetAll",
		&get_all_entities,
		"FindByClass",
		&get_entities_by_class
	);

	state.set("ents", entities);
}