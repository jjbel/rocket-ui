/*
 * Example for Samarium
 * Project homepage: https://github.com/strangeQuark1041/samarium
 */

#include "samarium/samarium.hpp"

int main()
{
    fmt::print(fmt::emphasis::bold, "\nSuccessful installation!\n");
    fmt::print(fmt::emphasis::bold, "Welcome to {}\n", sm::version);
    sm::print("A Vector2:", sm::Vector2{.x = 5, .y = -3});
    sm::print("A Color:  ", sm::Color{.r = 5, .g = 200, .b = 255});
}
