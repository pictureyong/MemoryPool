// Author : lzy
// Email  : liuzhengyong@trio.ai
// Date   : 2020-05-30 21:30
// Description: 

#ifndef _OBJECT_ALLOCATOR_H__
#define _OBJECT_ALLOCATOR_H__

#include <cstddef>
#include <limits>
#include <utility>
#include <cassert>
#include <memory>

namespace mp {

template <typename T, class Allocator>
class CObjectAllocator {
public:
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef T           value_type;

    template <class T1> struct rebind {
        typedef CObjectAllocator<T1, Allocator> other;
    };

    CObjectAllocator() {
        _alloc.reset( new Allocator() );
    }
    
    CObjectAllocator(const CObjectAllocator&) = default;
    ~CObjectAllocator() = default;

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }

    pointer allocate(size_type n, const void* = 0) {
        assert((n * sizeof(T)) / sizeof(T) == n);
        assert(_alloc != nullptr);
        return static_cast<T*>(_alloc->Allocate(n*sizeof(T)));
    }

    void deallocate(pointer p, size_type n) {
        assert(_alloc != nullptr);
        _alloc->Free(p, n*sizeof(T));
    }

    size_type max_size() {
        return size_t(-1) / sizeof(T);
    }
    
    template <class T1, class... Args>
    void construct (T1* p, Args&&... args) {
        ::new ((void*)p) T1 (std::forward<Args>(args)...);
    }

    void destroy(pointer p) {
        p->~T();
    }

    bool operator==(const CObjectAllocator&) const {
        return true;
    }

private:
    std::shared_ptr<Allocator>  _alloc;
};

}

#endif
