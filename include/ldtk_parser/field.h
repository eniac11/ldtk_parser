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
#include "types.h"
#include <optional>
#include <uuid.h>
#include <beman/indirect/indirect.hpp>
#include <beman/indirect/polymorphic.hpp>
#include <glm/vec2.hpp>

#include "defs/enum_def.h"

namespace ldtk {
    struct EntityInstance;
    struct IFieldDef;

    struct IFieldInstance {
        virtual ~IFieldInstance() = default;

        IFieldInstance(std::string const& name, FieldType type, std::optional<TileRect> const& tile_rect,
            uuid const& entity_instance_uuid, const EntityInstance* entity_def, uint32_t def_uid,
            const beman::indirect::polymorphic<IFieldDef>* def)
            : name(name),
              type(type),
              tile_rect(tile_rect),
              entity_instance_uuid(entity_instance_uuid),
              entity_def(entity_def),
              def_uid(def_uid),
              def(def) {
        }

        std::string name;
        FieldType type;
        std::optional<TileRect> tile_rect;

        uuid entity_instance_uuid;;
        const EntityInstance* entity_def;
        uint32_t def_uid;
        const beman::indirect::polymorphic<IFieldDef>* def;
    };

    template <typename T>
    struct FieldInstance : IFieldInstance {
        FieldInstance(std::string const& name, FieldType type, std::optional<TileRect> const& tile_rect,
            uuid const& entity_instance_uuid, const EntityInstance* entity_def, uint32_t def_uid,
            const beman::indirect::polymorphic<IFieldDef>* def, std::optional<T> const& value)
            : IFieldInstance(name, type, tile_rect, entity_instance_uuid, entity_def, def_uid, def),
              value(value) {
        }

        explicit FieldInstance(IFieldInstance const& instance, std::optional<T> const& value)
            : IFieldInstance(instance), value(value) {
        }

        std::optional<T> value;

    };


    namespace detail {

        // clang-format off
        template<FieldType> struct fieldinstance_type_from_enum_;
        template<> struct fieldinstance_type_from_enum_<FieldType::Int>       { using type = FieldInstance<int>; };
        template<> struct fieldinstance_type_from_enum_<FieldType::Float>     { using type = FieldInstance<float>; };
        template<> struct fieldinstance_type_from_enum_<FieldType::Bool>      { using type = FieldInstance<bool>; };
        template<> struct fieldinstance_type_from_enum_<FieldType::String>    { using type = FieldInstance<std::string>; };
        // template<> struct field_type_from_enum_<FieldType::Colour>     { using type = Field<Color>; };
        template<> struct fieldinstance_type_from_enum_<FieldType::Point>     { using type = FieldInstance<glm::vec2>; };
        template<> struct fieldinstance_type_from_enum_<FieldType::Enum>      { using type = FieldInstance<EnumInstance>; };
        // template<> struct field_type_from_enum_<FieldType::FilePath>  { using type = Field<FilePath>;  };
        template<> struct fieldinstance_type_from_enum_<FieldType::Tile>      { using type = FieldInstance<TileRect>; };
        template<> struct fieldinstance_type_from_enum_<FieldType::EntityRef> { using type = FieldInstance<EntityRef>; };

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
    using getFieldInstanceType = typename detail::fieldinstance_type_from_enum_<T>::type;
}