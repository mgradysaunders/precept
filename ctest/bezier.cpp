#include "doctest.h"
#include <pre/random>
#include <pre/shapes/bezier>

template <std::floating_point Float, size_t D>
static void run_checks() {
    pre::Pcg32 gen(getContextOptions()->rand_seed);
    for (int check = 0; check < 256; check++) {
        pre::Bezier<Float, 2, D> bez;
        for (int k = 0; k < int(D + 1); k++) {
            bez[k][0] = gen(-4.0, 4.0);
            bez[k][1] = gen(-4.0, 4.0);
        }
        // Random parameters.
        Float t = gen(0.0, 1.0);
        Float u = gen(0.0, 1.0);

        // Check that deriv() agrees with numerical approximation.
        Float h = 1e-3;
        CHECK(pre::distance(
                      bez.deriv()(t), (bez(t + h) - bez(t - h)) / (2 * h)) <
              5e-3);

        // Cutting the curve should preserve value() for appropriate
        // scaling of the parameters.
        auto [cut0, cut1] = bez.cut(t);
        CHECK(pre::distance(cut0(u), bez(t * u)) < 5e-3);
        CHECK(pre::distance(cut1(u), bez(t + (1 - t) * u)) < 5e-3);

        if constexpr (D <= 3) {
            // Check that length() agrees with numerical approximation.
            double len = 0;
            for (int pos = 0; pos < 2048; pos++) {
                auto point0 = bez((pos + 0) / 2048.0);
                auto point1 = bez((pos + 1) / 2048.0);
                len += pre::distance(point0, point1);
            }
            CHECK(len == Approx(bez.length()).epsilon(0.01));
        }
    }
}

TEST_CASE_TEMPLATE("Bezier", Float, float, double) {
    run_checks<Float, 1>();
    run_checks<Float, 2>();
    run_checks<Float, 3>();
    run_checks<Float, 4>();
}

#if 0
        // Check that length_to_param() works as expected.
        len = bez.length() * t;
        CHECK(bez.subset(0, bez.length_to_param(len)).length() ==
                Approx(len).epsilon(0.01));

        // A centered subset should preserve value() at midpoint.
        CHECK(pre::distance(
                bez.value(0.5),
                bez.subset(t, 1 - t).value(0.5)) < 5e-3);

        // The nearest point to a point already on curve should be 
        // the point itself.
        CHECK(pre::distance(
                bez.value(t),
                bez.value(bez.nearest(bez.value(t)))) < 5e-3);
#endif
