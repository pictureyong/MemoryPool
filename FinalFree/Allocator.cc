#include "Allocator.h"

namespace mp {

static const int kBlockSize = 8*4096;

CAllocator::CAllocator() : _memory_usage(0) {
    std::cout << "CAllocator()" << std::endl;
    _alloc_ptr = nullptr;
    _alloc_bytes_remaining = 0;
}

CAllocator::~CAllocator() {
    std::cout << "~CAllocator()" << std::endl;
    for (size_t i = 0;i < _blocks.size(); ++i) {
//        delete[] _blocks[i];
        free(_blocks[i]);
    }
}

void* CAllocator::AllocateFallback(size_t bytes) {
    if (bytes > kBlockSize / 4) {
        void* result = AllocateNewBlock(bytes);
        return result;
    }
    _alloc_ptr = AllocateNewBlock(kBlockSize);
    _alloc_bytes_remaining = kBlockSize;
    return Allocate(bytes);
}

void* CAllocator::AllocateAligned(size_t bytes) {
    const int align = (sizeof(void*) > 8) ? sizeof(void*) : 8;
    assert(align & (align-1) == 0);
    size_t current_mod = reinterpret_cast<uintptr_t>(_alloc_ptr) & (align-1);
    size_t slop = (current_mod == 0 ? 0 : align - current_mod);
    size_t needed = bytes + slop;
    void* result;
    if (needed <= _alloc_bytes_remaining) {
        result = _alloc_ptr + slop;
        _alloc_ptr += needed;
        _alloc_bytes_remaining -= needed;
    } else {
        result = AllocateFallback(bytes);
    }
    assert((reinterpret_cast<uintptr_t>(result) & (align-1)) == 0);
    return result;
}

char* CAllocator::AllocateNewBlock(size_t block_bytes) {
//    std::cout << "AllocateNewBlock" << std::endl;
    char* result = new char[block_bytes];
    // void* result = malloc(block_bytes);
    _blocks.push_back(result);
    _memory_usage.fetch_add(block_bytes + sizeof(char*), 
            std::memory_order_relaxed);
    return result;
}

}
