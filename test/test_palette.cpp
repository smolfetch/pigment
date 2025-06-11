#include <doctest/doctest.h>
#include <pigment/pigment.hpp>
#include <vector>

using namespace pigment;

TEST_CASE("Palette Tests") {
    SUBCASE("Palette Construction") {
        Palette empty_palette;
        CHECK(empty_palette.empty());
        CHECK(empty_palette.size() == 0);

        std::vector<RGB> colors = {RGB::red(), RGB::green(), RGB::blue()};
        Palette palette(colors);
        CHECK(palette.size() == 3);
        CHECK_FALSE(palette.empty());
        CHECK(palette[0] == RGB::red());
        CHECK(palette[1] == RGB::green());
        CHECK(palette[2] == RGB::blue());
    }

    SUBCASE("Palette Operations") {
        Palette palette;
        palette.add(RGB::red());
        palette.add(RGB::green());
        CHECK(palette.size() == 2);

        std::vector<RGB> more_colors = {RGB::blue(), RGB::yellow()};
        palette.add(more_colors);
        CHECK(palette.size() == 4);

        palette.clear();
        CHECK(palette.empty());
    }

    SUBCASE("Palette Gradients") {
        auto gradient = Palette::gradient(RGB::red(), RGB::blue(), 5);
        CHECK(gradient.size() == 5);
        CHECK(gradient[0] == RGB::red());
        CHECK(gradient[4] == RGB::blue());

        // Check that intermediate colors are mixed
        RGB middle = gradient[2];
        CHECK(middle.r > 0);
        CHECK(middle.b > 0);
        CHECK(middle.g == 0);
    }

    SUBCASE("Predefined Palettes") {
        auto material = Palette::material_design();
        CHECK(material.size() == 16);

        auto warm = Palette::warm();
        CHECK(warm.size() == 5);

        auto cool = Palette::cool();
        CHECK(cool.size() == 5);

        auto mono = Palette::monochromatic(RGB::red(), 7);
        CHECK(mono.size() == 7);

        auto analogous_pal = Palette::analogous(RGB::blue(), 5);
        CHECK(analogous_pal.size() == 5);

        auto complementary = Palette::complementary(RGB::green());
        CHECK(complementary.size() == 2);
    }

    SUBCASE("Palette Export") {
        Palette palette({RGB::red(), RGB::green(), RGB::blue()});
        auto hex_colors = palette.to_hex();
        CHECK(hex_colors.size() == 3);
        CHECK(hex_colors[0] == "#ff0000");
        CHECK(hex_colors[1] == "#00ff00");
        CHECK(hex_colors[2] == "#0000ff");
    }
}
