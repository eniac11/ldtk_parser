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

#include <optional>
#include <ldtk_parser/types.h>
#include <string>
#include <glm/vec2.hpp>
#include <variant>

#include "enum_def.h"

namespace ldtk {
    struct EntityDef;
    struct Tileset;

    struct IFieldDef {
        virtual ~IFieldDef() = default;
        std::string name;
        uint32_t uid;
        FieldType type;
        bool is_array;
        bool can_be_null;
        std::optional<MinMax<uint32_t>> element_limits;
        std::optional<std::string> value_regex;
        std::optional<std::string> accepted_filetype;
        bool symetrical_ref;
        bool auto_chain_ref;
        bool allow_out_of_level_ref;
        FieldAllowedRefs allowed_refs;
        std::optional<uint32_t> allowed_ref_entity_uid;
        EntityDef* allow_ref_entity_def;
        std::optional<uint32_t> tileset_uid;
        Tileset* tileset;

        IFieldDef(std::string const& name, uint32_t uid, FieldType type, bool is_array, bool can_be_null,
            std::optional<MinMax<uint32_t>> const& element_limits, std::optional<std::string> const& value_regex,
            std::optional<std::string> const& accepted_filetype, bool symetrical_ref, bool auto_chain_ref,
            bool allow_out_of_level_ref, FieldAllowedRefs allowed_refs,
            std::optional<uint32_t> const& allowed_ref_entity_uid, EntityDef* allow_ref_entity_def,
            std::optional<uint32_t> const& tileset_uid, Tileset* tileset)
            : name(name),
              uid(uid),
              type(type),
              is_array(is_array),
              can_be_null(can_be_null),
              element_limits(element_limits),
              value_regex(value_regex),
              accepted_filetype(accepted_filetype),
              symetrical_ref(symetrical_ref),
              auto_chain_ref(auto_chain_ref),
              allow_out_of_level_ref(allow_out_of_level_ref),
              allowed_refs(allowed_refs),
              allowed_ref_entity_uid(allowed_ref_entity_uid),
              allow_ref_entity_def(allow_ref_entity_def),
              tileset_uid(tileset_uid),
              tileset(tileset) {
        }
    };

    template <typename T>
    struct FieldDef : IFieldDef {
        FieldDef(std::string const& name, uint32_t uid, FieldType type, bool is_array, bool can_be_null,
            std::optional<MinMax<uint32_t>> const& element_limits, std::optional<std::string> const& value_regex,
            std::optional<std::string> const& accepted_filetype, bool symetrical_ref, bool auto_chain_ref,
            bool allow_out_of_level_ref, FieldAllowedRefs allowed_refs,
            std::optional<uint32_t> const& allowed_ref_entity_uid, EntityDef* allow_ref_entity_def,
            std::optional<uint32_t> const& tileset_uid, Tileset* tileset, std::optional<MinMax<T>> const& value_limits,
            std::optional<T> const& default_override)
            : IFieldDef(
                  name, uid, type, is_array, can_be_null, element_limits, value_regex, accepted_filetype, symetrical_ref,
                  auto_chain_ref, allow_out_of_level_ref, allowed_refs, allowed_ref_entity_uid, allow_ref_entity_def,
                  tileset_uid, tileset),
              value_limits(value_limits),
              default_override(default_override) {
        }
        FieldDef(IFieldDef const& memo_def, std::optional<MinMax<T>> const& value_limits,
            std::optional<T> const& default_override)
            : IFieldDef(memo_def),
              value_limits(value_limits),
              default_override(default_override) {
        }

        std::optional<MinMax<T>> value_limits;
        std::optional<T> default_override;
    };

