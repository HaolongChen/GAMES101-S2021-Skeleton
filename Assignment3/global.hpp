#pragma once

// Modern replacement for the old macro typedefs:
//   u08  -> std::uint8_t (<cstdint>)
//   MY_PI / TWO_PI -> std::numbers::pi (<numbers>)
#include <cstdint>

using u08 = std::uint8_t;
