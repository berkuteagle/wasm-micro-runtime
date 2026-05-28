#ifndef _WASM_NATIVE_MEMORY_H
#define _WASM_NATIVE_MEMORY_H

#include "bh_common.h"
#include "../include/wasm_export.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NativeMemoriesNode {
    struct NativeMemoriesNode *next;
    const char *module_name;
    NativeMemory *native_memories;
    uint32 n_native_memories;
} NativeMemoryNode, *NativeMemoryList;

bool
wasm_native_memory_register_memories(const char *module_name,
                                     NativeMemory *native_memories,
                                     uint32 n_native_memories);
bool
wasm_native_memory_unregister_memories(const char *module_name,
                                       NativeMemory *native_memories);

#ifdef __cplusplus
}
#endif

#endif /* end of _WASM_NATIVE_MEMORY_H */
