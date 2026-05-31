#include "wasm_native_memory.h"

static NativeMemoryList g_native_memories_list = NULL;

static int
native_memory_cmp(const void *native_memory1, const void *native_memory2)
{
    return strcmp(((const NativeMemory *)native_memory1)->memory,
                  ((const NativeMemory *)native_memory2)->memory);
}

static NativeMemory *
lookup_native_memory(NativeMemory *native_memories, uint32 n_native_memories,
                     const char *memory)
{
    NativeMemory *native_memory, key = { 0 };

    key.memory = memory;

    if ((native_memory = bsearch(&key, native_memories, n_native_memories,
                                 sizeof(NativeSymbol), native_memory_cmp))) {
        return native_memory;
    }

    return NULL;
}

NativeMemory *
wasm_native_memory_resolve_memory(const char *module_name,
                                  const char *field_name)
{
    NativeMemoryList node, node_next;
    NativeMemory *native_memory;

    node = g_native_memories_list;
    while (node) {
        node_next = node->next;
        if (!strcmp(node->module_name, module_name)) {
            if ((native_memory =
                     lookup_native_memory(node->native_memories,
                                          node->n_native_memories, field_name)))
                break;
        }
        node = node_next;
    }

    return native_memory;
}

bool
wasm_native_memory_register_memories(const char *module_name,
                                     NativeMemory *native_memories,
                                     uint32 n_native_memories)
{
    NativeMemoryNode *node;

    if (!(node = wasm_runtime_malloc(sizeof(NativeMemoryNode))))
        return false;

#if WASM_ENABLE_MEMORY_TRACING != 0
    LOG_VERBOSE("Register native memory, size: %u", sizeof(NativeMemoryNode));
#endif

    node->module_name = module_name;
    node->native_memories = native_memories;
    node->n_native_memories = n_native_memories;

    /* Add to list head */
    node->next = g_native_memories_list;
    g_native_memories_list = node;

    qsort(native_memories, n_native_memories, sizeof(NativeMemory),
          native_memory_cmp);

    return true;
}

bool
wasm_native_memory_unregister_memories(const char *module_name,
                                       NativeMemory *native_memories)
{

    NativeMemoryNode **prevp;
    NativeMemoryNode *node;

    prevp = &g_native_memories_list;
    while ((node = *prevp) != NULL) {
        if (node->native_memories == native_memories
            && !strcmp(node->module_name, module_name)) {
            *prevp = node->next;
            wasm_runtime_free(node);
            return true;
        }
        prevp = &node->next;
    }
    return false;
}

bool
wasm_native_memory_init()
{
    return true;
}

void
wasm_native_memory_destroy()
{
    NativeMemoryNode *node, *node_next;

    node = g_native_memories_list;
    while (node) {
        node_next = node->next;
        wasm_runtime_free(node);
        node = node_next;
    }

    g_native_memories_list = NULL;
}
