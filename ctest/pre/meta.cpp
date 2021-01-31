#include "../doctest.h"
#include <list>
#include <vector>
#include <pre/random>
#include <pre/meta>

template <typename Link>
static void check_LinkList() {
    pre::LinkList<Link*> list;
    CHECK(list.empty());

    // Links.
    Link links[4];
    links[0].value = 3;
    links[1].value = 5;
    links[2].value = 7;
    links[3].value = 9;
    list.prepend(&links[0]);
    list.prepend(&links[1]);
    list.append(&links[2]);
    list.append(&links[3]);
    CHECK(list.size() == 4);

    // List links should appear in order 5, 3, 7, 9.
    auto pos = list.begin();
    CHECK(*pos++ == 5);
    CHECK(*pos++ == 3);
    CHECK(*pos++ == 7);
    CHECK(*pos++ == 9);
    CHECK(pos == list.end());

    // Remove.
    list.remove(std::next(list.begin(), 2));
    pos = list.begin();
    CHECK(*pos++ == 5);
    CHECK(*pos++ == 3);
    CHECK(*pos++ == 9);
    CHECK(pos == list.end());
}

TEST_CASE("Meta") {
    SUBCASE("IteratorRange") {
        pre::Pcg32 gen;
        std::list<int> vals0;
        std::vector<int> vals1;
        for (int j = 0; j < 256; j++) {
            vals0.emplace_back(gen());
            vals1.emplace_back(gen());
        }
        // Make ranges.
        auto range0 = pre::IteratorRange(vals0);
        auto range1 = pre::IteratorRange(vals1);
        auto itr0 = range0.begin();
        auto itr1 = range1.begin();
        CHECK(range0.size() == 256);
        CHECK(range1.size() == 256);
        // Also make ranges using iterators and pointers.
        auto also_range0 = pre::IteratorRange(vals0.begin(), vals0.end());
        auto also_range1 = pre::IteratorRange(&vals1[0], &vals1[0] + 256);
        auto also_itr0 = also_range0.begin();
        auto also_itr1 = also_range1.begin();
        CHECK(also_range0.size() == 256);
        CHECK(also_range1.size() == 256);
        for (int j = 0; j < 256; j++) {
            CHECK(*itr0 == *also_itr0);
            CHECK(*itr1 == *also_itr1);
            ++itr0;
            ++itr1;
            ++also_itr0;
            ++also_itr1;
        }
        CHECK(itr0 == range0.end());
        CHECK(itr1 == range1.end());
        CHECK(also_itr0 == also_range0.end());
        CHECK(also_itr1 == also_range1.end());
        // Indexing works even on list iterators, because the implementation
        // uses std::advance() to increment generically.
        int ind = gen(256);
        CHECK(range0[ind] == also_range0[ind]);
        CHECK(range1[ind] == also_range1[ind]);
    }
    SUBCASE("LinkList") {
        struct FwdLink {
            FwdLink* next = nullptr;
            int value = 0;
        };
        struct BidirLink {
            BidirLink* next = nullptr;
            BidirLink* prev = nullptr;
            int value = 0;
        };
        check_LinkList<FwdLink>();
        check_LinkList<BidirLink>();
    }
}
