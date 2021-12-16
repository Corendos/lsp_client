add_executable(lsp_client_tests)

target_sources(lsp_client_tests PRIVATE all.cpp)
target_link_libraries(lsp_client_tests PRIVATE libstd)

set_target_properties(lsp_client_tests PROPERTIES
                                 RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/tests
                                 PDB_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/tests)

if (DEFINED CMAKE_BUILD_TYPE)
set_target_properties(lsp_client PROPERTIES
                                 RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/tests/Debug
                                 PDB_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/tests/Debug
                                 RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/tests/Release
                                 PDB_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/tests/Release)
endif()

add_test(lsp_client_tests COMMAND lsp_client_tests)