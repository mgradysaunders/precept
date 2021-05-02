#include "../doctest.h"
#include <vector>
#include <pre/design_patterns/ref>
#include <pre/design_patterns/preparable>

TEST_CASE("Preparable") {
    // Call counters
    static int Foo_prepare_calls = 0;
    static int Foo_unprepare_calls = 0;

    class Foo final : virtual public pre::RefCountable,
                      virtual public pre::Preparable<int, char> {
      public:
        void on_prepare(int i, char c) {
            Foo_prepare_calls++;
            for (auto& foo : foos)
                foo->prepare(i, c);
        }
        void on_unprepare() {
            Foo_unprepare_calls++;
            for (auto& foo : foos)
                foo->unprepare();
        }

        std::vector<pre::Ref<Foo>> foos;
    };

    // Some foos
    pre::Ref foo1(new Foo());
    pre::Ref foo2(new Foo());
    pre::Ref foo3(new Foo());
    pre::Ref foo4(new Foo());
    foo1->foos = {foo2};
    foo2->foos = {foo3, foo4};
    foo4->foos = {foo1, foo2};

    // Prepare foo1, which prepares foo2, foo3, foo4
    foo1->prepare(2, 'a');
    CHECK(Foo_prepare_calls == 4);

    // Prepare foo3 again
    foo3->prepare(2, 'a');
    CHECK(Foo_prepare_calls == 4); // No effect, it is already prepared

    // Unprepare foo3
    foo3->unprepare();
    CHECK(Foo_unprepare_calls == 1);

    // Unprepare foo1, which unprepares foo2, foo4
    foo1->unprepare();
    CHECK(Foo_unprepare_calls == 4);

    // Unprepare foo1 again
    foo1->unprepare();
    CHECK(Foo_unprepare_calls == 4); // No effect, it is already unprepared
}
