add_executable(tests_main test_parser.cpp)
target_link_libraries(tests_main PRIVATE ldtk_parser ldtk_parser_io GTest::gtest_main)

gtest_discover_tests(tests_main WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)
