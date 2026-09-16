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
#include "ldtk_parser/types.h"

#include <string>

using namespace ldtk;

constexpr bool parse_render_mode(std::string const& render_mode, RenderMode* mode) {
    if (render_mode == "Tile") {
        *mode = RenderMode::Tile;
        return true;
    }
    if (render_mode == "Ellipse") {
        *mode = RenderMode::Ellipse;
        return true;
    }
    if (render_mode == "Rectangle") {
        *mode = RenderMode::Rectangle;
        return true;
    }
    if (render_mode == "Cross") {
        *mode = RenderMode::Cross;
    }
    return false;
}

constexpr bool parse_tile_render_mode(std::string const& tile_render_string, TileRenderMode* mode) {
    if (tile_render_string == "Cover") {
        *mode = TileRenderMode::Cover;
        return true;
    }
    if (tile_render_string == "FitInside") {
        *mode = TileRenderMode::FitInside;
        return true;
    }
    if (tile_render_string == "Repeat") {
        *mode = TileRenderMode::Repeat;
        return true;
    }
    if (tile_render_string == "Stretch") {
        *mode = TileRenderMode::Stretch;
        return true;
    }
    if (tile_render_string == "FullSizedCropped") {
        *mode = TileRenderMode::FullSizedCropped;
        return true;
    }
    if (tile_render_string == "FullSizedUncropped") {
        *mode = TileRenderMode::FullSizedUncropped;
        return true;
    }
    if (tile_render_string == "NineSlice") {
        *mode = TileRenderMode::NineSlice;
        return true;
    }
    return false;
}

constexpr bool parse_limit_scope(std::string const& scope_string, LimitScope* scope) {
    if (scope_string == "PerLayer") {
        *scope = LimitScope::PerLayer;
        return true;
    }
    if (scope_string == "PerLevel") {
        *scope = LimitScope::PerLevel;
        return true;
    }
    if (scope_string == "PerWorld") {
        *scope = LimitScope::PerWorld;
        return true;
    }
    return false;
}

constexpr bool parse_limit_behaviour(std::string const& behaviour_string, LimitBehaviour* behaviour) {
    if (behaviour_string == "DiscardOldOnes") {
        *behaviour = LimitBehaviour::DiscardOldOnes;
        return true;
    }
    if (behaviour_string == "PreventAdding") {
        *behaviour = LimitBehaviour::PreventAdding;
        return true;
    }
    if (behaviour_string == "MoveLastOne") {
        *behaviour = LimitBehaviour::MoveLastOne;
        return true;
    }
    return false;
}

constexpr bool parse_layerdef_type(std::string const& slayerdef_type, LayerType* layertype) {
    if (slayerdef_type == "Entities") {
        *layertype= LayerType::Entities;
        return true;
    }
    if (slayerdef_type == "IntGrid") {
        *layertype= LayerType::IntGrid;
        return true;
    }
    if (slayerdef_type == "Tiles") {
        *layertype= LayerType::Tiles;
        return true;
    }
    if (slayerdef_type == "AutoLayer") {
        *layertype= LayerType::AutoLayer;
        return true;
    }
    return false;
}

constexpr bool parse_layout(std::string const& slayout, WorldLayout* layout) {
    if (slayout == "Free") {
        *layout = WorldLayout::Free;
        return true;
    }
    if (slayout == "GridVania") {
        *layout = WorldLayout::GridVania;
        return true;
    }
    if (slayout == "LinearHorizontal") {
        *layout = WorldLayout::LinearHorizontal;
        return true;
    }
    if (slayout == "LinearVertical") {
        *layout = WorldLayout::LinearVertical;
        return true;
    }
    return false;
}

constexpr bool parse_field_type(std::string const& sfield_type, FieldType* type) {
    if (sfield_type.find("F_Enum") != std::string::npos) {
        *type = FieldType::Enum;
        return true;
    }
    if (sfield_type == "F_Int") {
        *type = FieldType::Int;
        return true;
    }
    if (sfield_type == "F_Float") {
        *type = FieldType::Float;
        return true;
    }
    if (sfield_type == "F_String") {
        *type = FieldType::String;
        return true;
    }
    if (sfield_type == "F_Text") {
        *type = FieldType::Text;
        return true;
    }
    if (sfield_type == "F_Bool") {
        *type = FieldType::Bool;
        return true;
    }
    if (sfield_type == "F_Point") {
        *type = FieldType::Point;
        return true;
    }
    if (sfield_type == "F_EntityRef") {
        *type = FieldType::EntityRef;
        return true;
    }
    if (sfield_type == "F_Tile") {
        *type = FieldType::Tile;
        return true;
    }
    return false;
}

constexpr bool parse_human_readable_field_type(std::string const& sfield_type, FieldType* type) {
    if (sfield_type.find("LocalEnum") != std::string::npos or sfield_type.find("ExternEnum") != std::string::npos ) {
        *type = FieldType::Enum;
        return true;
    }
    if (sfield_type.find("Array") != std::string::npos) {
        *type = FieldType::Array;
        return true;
    }
    if (sfield_type == "Int") {
        *type = FieldType::Int;
        return true;
    }
    if (sfield_type == "Float") {
        *type = FieldType::Float;
        return true;
    }
    if (sfield_type == "String") {
        *type = FieldType::String;
        return true;
    }
    if (sfield_type == "Text") {
        *type = FieldType::Text;
        return true;
    }
    if (sfield_type == "Bool") {
        *type = FieldType::Bool;
        return true;
    }
    if (sfield_type == "Point") {
        *type = FieldType::Point;
        return true;
    }
    if (sfield_type == "EntityRef") {
        *type = FieldType::EntityRef;
        return true;
    }
    if (sfield_type == "Tile") {
        *type = FieldType::Tile;
        return true;
    }
    return false;
}

constexpr bool parse_allow_refs(std::string const& sfield_allowed_refs, FieldAllowedRefs* allowed_refs) {
    if (sfield_allowed_refs == "Any") {
        *allowed_refs = FieldAllowedRefs::Any;
        return true;
    }
    if (sfield_allowed_refs == "OnlySame") {
        *allowed_refs = FieldAllowedRefs::OnlySame;
        return true;
    }
    if (sfield_allowed_refs == "OnlyTags") {
        *allowed_refs = FieldAllowedRefs::OnlyTags;
        return true;
    }
    if (sfield_allowed_refs == "OnlySpecificEntity") {
        *allowed_refs = FieldAllowedRefs::OnlySpecificEntity;
        return true;
    }
    return false;
}
