/*-*- C++ -*-*/
#pragma once

namespace pre {

namespace microsurface {

class Slope {
  public:
    virtual ~Slope() = default;

    virtual double Lambda11(Vec3<double> wo) const noexcept = 0;

    virtual double Aperp11(Vec3<double> wo) const noexcept = 0;

    virtual double P11(Vec2<double> m) const noexcept = 0;

    virtual Vec2<double> P11_sample(
            double u0, double u1, double cos_thetao) const noexcept = 0;
};

class TrowbridgeReitzSlope final : public Slope {
  public:
    double Lambda11(Vec3<double> wo) const noexcept {
        return 0.5 * pre::sqrt(dot(wo, wo)) / wo[2] - 0.5;
    }

    double Aperp11(Vec3<double> wo) const noexcept {
        return 0.5 * pre::sqrt(dot(wo, wo)) + 0.5 * wo[2];
    }

    double P11(Vec2<double> m) const noexcept {
        return inv_pi * nthpow(1 + dot(m, m), 2);
    }

    Vec2<double> P11_sample(
            double u0, double u1, double cos_thetao) const noexcept {
        if (cos_thetao > 0.99999) {
            double r = pre::sqrt(u0 / (1 - u0));
            double phi = 2 * pi * u1;
            return {r * pre::cos(phi), //
                    r * pre::sin(phi)};
        }
        else {
            double sin_thetao = pre::sqrt(1 - cos_thetao * cos_thetao);
            double tan_thetao = sin_thetao / cos_thetao;
            double mu = u0 * (1 + 1 / cos_thetao) - 1;
            double nu = 1 / (1 - mu * mu);
            double q = pre::sqrt(pre::fmax(
                    0.0,
                    mu * mu * nu - nu * (1 - nu) * tan_thetao * tan_thetao));
            double t0 = -nu * tan_thetao - q;
            double t1 = -nu * tan_thetao + q;
            double m0 = mu < 0 or t1 * sin_thetao > cos_thetao ? t0 : t1;
            double m1 = 1;
            if (u1 > 0.5) {
                u1 = 2 * u1 - 1;
            }
            else {
                u1 = 1 - 2 * u1;
                m1 = -1;
            }
            m1 *= pre::sqrt(1 + m0 * m0) *
                  (u1 * (u1 * (u1 * 0.273850 - 0.733690) + 0.463410)) /
                  (u1 * (u1 * (u1 * 0.093073 + 0.309420) - 1.000000) +
                   0.597999);
            return {m0, m1};
        }
    }
};

class BeckmannSlope final : public Slope {
  public:
    double Lambda11(Vec3<double> wo) const noexcept {
        double r = std::hypot(wo[0], wo[1]);
        double a = wo[2] / r;
        return 0.5 * inv_sqrtpi * std::exp(-a * a) / a - 0.5 * std::erfc(a);
    }

    double Aperp11(Vec3<double> wo) const noexcept {
        double r = std::hypot(wo[0], wo[1]);
        double a = wo[2] / r;
        return 0.5 * inv_sqrtpi * std::exp(-a * a) * r +
               0.5 * wo[2] * std::erfc(-a);
    }

    double P11(Vec2<double> m) const noexcept {
        return inv_pi * std::exp(-dot(m, m));
    }

    Vec2<double> P11_sample(
            double u0, double u1, double cos_thetao) const noexcept {
        if (cos_thetao > 0.99999) {
            double r = std::sqrt(-std::log1p(-u0));
            double phi = 2 * pi * u1;
            return {r * std::cos(phi), //
                    r * std::sin(phi)};
        }
        else {
            u0 = std::fmax(u0, 1e-6);
            double sin_thetao = pre::sqrt(1 - cos_thetao * cos_thetao);
            double cot_thetao = cos_thetao / sin_thetao;
            auto c11 = [=](double a) {
                return 0.5 * inv_sqrtpi * sin_thetao * std::exp(-a * a) +
                       0.5 * cos_thetao * std::erfc(-a);
            };
            double cnorm = 1 / c11(cot_thetao);
            if (cnorm > 1e+6 or not std::isfinite(cnorm))
                return {};
            double xmin = -1;
            double xmax = std::erf(cot_thetao);
            Converger<double> converger;
            converger.max_iters = 10;
            converger.lower_bound = xmin;
            converger.upper_bound = xmax;
            converger.target = u0;
            converger.cutoff = 1e-6;
            auto f = [&](double x) {
                double a = erfinv(x);
                return x >= cot_thetao ? 1 : cnorm * c11(a);
            };
            auto g = [&](double x) {
                double a = erfinv(x);
                return cnorm * (cos_thetao - a * sin_thetao) / 2;
            };
            // Improved initial guess lifted from PBRT-v3 source.
            double thetao = std::acos(cos_thetao);
            double x = -0.0564;
            x = std::fma(thetao, x, +0.4265);
            x = std::fma(thetao, x, -0.876);
            x = std::fma(thetao, x, +1.0);
            x = xmax - (1 + xmax) * std::pow(1 - u0, x);
            // Do numerical inversion.
            if (converger(x, f, g))
                return {erfinv(x), //
                        erfinv(2 * u1 - 1)};
        }
        return {};
    }
};

inline TrowbridgeReitzSlope trowbridge_reitz_slope;

inline BeckmannSlope beckmann_slope;

} // namespace microsurface

} // namespace pre
