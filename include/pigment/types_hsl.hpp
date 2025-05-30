#pragma once

#include "types_basic.hpp"
#include <algorithm>
#include <cmath>

namespace pigment {

    struct HSL {
        double h = 0.0;  // 0-360 degrees
        double s = 0.0;  // 0-1 saturation
        double l = 0.0;  // 0-1 lightness
        int a = 255;     // 0-255 alpha
        
        HSL() = default;
        HSL(double h_, double s_, double l_, int a_ = 255) 
            : h(h_), s(s_), l(l_), a(a_) {
            normalize();
        }
        
        void normalize() {
            // Wrap hue to [0, 360)
            while (h >= 360.0) h -= 360.0;
            while (h < 0.0) h += 360.0;
            
            // Clamp saturation and lightness
            s = std::clamp(s, 0.0, 1.0);
            l = std::clamp(l, 0.0, 1.0);
            a = std::clamp(a, 0, 255);
        }
        
        // Convert from RGB
        static HSL fromRGB(const RGB& rgb) {
            double r = rgb.r / 255.0;
            double g = rgb.g / 255.0;
            double b = rgb.b / 255.0;
            
            double max_val = std::max({r, g, b});
            double min_val = std::min({r, g, b});
            double delta = max_val - min_val;
            
            HSL hsl;
            
            // Lightness
            hsl.l = (max_val + min_val) / 2.0;
            
            if (delta == 0) {
                hsl.h = hsl.s = 0; // achromatic
            } else {
                // Saturation
                hsl.s = hsl.l > 0.5 ? delta / (2.0 - max_val - min_val) : delta / (max_val + min_val);
                
                // Hue
                if (max_val == r) {
                    hsl.h = (g - b) / delta + (g < b ? 6 : 0);
                } else if (max_val == g) {
                    hsl.h = (b - r) / delta + 2;
                } else {
                    hsl.h = (r - g) / delta + 4;
                }
                hsl.h /= 6;
            }
            hsl.h *= 360;
            hsl.a = rgb.a;
            hsl.normalize();
            
            return hsl;
        }
        
        // Convert to RGB
        RGB to_rgb() const {
            if (s == 0) {
                int val = static_cast<int>(l * 255);
                return RGB(val, val, val, a);
            }
            
            auto hue_to_rgb = [](double p, double q, double t) {
                if (t < 0) t += 1;
                if (t > 1) t -= 1;
                if (t < 1.0/6) return p + (q - p) * 6 * t;
                if (t < 1.0/2) return q;
                if (t < 2.0/3) return p + (q - p) * (2.0/3 - t) * 6;
                return p;
            };
            
            double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
            double p = 2 * l - q;
            double h_norm = h / 360.0;
            
            double r = hue_to_rgb(p, q, h_norm + 1.0/3);
            double g = hue_to_rgb(p, q, h_norm);
            double b = hue_to_rgb(p, q, h_norm - 1.0/3);
            
            return RGB(
                static_cast<int>(std::round(r * 255)),
                static_cast<int>(std::round(g * 255)),
                static_cast<int>(std::round(b * 255)),
                a
            );
        }
        
        // Color adjustments
        HSL adjust_hue(double degrees) const {
            return HSL(h + degrees, s, l, a);
        }
        
        HSL adjust_saturation(double factor) const {
            return HSL(h, s * factor, l, a);
        }
        
        HSL adjust_lightness(double factor) const {
            return HSL(h, s, l * factor, a);
        }
        
        HSL saturate(double amount = 0.1) const {
            return HSL(h, std::clamp(s + amount, 0.0, 1.0), l, a);
        }
        
        HSL desaturate(double amount = 0.1) const {
            return HSL(h, std::clamp(s - amount, 0.0, 1.0), l, a);
        }
        
        HSL lighten(double amount = 0.1) const {
            return HSL(h, s, std::clamp(l + amount, 0.0, 1.0), a);
        }
        
        HSL darken(double amount = 0.1) const {
            return HSL(h, s, std::clamp(l - amount, 0.0, 1.0), a);
        }
        
        // Complementary color
        HSL complement() const {
            return adjust_hue(180.0);
        }
        
        // Triadic colors
        std::vector<HSL> triadic() const {
            return {
                *this,
                adjust_hue(120.0),
                adjust_hue(240.0)
            };
        }
        
        // Analogous colors
        std::vector<HSL> analogous(double angle = 30.0) const {
            return {
                adjust_hue(-angle),
                *this,
                adjust_hue(angle)
            };
        }
        
        // Split complementary
        std::vector<HSL> split_complementary(double angle = 30.0) const {
            return {
                *this,
                adjust_hue(180.0 - angle),
                adjust_hue(180.0 + angle)
            };
        }
        
        static HSL random() {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<double> hue_dist(0.0, 360.0);
            std::uniform_real_distribution<double> sat_dist(0.0, 1.0);
            std::uniform_real_distribution<double> light_dist(0.0, 1.0);
            
            return HSL(hue_dist(gen), sat_dist(gen), light_dist(gen), 255);
        }
    };

} // namespace pigment
