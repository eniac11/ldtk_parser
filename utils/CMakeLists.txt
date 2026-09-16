set_property(SOURCE sha1.h PROPERTY LANGUAGE CXX)
set_property(SOURCE uuid.h PROPERTY LANGUAGE CXX)
add_library(utils INTERFACE)
target_sources(utils
        PUBLIC
        FILE_SET public_headers
        TYPE HEADERS
        FILES
        sha1.h
        uuid.h
)
#target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})