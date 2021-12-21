cmake_minimum_required(VERSION 3.15)
project(lsp_client LANGUAGES C CXX)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(PLATFORM_DEFINES "-DPLATFORM_WIN32")
else()
	message(FATAL_ERROR "Missing platform defines for platform \"${CMAKE_SYSTEM_NAME}\"")
endif()

add_subdirectory(third_party)
add_subdirectory(libs)
add_subdirectory(client)
add_subdirectory(tests)