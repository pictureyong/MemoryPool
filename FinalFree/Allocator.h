// Author : lzy
// Email  : liuzhengyong@trio.ai
// Date   : 2020-05-30 20:51
// Description: 

#ifndef _ALLOCATOR_H__
#define _ALLOCATOR_H__

#include <atomic>
#include <cstddef>
#include <cassert>
#include <vector>

namespace mp {

class CAllocator {
public:
    CAllocator();
    ~CAllocator();

    void* Allocate(size_t bytes);
    void Free(void* p, size_t) {;}
    void* AllocateAligned(size_t bytes);

    size_t MemoryUsage() const {
        return _memory_usage.load(std::memory_order_relaxed);
    }

private:
    CAllocator(const CAllocator&);
    void operator=(const CAllocator&);

private:
    void* AllocateFallback(size_t bytes);
    char* AllocateNewBlock(size_t block_bytes);

    char* _alloc_ptr;
    size_t _alloc_bytes_remaining;
    std::vector<char*> _blocks;
    std::atomic<size_t> _memory_usage;
};

inline void* CAllocator::Allocate(size_t bytes) {
    assert(bytes > 0);
    if (bytes < _alloc_bytes_remaining) {
        void* result = _alloc_ptr;
        _alloc_ptr += bytes;
        _alloc_bytes_remaining -= bytes;
        return result;
    }
    return AllocateFallback(bytes);
}

}

#endif
