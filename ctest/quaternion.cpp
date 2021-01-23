#include "doctest.h"
#include <iostream>
#include <pre/random>
#include <pre/quaternion>

template <typename Float>
static bool is_approx(pre::Quaternion<Float> lhs, pre::Quaternion<Float> rhs) {
    if (lhs[0] < 0)
        lhs = -lhs;
    if (rhs[0] < 0)
        rhs = -rhs;
    return lhs[0] == Approx(rhs[0]).epsilon(0.05) &&
           lhs[1] == Approx(rhs[1]).epsilon(0.05) &&
           lhs[2] == Approx(rhs[2]).epsilon(0.05) &&
           lhs[3] == Approx(rhs[3]).epsilon(0.05);
}

TEST_CASE_TEMPLATE("Quaternion", Float, float, double) {
    pre::Pcg32 gen(getContextOptions()->rand_seed);

    for (int check = 0; check < 1024; check++) {
        // Random angle.
        Float ang = pre::generate_canonical<Float>(gen) * 4 - 2;

        // Random direction.
        pre::Vec3<Float> vec = pre::normalize(pre::Vec3<Float>{
                pre::generate_canonical<Float>(gen) * 2 - 1,
                pre::generate_canonical<Float>(gen) * 2 - 1,
                pre::generate_canonical<Float>(gen) * 2 - 1});
        if ((vec == 0).all())
            vec = {0, 0, 1};

        // Make quaternion.
        pre::Quaternion<Float> quat = pre::Quaternion<Float>::rotate(ang, vec);

        // Check that length is 1.
        CHECK(quat.length() == Approx(1.0).epsilon(0.01));

        // Check that conjugate is inverse since length is 1.
        CHECK(is_approx(quat * quat.conj(), pre::Quaternion<Float>(1)));

        // Check that matrix conversion works.
        CHECK(is_approx(quat, pre::Quaternion<Float>(pre::Mat3<Float>(quat))));

        // Check that quaternion rotate agrees with matrix rotate.
        CHECK(is_approx(
                quat,
                pre::Quaternion<Float>(pre::Mat3<Float>::rotate(ang, vec))));

        // Check that quaternion rotate agrees with observers.
        CHECK(is_approx(
                quat, pre::Quaternion<Float>::rotate(
                              quat.rotation_angle(), quat.rotation_axis())));
    }
}
