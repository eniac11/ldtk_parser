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

#pragma once

#include <ldtk_parser/world.h>
#include <ldtk_parser/defs/entity_def.h>
#include <ldtk_parser/defs/layer_def.h>
#include <ldtk_parser/defs/enum_def.h>


#include <cstdint>
#include <unordered_map>

#include <uuid.h>

namespace ldtk {
    struct Project {
        uuid iid;
        std::unordered_map<uuid, World> worlds;
        std::unordered_map<uint32_t, EnumDef> enum_defs;
        std::unordered_map<uint32_t, EntityDef> entity_defs;
        std::unordered_map<uint32_t, LayerDef> layer_defs;

        [[nodiscard]] World const& get_world(uuid const& w_iid) const;

        const EnumValue* get_enum_value(std::string const& enum_type, std::string const& enum_id) const;
        const EnumValue* get_enum_value(uint32_t enum_uid, std::string const& enum_id) const;
        bool get_enum(std::string const& enum_type, uint32_t* enum_uid, const EnumDef** enum_def) const;
        const EnumDef* get_enum(uint32_t enum_uid) const;

        EntityInstance const& get_entity_instance(EntityRef const& ref) const;


        World& get_world(uuid const& w_iid);
    };
}