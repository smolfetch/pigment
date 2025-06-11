#include <cmath>
#include <doctest/doctest.h>
#include <pigment/pigment.hpp>
#include <vector>

using namespace pigment;

TEST_CASE("Utility Functions Tests") {
    SUBCASE("Contrast Ratio") {
        RGB white = RGB::white();
        RGB black = RGB::black();

        double contrast = utils::contrast_ratio(black, white);
        CHECK(contrast > 20.0); // Should be maximum contrast

        double self_contrast = utils::contrast_ratio(white, white);
        CHECK(self_contrast == 1.0); // Same color has contrast ratio of 1
    }

    SUBCASE("Accessibility") {
        RGB white = RGB::white();
        RGB black = RGB::black();

        auto level = utils::check_accessibility(black, white);
        CHECK(level == utils::AccessibilityLevel::AAA_NORMAL);

        auto best_contrast = utils::best_contrast_color(RGB::red());
        CHECK((best_contrast == RGB::white() || best_contrast == RGB::black()));
    }

    SUBCASE("Color Temperature") {
        RGB warm_color(255, 200, 100);
        RGB cool_color(100, 200, 255);

        CHECK(utils::is_warm_color(warm_color));
        CHECK_FALSE(utils::is_warm_color(cool_color));

        double temp1 = utils::color_temperature(warm_color);
        double temp2 = utils::color_temperature(cool_color);
        CHECK(temp1 < temp2); // Warm colors have lower temperature
    }

    SUBCASE("Color Harmonies") {
        RGB base(100, 150, 200);

        auto complementary = utils::generate_harmony(base, "complementary");
        CHECK(complementary.size() == 2);

        auto triadic = utils::generate_harmony(base, "triadic");
        CHECK(triadic.size() == 3);

        auto analogous = utils::generate_harmony(base, "analogous");
        CHECK(analogous.size() >= 2);

        auto tetradic = utils::generate_harmony(base, "tetradic");
        CHECK(tetradic.size() == 4);
    }

    SUBCASE("Color Distance and Sorting") {
        RGB red = RGB::red();
        RGB blue = RGB::blue();
        RGB green = RGB::green();

        double distance = utils::color_distance(red, blue);
        CHECK(distance > 100.0); // Should be large distance

        std::vector<RGB> colors = {blue, red, green};
        utils::sort_by_hue(colors);
        // After sorting by hue: red (0°), green (120°), blue (240°)
        CHECK(colors[0] == red);

        std::vector<RGB> brightness_colors = {RGB::black(), RGB::white(), RGB(128, 128, 128)};
        utils::sort_by_brightness(brightness_colors);
        CHECK(brightness_colors[0] == RGB::black());
        CHECK(brightness_colors[2] == RGB::white());
    }

    SUBCASE("Color Blindness Simulation") {
        RGB original(255, 100, 50);

        RGB protanopia = utils::ColorBlindness::simulate(original, utils::ColorBlindness::PROTANOPIA);
        RGB deuteranopia = utils::ColorBlindness::simulate(original, utils::ColorBlindness::DEUTERANOPIA);
        RGB tritanopia = utils::ColorBlindness::simulate(original, utils::ColorBlindness::TRITANOPIA);

        // The simulated colors should be different from original
        CHECK(protanopia != original);
        CHECK(deuteranopia != original);
        CHECK(tritanopia != original);

        // Test anomaly simulation (should be mix of full blindness and original)
        RGB protanomaly = utils::ColorBlindness::simulate(original, utils::ColorBlindness::PROTANOMALY);
        CHECK(protanomaly != original);
        CHECK(protanomaly != protanopia);
    }

    SUBCASE("Palette Quantization") {
        std::vector<RGB> original_colors = {RGB(255, 0, 0),  RGB(250, 10, 5), RGB(0, 255, 0),
                                            RGB(5, 250, 10), RGB(0, 0, 255),  RGB(10, 5, 250)};

        std::vector<RGB> palette = {RGB::red(), RGB::green(), RGB::blue()};

        auto quantized = utils::quantize_to_palette(original_colors, palette);
        CHECK(quantized.size() == original_colors.size());

        // Similar reds should map to red, etc.
        CHECK(quantized[0] == RGB::red());
        CHECK(quantized[1] == RGB::red());
        CHECK(quantized[2] == RGB::green());
        CHECK(quantized[3] == RGB::green());
        CHECK(quantized[4] == RGB::blue());
        CHECK(quantized[5] == RGB::blue());
    }
}
