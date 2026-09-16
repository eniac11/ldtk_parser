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

#include "ldtk_parser/reader.h"
#include "ldtk_parser/project.h"

#include <json/json.h>
#include <ldtk_parser/defs/enum_def.h>

#include "parse_types.h"

namespace ldtk {
    TileRect parse_tile_rect(Json::Value const& json, ReaderOptions const& options) {
        uint32_t tileset_uid = json["tilesetUid"].asUInt();
        int const x = json["x"].asInt();
        int const y = json["y"].asInt();
        int const w = json["w"].asInt();
        int const h = json["h"].asInt();
        return TileRect{
            .tilesetUid = tileset_uid,
            .rect = {
                x, y, w, h
            }
        };
    }

    #include "reader.fields.cpp"



    LayerDef parse_layer_def(Project& project, Json::Value const& json, ReaderOptions const& options) {
        LayerType type;
        if (!parse_layerdef_type(json["__type"].asString(), &type)) {
            throw ldtk_decode_exception(std::format("Failed to decode LayerDef LayerType. Got '{}'",
                                                    json["__type"].asString()));
        }
        std::string name = json["identifier"].asString();
        uint32_t uid = json["uid"].asUInt();
        uint32_t grid_size = json["gridSize"].asUInt();

        int32_t pxoffx = json["pxOffsetX"].asInt();
        int32_t pxoffy = json["pxOffsetY"].asInt();
        glm::ivec2 pixel_offset{pxoffx, pxoffy};

        float parallaxFactx = json["parallaxFactorX"].asFloat();
        float parallaxFacty = json["parallaxFactorY"].asFloat();
        glm::vec2 parallax_factor{parallaxFactx, parallaxFacty};
        bool parallax_scaling = json["parallaxScaling"].asBool();

        LayerDef def{
            .type = type,
            .name = name,
            .uid = uid,
            .grid_size = grid_size,
            .pixel_offset = pixel_offset,
            .parallax_factor = parallax_factor,
            .parallax_scaling = parallax_scaling
        };
        return def;
    }

    EntityDef parse_entity_def(Project& project, Json::Value const& json, ReaderOptions const& options) {
        {
            auto name = json["identifier"].asString();
            uint32_t const uid = json["uid"].asUInt();
            uint32_t const width = json["width"].asUInt();
            uint32_t const height = json["height"].asUInt();
            bool resizeable_x = json["resizeableX"].asBool();
            std::optional<uint32_t> minWidth = std::nullopt;
            std::optional<uint32_t> maxWidth = std::nullopt;
            if (resizeable_x and !json["minWidth"].isNull() and !json["maxWidth"].isNull()) {
                minWidth = json["minWidth"].asUInt();
                maxWidth = json["maxWidth"].asUInt();
            }
            bool resizeable_y = json["resizeableY"].asBool();
            std::optional<uint32_t> minHeight = std::nullopt;
            std::optional<uint32_t> maxHeight = std::nullopt;
            if (resizeable_y and !json["minHeight"].isNull() and !json["maxHeight"].isNull()) {
                minHeight = json["minHeight"].asUInt();
                maxHeight = json["maxHeight"].asUInt();
            }
            bool keepAspectRatio = json["keepAspectRatio"].asBool();
            float tile_opacity = json["tileOpacity"].asFloat();
            float fill_opacity = json["fillOpacity"].asFloat();
            float line_opacity = json["lineOpacity"].asFloat();
            bool hollow = json["hollow"].asBool();

            RenderMode render_mode;
            if (!parse_render_mode(json["renderMode"].asString(), &render_mode)) {
                throw ldtk_decode_exception(std::format("Failed to decode EntityDef RenderMode. Got '{}'",
                                                        json["renderMode"].asString()));
            }

            TileRenderMode tile_render_mode;
            if (!parse_tile_render_mode(json["tileRenderMode"].asString(), &tile_render_mode)) {
                throw ldtk_decode_exception(std::format("Failed to decode EntityDef TileRenderMode. Got '{}'",
                                                        json["tileRenderMode"].asString()));
            }
            uint32_t maxCount = json["maxCount"].asUInt();
            LimitScope limit_scope;
            if (!parse_limit_scope(json["limitScope"].asString(), &limit_scope)) {
                throw ldtk_decode_exception(std::format("Failed to decode EntityDef LimitScope. Got '{}'",
                                                        json["limitScope"].asString()));
            }
            LimitBehaviour limit_behaviour;
            if (!parse_limit_behaviour(json["limitBehavior"].asString(), &limit_behaviour)) {
                throw ldtk_decode_exception(std::format("Failed to decode EntityDef LimitBehaviour. Got '{}'",
                                                        json["limitBehavior"].asString()));
            }


            EntityDef def{
                .name = name,
                .uid = uid,
                .tilesize = {width, height},
                .resizeable_x = resizeable_x,
                .min_width = minWidth,
                .max_width = maxWidth,
                .resizeable_y = resizeable_y,
                .min_height = minHeight,
                .max_height = maxHeight,
                .keep_aspect_ratio = keepAspectRatio,
                .tile_opacity = tile_opacity,
                .fill_opacity = fill_opacity,
                .line_opacity = line_opacity,
                .hollow = hollow,
                .render_mode = render_mode,
                .tile_render_mode = tile_render_mode,
                .tile_rect = parse_tile_rect(json["tileRect"], options),
                .max_count = maxCount,
                .limit_scope = limit_scope,
                .limit_behaviour = limit_behaviour

            };

            for (auto const& jfieldDef : json["fieldDefs"]) {
                auto field = parse_field_def(project, jfieldDef, options);
                def.field_defs.insert_or_assign(field->uid, field);
            }

            return def;
        }
    }

