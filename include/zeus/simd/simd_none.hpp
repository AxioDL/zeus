#pragma once
#ifndef _ZEUS_SIMD_INCLUDED
#error simd_none.hpp must not be included directly. Include simd.hpp instead.
#endif
namespace zeus::_simd {
using m128_abi = __simd_abi<_StorageKind::_Array, 4>;
using m128d_abi = __simd_abi<_StorageKind::_Array, 4>;

// m128 ABI
template <>
inline simd<float, m128_abi> simd<float, m128_abi>::operator-() const {
  return {-__s_.__storage_[0], -__s_.__storage_[1], -__s_.__storage_[2], -__s_.__storage_[3]};
}

inline simd<float, m128_abi> operator+(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  return {a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]};
}

inline simd<float, m128_abi> operator-(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  return {a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]};
}

inline simd<float, m128_abi> operator*(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  return {a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]};
}

inline simd<float, m128_abi> operator/(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  return {a[0] / b[0], a[1] / b[1], a[2] / b[2], a[3] / b[3]};
}

inline simd<float, m128_abi>& operator+=(simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  a[0] += b[0];
  a[1] += b[1];
  a[2] += b[2];
  a[3] += b[3];
  return a;
}

inline simd<float, m128_abi>& operator-=(simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  a[0] -= b[0];
  a[1] -= b[1];
  a[2] -= b[2];
  a[3] -= b[3];
  return a;
}

inline simd<float, m128_abi>& operator*=(simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  a[0] *= b[0];
  a[1] *= b[1];
  a[2] *= b[2];
  a[3] *= b[3];
  return a;
}

inline simd<float, m128_abi>& operator/=(simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  a[0] /= b[0];
  a[1] /= b[1];
  a[2] /= b[2];
  a[3] /= b[3];
  return a;
}

inline simd<float, m128_abi>::mask_type operator==(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret[0] = a[0] == b[0];
  ret[1] = a[1] == b[1];
  ret[2] = a[2] == b[2];
  ret[3] = a[3] == b[3];
  return ret;
}

inline simd<float, m128_abi>::mask_type operator!=(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret[0] = a[0] != b[0];
  ret[1] = a[1] != b[1];
  ret[2] = a[2] != b[2];
  ret[3] = a[3] != b[3];
  return ret;
}

inline simd<float, m128_abi>::mask_type operator>=(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret[0] = a[0] >= b[0];
  ret[1] = a[1] >= b[1];
  ret[2] = a[2] >= b[2];
  ret[3] = a[3] >= b[3];
  return ret;
}

inline simd<float, m128_abi>::mask_type operator<=(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret[0] = a[0] <= b[0];
  ret[1] = a[1] <= b[1];
  ret[2] = a[2] <= b[2];
  ret[3] = a[3] <= b[3];
  return ret;
}

inline simd<float, m128_abi>::mask_type operator>(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret[0] = a[0] > b[0];
  ret[1] = a[1] > b[1];
  ret[2] = a[2] > b[2];
  ret[3] = a[3] > b[3];
  return ret;
}

inline simd<float, m128_abi>::mask_type operator<(const simd<float, m128_abi>& a, const simd<float, m128_abi>& b) {
  simd<float, m128_abi>::mask_type ret;
  ret[0] = a[0] < b[0];
  ret[1] = a[1] < b[1];
  ret[2] = a[2] < b[2];
  ret[3] = a[3] < b[3];
  return ret;
}

// m128d ABI
template <>
inline simd<double, m128d_abi> simd<double, m128d_abi>::operator-() const {
  return {-__s_.__storage_[0], -__s_.__storage_[1], -__s_.__storage_[2], -__s_.__storage_[3]};
}

inline simd<double, m128d_abi> operator+(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  return {a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]};
}

inline simd<double, m128d_abi> operator-(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  return {a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]};
}

inline simd<double, m128d_abi> operator*(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  return {a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]};
}

inline simd<double, m128d_abi> operator/(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  return {a[0] / b[0], a[1] / b[1], a[2] / b[2], a[3] / b[3]};
}

inline simd<double, m128d_abi>& operator+=(simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  a[0] += b[0];
  a[1] += b[1];
  a[2] += b[2];
  a[3] += b[3];
  return a;
}

inline simd<double, m128d_abi>& operator-=(simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  a[0] -= b[0];
  a[1] -= b[1];
  a[2] -= b[2];
  a[3] -= b[3];
  return a;
}

inline simd<double, m128d_abi>& operator*=(simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  a[0] *= b[0];
  a[1] *= b[1];
  a[2] *= b[2];
  a[3] *= b[3];
  return a;
}

inline simd<double, m128d_abi>& operator/=(simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  a[0] /= b[0];
  a[1] /= b[1];
  a[2] /= b[2];
  a[3] /= b[3];
  return a;
}

inline simd<double, m128d_abi>::mask_type operator==(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  ret[0] = a[0] == b[0];
  ret[1] = a[1] == b[1];
  ret[2] = a[2] == b[2];
  ret[3] = a[3] == b[3];
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator!=(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  ret[0] = a[0] != b[0];
  ret[1] = a[1] != b[1];
  ret[2] = a[2] != b[2];
  ret[3] = a[3] != b[3];
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator>=(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  ret[0] = a[0] >= b[0];
  ret[1] = a[1] >= b[1];
  ret[2] = a[2] >= b[2];
  ret[3] = a[3] >= b[3];
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator<=(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  ret[0] = a[0] <= b[0];
  ret[1] = a[1] <= b[1];
  ret[2] = a[2] <= b[2];
  ret[3] = a[3] <= b[3];
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator>(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  ret[0] = a[0] > b[0];
  ret[1] = a[1] > b[1];
  ret[2] = a[2] > b[2];
  ret[3] = a[3] > b[3];
  return ret;
}

inline simd<double, m128d_abi>::mask_type operator<(const simd<double, m128d_abi>& a, const simd<double, m128d_abi>& b) {
  simd<double, m128d_abi>::mask_type ret;
  ret[0] = a[0] < b[0];
  ret[1] = a[1] < b[1];
  ret[2] = a[2] < b[2];
  ret[3] = a[3] < b[3];
  return ret;
}
} // namespace zeus::_simd
