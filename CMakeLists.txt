cmake_minimum_required(VERSION 3.15)
project(lsp_client LANGUAGES C CXX)

add_subdirectory(third_party)
add_subdirectory(libs)
add_subdirectory(client)
add_subdirectory(tests)