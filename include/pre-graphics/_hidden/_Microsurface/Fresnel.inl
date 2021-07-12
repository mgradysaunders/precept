/*-*- C++ -*-*/
#pragma once

namespace pre {

namespace microsurface {

class Fresnel {
  public:
    virtual ~Fresnel() = default;

    virtual std::pair<double, double> F(
            double etai,
            double etat,
            double cos_thetai,
            double* cos_thetat = nullptr) const noexcept {
        auto fresnel = pre::Fresnel(etai / etat, cos_thetai);
        if (cos_thetat)
            *cos_thetat = fresnel.cos_thetat;
        return {fresnel.fr, //
                fresnel.ft};
    }
};

inline Fresnel fresnel;

// TODO class ThinfilmFresnel

} // namespace microsurface

} // namespace pre
