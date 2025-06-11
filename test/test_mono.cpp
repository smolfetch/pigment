#include <doctest/doctest.h>
#include <pigment/pigment.hpp>

using namespace pigment;

TEST_CASE("MONO Color Tests") {
    SUBCASE("MONO Construction") {
        MONO gray(128);
        CHECK(gray.v == 128);

        MONO black = MONO::black();
        CHECK(black.v == 0);

        MONO white = MONO::white();
        CHECK(white.v == 255);

        MONO mid_gray = MONO::gray();
        CHECK(mid_gray.v == 128);
    }
}
