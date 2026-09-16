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
#include <bitset>
#include <optional>
#include <glm/vec2.hpp>

namespace ldtk {
    struct Tile {
        glm::ivec2 layer_pixel_coord;
        glm::ivec2 tileset_coord;
        uint32_t tileid;
        uint8_t flip_bits : 2;
        uint32_t coordid;
        std::optional<uint32_t> ruleid;
        float alpha;

        bool flip_x() const {
            return std::bitset<2>(flip_bits).test(0);
        }
        bool flip_y() const {
            return std::bitset<2>(flip_bits).test(1);
        }
    };
}