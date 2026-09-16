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
#include <ldtk_parser/defs/field_def.h>


#include <glm/vec2.hpp>


#include <cstdint>
#include <optional>
#include <map>
#include <beman/indirect/polymorphic.hpp>

namespace ldtk {
    struct Project;

    struct EntityDef {
        friend struct Project;
        std::string name;
        uint32_t uid;
        glm::ivec2 tilesize;
        bool resizeable_x;
        std::optional<uint32_t> min_width;
        std::optional<uint32_t> max_width;

        bool resizeable_y;
        std::optional<uint32_t> min_height;
        std::optional<uint32_t> max_height;

        bool keep_aspect_ratio;
        float tile_opacity;
        float fill_opacity;
        float line_opacity;
        bool hollow;

        RenderMode render_mode;
        TileRenderMode tile_render_mode;
        TileRect tile_rect;

        // TODO: nineSliceBorders

        uint32_t max_count;
        LimitScope limit_scope;
        LimitBehaviour limit_behaviour;

        std::map<uint32_t, beman::indirect::polymorphic<IFieldDef>> field_defs;

        template <FieldType T>
        getFieldType<T> const& get_field(uint32_t uid_) {
            auto const& it = field_defs.find(uid_);
            if (it == field_defs.end()) {
                throw std::out_of_range(std::format("Could not find field: '{}'", uid_));
            }
            auto const& base_field = it->second;

            assert(T == base_field->type);
            auto const& field = dynamic_cast<getFieldType<T> const&>(*base_field);
            return field;
        }

        template <FieldType T>
        getFieldType<T> const& get_field(uint32_t uid_) const {
            auto const& it = field_defs.find(uid_);
            if (it == field_defs.end()) {
                throw std::out_of_range(std::format("Could not find field: '{}'", uid_));
            }
            auto const& base_field = it->second;

            assert(T == base_field->type);
            auto const& field = dynamic_cast<getFieldType<T> const&>(*base_field);
            return field;
        }
    };
}