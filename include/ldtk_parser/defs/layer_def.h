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

#include <ldtk_parser/types.h>

#include <glm/vec2.hpp>


namespace ldtk {
    struct Project;


    struct LayerDef {
        LayerType type;
        std::string name;
        uint32_t uid;
        uint32_t grid_size;
        glm::ivec2 pixel_offset;
        glm::vec2 parallax_factor;
        bool parallax_scaling;
    };
}