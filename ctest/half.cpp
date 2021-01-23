#include "doctest.h"
#include <pre/random>
#include <pre/half>

TEST_CASE("Half") {    
    // Check accuracy.
    pre::Pcg32 pcg(getContextOptions()->rand_seed);
    for (int check = 0; check < 1024; check++) {
        float u0 = pre::generate_canonical<float>(pcg);
        float x0 = (2 * u0 - 1) * 65504.0f;
        float xh = pre::Half(x0);
        CHECK(pre::fabs(xh - x0) / pre::fabs(x0) <= 0x1p-11f);
    }

    // Check Infs.
    pre::Half positive_inf(+pre::numeric_limits<float>::infinity());
    pre::Half negative_inf(-pre::numeric_limits<float>::infinity());
    CHECK(positive_inf.isinf());
    CHECK(negative_inf.isinf());
    CHECK(pre::isinf(float(positive_inf)));
    CHECK(pre::isinf(float(negative_inf)));

    // Check NaNs.
    pre::Half positive_nan(+pre::numeric_limits<float>::quiet_NaN());
    pre::Half negative_nan(-pre::numeric_limits<float>::quiet_NaN());
    CHECK(positive_nan.isnan());
    CHECK(negative_nan.isnan());
    CHECK(pre::isnan(float(positive_nan)));
    CHECK(pre::isnan(float(negative_nan)));
}
