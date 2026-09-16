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


#include <ldtk_parser/entity.h>
#include <ldtk_parser/tile.h>


#include <glm/vec2.hpp>

#include <unordered_map>
#include <string>
#include <optional>
#include <functional>

#include <uuid.h>

namespace ldtk {
    struct Project;
    struct Level;
    struct LayerDef;
    struct Tileset;

    struct LayerInstance {
        uuid iid;
        std::string name;
        LayerType type;

        glm::ivec2 computed_tilesize;
        uint32_t computed_grid_size;
        float computed_opacity;
        glm::ivec2 computed_pixel_offset;

        uint32_t level_uid;
        const Level* level;
        uint32_t layerdef_uid;
        const LayerDef* layerDef;
        std::optional<uint32_t> tilesetdef_uid;
        Tileset* tileset;


        glm::ivec2 layer_pixel_offset;

        std::vector<Tile> tiles;
        std::map<uint32_t, std::reference_wrapper<Tile>> coord_tile_mapping;

        std::unordered_map<uuid, EntityInstance> entities;

        [[nodiscard]] glm::ivec2 pixel_offset() const;

    };
}