#include "doctest.h"
#include <pre/random>
#include <pre/shapes/bound_box>

TEST_CASE_TEMPLATE("BoundBox2", Float, float, double) {
    pre::Pcg32 gen(getContextOptions()->rand_seed);
    pre::BoundBox2<Float> box;

    // Box invalid by default.
    CHECK(!box);

    for (int value = 0; value < 32; value++) {
        // Random point.
        pre::Vec2<Float> point = {
                pre::generate_canonical<Float>(gen) * 32 - 16,
                pre::generate_canonical<Float>(gen) * 32 - 16};
        box |= point;

        // Check that box contains point.
        CHECK(box.contains(point));
    }

    // Check that box overlaps itself.
    CHECK(box.overlaps(box));

    // Check that box contains itself.
    CHECK(box.contains(box));

    // Check that index 0 is minimum.
    CHECK((box[0] == box.min()).all());

    // Check that index 1 is maximum.
    CHECK((box[1] == box.max()).all());

    // Check that expand by positive margin works as expected.
    CHECK(box.expand(+0.1).overlaps(box));
    CHECK(box.expand(+0.1).contains(box));

    // Check that expand by negative margin works as expected.
    CHECK(box.expand(-0.1).overlaps(box));
    CHECK(box.expand(-0.1).contains(box) == false);

    // Check that expand relative by positive factor works as expected.
    CHECK(box.expand_relative(+0.1).overlaps(box));
    CHECK(box.expand_relative(+0.1).contains(box));

    // Check that expand relative by negative factor works as expected.
    CHECK(box.expand_relative(-0.1).overlaps(box));
    CHECK(box.expand_relative(-0.1).contains(box) == false);

    // Check that box contains point interpolated inside.
    CHECK(box.contains(box(Float(0.2), Float(0.3))));

    // Check that box does not contain point interpolated outside.
    CHECK(box.contains(box(Float(1.0), Float(-0.1))) == false);

    // Check that box does contain point interpolated outside after clamp.
    CHECK(box.contains(box.clamp(box(Float(-3.0), Float(+1.7)))));
}

TEST_CASE_TEMPLATE("BoundBox3", Float, float, double) {
    pre::Pcg32 gen(getContextOptions()->rand_seed);
    pre::BoundBox3<Float> box;

    // Box invalid by default.
    CHECK(!box);

    for (int value = 0; value < 32; value++) {
        // Random point.
        pre::Vec3<Float> point = {
                pre::generate_canonical<Float>(gen) * 32 - 16,
                pre::generate_canonical<Float>(gen) * 32 - 16,
                pre::generate_canonical<Float>(gen) * 32 - 16};
        box |= point;

        // Check that box contains point.
        CHECK(box.contains(point));
    }

    // Check that box overlaps itself.
    CHECK(box.overlaps(box));

    // Check that box contains itself.
    CHECK(box.contains(box));

    // Check that index 0 is minimum.
    CHECK((box[0] == box.min()).all());

    // Check that index 1 is maximum.
    CHECK((box[1] == box.max()).all());

    // Check that expand by positive margin works as expected.
    CHECK(box.expand(+0.1).overlaps(box));
    CHECK(box.expand(+0.1).contains(box));

    // Check that expand by negative margin works as expected.
    CHECK(box.expand(-0.1).overlaps(box));
    CHECK(box.expand(-0.1).contains(box) == false);

    // Check that expand relative by positive factor works as expected.
    CHECK(box.expand_relative(+0.1).overlaps(box));
    CHECK(box.expand_relative(+0.1).contains(box));

    // Check that expand relative by negative factor works as expected.
    CHECK(box.expand_relative(-0.1).overlaps(box));
    CHECK(box.expand_relative(-0.1).contains(box) == false);

    // Check that box contains point interpolated inside.
    CHECK(box.contains(box(Float(0.2), Float(0.3), Float(0.8))));

    // Check that box does not contain point interpolated outside.
    CHECK(box.contains(box(Float(1.0), Float(-0.1), Float(0.5))) == false);

    // Check that box does contain point interpolated outside after clamp.
    CHECK(box.contains(box.clamp(box(Float(-3.0), Float(+1.7), Float(0.4)))));
}
