// JSON parser for ldtk project files
// Copyright (C) 2026. Hadley Epstein
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "ldtk_parser/project.h"

#include "ldtk_parser/world.h"
#include "ldtk_parser/defs/entity_def.h"
#include "ldtk_parser/defs/layer_def.h"
#include "ldtk_parser/level.h"

namespace ldtk {
    World const& Project::get_world(uuid const& w_iid) const {
        return worlds.at(w_iid);
    }

    const EnumValue* Project::get_enum_value(std::string const& enum_type, std::string const& enum_id) const {
        for (auto const& edef : enum_defs) {
            if (edef.second.name == enum_type) {
                auto vIt = edef.second.values.find(enum_id);
                if (vIt == edef.second.values.end()) {
                    return nullptr;
                }
                return &vIt->second;
            }
        }
        return nullptr;
    }

    const EnumValue* Project::get_enum_value(uint32_t enum_uid, std::string const& enum_id) const {
        auto const it = enum_defs.find(enum_uid);
        if (it == enum_defs.end()) {
            return nullptr;
        }
        auto const vIt = it->second.values.find(enum_id);
        if (vIt == it->second.values.end()) {
            return nullptr;
        }

        return &vIt->second;
    }

    bool Project::get_enum(std::string const& enum_type, uint32_t* enum_uid, const EnumDef** enum_def) const {
        for (auto const& edef : enum_defs) {
            if (edef.second.name == enum_type) {
                if (enum_uid) {
                    *enum_uid = edef.first;
                }
                if (enum_def) {
                    *enum_def = &enum_defs.at(edef.first);
                }
                return true;
            }
        }
        return false;
    }

    const EnumDef* Project::get_enum(uint32_t enum_uid) const {
        auto const it = enum_defs.find(enum_uid);
        if (it == enum_defs.end()) {
            return nullptr;
        }
        return &it->second;
    }

    EntityInstance const& Project::get_entity_instance(EntityRef const& ref) const {
        return worlds.at(ref.world_iid).get_level(ref.level_iid).layers.at(ref.layer_iid).entities.at(ref.entity_iid);
    }

    World& Project::get_world(uuid const& w_iid) {
        return worlds.at(w_iid);
    }
}