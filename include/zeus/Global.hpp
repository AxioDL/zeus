#pragma once

#if ZE_ATHENA_TYPES
#include "athena/IStreamReader.hpp"
#include "athena/simd/simd.hpp"
#else
#include "simd/simd.hpp"
#endif

namespace zeus {
#if ZE_ATHENA_TYPES
template <typename T>
using simd = athena::simd<T>;
using simd_floats = athena::simd_floats;
using simd_doubles = athena::simd_doubles;
#endif
} // namespace zeus

inline int rotr(int x, int n) { return ((x >> n) | (x << (32 - n))); }
inline int rotl(int x, int n) { return ((x << n) | (x >> (32 - n))); }
