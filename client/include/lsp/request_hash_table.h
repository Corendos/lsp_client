#ifndef REQUEST_HASH_TABLE_H
#define REQUEST_HASH_TABLE_H

#include <reborn/types.h>
#include <reborn/memory/allocator.h>
#include <lsp/types.h>

struct RequestHashTableEntries {
    u64 request_id;
    ClientRequestType request_type;
    struct RequestHashTableEntries* next;
};

struct RequestHashTable {
    u64 size;
    RequestHashTableEntries** entries;
};

struct RequestHashTableLookupResult {
    bool found;
    ClientRequestType request_type;
};

RequestHashTable make_request_hash_table(Allocator* allocator, u64 capacity);
void destroy_request_hash_table(Allocator* allocator, RequestHashTable* hash_table);

bool add_request_hash_table(Allocator* allocator, RequestHashTable* hash_table, u64 request_id, ClientRequestType request_type);
void remove_request_hash_table(Allocator* allocator, RequestHashTable* hash_table, u64 request_id);

RequestHashTableLookupResult lookup_request_hash_table(RequestHashTable* hash_table, u64 request_id);

#endif //REQUEST_HASH_TABLE_H
