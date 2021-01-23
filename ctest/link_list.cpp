#include "doctest.h"
#include <pre/data_structures/link_list>

struct FwdLink {
    FwdLink* next = nullptr;
    int value = 0;
    constexpr auto* value_ptr() noexcept {
        return &value;
    }
    constexpr auto* value_ptr() const noexcept {
        return &value;
    }
};

struct BidirLink {
    BidirLink* next = nullptr;
    BidirLink* prev = nullptr;
    int value = 0;
    constexpr auto* value_ptr() noexcept {
        return &value;
    }
    constexpr auto* value_ptr() const noexcept {
        return &value;
    }
};

TEST_CASE_TEMPLATE("LinkList", Link, FwdLink, BidirLink) {
    pre::LinkList<Link*> list;

    // List should be empty by default.
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

    // List size should be 4.
    CHECK(list.size() == 4);

    // List links should appear in order 5, 3, 7, 9.
    auto pos = list.begin();
    CHECK(*pos++ == 5);
    CHECK(*pos++ == 3);
    CHECK(*pos++ == 7);
    CHECK(*pos++ == 9);

    // Position should be at end.
    CHECK(pos == list.end());

    list.remove(std::next(list.begin(), 2));
    pos = list.begin();
    CHECK(*pos++ == 5);
    CHECK(*pos++ == 3);
    CHECK(*pos++ == 9);

    // Position should be at end.
    CHECK(pos == list.end());
}
