// Copyright 2026 Hadley Epstein
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <fstream>
#include <ldtk_parser/reader.h>
#include <print>
#include <iostream>
#include <array>
#include <unordered_map>
#include <json/json.h>

#include <ranges>
#include <bitset>
#include <uuid.h>



struct Chunk {
    glm::ivec2 chunk_id;
    // std::array<std::array<std::uint32_t, 16>, 16> tileids;
    std::map<std::tuple<uint32_t, uint32_t>, std::vector<uint32_t>> tiles;
};

int main(int argc, char* argv[]) {
    std::ifstream ifs("data/99-topdown_example.ldtk", std::ios::in);
    Json::Value root;

    Json::CharReaderBuilder builder;
    Json::String errs;
    if (!Json::parseFromStream(builder, ifs, &root, &errs)) {
        std::println("{}", errs);
        return EXIT_FAILURE;
    }

    ldtk::Project project = ldtk::parse_project_json(root);
    // project.loadFromFile("topdown_example.ldtk");
    std::println( "Name: {::x}",  project.iid);

    std::cout << "Worlds: " << std::endl;

    for (auto const& [iid, world] : project.worlds) {
        std::cout << "\t- World: " << std::quoted(world.name) << std::endl;
        std::println( "\t  IID: {::x}",  iid);
        std::cout << "\t  Levels: " << std::endl;
        for (auto const& [iid, level] : world.levels) {

            std::println( "\t\t  Level: {::x}",  iid);
            std::cout << "\t\t  Layers: " << std::endl;
            for (auto const& [iid, layer] : level->layers) {
                std::cout << "\t\t\t- Layer: " << std::quoted(layer.name) << std::endl;
                std::println( "\t\t\t  IID: {::x}",  iid);
                // std::cout << "\t\t\t  IID: " << iid << std::endl;
                if (layer.type == ldtk::LayerType::Entities) {
                    std::cout << "\t\t\t  Entities: " << std::endl;
                    for (auto const& [iid, ent] : layer.entities) {
                        std::cout << "\t\t\t\t- Entity: " << std::quoted(ent.name) << std::endl;
                        for (auto const& [uid, field_def] : ent.def->field_defs) {
                            std::cout <<"\t\t\t\t  Field: " << std::quoted(field_def->name) << std::endl;

                            // if (field_def->type == FieldType::Enum) {
                            //     auto const& field = ent.getField<ldtk::FieldType::Enum>(field_def.name);
                            //     if (!field.is_null()) {
                            //         std::cout << "\t\t\t\t  Value: " << field.value().type.name << "." << field.value().name << std::endl;
                            //     } else {
                            //         std::cout << "\t\t\t\t  Value: null" << std::endl;
                            //     }
                            // }

                        }
                    }
                }
                if (true and (layer.type == ldtk::LayerType::Tiles or layer.type == ldtk::LayerType::IntGrid or layer.
                    type == ldtk::LayerType::AutoLayer)) {
                    std::cout << "\t\t\t  Tiles: " << std::endl;
                    std::vector<Chunk> chunks;



                    for (auto const& tile : layer.tiles) {
                        // std::cout << "\t\t\t\t- Tile: " << tile.coordId << std::endl;
                        // std::cout << "\t\t\t\t  ID: " << tile.tileId << std::endl;
                        // std::cout << "\t\t\t\t  rect: " << tile.getTextureRect() << std::endl;
                        // std::cout << "\t\t\t\t  pos: " << tile.getGridPosition() << std::endl;
                        auto tilePos = tile.layer_pixel_coord;
                        auto grid = tilePos / layer.computed_tilesize;
                        auto chunk_id = grid / 16;
                        auto it = std::ranges::find_if(chunks, [chunk_id](auto const& chunk) {
                            return chunk.chunk_id == chunk_id;
                        });
                        if (it == chunks.end()) {
                            auto& chunk = chunks.emplace_back(chunk_id);
                            // for (auto& row : chunk.tile) {
                            //     std::ranges::fill(row, 1000);
                            // }
                        }
                        it = std::ranges::find_if(chunks, [chunk_id](auto const& chunk) {
                            return chunk.chunk_id == chunk_id;
                        });
                        auto offset = grid % 16;
                        // std::cout << offset << std::endl;
                        uint32_t tileid = tile.tileid;
                        std::bitset<32> set(tileid);
                        if (tile.flip_x()) {
                            set[31] = true;
                        }
                        if (tile.flip_y()) {
                            set[30] = true;
                        }

                        (*it).tiles[std::make_pair(offset.x, offset.y)].push_back(set.to_ulong());
                        // std::cout << "\t\t\t\t  chunk: " << chunk_id << std::endl;
                        // std::cout << "\t\t\t\t  offset: " << offset << std::endl;

                        // auto verts = tile.getVertices();
                        // std::cout << "\t\t\t\t  verts: " << std::endl;
                        // std::cout << "\t\t\t\t\t- " << verts[0].pos << verts[0].tex << std::endl;
                        // std::cout << "\t\t\t\t\t- " << verts[1].pos << verts[1].tex << std::endl;
                        // std::cout << "\t\t\t\t\t- " << verts[2].pos << verts[2].tex << std::endl;
                        // std::cout << "\t\t\t\t\t- " << verts[3].pos << verts[3].tex << std::endl;
                        // std::println("\t\t\t\t  verts: {}", tile.getVertices());
                    }
                    for (auto const& chunk : chunks) {
                        std::println("\t\t\t\t\t {{");
                        std::vector<std::array<uint32_t, 16*16>> result;
                        for (auto const& [pt, ve] : chunk.tiles) {
                            if (ve.size() > result.size()) {
                                    std::cout << "Resize" << std::endl;
                                    // result.resize(bucket.size());
                                    size_t extra = ve.size() - result.size();
                                    for (std::size_t j = 0; j < extra; ++j) {
                                        result.push_back({});
                                        result.back().fill(1000);
                                    }
                                }
                            auto [x, y] = pt;
                            size_t z = 0;
                            for (auto const& v : ve) {

                                result[z][y *16 + x] = v;
                                z++;
                            }
                        }


                        for (auto r : result) {
                            std::println("\t\t\t\t\t {{{:n}}},", r);

                        }
                        std::println("\t\t\t\t\t }}");

                    }

                }
            }
        }
    }
}