    EnumDef parse_enum_def(Project& project, Json::Value const& json, ReaderOptions const& options) {
        auto name = json["identifier"].asString();
        uint32_t uid = json["uid"].asUInt();
        uint32_t icon_tileset_uid = json["iconTilesetUid"].asUInt();
        Tileset* tileset = nullptr;
        if (options.resolve_pointers) {
            std::println("TODO: Resolve Tileset def");
        }
        EnumDef enum_def{
            .name = name,
            .uid = uid,
            .icon_tileset_uid = icon_tileset_uid,
            .tileset = tileset
        };
        for (auto const& value : json["values"]) {
            auto id = value["id"].asString();
            TileRect rect = parse_tile_rect(value["tileRect"], options);
            uint32_t colour = value["color"].asUInt();
            enum_def.values[id] = {id, rect, colour, uid};
        }
        return enum_def;
    }

    EntityInstance parse_entity_instance(Project& project, Json::Value const& json, ReaderOptions const& options) {
        auto name = json["__identifier"].asString();
        auto siid = json["iid"].asString();
        uuid iid = uuid_string_to_uuid(siid);
        glm::ivec2 grid_pos = {json["__grid"][0].asUInt(), json["__grid"][1].asUInt()};
        glm::ivec2 pivot = {json["__pivot"][0].asUInt(), json["__pivot"][1].asUInt()};

        TileRect tile_rect = parse_tile_rect(json["__tile"], options);

        uint32_t def_uid = json["defUid"].asUInt();
        EntityDef* def = nullptr;
        if (options.resolve_pointers) {
            def = &project.entity_defs[def_uid];
        }

        EntityInstance instance{
            .iid = iid,
            .name = name,
            .grid_position = grid_pos,
            .pivot = pivot,
            .tile_rect = tile_rect,
            .entity_def_uid = def_uid,
            .def = def
        };

        for (auto const& jfield_instance : json["fieldInstances"]) {
            auto field = parse_field_instance(project, iid, jfield_instance, options);
            instance.fields.push_back(field);
        }

        return instance;
    }

    #include "reader.layer.cpp"

    LayerInstance parse_layer_instance(Project& project, Level* level, Json::Value const& json,
                                       ReaderOptions const& options) {
        auto const name = json["__identifier"].asString();
        LayerType layer_type;
        if (!parse_layerdef_type(json["__type"].asString(), &layer_type)) {
            throw ldtk_decode_exception(std::format("Failed to decode LayerInstance LayerType. Got '{}'",
                                                    json["__type"].asString()));
        }
        auto const siid = json["iid"].asString();
        uuid li_iid = uuid_string_to_uuid(siid);
        uint32_t layerdef_uid = json["layerDefUid"].asUInt();

        uint32_t cWid = json["__cWid"].asUInt();
        uint32_t cHei = json["__cHei"].asUInt();
        glm::ivec2 computed_tilesize{cWid, cHei};

        uint32_t gridSize = json["__gridSize"].asUInt();

        int32_t pxTotalOffsetX = json["__pxTotalOffsetX"].asInt();
        int32_t pxTotalOffsetY = json["__pxTotalOffsetY"].asInt();
        glm::ivec2 computed_pixel_offset{pxTotalOffsetX, pxTotalOffsetY};
        float computed_opacity = json["__opacity"].asFloat();
        Tileset* tileset = nullptr;
        std::optional<uint32_t> tileset_uid = std::nullopt;
        if (auto const& tileset_def = json["__tilesetDefUid"]; !tileset_def.isNull()) {
            tileset_uid = tileset_def.asUInt();
            if (options.resolve_pointers) {
                std::println("TODO: Resolve Tileset def");
            }
        }
        uint32_t level_uid = json["levelId"].asUInt();
        if (!options.resolve_pointers) {
            level = nullptr;
        }
        LayerDef* layer_def = nullptr;
        if (options.resolve_pointers) {
            layer_def = &project.layer_defs[layerdef_uid];
        }


        LayerInstance instance{
            .iid = li_iid,
            .name = name,
            .type = layer_type,
            .computed_tilesize = computed_tilesize,
            .computed_grid_size = gridSize,
            .computed_opacity = computed_opacity,
            .computed_pixel_offset = computed_pixel_offset,
            .level_uid = level_uid,
            .level = level,
            .layerdef_uid = layerdef_uid,
            .layerDef = layer_def,
            .tilesetdef_uid = tileset_uid,
            .tileset = tileset
        };
        if (layer_type == LayerType::Tiles or layer_type == LayerType::AutoLayer or layer_type == LayerType::IntGrid) {
            if (layer_type == LayerType::Tiles) {
                instance.tiles = parse_layer_tiles(json["gridTiles"], false);
            } else if (layer_type == LayerType::AutoLayer or layer_type == LayerType::IntGrid) {
                instance.tiles = parse_layer_tiles(json["autoLayerTiles"], true);
            }
            auto it = instance.tiles.begin();
            while (it != instance.tiles.end()) {
                instance.coord_tile_mapping.insert_or_assign(it->coordid, *it);
                ++it;
            }
        }

        if (layer_type == LayerType::Entities) {
            for (auto const& jentityInstance : json["entityInstances"]) {
                EntityInstance entityInstance = parse_entity_instance(project, jentityInstance, options);
                instance.entities[entityInstance.iid] = entityInstance;
            }
        }
        return instance;
    }

