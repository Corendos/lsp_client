/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_FILE_H
#define REBORN_FILE_H

#include <reborn/memory.h>
#include <reborn/strings.h>

typedef u32 FileAccessMode;
enum {
    FileAccessMode_Read = 0x01,
    FileAccessMode_Write = 0x02,
    FileAccessMode_ReadWrite = FileAccessMode_Read | FileAccessMode_Write,
};

typedef u32 FileOpenOption;
enum {
    FileOpenOption_CreateAlways,
    FileOpenOption_CreateNew,
    FileOpenOption_OpenAlways,
    FileOpenOption_OpenExisting,
    FileOpenOption_TruncateExisting,
};

struct File {
    u64 native_handle;
};

struct FileOpenResult {
    File file;
    bool success;
};

EXPORT FileOpenResult open_file(ConstStringU8 filename, FileAccessMode access_mode = FileAccessMode_Read,
                                FileOpenOption options = FileOpenOption_OpenAlways);
EXPORT FileOpenResult open_file(ConstStringU16 filename, FileAccessMode access_mode = FileAccessMode_Read,
                                FileOpenOption options = FileOpenOption_OpenAlways);

EXPORT void close_file(File* file);

EXPORT u64 get_file_size(File file);

EXPORT Buffer read_entire_file(Allocator* allocator, File file);
EXPORT bool write_to_file(File file, ConstBuffer buffer);

#endif // REBORN_FILE_H
