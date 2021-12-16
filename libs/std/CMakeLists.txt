add_library(libstd)

target_include_directories(libstd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(libstd PRIVATE src/strings.cpp src/memory.cpp)
set_target_properties(libstd PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if (DEFINED CMAKE_BUILD_TYPE)
set_target_properties(libstd PROPERTIES
                          ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/Debug
                          ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release)
endif()
