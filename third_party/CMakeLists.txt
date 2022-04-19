add_subdirectory(json)

add_library(libreborn STATIC IMPORTED GLOBAL)
set_target_properties(libreborn PROPERTIES
					  IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/reborn/lib/libreborn_static_debug.lib
					  IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/reborn/lib/libreborn_static.lib)
target_include_directories(libreborn INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/reborn/include)