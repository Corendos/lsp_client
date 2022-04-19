#include <lsp/request_hash_table.h>

RequestHashTable make_request_hash_table(Allocator* allocator, u64 capacity) {
    RequestHashTable hash_table = {0};
    hash_table.size = capacity;
    
    u64 entries_allocation_size = capacity * sizeof(RequestHashTableEntries*);
    hash_table.entries = (RequestHashTableEntries**)allocate(allocator, entries_allocation_size);
    
    return hash_table;
}

void destroy_request_hash_table(Allocator* allocator, RequestHashTable* hash_table) {
    for (u64 i = 0;i < hash_table->size;++i) {
        RequestHashTableEntries* current = hash_table->entries[i];
        while (current) {
            RequestHashTableEntries* next = current->next;
            free(allocator, current);
            current = next;
        }
    }
    
    free(allocator, hash_table->entries);
    hash_table->entries = 0;
    hash_table->size = 0;
}

bool add_request_hash_table(Allocator* allocator, RequestHashTable* hash_table, u64 request_id, ClientRequestType request_type) {
    u64 bin = request_id % hash_table->size;
    
    RequestHashTableEntries* new_node = (RequestHashTableEntries*)allocate(allocator, sizeof(RequestHashTableEntries));
    new_node->request_id = request_id;
    new_node->request_type = request_type;
    new_node->next = hash_table->entries[bin];
    hash_table->entries[bin] = new_node;
    
    return true;
}

void remove_request_hash_table(Allocator* allocator, RequestHashTable* hash_table, u64 request_id) {
    u64 bin = request_id % hash_table->size;
    
    RequestHashTableEntries* previous = 0;
    RequestHashTableEntries* current = hash_table->entries[bin];
    
    while (current) {
        if (current->request_id == request_id) {
            if (previous != 0) {
                previous->next = current->next;
                free(allocator, current);
            } else {
                hash_table->entries[bin] = current->next;
            }
            
            return;
        }
        previous = current;
        current = current->next;
    }
}

RequestHashTableLookupResult lookup_request_hash_table(RequestHashTable* hash_table, u64 request_id) {
    RequestHashTableLookupResult result = {0};
    
    u64 bin = request_id % hash_table->size;
    RequestHashTableEntries* current = hash_table->entries[bin];
    
    while (current) {
        if (current->request_id == request_id) {
            result.found = true;
            result.request_type = current->request_type;
            return result;
        }
        current = current->next;
    }
    
    return result;
}