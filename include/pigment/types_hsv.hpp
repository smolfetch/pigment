#pragma once

#include "types_basic.hpp"
#include <cmath>

namespace pigment {

    struct HSV {
        double h = 0.0;
        double s = 0.0;
        double v = 0.0;

        HSV() = default;
        HSV(double h_, double s_, double v_) : h(h_), s(s_), v(v_) {}
        explicit HSV(const RGB &rgb) noexcept;
    };

} // namespace pigment
