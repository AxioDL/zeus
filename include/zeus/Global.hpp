#pragma once

#if ZE_ATHENA_TYPES
#include "athena/IStreamReader.hpp"
#include "athena/simd/simd.hpp"
#else
#include "simd/simd.hpp"
#endif

#if ZE_HSH_TYPES
#include "hsh/hsh.h"
#endif

namespace zeus {
#if ZE_ATHENA_TYPES
template <typename T>
using simd = athena::simd<T>;
using simd_floats = athena::simd_floats;
using simd_doubles = athena::simd_doubles;
#endif

template <typename SizeT>
constexpr void hash_combine_impl(SizeT& seed, SizeT value) noexcept {
  seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
} // namespace zeus
