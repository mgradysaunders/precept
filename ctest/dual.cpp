#include <sstream>
#include "doctest.h"
#include <pre/random>
#include <pre/dual>

#define CHECK_DUAL_APPROX(x0, x1) do {\
    CHECK((x0).real() == Approx((x1).real()).epsilon(0.05));\
    CHECK((x0).dual() == Approx((x1).dual()).epsilon(0.05));\
} while (false)

TEST_CASE_TEMPLATE("Dual", Float, float, double) {

    // Random generator.
    pre::Pcg32 gen(getContextOptions()->rand_seed);

    // Random x = (a, b).
    Float a = 2 * pre::generate_canonical<Float>(gen) - 1;
    Float b = 2 * pre::generate_canonical<Float>(gen) - 1;
    pre::Dual<Float> x = {a, b};

    // Random y = (c, d).
    Float c = 2 * pre::generate_canonical<Float>(gen) - 1;
    Float d = 2 * pre::generate_canonical<Float>(gen) - 1;
    pre::Dual<Float> y = {c, d};

    // Check real().
    CHECK(x.real() == a);

    // Check real() non-member version.
    CHECK(x.real() == pre::real(x));

    // Check dual().
    CHECK(x.dual() == b);

    // Check dual() non-member version.
    CHECK(x.dual() == pre::dual(x));

    // Check conj(). This is complex conjugate!
    CHECK(x.conj() == x);

    // Check conj() non-member version.
    CHECK(x.conj() == pre::conj(x));

    // Check dual_conj().
    CHECK(x.dual_conj() == pre::Dual<Float>{a, -b});

    // Check dual_conj() non-member version.
    CHECK(x.dual_conj() == pre::dual_conj(x));

    // Check that x + y works as expected.
    CHECK((x + y) == pre::Dual<Float>{a + c, b + d});

    // Check that x - y works as expected.
    CHECK((x - y) == pre::Dual<Float>{a - c, b - d});

    // Check that x * y works as expected.
    CHECK((x * y) == pre::Dual<Float>{a * c, a * d + b * c});

    // Check that x == log(exp(x))
    CHECK_DUAL_APPROX(x, pre::log(pre::exp(x)));

    // Check that x == log2(exp2(x))
    CHECK_DUAL_APPROX(x, pre::log2(pre::exp2(x)));

    // Check x == log1p(expm1(x))
    CHECK_DUAL_APPROX(x, pre::log1p(pre::expm1(x)));

    // Given x = sin(y), check that x == sin(asin(x)).
    x = pre::sin(y);
    CHECK_DUAL_APPROX(x, pre::sin(pre::asin(x)));

    // Given x = cos(y), check that x == cos(acos(x)).
    x = pre::cos(y);
    CHECK_DUAL_APPROX(x, pre::cos(pre::acos(x)));

    // Given x = tan(y), check that x == tan(atan(x)).
    x = pre::tan(y);
    CHECK_DUAL_APPROX(x, pre::tan(pre::atan(x)));

    // Given x = sinh(y), check that x == sinh(asinh(x)).
    x = pre::sinh(y);
    CHECK_DUAL_APPROX(x, pre::sinh(pre::asinh(x)));

    // Given x = cosh(y), check that x == cosh(acosh(x)).
    x = pre::cosh(y);
    CHECK_DUAL_APPROX(x, pre::cosh(pre::acosh(x)));

    // Given x = tanh(y), check that x == tanh(atanh(x)).
    x = pre::tanh(y);
    CHECK_DUAL_APPROX(x, pre::tanh(pre::atanh(x)));

    // Given x = (abs(a), b), check that x == sqrt(x * x).
    x = {pre::abs(a), b};
    CHECK_DUAL_APPROX(x, pre::sqrt(x * x));

    // Given x = (a, b), check that x == cbrt(x * x * x).
    x = {a, b};
    CHECK_DUAL_APPROX(x, pre::cbrt(x * x * x));
}
