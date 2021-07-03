/*-*- C++ -*-*/
#pragma once

namespace pre {

template <std::floating_point Float>
struct Converger {
  public:
    /// Converge with interval search.
    template <std::invocable<Float> F>
    constexpr bool operator()(Float& xk, F&& f) const {
        ASSERT(lower_bound);
        ASSERT(upper_bound);
        Float x0 = *lower_bound, f0 = std::invoke(f, x0) - target;
        Float x1 = *upper_bound;
        for (num_iters = 0; num_iters < max_iters; num_iters++) {
            // Check convergence.
            xk = (x0 + x1) * Float(0.5);
            Float fk = std::invoke(f, xk) - target;
            if (std::abs(fk) < cutoff)
                return true;

            // Do update.
            if (std::signbit(fk) == std::signbit(f0))
                x0 = xk;
            else
                x1 = xk;
        }
        return false;
    }

    /// Converge with Newton's method.
    template <std::invocable<Float> F, std::invocable<Float> G>
    constexpr bool operator()(Float& xk, F&& f, G&& g, int m = 1) const {
        bool clamped_lower = false;
        bool clamped_upper = false;
        for (num_iters = 0; num_iters < max_iters; num_iters++) {
            // Check convergence.
            Float fk = std::invoke(f, xk) - target;
            if (std::abs(fk) < cutoff)
                return true;

            // Do update.
            Float gk = std::invoke(g, xk);
            xk -= fk / gk * m;
            if (not std::isfinite(xk))
                break;

            // If necessary, enforce lower bound.
            if (lower_bound and xk < *lower_bound) {
                xk = *lower_bound;
                if (clamped_lower == false)
                    clamped_lower = true;
                else
                    return true; // Don't spin forever!
            }
            else
                clamped_lower = false; // Reset

            // If necessary, enforce upper bound.
            if (upper_bound and xk > *upper_bound) {
                xk = *upper_bound;
                if (clamped_upper == false)
                    clamped_upper = true;
                else
                    return true; // Don't spin forever!
            }
            else
                clamped_upper = false; // Reset
        }
        return false;
    }

  public:
    int max_iters = 100;
    mutable int num_iters = 0;
    Float target = 0;
    Float cutoff = 1e-6;
    std::optional<Float> lower_bound = std::nullopt;
    std::optional<Float> upper_bound = std::nullopt;
};

} // namespace pre
