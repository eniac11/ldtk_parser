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
#include <cstdint>
#include <string>
#include <vector>
#include <ldtk_parser/types.h>
#include <map>

namespace ldtk {
    struct Tileset;
    struct EnumDef;

    struct EnumValue {
        std::string id;
        TileRect tile_rect;
        uint32_t colour;

        uint32_t enum_type_uid;
        EnumDef* enum_type_def;
    };

    struct EnumInstance {
        uint32_t def_uid;
        const EnumDef* def;
        std::string value_id;
        const EnumValue* value;
    };

    struct EnumDef {
        std::string name;
        uint32_t uid;
        uint32_t icon_tileset_uid;
        Tileset* tileset;

        std::map<std::string, EnumValue> values;
    };
}