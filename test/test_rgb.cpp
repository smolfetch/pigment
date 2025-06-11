#include "pigment/types_basic.hpp"
#include <cmath>
#include <doctest/doctest.h>
#include <pigment/pigment.hpp>

using namespace pigment;

TEST_CASE("RGB Color Tests") {
    RGB red(255, 0, 0);
    RGB green(0, 255, 0);
    RGB blue(0, 0, 255);

    SUBCASE("RGB Constructor and Hex Conversion") {
        CHECK(red.to_hex() == "#ff0000");
        CHECK(green.to_hex() == "#00ff00");
        CHECK(blue.to_hex() == "#0000ff");

        // Test with alpha
        RGB transparent_red(255, 0, 0, 128);
        CHECK(transparent_red.to_hex(true) == "#ff000080");
    }

    SUBCASE("RGB from Hex String Constructor") {
        RGB hex_red("#FF0000");
        RGB hex_green("#00FF00");
        RGB hex_blue("#0000FF");

        CHECK(hex_red.r == 255);
        CHECK(hex_red.g == 0);
        CHECK(hex_red.b == 0);

        CHECK(hex_green.r == 0);
        CHECK(hex_green.g == 255);
        CHECK(hex_green.b == 0);

        CHECK(hex_blue.r == 0);
        CHECK(hex_blue.g == 0);
        CHECK(hex_blue.b == 255);

        // Test short hex format
        RGB short_white("#FFF");
        CHECK(short_white.r == 255);
        CHECK(short_white.g == 255);
        CHECK(short_white.b == 255);

        // Test hex with alpha
        RGB hex_alpha("#FF000080");
        CHECK(hex_alpha.r == 255);
        CHECK(hex_alpha.g == 0);
        CHECK(hex_alpha.b == 0);
        CHECK(hex_alpha.a == 128);
    }

    SUBCASE("RGB Color Operations") {
        RGB white = RGB::white();
        RGB black = RGB::black();

        CHECK(white.r == 255);
        CHECK(white.g == 255);
        CHECK(white.b == 255);

        CHECK(black.r == 0);
        CHECK(black.g == 0);
        CHECK(black.b == 0);

        // Test color mixing
        RGB mixed = red.mix(blue);
        CHECK(mixed.r > 0);
        CHECK(mixed.b > 0);
        CHECK(mixed.g == 0);

        // Test mixing ratios
        RGB mixed_25 = red.mix(blue, 0.25);
        CHECK(mixed_25.r > mixed_25.b);

        RGB mixed_75 = red.mix(blue, 0.75);
        CHECK(mixed_75.b > mixed_75.r);
    }

    SUBCASE("RGB Predefined Colors") {
        CHECK(RGB::red() == RGB(255, 0, 0));
        CHECK(RGB::green() == RGB(0, 255, 0));
        CHECK(RGB::blue() == RGB(0, 0, 255));
        CHECK(RGB::yellow() == RGB(255, 255, 0));
        CHECK(RGB::cyan() == RGB(0, 255, 255));
        CHECK(RGB::magenta() == RGB(255, 0, 255));
        CHECK(RGB::transparent() == RGB(0, 0, 0, 0));
    }

    SUBCASE("RGB Arithmetic Operations") {
        RGB color1(100, 50, 200);
        RGB color2(50, 100, 55);

        RGB sum = color1 + color2;
        CHECK(sum.r == 150);
        CHECK(sum.g == 150);
        CHECK(sum.b == 255); // Clamped

        RGB diff = color1 - color2;
        CHECK(diff.r == 50);
        CHECK(diff.g == 0); // Clamped
        CHECK(diff.b == 145);

        RGB scaled = color1 * 0.5;
        CHECK(scaled.r == 50);
        CHECK(scaled.g == 25);
        CHECK(scaled.b == 100);
    }

    SUBCASE("RGB Brightness and Contrast") {
        RGB base(128, 128, 128);

        RGB brighter = base.brighten(0.2);
        CHECK(brighter.r > base.r);
        CHECK(brighter.g > base.g);
        CHECK(brighter.b > base.b);

        RGB darker = base.darken(0.2);
        CHECK(darker.r < base.r);
        CHECK(darker.g < base.g);
        CHECK(darker.b < base.b);

        // Test luminance
        CHECK(RGB::white().luminance() > RGB::black().luminance());
        CHECK(RGB::white().is_light());
        CHECK(RGB::black().is_dark());
    }

    SUBCASE("RGB Color Temperature") {
        RGB base(128, 128, 128);

        RGB warmer = base.warm(0.1);
        CHECK(warmer.r >= base.r);
        CHECK(warmer.g >= base.g);
        CHECK(warmer.b == base.b);

        RGB cooler = base.cool(0.1);
        CHECK(cooler.r == base.r);
        CHECK(cooler.g >= base.g);
        CHECK(cooler.b >= base.b);
    }

    SUBCASE("RGB Utility Functions") {
        RGB color(200, 100, 50);

        RGB grayscale = color.to_grayscale();
        CHECK(grayscale.r == grayscale.g);
        CHECK(grayscale.g == grayscale.b);

        RGB inverted = color.invert();
        CHECK(inverted.r == 255 - color.r);
        CHECK(inverted.g == 255 - color.g);
        CHECK(inverted.b == 255 - color.b);

        RGB contrasted = color.adjust_contrast(0.5);
        CHECK(contrasted.r != color.r);
        CHECK(contrasted.g != color.g);
        CHECK(contrasted.b != color.b);
    }
}
