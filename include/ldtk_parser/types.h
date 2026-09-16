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
#include <glm/vec4.hpp>
#include <stdexcept>
#include <uuid.h>

namespace ldtk {
    class ldtk_decode_exception : public std::runtime_error {
        public:
            explicit ldtk_decode_exception(const std::string& __arg)
                : runtime_error(__arg) {
            }

            explicit ldtk_decode_exception(const char* string)
                : runtime_error(string) {
            }

    };

    using Rect = glm::ivec4;


    struct TileRect {
        uint32_t tilesetUid;
        Rect rect;
    };

    enum struct RenderMode {
        Tile,
        Ellipse,
        Rectangle,
        Cross
    };



    enum struct TileRenderMode {
        Cover,
        FitInside,
        Repeat,
        Stretch,
        FullSizedCropped,
        FullSizedUncropped,
        NineSlice
    };



    enum struct LimitScope {
        PerLayer,
        PerLevel,
        PerWorld
    };



    enum struct LimitBehaviour {
        DiscardOldOnes,
        PreventAdding,
        MoveLastOne
    };


    enum struct LayerType {
        Entities,
        IntGrid,
        Tiles,
        AutoLayer
    };

    enum struct WorldLayout {
        Free,
        GridVania,
        LinearHorizontal,
        LinearVertical,
    };

    enum struct FieldType {
        Int,
        Float,
        String,
        Text,
        Bool,
        Enum,
        Point,
        EntityRef,
        Tile,
        Array
    };

    enum struct FieldAllowedRefs {
        Any,
        OnlySame,
        OnlyTags,
        OnlySpecificEntity
    };

    template <typename T>
    struct MinMax {
        T min;
        T max;
    };

    struct EntityRef {
        uuid world_iid;
        uuid level_iid;
        uuid layer_iid;
        uuid entity_iid;
    };
}