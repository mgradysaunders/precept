#include "../../doctest.h"
#include <pre/memory/tracking_allocator>
#include <pre/data_structures/adjacency_lists>

TEST_CASE("AdjacencyLists") {
    enum Node { Foo, Bar, Baz, Qux };

    pre::TrackingAllocator<Node> alloc;
    {
        pre::AdjacencyLists<Node, pre::TrackingAllocator<Node>> lists(alloc);

        // Connections.
        /* Foo------Bar------Qux-------+
            |        |        |        |
            |        |        |        |
           Baz-------+        +--------+ */
        lists.connect(Foo, {Bar, Baz});
        lists.connect(Bar, {Qux, Baz, Foo});
        lists.connect(Qux, Qux);
        lists.disconnect(Baz, Foo);
        lists.disconnect(Baz, Bar);
        lists.connect(Baz, Bar);
        lists.connect(Foo, Baz);

        // Foo is neighbors with Baz and Bar.
        CHECK(lists[Foo].size() == 2);
        CHECK(lists[Foo].contains(Baz));
        CHECK(lists[Foo].contains(Bar));

        // Bar is neighbors with Foo, Baz, and Qux.
        CHECK(lists[Bar].size() == 3);
        CHECK(lists[Bar].contains(Foo));
        CHECK(lists[Bar].contains(Baz));
        CHECK(lists[Bar].contains(Qux));

        // Baz is neighbors with Bar and Foo.
        CHECK(lists[Baz].size() == 2);
        CHECK(lists[Baz].contains(Bar));
        CHECK(lists[Baz].contains(Foo));

        // Qux is neighbors with Bar and Qux.
        CHECK(lists[Qux].size() == 2);
        CHECK(lists[Qux].contains(Bar));
        CHECK(lists[Qux].contains(Qux));

        lists.clear();
    }

    // No dangling allocations.
    CHECK(alloc.info().allocate_calls == alloc.info().deallocate_calls);
    CHECK(alloc.info().allocation == 0);
}

// Use this instead of string because MacOS compiler does
// not know how to link to its own standard library.
enum EdgeName { Foo, Bar, Baz, Baz1, Baz2, Loop };

TEST_CASE("UndirectedEdgeList") {

    pre::UndirectedEdgeList<int, EdgeName> edges;

    // Check that edges is empty by default.
    CHECK(edges.empty());

    // Initialize.
    /* 0----Foo----1--------+
       |           |        |
       |           |        |
      Bar          +--Loop--+
       |
       |
       3----Baz-------------2 */
    edges(0, 1) = Foo;
    edges(0, 3) = Bar;
    edges(3, 2) = Baz;
    edges(1, 1) = Loop;

    // Check that size() is number of undirected edges.
    CHECK(edges.size() == 4);

    // Check that edges contains (0, 1).
    CHECK(edges.contains({0, 1}));

    // Check that edges contains (0, 3).
    CHECK(edges.contains({0, 3}));

    // Check that edges contains (3, 2).
    CHECK(edges.contains({3, 2}));

    // Check that edges contains (1, 0) because direction doesn't matter.
    CHECK(edges.contains({1, 0}));

    // Check that edges contains (3, 0) because direction doesn't matter.
    CHECK(edges.contains({3, 0}));

    // Check that edges contains (2, 3) because direction doesn't matter.
    CHECK(edges.contains({2, 3}));

    // Check that edges contains (1, 1).
    CHECK(edges.contains({1, 1}));

    // Check that edges does not contain (0, 2).
    CHECK(edges.contains({0, 2}) == false);

    // Check that at() throws if edge doesn't exist.
    CHECK_THROWS(edges.at({0, 2}));

    // Check that (0, 1) is Foo.
    CHECK(edges(0, 1) == Foo);

    // Check that (3, 0) is Bar.
    CHECK(edges(3, 0) == Bar);

    // Check that (2, 3) is Baz.
    CHECK(edges(2, 3) == Baz);

    // Check that (1, 1) is Loop.
    CHECK(edges(1, 1) == Loop);

    edges.clear();

    // Check that edges is empty after clear.
    CHECK(edges.empty());
}

TEST_CASE("DirectedEdgeList") {
    pre::DirectedEdgeList<int, EdgeName> edges;

    // Check that edges is empty by default.
    CHECK(edges.empty());

    // Initialize.
    /* 0----Foo--->1--------+
       ^           |        |
       |           |        |
      Bar          +--Loop--+
       |
       |
       3----Baz1----------->2
        <---Baz2------------  */
    edges(0, 1) = Foo;
    edges(3, 0) = Bar;
    edges(3, 2) = Baz1;
    edges(2, 3) = Baz2;
    edges(1, 1) = Loop;

    // Check that size() is number of directed edges.
    CHECK(edges.size() == 5);

    // Check that edges contains (0, 1).
    CHECK(edges.contains({0, 1}));

    // Check that edges contains (3, 0).
    CHECK(edges.contains({3, 0}));

    // Check that edges contains (3, 2).
    CHECK(edges.contains({3, 2}));

    // Check that edges contains (2, 3).
    CHECK(edges.contains({2, 3}));

    // Check that edges does not contain (1, 0).
    CHECK(edges.contains({1, 0}) == false);

    // Check that edges does not contain (0, 3).
    CHECK(edges.contains({0, 3}) == false);

    // Check that at() throws if edge doesn't exist.
    CHECK_THROWS(edges.at({1, 0}));

    // Check that (0, 1) is Foo.
    CHECK(edges(0, 1) == Foo);

    // Check that (3, 0) is Bar.
    CHECK(edges(3, 0) == Bar);

    // Check that (3, 2) is Baz1.
    CHECK(edges(3, 2) == Baz1);

    // Check that (2, 3) is Baz2.
    CHECK(edges(2, 3) == Baz2);

    // Check that (1, 1) is Loop.
    CHECK(edges(1, 1) == Loop);

    // Erase (2, 3).
    edges.erase(edges.find({2, 3}));

    // Check that edges does not contain (2, 3).
    CHECK(edges.contains({2, 3}) == false);

    // Check that at() throws if edge does not exist.
    CHECK_THROWS(edges.at({2, 3}));

    // Clear.
    edges.clear();

    // Check that edges is empty after clear.
    CHECK(edges.empty());
}
