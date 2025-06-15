#include "doctest/doctest.h"
#include "pigment/named_colors.hpp"
#include "pigment/palette.hpp"
#include <algorithm>
#include <set>

TEST_CASE("Named Colors - Basic Access") {
    SUBCASE("Color retrieval works") {
        auto red = pigment::colors::red();
        CHECK(red.r == 255);
        CHECK(red.g == 0);
        CHECK(red.b == 0);
        CHECK(red.a == 255);

        auto skyblue = pigment::colors::skyblue();
        CHECK(skyblue.r == 135);  // #87
        CHECK(skyblue.g == 206);  // #CE
        CHECK(skyblue.b == 235);  // #EB
        CHECK(skyblue.a == 255);
    }

    SUBCASE("Basic colors are accessible") {
        auto red = pigment::colors::red();
        auto blue = pigment::colors::blue();
        auto green = pigment::colors::green();
        auto yellow = pigment::colors::yellow();
        auto white = pigment::colors::white();
        auto black = pigment::colors::black();

        // Verify red
        CHECK(red.r == 255);
        CHECK(red.g == 0);
        CHECK(red.b == 0);

        // Verify blue
        CHECK(blue.r == 0);
        CHECK(blue.g == 0);
        CHECK(blue.b == 255);

        // Verify green
        CHECK(green.r == 0);
        CHECK(green.g == 128);
        CHECK(green.b == 0);
        
        // Verify yellow
        CHECK(yellow.r == 255);
        CHECK(yellow.g == 255);
        CHECK(yellow.b == 0);
        
        // Verify white and black
        CHECK(white.r == 255);
        CHECK(white.g == 255);
        CHECK(white.b == 255);
        CHECK(black.r == 0);
        CHECK(black.g == 0);
        CHECK(black.b == 0);
    }

    SUBCASE("Named colors return correct RGB values") {
        auto crimson = pigment::colors::crimson();
        CHECK(crimson.r == 0xDC);
        CHECK(crimson.g == 0x14);
        CHECK(crimson.b == 0x3C);
    }
}

TEST_CASE("Named Colors - Color Categories") {
    SUBCASE("Basic colors exist") {
        // Test that basic colors can be called and return expected values
        auto red = pigment::colors::red();
        auto green = pigment::colors::green();
        auto blue = pigment::colors::blue();
        auto yellow = pigment::colors::yellow();
        auto black = pigment::colors::black();
        auto white = pigment::colors::white();
        
        CHECK(red.r == 255);
        CHECK(red.g == 0);
        CHECK(red.b == 0);
        
        CHECK(green.r == 0);
        CHECK(green.g == 128);
        CHECK(green.b == 0);
        
        CHECK(blue.r == 0);
        CHECK(blue.g == 0);
        CHECK(blue.b == 255);
        
        CHECK(yellow.r == 255);
        CHECK(yellow.g == 255);
        CHECK(yellow.b == 0);
        
        CHECK(white.r == 255);
        CHECK(white.g == 255);
        CHECK(white.b == 255);
        
        CHECK(black.r == 0);
        CHECK(black.g == 0);
        CHECK(black.b == 0);
    }

    SUBCASE("Shades of colors exist") {
        // Test red shades - just verify a few key ones
        auto crimson = pigment::colors::crimson();
        auto red = pigment::colors::red();
        auto darkred = pigment::colors::darkred();
        
        // Verify they return valid RGB values
        CHECK(red.r == 255);
        CHECK(red.g == 0);
        CHECK(red.b == 0);
        CHECK(crimson.r == 0xDC);
        CHECK(crimson.g == 0x14); 
        CHECK(crimson.b == 0x3C);
        CHECK(darkred.r == 0x8B);
        CHECK(darkred.g == 0);
        CHECK(darkred.b == 0);

        // Test blue shades - just verify a few key ones
        auto blue = pigment::colors::blue();
        auto navy = pigment::colors::navy();
        auto darkblue = pigment::colors::darkblue();
        
        // Verify they return valid RGB values
        CHECK(blue.r == 0);
        CHECK(blue.g == 0);
        CHECK(blue.b == 255);
        CHECK(navy.r == 0);
        CHECK(navy.g == 0);
        CHECK(navy.b == 0x80);
        CHECK(darkblue.r == 0);
        CHECK(darkblue.g == 0);
        CHECK(darkblue.b == 0x8B);
    }
}

TEST_CASE("Named Colors - Integration with Palette") {
    SUBCASE("Create palette from named colors") {
        pigment::Palette palette({
            pigment::colors::red(),
            pigment::colors::green(),
            pigment::colors::blue()
        });
        
        CHECK(palette.size() == 3);
    }

    SUBCASE("Create gradient from named colors") {
        auto start_color = pigment::colors::red();
        auto end_color = pigment::colors::blue();
        auto gradient = pigment::Palette::gradient(start_color, end_color, 5);
        
        CHECK(gradient.size() == 5);
        CHECK(gradient[0] == start_color);
        CHECK(gradient[gradient.size() - 1] == end_color);
    }

    SUBCASE("Create multi-color gradient") {
        std::vector<pigment::RGB> colors = {
            pigment::colors::red(),
            pigment::colors::yellow(),
            pigment::colors::blue()
        };
        
        auto gradient = pigment::Palette::gradient(colors, 3);
        CHECK(gradient.size() == 6);  // (3-1) * 3 steps
    }
}

