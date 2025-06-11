#include <cmath>
#include <doctest/doctest.h>
#include <pigment/pigment.hpp>

using namespace pigment;

TEST_CASE("LAB Color Tests") {
    SUBCASE("LAB Construction") {
        LAB lab(50.0, 20.0, -30.0);
        CHECK(lab.l == 50.0);
        CHECK(lab.a == 20.0);
        CHECK(lab.b == -30.0);
        CHECK(lab.alpha == 255);
    }

    SUBCASE("LAB RGB Conversion") {
        RGB original(180, 120, 200);
        LAB lab = LAB::fromRGB(original);
        RGB converted = lab.to_rgb();

        // Allow for conversion rounding errors
        CHECK(std::abs(converted.r - original.r) <= 2);
        CHECK(std::abs(converted.g - original.g) <= 2);
        CHECK(std::abs(converted.b - original.b) <= 2);
    }

    SUBCASE("LAB Color Difference") {
        RGB red = RGB::red();
        RGB blue = RGB::blue();

        LAB lab_red = LAB::fromRGB(red);
        LAB lab_blue = LAB::fromRGB(blue);

        double delta_e = lab_red.delta_e(lab_blue);
        CHECK(delta_e > 100.0); // Red and blue should be very different

        // Test similarity
        LAB similar_red(lab_red.l, lab_red.a + 1.0, lab_red.b);
        CHECK(lab_red.is_similar(similar_red, 5.0));
        CHECK_FALSE(lab_red.is_similar(lab_blue, 5.0));
    }

    SUBCASE("LAB Operations") {
        LAB lab(50.0, 10.0, -20.0);

        LAB lighter = lab.adjust_lightness(10.0);
        CHECK(lighter.l == 60.0);

        LAB darker = lab.adjust_lightness(-10.0);
        CHECK(darker.l == 40.0);

        LAB other(60.0, -5.0, 15.0);
        LAB mixed = lab.mix(other, 0.5);
        CHECK(std::abs(mixed.l - 55.0) < 0.1);
    }
}
