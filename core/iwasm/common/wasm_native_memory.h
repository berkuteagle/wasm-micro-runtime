#ifndef _WASM_NATIVE_MEMORY_H
#define _WASM_NATIVE_MEMORY_H

#include "bh_common.h"
#include "../include/wasm_export.h"
#include "../interpreter/wasm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NativeMemoriesNode {
    struct NativeMemoriesNode *next;
    const char *module_name;
    NativeMemory *native_memories;
    uint32 n_native_memories;
} NativeMemoryNode, *NativeMemoryList;

NativeMemory *
wasm_native_memory_resolve_memory(const char *module_name,
                                  const char *field_name);

bool
wasm_native_memory_register_memories(const char *module_name,
                                     NativeMemory *native_memories,
                                     uint32 n_native_memories);
bool
wasm_native_memory_unregister_memories(const char *module_name,
                                       NativeMemory *native_memories);

bool
wasm_native_memory_init(void);

void
wasm_native_memory_destroy(void);

#ifdef __cplusplus
}
#endif

#endif /* end of _WASM_NATIVE_MEMORY_H */