TEST_CASE("Named Colors - Color Manipulation") {
    SUBCASE("Mix named colors") {
        auto red = pigment::colors::red();
        auto blue = pigment::colors::blue();
        auto purple = red.mix(blue);
        
        CHECK(purple.r == 127);  // Mixing is done with integer arithmetic
        CHECK(purple.g == 0);
        CHECK(purple.b == 127);  // Mixing is done with integer arithmetic
    }

    SUBCASE("Brighten and darken named colors") {
        auto red = pigment::colors::red();
        
        auto brightened = red.brighten(0.2);
        CHECK(brightened.r == 255);  // Already at max
        CHECK(brightened.g >= 0);    // May not increase if at 0
        CHECK(brightened.b >= 0);    // May not increase if at 0

        auto darkened = red.darken(0.2);
        CHECK(darkened.r < 255);
        CHECK(darkened.g == 0);
        CHECK(darkened.b == 0);
    }

    SUBCASE("Convert named colors to grayscale") {
        auto red = pigment::colors::red();
        auto gray = red.to_grayscale();
        
        // Red's grayscale value based on luminance (0.299 * 255)
        CHECK(gray.r == gray.g);
        CHECK(gray.g == gray.b);
        CHECK(gray.r == 76);  // Approx. 0.299 * 255
    }
}

TEST_CASE("Named Colors - Advanced Features") {
    SUBCASE("Create warm palette from named colors") {
        std::vector<pigment::RGB> warm_colors = {
            pigment::colors::red(),
            pigment::colors::orange(),
            pigment::colors::yellow()
        };
        
        pigment::Palette warm_palette(warm_colors);
        CHECK(warm_palette.size() == 3);
    }

    SUBCASE("Create cool palette from named colors") {
        std::vector<pigment::RGB> cool_colors = {
            pigment::colors::blue(),
            pigment::colors::cyan(),
            pigment::colors::purple()
        };
        
        pigment::Palette cool_palette(cool_colors);
        CHECK(cool_palette.size() == 3);
    }

    SUBCASE("Create monochromatic variations") {
        auto base_color = pigment::colors::blue();
        auto mono_palette = pigment::Palette::monochromatic(base_color, 5);
        
        CHECK(mono_palette.size() == 5);
    }

    SUBCASE("Create complementary pairs") {
        auto base_color = pigment::colors::red();
        auto comp_palette = pigment::Palette::complementary(base_color);
        
        CHECK(comp_palette.size() == 2);
    }
}

TEST_CASE("Named Colors - Color Properties") {
    SUBCASE("Check color brightness") {
        auto white = pigment::colors::white();
        auto black = pigment::colors::black();
        auto gray = pigment::colors::gray();
        
        CHECK(white.luminance() > gray.luminance());
        CHECK(gray.luminance() > black.luminance());
        CHECK(white.is_light());
        CHECK(black.is_dark());
    }

    SUBCASE("Color temperature") {
        auto base_color = pigment::colors::red();
        
        auto warmer = base_color.warm(0.1);
        CHECK(warmer.r >= base_color.r);
        
        auto cooler = base_color.cool(0.1);
        CHECK(cooler.b > base_color.b);
    }

    SUBCASE("Color inversion") {
        auto color = pigment::colors::red();
        auto inverted = color.invert();
        
        CHECK(inverted.r == (255 - color.r));
        CHECK(inverted.g == (255 - color.g));
        CHECK(inverted.b == (255 - color.b));
    }
}

TEST_CASE("Named Colors - Color Families") {
    SUBCASE("Test purple and violet family") {
        auto lavender = pigment::colors::lavender();
        auto violet = pigment::colors::violet();
        auto purple = pigment::colors::purple();
        auto indigo = pigment::colors::indigo();
        
        // Verify they are different colors
        CHECK_FALSE(lavender == violet);
        CHECK_FALSE(purple == indigo);
        
        // Check alpha values are set
        CHECK(lavender.a == 255);
        CHECK(violet.a == 255);
        CHECK(purple.a == 255);
        CHECK(indigo.a == 255);
    }

    SUBCASE("Test earth tones") {
        auto brown = pigment::colors::brown();
        auto tan = pigment::colors::tan();
        auto sienna = pigment::colors::sienna();
        auto chocolate = pigment::colors::chocolate();
        
        // Verify they return valid colors
        CHECK(brown.r > 0);
        CHECK(tan.r > 0);
        CHECK(sienna.r > 0);
        CHECK(chocolate.r > 0);
    }

    SUBCASE("Test cyan and turquoise family") {
        auto cyan = pigment::colors::cyan();
        auto turquoise = pigment::colors::turquoise();
        auto teal = pigment::colors::teal();
        auto aqua = pigment::colors::aqua();
        
        // Verify cyan and aqua are the same
        CHECK(cyan == aqua);
        
        // Verify they have blue/green components
        CHECK(turquoise.g > 0);
        CHECK(turquoise.b > 0);
        CHECK(teal.g > 0);
        CHECK(teal.b > 0);
    }
}