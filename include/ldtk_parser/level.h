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
#include <ldtk_parser/layer.h>

#include <cstdint>
#include <unordered_map>
#include <string>

#include <uuid.h>

namespace ldtk {
    struct Project;
    struct World;

    struct Level {
        friend struct Project;
        friend struct World;
        uuid iid;
        uint32_t uid;
        std::string name;

        /// World depth
        /// @code
        /// default: 0
        /// > 0: above
        /// < 0: below
        /// @endcode
        std::int32_t depth;

        std::unordered_map<uuid, LayerInstance> layers;
    };
}