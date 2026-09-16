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

#include "ldtk_parser/defs/field_def.h"
#include "ldtk_parser/field.h"

#include "ldtk_parser/reader.h"
#include "ldtk_parser/project.h"
#include "parse_types.h"

#include <json/json.h>

using namespace ldtk;

uuid get_uuid_from_json_kv(std::string const& key, Json::Value const& json) {
    auto siid = json[key].asString();
    return uuid_string_to_uuid(siid);
}

EntityRef parse_entity_ref(Json::Value const& value) {

    EntityRef eref{
        .world_iid = get_uuid_from_json_kv("worldIid", value),
        .level_iid = get_uuid_from_json_kv("levelIid", value),
        .layer_iid = get_uuid_from_json_kv("layerIid", value),
        .entity_iid = get_uuid_from_json_kv("entityIid", value),
    };
    return eref;
}

template <typename T>
beman::indirect::polymorphic<IFieldDef> create_field(IFieldDef const& memo_def, Json::Value const& min, Json::Value const& max, Json::Value const& defaultOverride) {
    std::optional<T> default_override = std::nullopt;
    if (defaultOverride) {
        auto value = defaultOverride["params"][0];
        default_override = value.as<T>();
    }
    std::optional<MinMax<T>> value_limits = std::nullopt;
    if (min and max) {
        value_limits = {min.as<T>(), max.as<T>()};
    }
    return beman::indirect::polymorphic<IFieldDef>(FieldDef<T>(memo_def, value_limits, default_override));

}

template <>
beman::indirect::polymorphic<IFieldDef> create_field<bool>(IFieldDef const& memo_def, Json::Value const& min, Json::Value const& max, Json::Value const& defaultOverride) {
    std::optional<bool> default_override = std::nullopt;
    if (defaultOverride) {
        default_override = defaultOverride.asBool();
    }

    return beman::indirect::polymorphic<IFieldDef>(FieldDef<bool>(memo_def, std::nullopt, default_override));

}

uint32_t parse_enum_type_from_haxe_type(std::string const& stype) {
    auto const start = stype.find("(");
    auto const end = stype.rfind(")");
    if (start == std::string::npos or end == std::string::npos) {
        throw ldtk_decode_exception(std::format("Could not parse Haxe Enum type. Got '{}'", stype));
    }
    size_t const length = end - (start+1);
    std::string const sub = stype.substr(start+1, length);
    return std::stoull(sub);
}

beman::indirect::polymorphic<IFieldDef> create_field_enum(Project const& project, IFieldDef const& memo_def, uint32_t enum_uid, Json::Value const& defaultOverride, ReaderOptions const& options) {
    std::optional<EnumInstance> default_override = std::nullopt;
    const EnumDef* enum_def = nullptr;
    if (options.resolve_pointers) {
        enum_def = project.get_enum(enum_uid);
    }

    if (defaultOverride) {
        auto value = defaultOverride["params"][0];
        auto svalue = value.asString();
        EnumInstance instance{
            .def_uid = enum_uid,
            .value_id = svalue
        };
        if (options.resolve_pointers) {
            instance.def = enum_def;
            instance.value = project.get_enum_value(enum_uid, svalue);
        }
    }

    return beman::indirect::polymorphic<IFieldDef>(EnumFieldDef(memo_def, std::nullopt, default_override, enum_uid, enum_def));

}

beman::indirect::polymorphic<IFieldDef> parse_field_def(Project& project, Json::Value const& json,
                                                        ReaderOptions const& options) {
    auto name = json["identifier"].asString();
    uint32_t uid = json["uid"].asUInt();
    auto stype = json["type"].asString();
    FieldType type;
    if (!parse_field_type(stype, &type)) {
        throw ldtk_decode_exception(std::format("Failed to decode FieldDef type. Got '{}'",
                                                stype));
    }
    bool is_array = json["isArray"].asBool();
    bool can_be_null = json["canBeNull"].asBool();
    auto const& array_min_length = json["arrayMinLength"];
    auto const& array_max_length = json["arrayMaxLength"];
    std::optional<MinMax<uint32_t>> elements_limits = std::nullopt;
    if (is_array and array_min_length and array_max_length) {
        elements_limits = {array_min_length.asUInt(), array_max_length.asUInt()};
    }

    std::optional<std::string> regex = std::nullopt;
    if (auto const& jregex = json["regex"]) {
        regex = jregex.asString();
    }
    std::optional<std::string> accepted_file_types = std::nullopt;
    if (auto const& jaccepted_file_types = json["acceptFileTypes"]) {
        accepted_file_types = jaccepted_file_types.asString();
    }
    bool symmetrical_ref = json["symmetricalRef"].asBool();
    bool auto_chain_ref = json["autoChainRef"].asBool();
    bool allow_out_of_level_ref = json["allowOutOfLevelRef"].asBool();
    FieldAllowedRefs allowed_refs;
    if (!parse_allow_refs(json["allowedRefs"].asString(), &allowed_refs)) {
        throw ldtk_decode_exception(std::format("Failed to decode FieldDef allowedRefs. Got '{}'",
                                                json["allowedRefs"].asString()));
    }
    std::optional<uint32_t> allowed_refs_entity_uid = std::nullopt;
    if (auto const& jallowed_refs_entity_uid = json["allowedRefsEntityUid"]) {
        allowed_refs_entity_uid = jallowed_refs_entity_uid.asUInt();
    }

    if (is_array) {
        std::println("TODO: array types");
        return beman::indirect::polymorphic<IFieldDef>(IFieldDef(
            name,
            uid,
            type,
            is_array,
            can_be_null,
            elements_limits,
            regex,
            accepted_file_types,
            symmetrical_ref,
            auto_chain_ref,
            allow_out_of_level_ref,
            allowed_refs,
            allowed_refs_entity_uid,
            nullptr,
            std::nullopt,
            nullptr
        ));
    }
    IFieldDef const memo_def(
        name,
        uid,
        type,
        is_array,
        can_be_null,
        elements_limits,
        regex,
        accepted_file_types,
        symmetrical_ref,
        auto_chain_ref,
        allow_out_of_level_ref,
        allowed_refs,
        allowed_refs_entity_uid,
        nullptr,
        std::nullopt,
        nullptr
    );

    auto const& min = json["min"];
    auto const& max = json["max"];
    bool has_min_max = min and max;
    auto const& defaultOverride = json["defaultOverride"];

    switch (type) {
        case FieldType::Int:
            return create_field<int32_t>(memo_def, min, max, defaultOverride);
        case FieldType::Float:
            return create_field<float>(memo_def, min, max, defaultOverride);
        case FieldType::String:
            return create_field<std::string>(memo_def, min, max, defaultOverride);

        case FieldType::Text:
            break;
        case FieldType::Bool:
            return create_field<bool>(memo_def, min, max, defaultOverride);
        case FieldType::Enum:
            {
                uint32_t enum_uid = parse_enum_type_from_haxe_type(stype);
                return create_field_enum(project, memo_def, enum_uid, defaultOverride, options);
            }
        case FieldType::Point:
            break;
        case FieldType::EntityRef:
            break;
        case FieldType::Tile:
            break;
    }
    return beman::indirect::polymorphic<IFieldDef>(memo_def);
}

