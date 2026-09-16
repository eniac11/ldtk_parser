
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
#include <ldtk_parser/level.h>

#include <ldtk_parser/types.h>

#include <uuid.h>

#include <glm/vec2.hpp>


#include <unordered_map>
#include <memory>

namespace ldtk {
    struct Project;
    struct Level;

    struct World {
        friend struct Project;
        uuid iid;
        std::string name;
        glm::ivec2 grid;
        WorldLayout layout;

        std::unordered_map<uuid, std::unique_ptr<Level>> levels;

        [[nodiscard]] Level const& get_level(uuid const& l_iid) const;

        Level& get_level(uuid const& l_iid);

    };
}