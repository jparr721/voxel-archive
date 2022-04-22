#pragma once

#include <Eigen/Dense>
#include <cinttypes>
#include <cstddef>
#include <cstdint>

#define GLM_FORCE_RADIANS
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using usize = std::size_t;
using f32 = float;
using f64 = double;

using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::quat;
using glm::vec2;
using glm::vec3;
using glm::vec4;

// Numeric constants
constexpr float kPi = 3.1415926535897932384626433832795f;
constexpr float kPi2 = 6.2831853071795864769252867665590f;
constexpr float kInvPi = 1.0f / kPi;
constexpr float kPiHalf = 1.5707963267948966192313216916398f;
constexpr float kPiQuarter = 0.7853981633974483096156608458199f;
constexpr float kSqrt2 = 1.4142135623730950488016887242097f;
constexpr float kLogNat10 = 2.3025850929940456840179914546844f;
constexpr float kInvLogNat2 = 1.4426950408889634073599246810019f;
constexpr float kLogNat2Hi = 0.6931471805599453094172321214582f;
constexpr float kLogNat2Lo = 1.90821492927058770002e-10f;
constexpr float kE = 2.7182818284590452353602874713527f;
constexpr float kNearZero = 1.0f / float(1 << 28);
constexpr float kFloatMin = 1.175494e-38f;
constexpr float kFloatMax = 3.402823e+38f;

namespace vx {
    inline auto mad(float a, float b, float c) -> float { return a * b + c; }
    inline auto mad(const vec3 &a, float b, const vec3 &c) -> vec3 { return a * b + c; }
    inline auto mad(const vec3 &a, const vec3 &b, const vec3 &c) -> vec3 { return a * b + c; }
}// namespace vx
