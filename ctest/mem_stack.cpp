#include "doctest.h"
#include <pre/random>
#include <pre/memory/tracking_allocator>
#include <pre/memory/mem_stack>

static void random_allocs(pre::Pcg32& gen, auto& mem, int depth = 0) {
    if (depth > 6)
        return;
    int n = gen(8) + 8;
    for (int j = 0; j < n; j++) {
        if (gen(2) == 0) {
            mem.push();
            random_allocs(gen, mem, depth + 1);
            mem.pop();
        }
        else {
            for (int k = 0; k < 4; k++) {
                void* ptr = mem.allocate(gen(16) + 1);
                (void)ptr;
            }
        }
    }
}

TEST_CASE("MemStack") {
    pre::Pcg32 gen(getContextOptions()->rand_seed);
    pre::TrackingAllocator<std::byte> alloc;
    {
        pre::MemStack<pre::TrackingAllocator<std::byte>> mem(64, alloc);
        random_allocs(gen, mem);
    }
    CHECK(alloc.info().allocate_calls == alloc.info().deallocate_calls);
    CHECK(alloc.info().allocation == 0);
}
