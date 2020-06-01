#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#include "ObjectAllocator.h"
#include "Allocator.h"

using namespace mp;

typedef std::basic_string<char, std::char_traits<char>, mp::CObjectAllocator<char, CAllocator> > SString;

inline long long GetTimeInMs() {
  struct timeval now;
  gettimeofday(&now, NULL);
  return static_cast<long long>(now.tv_sec * 1000 + now.tv_usec / 1000);
}

void TEST_Efficiency() {
    long long st = GetTimeInMs(); 
    for (int i = 0;i < 1024*1024*1024; ++i) {
        char* ch = new char[i%23+4];
        memset(ch, i%23+4, 0);
//        delete ch;
    }
    std::cout << "HS: " << GetTimeInMs() - st << std::endl;
    char ch = getchar();
    long long at = GetTimeInMs();
    mp::CObjectAllocator<char, CAllocator> *alloc = new mp::CObjectAllocator<char, CAllocator>();
    for (int i = 0;i < 1024*1024*1024; ++i) {
        char* ch = alloc->allocate(i%23+4);
        memset(ch, i%23+4, 0);
    }
    delete alloc;
    
    long long bt = GetTimeInMs();
    std::cout << "HS:" << bt - at << std::endl;
    ch = getchar();
}

void TEST_STL_Vector() {
    typedef std::vector<int, mp::CObjectAllocator<int, CAllocator> > ObjectVector;
    mp::CObjectAllocator<int, mp::CAllocator> alloc;
    ObjectVector vet(alloc);
    for (int i = 0;i < 100; ++i) {
        vet.push_back(i);
    }
}

int main(int argc, char* argv[])
{
    // TEST_Efficiency();
    TEST_STL_Vector();
    return 0;
}
