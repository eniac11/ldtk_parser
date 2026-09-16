cmake_minimum_required(VERSION 3.30)
project(ldtk_parser VERSION 0.1.0)

include(FetchContent)
include(FeatureSummary)
include(CMakeDependentOption)
# For enable_testing option
include(CTest)

option(LDTK_PARSER_BUILD_EXAMPLES "Build ldtk_parser examples" OFF)
option(LDTK_PARSER_INSTALL "Create install artifacts" ON)


cmake_dependent_option(LDTK_PARSER_BUILD_TESTING "Build parser tests" ON "BUILD_TESTING" OFF)



include(GoogleTest)


find_package(GTest 1.14 CONFIG)
set_package_properties(GTest PROPERTIES TYPE OPTIONAL)
find_package(glm 0.9.9)
set_package_properties(glm PROPERTIES TYPE REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS  OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
cmake_policy(SET CMP0155 OLD)
unset(CMAKE_CXX_SCANDEP_SOURCE)
unset(CMAKE_CXX_MODULE_MAP_FORMAT)
unset(CMAKE_CXX_MODULE_MAP_FLAG)
unset(CMAKE_CXX_MODULE_BMI_ONLY_FLAG)

find_package(jsoncpp 1.9 CONFIG)
set_package_properties(jsoncpp PROPERTIES TYPE REQUIRED)

FetchContent_Declare(
        beman_indirect
        GIT_REPOSITORY https://github.com/bemanproject/indirect.git
        GIT_TAG 36579dd72730156afed6247abf24835a222d7368
        FIND_PACKAGE_ARGS 0.1.0 NAMES beman.indirect
)
FetchContent_MakeAvailable(beman_indirect)
set_package_properties(beman.indirect PROPERTIES TYPE REQUIRED)

# Feature Info
add_feature_info(ldtk_parser.examples "LDTK_PARSER_BUILD_EXAMPLES" "Build ldtk_parser examples")
add_feature_info(ldtk_parser.tests "LDTK_PARSER_BUILD_TESTING AND GTest_FOUND" "Build ldtk_parser tests")

add_subdirectory(utils)

add_library(ldtk_parser)
target_link_libraries(ldtk_parser PUBLIC  beman::indirect glm::glm utils)
target_compile_features(ldtk_parser PUBLIC cxx_std_23)

target_sources(ldtk_parser
    PUBLIC
    FILE_SET public_headers
    TYPE HEADERS
    BASE_DIRS include
    FILES
        include/ldtk_parser/defs/entity_def.h
        include/ldtk_parser/defs/layer_def.h
        include/ldtk_parser/defs/field_def.h
        include/ldtk_parser/defs/enum_def.h
        include/ldtk_parser/project.h
        include/ldtk_parser/world.h
        include/ldtk_parser/layer.h
        include/ldtk_parser/level.h
        include/ldtk_parser/entity.h
        include/ldtk_parser/field.h
        include/ldtk_parser/tile.h
        include/ldtk_parser/types.h
        include/ldtk_parser/reader.h
    PRIVATE
        src/project.cpp
        src/world.cpp
        src/layer.cpp
        src/level.cpp
        src/entity.cpp
        src/types.cpp
        src/defs/entity_def.cpp
        src/defs/layer_def.cpp
        src/parse_types.h

#        src/reader.cpp
)

add_library(ldtk_parser_io)
target_sources(ldtk_parser_io
        PRIVATE
        src/reader.cpp
)
target_link_libraries(ldtk_parser_io PUBLIC jsoncpp_lib ldtk_parser)

if (LDTK_PARSER_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (LDTK_PARSER_BUILD_TESTING AND GTest_FOUND)
    add_subdirectory(tests)
endif()

if (LDTK_PARSER_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    configure_package_config_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/ldtk_parserConfig.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/ldtk_parserConfig.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ldtk_parser
    )

    write_basic_package_version_file(
            ${CMAKE_CURRENT_BINARY_DIR}/ldtk_parserVersion.cmake
            COMPATIBILITY SameMajorVersion
    )

    install(TARGETS ldtk_parser ldtk_parser_io utils EXPORT ldtk_parserTargets
            RUNTIME
            LIBRARY
            FILE_SET public_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    install(EXPORT ldtk_parserTargets
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ldtk_parser
            NAMESPACE ldtk_parser::
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ldtk_parserConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/ldtk_parserVersion.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ldtk_parser

    )
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)