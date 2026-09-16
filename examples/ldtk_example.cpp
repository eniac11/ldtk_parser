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
#include <print>
#include <cstdlib>
#include <iostream>

// #include <uuid.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <exception>
#include <ldtk_parser/reader.h>

#include <json/json.h>
using namespace engine::uuid_literals;



int main() {
    std::ifstream ifs("data/_entity_with_entityref.ldtk", std::ios::in);

    Json::Value root;

    Json::CharReaderBuilder builder;
    Json::String errs;
    if (!Json::parseFromStream(builder, ifs, &root, &errs)) {
        std::println("{}", errs);
        return EXIT_FAILURE;
    }

    ldtk::Project const p = ldtk::parse_project_json(root);



    std::cout << root << std::endl;
    return 0;
}
