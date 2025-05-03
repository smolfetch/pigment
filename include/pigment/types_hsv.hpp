#pragma once

#include "types_basic.hpp"
#include <algorithm>
#include <cmath>

namespace pigment {

    struct HSV {
        // Hue in [0,360), saturation/value in [0,1]
        float h = 0.0f;
        float s = 0.0f;
        float v = 0.0f;

        HSV() = default;
        HSV(float h_, float s_, float v_) : h(h_), s(s_), v(v_) { normalize(); }

        // Clamp fields into valid ranges
        void normalize() {
            // wrap hue into [0,360)
            if (h < 0.0f || h >= 360.0f) {
                h = std::fmod(h, 360.0f);
                if (h < 0.0f)
                    h += 360.0f;
            }
            s = std::clamp(s, 0.0f, 1.0f);
            v = std::clamp(v, 0.0f, 1.0f);
        }

        // Create HSV from an RGB (alpha ignored)
        static HSV fromRGB(const RGB &c) {
            float rf = c.r / 255.0f;
            float gf = c.g / 255.0f;
            float bf = c.b / 255.0f;

            float mx = std::max({rf, gf, bf});
            float mn = std::min({rf, gf, bf});
            float delta = mx - mn;

            HSV out;

            // Hue calculation
            if (delta < 1e-6f) {
                out.h = 0.0f;
            } else if (mx == rf) {
                out.h = 60.0f * std::fmod((gf - bf) / delta, 6.0f);
            } else if (mx == gf) {
                out.h = 60.0f * (((bf - rf) / delta) + 2.0f);
            } else {
                out.h = 60.0f * (((rf - gf) / delta) + 4.0f);
            }
            if (out.h < 0)
                out.h += 360.0f;

            // Saturation & Value
            out.s = (mx < 1e-6f ? 0.0f : (delta / mx));
            out.v = mx;

            out.normalize();
            return out;
        }

        // Convert this HSV to RGB (alpha = 255)
        RGB toRGB() const {
            float C = v * s;
            float X = C * (1 - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1));
            float m = v - C;

            float rp, gp, bp;
            if (h < 60.0f) {
                rp = C;
                gp = X;
                bp = 0;
            } else if (h < 120.0f) {
                rp = X;
                gp = C;
                bp = 0;
            } else if (h < 180.0f) {
                rp = 0;
                gp = C;
                bp = X;
            } else if (h < 240.0f) {
                rp = 0;
                gp = X;
                bp = C;
            } else if (h < 300.0f) {
                rp = X;
                gp = 0;
                bp = C;
            } else {
                rp = C;
                gp = 0;
                bp = X;
            }

            RGB out;
            out.r = int(std::round((rp + m) * 255));
            out.g = int(std::round((gp + m) * 255));
            out.b = int(std::round((bp + m) * 255));
            out.a = 255;
            return out;
        }

        // Brightness = scale the V component
        inline void adjustBrightness(float factor) { v = std::clamp(v * factor, 0.0f, 1.0f); }

        // Saturation = scale the S component
        inline void adjustSaturation(float factor) { s = std::clamp(s * factor, 0.0f, 1.0f); }
    };

} // namespace pigment
