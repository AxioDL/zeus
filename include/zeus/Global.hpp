#pragma once

#include "simd/simd.hpp"

namespace zeus {
template <typename SizeT>
constexpr void hash_combine_impl(SizeT& seed, SizeT value) noexcept {
  seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
} // namespace zeus
