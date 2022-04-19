/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_PATHS_H
#define REBORN_PATHS_H

#include <reborn/memory.h>
#include <reborn/strings.h>

DECLARE_RESULT(Size, u64, size);

EXPORT ConstStringU8 get_current_working_directory(Allocator* allocator);
EXPORT ConstStringU8 get_executable_path(Allocator* allocator);
EXPORT bool change_directory(ConstStringU8 path);

EXPORT ConstStringU8 join_paths(ConstStringU8Array paths);

EXPORT bool is_directory(ConstStringU8 path);
EXPORT bool is_file(ConstStringU8 path);
EXPORT bool is_absolute(ConstStringU8 path);
EXPORT bool is_relative(ConstStringU8 path);
EXPORT bool exists(ConstStringU8 path);

EXPORT SizeResult get_size(ConstStringU8 path);
EXPORT ConstStringU8Result get_absolute_path(Allocator* allocator, ConstStringU8 path);
EXPORT ConstStringU8Result get_base_name(Allocator* allocator, ConstStringU8 path);
EXPORT ConstStringU8Result get_common_path(Allocator* allocator, ConstStringU8Array paths);
EXPORT ConstStringU8Result get_common_prefix(Allocator* allocator, ConstStringU8Array paths);
EXPORT ConstStringU8 get_directory_name(Allocator* allocator, ConstStringU8 path);

#endif // REBORN_PATHS_H
