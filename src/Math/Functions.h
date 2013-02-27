#ifndef Magnum_Math_Functions_h
#define Magnum_Math_Functions_h
/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include <cmath>
#include <type_traits>
#include <limits>

#include "Math/Vector.h"

#include "magnumVisibility.h"

/** @file
 * @brief Functions usable with scalar and vector types
 */

namespace Magnum { namespace Math {

#ifndef DOXYGEN_GENERATING_OUTPUT
namespace Implementation {
    template<std::uint32_t exponent> struct Pow {
        Pow() = delete;

        template<class T> inline constexpr static T pow(T base) {
            return base*Pow<exponent-1>::pow(base);
        }
    };
    template<> struct Pow<0> {
        Pow() = delete;

        template<class T> inline constexpr static T pow(T) { return 1; }
    };
}
#endif

/**
 * @brief Integral power
 *
 * Returns integral power of base to the exponent.
 */
template<std::uint32_t exponent, class T> inline constexpr T pow(T base) {
    return Implementation::Pow<exponent>::pow(base);
}

/**
 * @brief Base-2 integral logarithm
 *
 * Returns integral logarithm of given number with base `2`.
 * @see log()
 */
std::uint32_t MAGNUM_EXPORT log2(std::uint32_t number);

/**
 * @brief Integral logarithm
 *
 * Returns integral logarithm of given number with given base.
 * @see log2()
 */
std::uint32_t MAGNUM_EXPORT log(std::uint32_t base, std::uint32_t number);

/** @brief Sine */
template<class T> inline T sin(Rad<T> angle) { return std::sin(T(angle)); }

/** @brief Cosine */
template<class T> inline T cos(Rad<T> angle) { return std::cos(T(angle)); }

/** @brief Tangent */
template<class T> inline T tan(Rad<T> angle) { return std::tan(T(angle)); }

/** @todo Can't trigonometric functions be done with only one overload? */
#ifndef DOXYGEN_GENERATING_OUTPUT
template<class T> inline T sin(Deg<T> angle) { return sin(Rad<T>(angle)); }
template<class T> inline T cos(Deg<T> angle) { return cos(Rad<T>(angle)); }
template<class T> inline T tan(Deg<T> angle) { return tan(Rad<T>(angle)); }
#endif

/** @brief Arc sine */
template<class T> inline Rad<T> asin(T value) { return Rad<T>(std::asin(value)); }

/** @brief Arc cosine */
template<class T> inline Rad<T> acos(T value) { return Rad<T>(std::acos(value)); }

/** @brief Arc tangent */
template<class T> inline Rad<T> atan(T value) { return Rad<T>(std::atan(value)); }

/**
@{ @name Scalar/vector functions

These functions are overloaded for both scalar and vector types. Scalar
versions function exactly as their possible STL equivalents, vector overloads
perform the operations component-wise.
*/

/**
@brief Minimum

@see min(), clamp()
*/
#ifdef DOXYGEN_GENERATING_OUTPUT
template<class T> inline T min(T a, T b);
#else
template<class T> inline typename std::enable_if<std::is_arithmetic<T>::value, T>::type min(T a, T b) {
    return std::min(a, b);
}
template<std::size_t size, class T> inline Vector<size, T> min(const Vector<size, T>& a, const Vector<size, T>& b) {
    Vector<size, T> out;
    for(std::size_t i = 0; i != size; ++i)
        out[i] = std::min(a[i], b[i]);
    return out;
}
#endif

/**
@brief Maximum

@see max(), clamp()
*/
#ifdef DOXYGEN_GENERATING_OUTPUT
template<class T> inline T max(const T& a, const T& b);
#else
template<class T> inline typename std::enable_if<std::is_arithmetic<T>::value, T>::type max(T a, T b) {
    return std::max(a, b);
}
template<std::size_t size, class T> Vector<size, T> max(const Vector<size, T>& a, const Vector<size, T>& b) {
    Vector<size, T> out;
    for(std::size_t i = 0; i != size; ++i)
        out[i] = std::max(a[i], b[i]);
    return out;
}
#endif

/** @brief Absolute value */
#ifdef DOXYGEN_GENERATING_OUTPUT
template<class T> inline T abs(const T& a);
#else
template<class T> inline typename std::enable_if<std::is_arithmetic<T>::value, T>::type abs(T a) {
    return std::abs(a);
}
template<std::size_t size, class T> Vector<size, T> abs(const Vector<size, T>& a) {
    Vector<size, T> out;
    for(std::size_t i = 0; i != size; ++i)
        out[i] = std::abs(a[i]);
    return out;
}
#endif

/** @brief Square root */
#ifdef DOXYGEN_GENERATING_OUTPUT
template<class T> inline T sqrt(const T& a);
#else
template<class T> inline typename std::enable_if<std::is_arithmetic<T>::value, T>::type sqrt(T a) {
    return std::sqrt(a);
}
template<std::size_t size, class T> Vector<size, T> sqrt(const Vector<size, T>& a) {
    Vector<size, T> out;
    for(std::size_t i = 0; i != size; ++i)
        out[i] = std::sqrt(a[i]);
    return out;
}
#endif

/**
@brief Clamp value

Values smaller than @p min are set to @p min, values larger than @p max are
set to @p max.
@see min(), max()
*/
#ifdef DOXYGEN_GENERATING_OUTPUT
template<class T, class U> inline T clamp(const T& value, U min, U max);
#else
template<class T> inline typename std::enable_if<std::is_arithmetic<T>::value, T>::type clamp(T value, T min, T max) {
    return std::min(std::max(value, min), max);
}
template<std::size_t size, class T> Vector<size, T> clamp(const Vector<size, T>& value, T min, T max) {
    Vector<size, T> out;
    for(std::size_t i = 0; i != size; ++i)
        out[i] = std::min(std::max(value[i], min), max);
    return out;
}
#endif

/**
@brief Linear interpolation of two values
@param a     First value
@param b     Second value
@param t     Interpolation phase (from range @f$ [0; 1] @f$)

The interpolation for vectors is done as in following, similarly for scalars: @f[
    \boldsymbol v_{LERP} = (1 - t) \boldsymbol v_A + t \boldsymbol v_B
@f]
@see Quaternion::lerp()
@todo http://fgiesen.wordpress.com/2012/08/15/linear-interpolation-past-present-and-future/
    (when SIMD is in place)
*/
#ifdef DOXYGEN_GENERATING_OUTPUT
template<class T, class U> inline T lerp(const T& a, const T& b, U t);
#else
template<class T, class U> inline T lerp(T a, T b, U t) {
    return (U(1) - t)*a + t*b;
}
template<std::size_t size, class T, class U> inline Vector<size, T> lerp(const Vector<size, T>& a, const Vector<size, T>& b, U t) {
    return (U(1) - t)*a + t*b;
}
#endif

/**
@brief Normalize integral value

Converts integral value from full range of given *unsigned* integral type to
value in range @f$ [0, 1] @f$ or from *signed* integral to range @f$ [-1, 1] @f$.

@note For best precision, resulting `FloatingPoint` type should be always
    larger that `Integral` type (e.g. `double` from `std::int32_t`, `long double`
    from `std::int64_t` and similarly for vector types).

@attention To ensure the integral type is correctly detected when using
    literals, this function should be called with both template parameters
    explicit, e.g.:
@code
// Literal type is (signed) char, but we assumed unsigned char, a != 1.0f
float a = normalize<float>('\xFF');

// b = 1.0f
float b = normalize<float, std::uint8_t>('\xFF');
@endcode

@see denormalize()
*/
#ifdef DOXYGEN_GENERATING_OUTPUT
template<class FloatingPoint, class Integral> inline FloatingPoint normalize(const Integral& value);
#else
template<class FloatingPoint, class Integral> inline typename std::enable_if<std::is_arithmetic<Integral>::value && std::is_unsigned<Integral>::value, FloatingPoint>::type normalize(Integral value) {
    static_assert(std::is_floating_point<FloatingPoint>::value && std::is_integral<Integral>::value,
                  "Math::normalize(): normalization must be done from integral to floating-point type");
    return value/FloatingPoint(std::numeric_limits<Integral>::max());
}
template<class FloatingPoint, class Integral> inline typename std::enable_if<std::is_arithmetic<Integral>::value && std::is_signed<Integral>::value, FloatingPoint>::type normalize(Integral value) {
    static_assert(std::is_floating_point<FloatingPoint>::value && std::is_integral<Integral>::value,
                  "Math::normalize(): normalization must be done from integral to floating-point type");
    return Math::max(value/FloatingPoint(std::numeric_limits<Integral>::max()), FloatingPoint(-1));
}
template<class FloatingPoint, class Integral> inline typename std::enable_if<std::is_unsigned<typename Integral::Type>::value, FloatingPoint>::type normalize(const Integral& value) {
    static_assert(std::is_floating_point<typename FloatingPoint::Type>::value && std::is_integral<typename Integral::Type>::value,
                  "Math::normalize(): normalization must be done from integral to floating-point type");
    return FloatingPoint(value)/typename FloatingPoint::Type(std::numeric_limits<typename Integral::Type>::max());
}
template<class FloatingPoint, class Integral> inline typename std::enable_if<std::is_signed<typename Integral::Type>::value, FloatingPoint>::type normalize(const Integral& value) {
    static_assert(std::is_floating_point<typename FloatingPoint::Type>::value && std::is_integral<typename Integral::Type>::value,
                  "Math::normalize(): normalization must be done from integral to floating-point type");
    return Math::max(FloatingPoint(value)/typename FloatingPoint::Type(std::numeric_limits<typename Integral::Type>::max()), FloatingPoint(-1));
}
#endif

/**
@brief Denormalize floating-point value

Converts floating-point value in range @f$ [0, 1] @f$ to full range of given
*unsigned* integral type or range @f$ [-1, 1] @f$ to full range of given *signed*
integral type.

@note For best precision, `FloatingPoint` type should be always larger that
    resulting `Integral` type (e.g. `double` to `std::int32_t`, `long double`
    to `std::int64_t` and similarly for vector types).

@attention Return value for floating point numbers outside the normalized
    range is undefined.

@see normalize()
*/
#ifdef DOXYGEN_GENERATING_OUTPUT
template<class Integral, class FloatingPoint> inline Integral denormalize(const FloatingPoint& value);
#else
template<class Integral, class FloatingPoint> inline typename std::enable_if<std::is_arithmetic<FloatingPoint>::value, Integral>::type denormalize(FloatingPoint value) {
    static_assert(std::is_floating_point<FloatingPoint>::value && std::is_integral<Integral>::value,
                  "Math::denormalize(): denormalization must be done from floating-point to integral type");
    return value*std::numeric_limits<Integral>::max();
}
template<class Integral, class FloatingPoint> inline typename std::enable_if<std::is_arithmetic<typename Integral::Type>::value, Integral>::type denormalize(const FloatingPoint& value) {
    static_assert(std::is_floating_point<typename FloatingPoint::Type>::value && std::is_integral<typename Integral::Type>::value,
                  "Math::denormalize(): denormalization must be done from floating-point to integral type");
    return Integral(value*std::numeric_limits<typename Integral::Type>::max());
}
#endif

/*@}*/

}}

#endif
