/*-*- C++ -*-*/
#pragma once

namespace pre {

namespace microsurface {

class Height {
  public:
    virtual ~Height() = default;

    virtual double P1(double h) const noexcept = 0;

    virtual double C1(double h) const noexcept = 0;

    virtual double C1inv(double u) const noexcept = 0;
};

class UniformHeight final : public Height {
  public:
    double P1(double h) const noexcept {
        return pre::UniformDistribution(-1, 1).pdf(h);
    }
    double C1(double h) const noexcept {
        return pre::UniformDistribution(-1, 1).cdf(h);
    }
    double C1inv(double u) const noexcept {
        return pre::UniformDistribution(-1, 1).cdf_inv(u);
    }
};

class NormalHeight final : public Height {
  public:
    double P1(double h) const noexcept {
        return pre::NormalDistribution(0, 1).pdf(h);
    }
    double C1(double h) const noexcept {
        return pre::NormalDistribution(0, 1).cdf(h);
    }
    double C1inv(double u) const noexcept {
        return pre::NormalDistribution(0, 1).cdf_inv(u);
    }
};

inline UniformHeight uniform_height;

inline NormalHeight normal_height;

} // namespace microsurface

} // namespace pre
