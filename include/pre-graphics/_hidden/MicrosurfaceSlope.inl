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
        return pi_inv * nthpow(1 + dot(m, m), 2);
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
        return 0.5 * sqrtpi_inv * std::exp(-a * a) / a - 0.5 * std::erfc(a);
    }

    double Aperp11(Vec3<double> wo) const noexcept {
        double r = std::hypot(wo[0], wo[1]);
        double a = wo[2] / r;
        return 0.5 * sqrtpi_inv * std::exp(-a * a) * r +
               0.5 * wo[2] * std::erfc(-a);
    }

    double P11(Vec2<double> m) const noexcept {
        return pi_inv * std::exp(-dot(m, m));
    }

    Vec2<double> P11_sample(double u0, double u1, double cos_thetao) {
        if (cos_thetao > 0.99999) {
            double r = std::sqrt(-std::log1p(-u0));
            double phi = 2 * pi * u1;
            return {r * std::cos(phi), //
                    r * std::sin(phi)};
        }
        else {
            double sin_thetao = pre::sqrt(1 - cos_thetao * cos_thetao);
            double cot_thetao = cos_thetao / sin_thetao;
            auto c11 = [=](double a) {
                return 0.5 * sqrtpi_inv * sin_thetao * std::exp(-a * a) +
                       0.5 * cos_thetao * std::erfc(-a);
            };
            double aperp = c11(cot_thetao);
            double cnorm = 1 / aperp;
            if (aperp < 0.00001 or not std::isfinite(aperp))
                return {};

            // Newton-Raphson iteration.
            double ymin = -0.99999;
            double ymax = std::erf(cot_thetao);
            if (ymax < ymin)
                ymax = ymin;
            double y = ymin / 2 + ymax / 2;
            while (ymax - ymin > 0.00001) {
                double a = erfinv(y); // y = erf(a)
                double c = (a >= cot_thetao ? 1 : cnorm * c11(a)) - u0;
                if (std::fabs(c) <= 0.00001)
                    break;
                if (c < 0) {
                    if (ymin != y)
                        ymin = y;
                    else
                        break;
                }
                else {
                    if (ymax != y)
                        ymax = y;
                    else
                        break;
                }
                y -= c / (0.5 * cnorm * (cos_thetao - a * sin_thetao));
                if (not(ymin <= y and y <= ymax))
                    y = ymin / 2 + ymax / 2; // Recenter.
            }
            y = std::fmax(y, ymin);
            y = std::fmin(y, ymax);
            double m0 = erfinv(y);
            double m1 = erfinv(2 * u1 - 1);
            return {m0, m1};
        }
    }
};

} // namespace microsurface

} // namespace pre
