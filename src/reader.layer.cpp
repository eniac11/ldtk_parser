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

#include "ldtk_parser/tile.h"
#include <json/json.h>
#include <vector>

using namespace ldtk;

std::vector<Tile> parse_layer_tiles(Json::Value const& json, bool autolayer=false) {
    std::vector<Tile> tiles;
    for (auto const& jtile : json) {
        glm::ivec2 px = {jtile["px"][0].asInt(),jtile["px"][1].asInt()};
        glm::ivec2 src ={jtile["src"][0].asInt(),jtile["src"][1].asInt()};
        uint8_t f = jtile["f"].asInt();
        uint32_t tileid = jtile["t"].asInt();
        std::optional<uint32_t> ruleid = std::nullopt;
        uint32_t coordid;
        if (autolayer) {
            coordid = jtile["d"][1].asInt();
            ruleid = jtile["d"][0].asInt();
        } else {
            coordid = jtile["d"][0].asInt();
        }
        float alpha = jtile["a"].asFloat();
        tiles.emplace_back(px, src, tileid, f, coordid, ruleid, alpha);
    }
    return tiles;
}