add_library(libjson)

target_include_directories(libjson PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(libjson PRIVATE src/json.c src/value.c src/json-dom.c)
set_target_properties(libjson PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/third_party/json)

if (DEFINED CMAKE_BUILD_TYPE)
set_target_properties(libjson PROPERTIES
                          ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/Debug/third_party/json
                          ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release/third_party/json)
endif()
