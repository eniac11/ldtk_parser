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

#include "ldtk_parser/world.h"

#include "ldtk_parser/level.h"

#include "parse_types.h"

namespace ldtk {
    Level const& World::get_level(uuid const& l_iid) const {
        return *levels.at(l_iid);
    }

    Level& World::get_level(uuid const& l_iid) {
        return *levels.at(l_iid);
    }
}