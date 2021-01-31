#include "doctest.h"
#include <pre/random>
#include <pre/memory/tracking_allocator>
#include <pre/data_structures/kd_tree>

static int Foo_ctor_counter = 0;
static int Foo_dtor_counter = 0;
struct Foo {
    int value;
    Foo(int v) : value(v) {
        ++Foo_ctor_counter;
    }
    Foo(const Foo& foo) : value(foo.value) {
        ++Foo_ctor_counter;
    }
    ~Foo() {
        ++Foo_dtor_counter;
    }
};

TEST_CASE_TEMPLATE("KdTree2", Float, float, double) {
    pre::Pcg32 gen(getContextOptions()->rand_seed);
    pre::TrackingAllocator<Foo> alloc;
    {
        pre::KdTree2<Float, Foo, pre::TrackingAllocator<Foo>> tree(alloc);

        // Add 512 points.
        for (int value = 0; value < 512; value++)
            tree.insert(
                    {{pre::generate_canonical<Float>(gen) * 8 - 4,
                      pre::generate_canonical<Float>(gen) * 8 - 4},
                     Foo(value)});

        // Check that size is 512.
        CHECK(tree.size() == 512);

        // Rebalance.
        tree.rebalance();

        // Check that size is still 512.
        CHECK(tree.size() == 512);

        // Check that all points within 6 units (true by construction).
        CHECK(tree.count_nearby({0, 0}, 6) == 512);

        // Check that no points within 0 units.
        CHECK(tree.count_nearby({0, 0}, 0) == 0);

        // Sum.
        int sum_before = 0;
        for (const auto& kv : tree)
            sum_before += kv.second.value;

        // Sum should be 1 + 2 + ... + 511 = 130816.
        CHECK(sum_before == 130816);

        // Random search area.
        pre::Vec2<Float> center = {
                pre::generate_canonical<Float>(gen) * 8 - 4,
                pre::generate_canonical<Float>(gen) * 8 - 4};
        Float radius = pre::generate_canonical<Float>(gen) * 4;

        // Sum erased values.
        int sum_erased = 0;
        tree.visit_nearby(center, radius, [&](auto pos) {
            sum_erased += pos->second.value;
            return true;
        });

        // Erase.
        size_t num_erased = tree.erase_nearby(center, radius);

        // Check that nearest to each is itself.
        for (auto pos = tree.begin(); pos != tree.end(); ++pos)
            CHECK(pos == tree.nearest(pos->first));

        // Check that erase removed nodes.
        CHECK(tree.size() + num_erased == 512);
        CHECK(tree.count_nearby(center, radius) == 0);

        // Rebalance again.
        tree.rebalance();

        // Sum again.
        int sum_after = 0;
        for (const auto& kv : tree)
            sum_after += kv.second.value;

        // Sum should be the sum before minus the sum erased.
        CHECK(sum_after == sum_before - sum_erased);

        // Clear.
        tree.clear();

        // Check that size returns to 0.
        CHECK(tree.size() == 0);
    }

    // No dangling allocations.
    CHECK(Foo_ctor_counter == Foo_dtor_counter);
    CHECK(alloc.info().allocate_calls == alloc.info().deallocate_calls);
    CHECK(alloc.info().allocation == 0);
}

TEST_CASE_TEMPLATE("KdTree3", Float, float, double) {
    pre::Pcg32 gen(getContextOptions()->rand_seed);
    pre::TrackingAllocator<Foo> alloc;
    {
        pre::KdTree3<Float, Foo, pre::TrackingAllocator<Foo>> tree(alloc);

        // Add 512 points.
        for (int value = 0; value < 512; value++)
            tree.insert(
                    {{pre::generate_canonical<Float>(gen) * 8 - 4,
                      pre::generate_canonical<Float>(gen) * 8 - 4,
                      pre::generate_canonical<Float>(gen) * 8 - 4},
                     Foo(value)});

        // Check that size is 512.
        CHECK(tree.size() == 512);

        // Rebalance.
        tree.rebalance();

        // Check that size is still 512.
        CHECK(tree.size() == 512);

        // Check that all points within 7 units (true by construction).
        CHECK(tree.count_nearby({0, 0}, 7) == 512);

        // Check that no points within 0 units.
        CHECK(tree.count_nearby({0, 0}, 0) == 0);

        // Sum.
        int sum_before = 0;
        for (const auto& kv : tree)
            sum_before += kv.second.value;

        // Sum should be 1 + 2 + ... + 511 = 130816.
        CHECK(sum_before == 130816);

        // Random search area.
        pre::Vec3<Float> center = {
                pre::generate_canonical<Float>(gen) * 8 - 4,
                pre::generate_canonical<Float>(gen) * 8 - 4,
                pre::generate_canonical<Float>(gen) * 8 - 4};
        Float radius = pre::generate_canonical<Float>(gen) * 4;

        // Sum erased values.
        int sum_erased = 0;
        tree.visit_nearby(center, radius, [&](auto pos) {
            sum_erased += pos->second.value;
            return true;
        });

        // Erase.
        tree.erase_nearby(center, radius);

        // Check that nearest to each is itself.
        for (auto pos = tree.begin(); pos != tree.end(); ++pos)
            CHECK(pos == tree.nearest(pos->first));

        // Check that erase removed nodes.
        CHECK(tree.count_nearby(center, radius) == 0);

        // Rebalance again.
        tree.rebalance();

        // Sum again.
        int sum_after = 0;
        for (const auto& kv : tree)
            sum_after += kv.second.value;

        // Sum should be the sum before minus the sum erased.
        CHECK(sum_after == sum_before - sum_erased);

        // Clear.
        tree.clear();

        // Check that size returns to 0.
        CHECK(tree.size() == 0);
    }

    // No dangling allocations.
    CHECK(Foo_ctor_counter == Foo_dtor_counter);
    CHECK(alloc.info().allocate_calls == alloc.info().deallocate_calls);
    CHECK(alloc.info().allocation == 0);
}
