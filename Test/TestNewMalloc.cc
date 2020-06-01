#include <memory>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void TestMalloc() {
    void* p = malloc(1024*1024*1024);
}

void TestNew() {
    char* p = new char[1024*1024*1024];
}

void TestNewConstruct() {
    char* p = new char[1024*1024*1024]();
}

int main(int argc, char* argv[])
{
    TestMalloc(); // VIRT
    char ch = getchar();
    TestNew(); // VIRT
    ch = getchar();
    TestNewConstruct(); // RES
    ch = getchar();
    return 0;
}