    struct EnumFieldDef : FieldDef<EnumInstance> {
        EnumFieldDef(std::string const& name, uint32_t uid, FieldType type, bool is_array, bool can_be_null,
            std::optional<MinMax<uint32_t>> const& element_limits, std::optional<std::string> const& value_regex,
            std::optional<std::string> const& accepted_filetype, bool symetrical_ref, bool auto_chain_ref,
            bool allow_out_of_level_ref, FieldAllowedRefs allowed_refs,
            std::optional<uint32_t> const& allowed_ref_entity_uid, EntityDef* allow_ref_entity_def,
            std::optional<uint32_t> const& tileset_uid, Tileset* tileset,
            std::optional<MinMax<EnumInstance>> const& value_limits, std::optional<EnumInstance> const& default_override,
            uint32_t enum_def_uid, const EnumDef* field_enum_type)
            : FieldDef<EnumInstance>(
                  name, uid, type, is_array, can_be_null, element_limits, value_regex, accepted_filetype, symetrical_ref,
                  auto_chain_ref, allow_out_of_level_ref, allowed_refs, allowed_ref_entity_uid, allow_ref_entity_def,
                  tileset_uid, tileset, value_limits, default_override),
              enum_def_uid(enum_def_uid),
              field_enum_type(field_enum_type) {
        }

        EnumFieldDef(IFieldDef const& memo_def, std::optional<MinMax<EnumInstance>> const& value_limits,
            std::optional<EnumInstance> const& default_override, uint32_t enum_def_uid, const EnumDef* field_enum_type)
            : FieldDef<EnumInstance>(memo_def, value_limits, default_override),
              enum_def_uid(enum_def_uid),
              field_enum_type(field_enum_type) {
        }

        uint32_t enum_def_uid;
        const EnumDef* field_enum_type;

    };

    namespace detail {

        // clang-format off
        template<FieldType> struct field_type_from_enum_;
        template<> struct field_type_from_enum_<FieldType::Int>       { using type = FieldDef<int>; };
        template<> struct field_type_from_enum_<FieldType::Float>     { using type = FieldDef<float>; };
        template<> struct field_type_from_enum_<FieldType::Bool>      { using type = FieldDef<bool>; };
        template<> struct field_type_from_enum_<FieldType::String>    { using type = FieldDef<std::string>; };
        // template<> struct field_type_from_enum_<FieldType::Colour>     { using type = Field<Color>; };
        template<> struct field_type_from_enum_<FieldType::Point>     { using type = FieldDef<glm::vec2>; };
        template<> struct field_type_from_enum_<FieldType::Enum>      { using type = EnumFieldDef; };
        // template<> struct field_type_from_enum_<FieldType::FilePath>  { using type = Field<FilePath>;  };
        template<> struct field_type_from_enum_<FieldType::Tile>      { using type = FieldDef<TileRect>; };
        template<> struct field_type_from_enum_<FieldType::EntityRef> { using type = FieldDef<EntityRef>; };

        // template<> struct field_type_from_enum_<FieldType::ArrayInt>      { using type = ArrayField<int>; };
        // template<> struct field_type_from_enum_<FieldType::ArrayFloat>    { using type = ArrayField<float>; };
        // template<> struct field_type_from_enum_<FieldType::ArrayBool>     { using type = ArrayField<bool>; };
        // template<> struct field_type_from_enum_<FieldType::ArrayString>   { using type = ArrayField<std::string>; };
        // template<> struct field_type_from_enum_<FieldType::ArrayColor>    { using type = ArrayField<Color>; };
        // template<> struct field_type_from_enum_<FieldType::ArrayPoint>    { using type = ArrayField<IntPoint>; };
        // template<> struct field_type_from_enum_<FieldType::ArrayEnum>     { using type = ArrayField<EnumValue>; };
        // template<> struct field_type_from_enum_<FieldType::ArrayFilePath> { using type = ArrayField<FilePath>; };
        // template<> struct field_type_from_enum_<FieldType::ArrayTile>     { using type = ArrayField<TileRect>; };
        // template<> struct field_type_from_enum_<FieldType::ArrayEntityRef> { using type = ArrayField<EntityRef>; };
        // // clang-format on

    } // namespace detail

    template <FieldType T>
    using getFieldType = typename detail::field_type_from_enum_<T>::type;
}