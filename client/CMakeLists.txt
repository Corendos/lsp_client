add_executable(lsp_client)

target_sources(lsp_client PRIVATE src/main.cpp src/lsp/types.cpp)
target_link_libraries(lsp_client PRIVATE libstd libjson)
target_include_directories(lsp_client PRIVATE include)
set_target_properties(lsp_client PROPERTIES
                                 RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
                                 PDB_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

if (DEFINED CMAKE_BUILD_TYPE)
set_target_properties(lsp_client PROPERTIES
                                 RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/Debug
                                 PDB_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/Debug
                                 RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/Release
                                 PDB_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/Release)
endif()
