#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include "sdk/tier0.h"

extern std::vector<sol::state> g_lua_instances;

template <typename... Args>
void call_lua_hook(const std::string& name, Args&&... args) {
    for (sol::state& state : g_lua_instances) {
        std::optional<sol::protected_function> maybe_function = state.get_or<std::optional<sol::protected_function>>(name, std::nullopt);

        if (maybe_function) {
            sol::protected_function_result result = (*maybe_function)(args...);

            if (!result.valid()) {
                sol::error error = result;
                std::string msg = std::string(error.what()) + '\n';
                g_warning(msg.c_str());
            }
        }
    }
}