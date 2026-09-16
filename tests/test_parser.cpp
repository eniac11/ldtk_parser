#include <filesystem>
#include <gtest/gtest.h>
#include <ldtk_parser/reader.h>
#include <fstream>
#include <jsoncpp/json/reader.h>


class MainParsingTests : public ::testing::TestWithParam<std::filesystem::path> {

};

TEST_P(MainParsingTests, ParsingTests) {
    std::filesystem::path datadir = "data";
    std::ifstream ifs(datadir / GetParam());
    ASSERT_TRUE(ifs.good());
    Json::Value root;
    Json::CharReaderBuilder builder;
    Json::String errs;
    EXPECT_TRUE(Json::parseFromStream(builder, ifs, &root, &errs)) << errs;
    ASSERT_NO_THROW({
        ldtk::Project p = ldtk::parse_project_json(root);
    });

}

TEST_P(MainParsingTests, ParsingTestsNoPointerResolution) {
    std::filesystem::path datadir = "data";
    std::ifstream ifs(datadir / GetParam());
    ASSERT_TRUE(ifs.good());
    Json::Value root;
    Json::CharReaderBuilder builder;
    Json::String errs;
    EXPECT_TRUE(Json::parseFromStream(builder, ifs, &root, &errs)) << errs;
    ASSERT_NO_THROW({
        ldtk::Project p = ldtk::parse_project_json(root, {.resolve_pointers = false});
    });

}

INSTANTIATE_TEST_SUITE_P(MainTestParameters, MainParsingTests, ::testing::Values<std::filesystem::path>(
"0-empty.ldtk",
"1-entities.ldtk",
"2-entities_with_fields.ldtk",
"3-tiles_layer.ldtk",
"4-tile_autolayer.ldtk",
"5-enums.ldtk",
"6-entity_with_enum.ldtk",
"7-entity_with_entityref.ldtk",
"99-topdown_example.ldtk"
    ));