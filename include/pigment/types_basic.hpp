#pragma once

#include <random>
#include <stdexcept>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

namespace pigment {

    struct RGB {
        int r = 0;
        int g = 0;
        int b = 0;
        int a = 255;

        RGB() = default;
        RGB(int r_, int g_, int b_, int a_ = 255) : r(r_), g(g_), b(b_), a(a_) {}

        RGB(const std::string &hex) {
            std::string h = hex;
            if (!h.empty() && h[0] == '#') {
                h.erase(0, 1);
            }
            if (h.size() == 3) {
                std::string tmp;
                tmp.reserve(6);
                for (char c : h) {
                    tmp.push_back(c);
                    tmp.push_back(c);
                }
                h = tmp;
            }
            if (h.size() == 6) {
                h += "ff";
            }
            if (h.size() != 8) {
                throw std::invalid_argument("Invalid hex color: '" + hex + "'");
            }
            r = std::stoi(h.substr(0, 2), nullptr, 16);
            g = std::stoi(h.substr(2, 2), nullptr, 16);
            b = std::stoi(h.substr(4, 2), nullptr, 16);
            a = std::stoi(h.substr(6, 2), nullptr, 16);
        }

        static RGB random() {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, 255);
            return RGB(dist(gen), dist(gen), dist(gen), 255);
        }
    };

    struct MONO {
        int v = 0;
        int a = 255;

        MONO() = default;
        MONO(int v_, int a_ = 255) : v(v_), a(a_) {}

        static MONO random() {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, 255);
            return MONO(dist(gen), 255);
        }
    };
} // namespace pigment
