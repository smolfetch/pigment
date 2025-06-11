#include "pigment/types_basic.hpp"
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
    }
}