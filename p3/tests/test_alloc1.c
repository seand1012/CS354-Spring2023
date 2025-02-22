// a simple 8 byte allocation
#include <assert.h>
#include <stdlib.h>
#include "p3Heap.h"

int main() {
    assert(init_heap(4096) == 0);
    disp_heap();
    void* ptr = balloc(8);
    disp_heap();
    assert(ptr != NULL);
    exit(0);
}
