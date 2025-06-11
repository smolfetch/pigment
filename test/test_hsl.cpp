#include <cmath>
#include <doctest/doctest.h>
#include <pigment/pigment.hpp>

using namespace pigment;

TEST_CASE("HSL Color Tests") {
    SUBCASE("HSL Construction and Normalization") {
        HSL hsl(120.0, 0.5, 0.7);
        CHECK(hsl.h == 120.0);
        CHECK(hsl.s == 0.5);
        CHECK(hsl.l == 0.7);

        // Test normalization
        HSL hsl_wrap(370.0, 1.5, -0.1);
        CHECK(hsl_wrap.h == 10.0); // 370 - 360
        CHECK(hsl_wrap.s == 1.0);  // Clamped
        CHECK(hsl_wrap.l == 0.0);  // Clamped
    }

    SUBCASE("HSL RGB Conversion") {
        RGB red = RGB::red();
        HSL hsl_red = HSL::fromRGB(red);
        RGB back_to_rgb = hsl_red.to_rgb();

        // Allow small rounding errors
        CHECK(std::abs(back_to_rgb.r - red.r) <= 1);
        CHECK(std::abs(back_to_rgb.g - red.g) <= 1);
        CHECK(std::abs(back_to_rgb.b - red.b) <= 1);

        // Test pure red
        CHECK(hsl_red.h == 0.0);
        CHECK(hsl_red.s == 1.0);
        CHECK(std::abs(hsl_red.l - 0.5) < 0.01);
    }

    SUBCASE("HSL Color Adjustments") {
        HSL base(180.0, 0.5, 0.5);

        HSL hue_adjusted = base.adjust_hue(30.0);
        CHECK(hue_adjusted.h == 210.0);

        HSL saturated = base.saturate(0.2);
        CHECK(saturated.s == 0.7);

        HSL desaturated = base.desaturate(0.2);
        CHECK(desaturated.s == 0.3);

        HSL lighter = base.lighten(0.2);
        CHECK(lighter.l == 0.7);

        HSL darker = base.darken(0.2);
        CHECK(darker.l == 0.3);
    }

    SUBCASE("HSL Color Harmonies") {
        HSL base(120.0, 0.8, 0.6);

        HSL complement = base.complement();
        CHECK(std::abs(complement.h - 300.0) < 0.1);

        auto triadic = base.triadic();
        CHECK(triadic.size() == 3);
        CHECK(triadic[0].h == base.h);
        CHECK(std::abs(triadic[1].h - (base.h + 120.0)) < 0.1);
        // Handle hue wrapping: 120 + 240 = 360, which normalizes to 0
        double expected_h2 = fmod(base.h + 240.0, 360.0);
        CHECK(std::abs(triadic[2].h - expected_h2) < 0.1);

        auto analogous = base.analogous();
        CHECK(analogous.size() == 3);
        CHECK(analogous[1].h == base.h); // Middle one is the base

        auto split_comp = base.split_complementary();
        CHECK(split_comp.size() == 3);
        CHECK(split_comp[0].h == base.h);
    }
}