template <typename T>
beman::indirect::polymorphic<IFieldInstance> create_field_instance(IFieldInstance const& memo_def, Json::Value const& value) {
    std::optional<T> default_override = std::nullopt;
    if (value) {
        // auto value = defaultOverride["params"][0];
        if constexpr (std::is_same_v<T, EntityRef>) {
            default_override = parse_entity_ref(value);
        } else {
        default_override = value.as<T>();
        }
    }
    return beman::indirect::polymorphic<IFieldInstance>(FieldInstance<T>(memo_def, default_override));

}

std::string get_human_readable_enum_type_name(std::string const& stype) {
    auto it = stype.find(".");
    if (it != std::string::npos and it+1 != std::string::npos) {
        return stype.substr(it+1);
    }
    it = stype.rfind(".");
    if (it != std::string::npos and it+1 != std::string::npos) {
        return stype.substr(it+1);
    }
    return "";
}




beman::indirect::polymorphic<IFieldInstance> create_field_instance_enum(Project const& project, IFieldInstance const& memo_def, std::string const& enum_type, Json::Value const& value, ReaderOptions const& options) {
    std::optional<EnumInstance> default_override = std::nullopt;
    if (value) {
        // auto value = defaultOverride["params"][0];
        auto senum_value = value.asString();
        auto evalue = project.get_enum_value(enum_type, senum_value);
        if (evalue) {
            EnumInstance instance{
                .def_uid = evalue->enum_type_uid,
                .value_id =  senum_value
            };
            if (options.resolve_pointers) {
                instance.def = evalue->enum_type_def;
                instance.value = evalue;
            }
            default_override = instance;
        } else {
            throw ldtk_decode_exception(std::format("Failed to find enum in project. Got '{}'",
                                                enum_type));
        }

    }
    return beman::indirect::polymorphic<IFieldInstance>(FieldInstance<EnumInstance>(memo_def, default_override));

}

beman::indirect::polymorphic<IFieldInstance> parse_field_instance(Project& project, uuid einstance_uuid, Json::Value const& json,
                                                        ReaderOptions const& options) {
    auto name = json["__identifier"].asString();
    auto stype = json["__type"].asString();
    FieldType type;
    if (!parse_human_readable_field_type(stype, &type)) {
        throw ldtk_decode_exception(std::format("Failed to decode FieldInstance type. Got '{}'",
                                                stype));
    }
    std::optional<TileRect> tile_rect = std::nullopt;
    Json::Value const& tile_rect_json = json["__tile"];
    if (tile_rect) {
        tile_rect = parse_tile_rect(tile_rect_json, options);
    }

    uint32_t def_uid = json["defUid"].asUInt();
    IFieldDef* def = nullptr;
    if (options.resolve_pointers) {
        def = nullptr;
        std::println("TODO: chase entity pointer for entity def then get field def");
    }

    IFieldInstance memo_def(
        name,
        type,
        tile_rect,
        einstance_uuid,
        nullptr,
        def_uid,
        nullptr
    );
    Json::Value const& value = json["__value"];
    switch (type) {
        case FieldType::Int:
            return create_field_instance<int32_t>(memo_def, value);
        case FieldType::Float:
            return create_field_instance<float>(memo_def, value);
        case FieldType::String:
            return create_field_instance<std::string>(memo_def, value);
        case FieldType::Text:
            break;
        case FieldType::Bool:
            return create_field_instance<bool>(memo_def, value);
        case FieldType::Enum:
            {
                std::string enum_type = get_human_readable_enum_type_name(stype);
                if (enum_type.empty()) {
                    throw ldtk_decode_exception(std::format("Failed to decode FieldInstance Enum type. Got '{}'",
                                                stype));
                }
                return create_field_instance_enum(project, memo_def, enum_type, value, options);
            }
        case FieldType::Point:
            break;
        case FieldType::EntityRef:
            return create_field_instance<EntityRef>(memo_def, value);
        case FieldType::Tile:
            break;
    }
    return beman::indirect::polymorphic<IFieldInstance>(IFieldInstance(memo_def));
}