    std::unique_ptr<Level> parse_level(Project& project, Json::Value const& json, ReaderOptions const& options) {
        auto const name = json["identifier"].asString();
        auto const siid = json["iid"].asString();
        uuid const liid = uuid_string_to_uuid(siid);
        int const depth = json["worldDepth"].asInt();
        uint32_t const uid = json["uid"].asUInt();
        auto level = std::unique_ptr<Level>(new Level{
            .iid = liid,
            .uid = uid,
            .name = name,
            .depth = depth,
        });
        for (auto const& jlayerInstance : json["layerInstances"]) {
            LayerInstance layerInstance = parse_layer_instance(project, level.get(), jlayerInstance, options);
            level->layers[layerInstance.iid] = layerInstance;
        }
        return level;
    }


    World parse_world(Project& project, Json::Value const& json, ReaderOptions const& options) {
        auto siid = json["iid"].asString();
        uuid wiid = uuid_string_to_uuid(siid);
        auto name = json["identifier"].asString();
        std::int32_t grid_width = json["worldGridWidth"].asInt();
        std::int32_t grid_height = json["worldGridHeight"].asInt();
        std::string world_layout = json["worldLayout"].asString();
        WorldLayout layout;
        if (!parse_layout(world_layout, &layout)) {
            throw ldtk_decode_exception(std::format("Failed to parse world layout. Got '{}'", world_layout));
        }
        World world{
            .iid = wiid,
            .name = name,
            .grid = {grid_width, grid_height},
            .layout = layout,
        };
        for (auto const& jlevel : json["levels"]) {
            auto level = parse_level(project, jlevel, options);
            world.levels[level->iid] = std::move(level);
        }
        return world;
    }

    void resolve_field_entity_refs(Project& project) {
        for (auto& world : project.worlds) {
            for (auto& level : world.second.levels) {
                for (auto& layer : level.second->layers) {
                    if (layer.second.type == LayerType::Entities) {
                        for (auto& e : layer.second.entities) {
                            for (auto& field : e.second.fields) {
                                assert(field->entity_instance_uuid == e.second.iid);
                                field->entity_def = &e.second;
                                field->def = &e.second.def->field_defs.at(field->def_uid);
                            }
                        }
                    }
                }
            }
        }
    }

    Project parse_project(Json::Value const& json, ReaderOptions const& options) {
        auto siid = json["iid"].asString();
        uuid piid = uuid_string_to_uuid(siid);
        Project p{
            .iid = piid
        };

        for (auto const& jenum : json["defs"]["enums"]) {
            EnumDef def = parse_enum_def(p, jenum, options);
            p.enum_defs[def.uid] = def;
        }
        if (options.resolve_pointers) {
            for (auto& edef : p.enum_defs) {
                for (auto& evalue : edef.second.values) {
                    evalue.second.enum_type_def = &edef.second;
                }
            }
        }

        for (auto const& jentity : json["defs"]["entities"]) {
            EntityDef def = parse_entity_def(p, jentity, options);
            p.entity_defs[def.uid] = def;
        }
        for (auto const& jlayer : json["defs"]["layers"]) {
            LayerDef def = parse_layer_def(p, jlayer, options);
            p.layer_defs[def.uid] = def;
        }

        for (auto const& world_value : json["worlds"]) {
            World world = parse_world(p, world_value, options);
            p.worlds[world.iid] = std::move(world);
        }

        if (options.resolve_pointers) {
            resolve_field_entity_refs(p);
        }
        return p;
    }

    Project parse_project_json(Json::Value const& json, ReaderOptions const& options) {
        return parse_project(json, options);
    }
}