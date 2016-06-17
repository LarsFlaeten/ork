#ifndef _PORTABLE_MATH_H_
#define _PORTABLE_MATH_H_

/**
 * Portable math header.
 * Instead of using <math.h> or/and <cmath>, use this.
 * Also provides helper functions:
 *     isNaN, isFinite, sincos, mix, step, smoothStep,
 *     square, signedSquare, clamp,
 *     degrees, radians, safe_asin, safe_acos, round, ipow
 */

#include <math.h>

#if defined(_MSC_VER)
#   include <float.h>
#endif

#include <cmath>

#if defined(__GNUC__)

// for size_t
#include <cstring>

#if 0 // if #include <cmath> bugs, uncomment this

// math function for GCC (basically what cmath would do,
// but including cmath is buggy here)

#undef sin
#undef cos
#undef tan
#undef sinh
#undef cosh
#undef tanh
#undef asin
#undef acos
#undef atan
#undef atan2
#undef exp
#undef log
#undef log10
#undef pow
#undef sqrt
#undef ceil
#undef floor
#undef fabs
#undef ldexp
#undef frexp
#undef modf
#undef fmod

inline double abs(double x)
{
    return __builtin_fabs(x);
}

inline float abs(float x)
{
    return __builtin_fabsf(x);
}

inline long double abs(long double x)
{
    return __builtin_fabsl(x);
}

inline float acos(float x)
{
    return __builtin_acosf(x);
}

inline double acos(double x)
{
    return __builtin_acos(x);
}

inline long double acos(long double x)
{
    return __builtin_acosl(x);
}

inline float asin(float x)
{
    return __builtin_asinf(x);
}

inline double asin(double x)
{
    return __builtin_asin(x);
}

inline long double asin(long double x)
{
    return __builtin_asinl(x);
}

inline float atan(float x)
{
    return __builtin_atanf(x);
}

inline double atan(double x)
{
    return __builtin_atan(x);
}

inline long double atan(long double x)
{
    return __builtin_atanl(x);
}

inline float atan2(float y, float x)
{
    return __builtin_atan2f(y, x);
}

inline double atan2(double y, double x)
{
    return __builtin_atan2(y, x);
}

inline long double atan2(long double y, long double x)
{
    return __builtin_atan2l(y, x);
}

inline float ceil(float x)
{
    return __builtin_ceilf(x);
}

inline double ceil(double x)
{
    return __builtin_ceil(x);
}

inline long double ceil(long double x)
{
    return __builtin_ceill(x);
}

inline float cos(float x)
{
    return __builtin_cosf(x);
}

inline double cos(double x)
{
    return __builtin_cos(x);
}

inline long double cos(long double x)
{
    return __builtin_cosl(x);
}

inline float sin(float x)
{
    return __builtin_sinf(x);
}

inline double sin(double x)
{
    return __builtin_sin(x);
}

inline long double sin(long double x)
{
    return __builtin_sinl(x);
}

inline float tan(float x)
{
    return __builtin_tanf(x);
}

inline double tan(double x)
{
    return __builtin_tan(x);
}

inline long double tan(long double x)
{
    return __builtin_tanl(x);
}

inline float cosh(float x)
{
    return __builtin_coshf(x);
}

inline double cosh(double x)
{
    return __builtin_cosh(x);
}

inline long double cosh(long double x)
{
    return __builtin_coshl(x);
}

inline float sinh(float x)
{
    return __builtin_sinhf(x);
}

inline double sinh(double x)
{
    return __builtin_sinh(x);
}

inline long double sinh(long double x)
{
    return __builtin_sinhl(x);
}

inline float tanh(float x)
{
    return __builtin_tanhf(x);
}

inline double tanh(double x)
{
    return __builtin_tanh(x);
}

inline long double tanh(long double x)
{
    return __builtin_tanhl(x);
}

inline float exp(float x)
{
    return __builtin_expf(x);
}

inline double exp(double x)
{
    return __builtin_exp(x);
}

inline long double exp(long double x)
{
    return __builtin_expl(x);
}

#define fabs abs

/*
inline float fabs(float x)
{
    return __builtin_fabsf(x)
}

inline double fabs(double x)
{
    return __builtin_fabs(x);
}

inline long double fabs(long double x)
{
    return __builtin_fabsl(x);
}
*/

inline float floor(float x)
{
    return __builtin_floorf(x);
}

inline double floor(double x)
{
    return __builtin_floor(x);
}

inline long double floor(long double x)
{
    return __builtin_floorl(x);
}

inline float fmod(float x, float y)
{
    return __builtin_fmodf(x, y);
}

inline double fmod(double x, double y)
{
    return __builtin_fmod(x, y);
}

inline long double fmod(long double x, long double y)
{
    return __builtin_fmodl(x, y);
}

inline float frexp(float x, int* exp)
{
    return __builtin_frexpf(x, exp);
}

inline double frexp(double x, int* exp)
{
    return __builtin_frexp(x, exp);
}

inline long double frexp(long double x, int* exp)
{
    return __builtin_frexpl(x, exp);
}

inline float ldexp(float x, int exp)
{
    return __builtin_ldexpf(x, exp);
}

inline double ldexp(double x, int exp)
{
    return __builtin_ldexp(x, exp);
}

inline long double ldexp(long double x, int exp)
{
    return __builtin_ldexpl(x, exp);
}

inline float log(float x)
{
    return __builtin_logf(x);
}

inline double log(double x)
{
    return __builtin_log(x);
}

inline long double log(long double x)
{
    return __builtin_logl(x);
}

inline float log10(float x)
{
    return __builtin_log10f(x);
}

inline double log10(double x)
{
    return __builtin_log10(x);
}

inline long double log10(long double x)
{
    return __builtin_log10l(x);
}

inline float modf(float x, float* iptr)
{
    return __builtin_modff(x, iptr);
}

inline double modf(double x, double* iptr)
{
    return __builtin_modf(x, iptr);
}

inline long double modf(long double x, long double* iptr)
{
    return __builtin_modfl(x, iptr);
}

/**
 * Raise to an integral power. Assumes exp >= 0.
 */
template<typename T>
T ipow(T base, int exp)
{
    if (exp < 0) {
        return (T) 1 / ipow(base, -exp);
    }

    T result = 1;
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }
    return result;
}

inline float pow(float x, float y)
{
    return __builtin_powf(x, y);
}

inline double pow(double x, double y)
{
    return __builtin_pow(x, y);
}

inline long double pow(long double x, long double y)
{
    return __builtin_powl(x, y);
}

inline float pow(float x, int n)
{
    return ipow(x, n);
}

inline double pow(double x, int n)
{
    return ipow(x, n);
}

inline long double pow(long double x, int n)
{
    return ipow(x, n);
}

inline float sqrt(float x)
{
    return __builtin_sqrtf(x);
}

inline double sqrt(double x)
{
    return __builtin_sqrt(x);
}

inline long double sqrt(long double x)
{
    return __builtin_sqrtl(x);
}

#endif

#else

//#include <cmath.h>

#endif

#undef M_E
#undef M_LOG2E
#undef M_LOG10E
#undef M_LN2
#undef M_LN10
#undef M_PI
#undef M_PI_2
#undef M_PI_4
#undef M_1_PI
#undef M_2_PI
#undef M_2_SQRTPI
#undef M_SQRT2
#undef M_SQRT1_2
#undef M_1_LN_2
#undef M_LN2

/**
 * Allows to define a floating point constant
 *
 * Ex: DEFINE_FLOAT_CONSTANT(M_ONE, 1.1645L)
 *
 *                  produces
 *
 *     const float M_ONE_F = (float)1.1645L;
 *     const double M_ONE = (double)1.1645L;
 *     const long double M_ONE_L = (long double)1.1645L;
 */
#define DEFINE_FLOAT_CONSTANT(name,expr)\
    const float name##_F = (const float)(expr);\
    const double name = (const double)(expr);\
    const long double name##_L = (const long double)(expr);

DEFINE_FLOAT_CONSTANT(M_E, 2.7182818284590452354L)
DEFINE_FLOAT_CONSTANT(M_LOG2E, 1.4426950408889634074L)
DEFINE_FLOAT_CONSTANT(M_LOG10E, 0.43429448190325182765L)
DEFINE_FLOAT_CONSTANT(M_LN2, 0.69314718055994530942L)
DEFINE_FLOAT_CONSTANT(M_LN10, 2.30258509299404568402L)
DEFINE_FLOAT_CONSTANT(M_PI, 3.14159265358979323846L)
DEFINE_FLOAT_CONSTANT(M_PI_X_2, 3.14159265358979323846L * 2.L)
DEFINE_FLOAT_CONSTANT(M_PI_2, 1.57079632679489661923L)
DEFINE_FLOAT_CONSTANT(M_PI_4, 0.78539816339744830962L)
DEFINE_FLOAT_CONSTANT(M_1_PI, 0.31830988618379067154L)
DEFINE_FLOAT_CONSTANT(M_2_PI, 0.63661977236758134308L)
DEFINE_FLOAT_CONSTANT(M_2_SQRTPI, 1.12837916709551257390L)
DEFINE_FLOAT_CONSTANT(M_SQRT2, 1.41421356237309504880L)
DEFINE_FLOAT_CONSTANT(M_SQRT1_2, 0.70710678118654752440L)
DEFINE_FLOAT_CONSTANT(M_1_LN_2,  1.44269504088896340736L)

#ifndef NAN
#if __GNUC__
#   define NAN __builtin_nan
#elif defined(_MSC_VER)
#   define NAN (sqrt(-1.f))
#else
#error NAN not defined for this compiler
#endif
#endif


// horrible hack for having INFINITY with MSVC
#ifndef INFINITY
    #ifdef _MSC_VER
        union MSVC_EVIL_FLOAT_HACK
        {
            unsigned __int8 Bytes[4];
            float v;
        };
        static union MSVC_EVIL_FLOAT_HACK INFINITY_HACK = {{0x00, 0x00, 0x80, 0x7F}};
        #define INFINITY (INFINITY_HACK.v)
    #endif
#endif

// portable isFinite, isNaN and isInf
// assumes long long int support
typedef union
{
    float f;
    unsigned int i;
} UIF32;

typedef union
{
    double f;
    unsigned long long int i;
} ULLIF32;

// portable union to cast from pointer to int type
union UIPTR {
    void* ptr;
    size_t ui;
};

/**
 * Check for pointer alignment.
 * @param p the pointer to chack.
 * @param alignment the alignment in bytes (must be a power of two).
 */
inline bool isPointerAligned(void* p, size_t alignment)
{
    UIPTR a;
    a.ptr = p;
    size_t mask = ~(alignment - 1);
    return (a.ui & mask) == a.ui;
}

/**
 * Align a pointer to the next position which satisfied an alignment.
 * @param p the pointer to chack.
 * @param alignment the alignment in bytes (must be a power of two).
 */
inline void* alignPointer(void* p, size_t alignment)
{
    UIPTR a;
    a.ptr = p;
    size_t mask = ~(alignment - 1);
    a.ui = (a.ui + alignment - 1) & mask;
    return a.ptr;
}


/**
 * Returns true if x is a NaN.
 */
inline bool isNaN(float x)
{
    // works because int is 32 bits on most platforms
    UIF32 u;
    u.f = x;
    return ((u.i & 0x7F800000UL) == 0x7F800000UL)
        && (u.i & 0x000FFFFFUL);
}

/**
 * Returns true if x is a NaN.
 */
inline bool isNaN(double x)
{
    ULLIF32 u;
    u.f = x;
    return ((u.i & 0x7FF0000000000000ULL) == 0x7FF0000000000000ULL)
         && (u.i & 0x000FFFFFFFFFFFFFULL);
}

/**
 * Returns true if x is a finite number (neither a NaN nor an infinite).
 */
inline bool isFinite(float x)
{
    UIF32 u;
    u.f = x;
    return (u.i & 0x7F800000UL) != 0x7F800000UL;
}

/**
 * Returns true if x is a finite number (neither a NaN nor an infinite).
 */
inline bool isFinite(double x)
{
    ULLIF32 u;
    u.f = x;
    return (u.i & 0x7FF0000000000000ULL) != 0x7FF0000000000000ULL;
}

/**
 * Returns true if x is an infinite number.
 */
inline bool isInf(float x)
{
    UIF32 u;
    u.f = x;
    return (u.i & 0x7FFFFFFFUL) == 0x7F800000UL;
}

/**
 * Returns true if x is an infinite number.
 */
inline bool isInf(double x)
{
    ULLIF32 u;
    u.f = x;
    return (u.i & 0x7FFFFFFFFFFFFFFFULL) == 0x7FF0000000000000ULL;
}

/**
 * Returns true if x is an infinite number.
 */
inline bool isSubnormal(float x)
{
    UIF32 u;
    u.f = x;
    return ((u.i & 0x7F800000UL) == 0) && ((u.i & 0x007FFFFF) != 0);
}

/**
 * Returns true if x is an infinite number.
 */
inline bool isSubnormal(double x)
{
    ULLIF32 u;
    u.f = x;
    return ((u.i & 0x7FF0000000000000ULL) == 0) && ((u.i & 0x000FFFFFFFFFFFFFULL) != 0);
}


/**
 * Returns the sinus and cosinus of an angle.
 */
template<typename T>
inline void sincos(T angleRad, T& sinResult, T& cosResult)
{
    sinResult = sin(angleRad);
    cosResult = cos(angleRad);
}

/**
 * Returns v^2.
 */
template<typename T>
inline T square(T v)
{
    return v * v;
}

/**
 * Returns sign(v) * v^2.
 */
template<typename T>
inline T signedSquare(T v)
{
    return v < (T) 0 ? -v * v : v * v;
}

/**
 * Returns v rounded to the nearest integer.
 */
template<typename T>
inline T round(T v)
{
    return v >= (T) 0 ? floor(v + (T) 0.5L) : ceil(v - (T) 0.5L);
}

/**
 * Returns (1-t) a + t b (like in GLSL).
 */
template<typename T>
inline T mix(T a, T b, T t)
{
    return ((T) 1 - t) * a + t * b;
}

/**
 * Returns a value between 0 and 1 depending on the value of t compared to
 * a and b (NOT like GLSL).
 *
 * @return 0 if t < a, 1 if t > b, and (t - a) / (b - a) otherwise.
 */
template<typename T>
inline T step(T a, T b, T t)
{
    if (t <= a) {
        return 0;
    }
    if (t >= b) {
        return 1;
    }
    return (t - a) / (b - a);
}

/**
 * Returns a value between 0 and 1 depending on the value of t compared to
 * a and b (like GLSL's smoothstep).
 *
 * @return 0 if t < a, 1 if t > b, and a smooth transition between a and b.
 */
template<typename T>
inline T smoothStep(T a, T b, T t)
{
    if (t <= a) {
        return 0;
    }
    if (t >= b) {
        return 1;
    }
    T x = (t - a) / (b - a);
    return x * x * (static_cast<T>(3) - static_cast<T>(2) * x);
}

/**
 * Returns x converted from radians to degrees (like the GLSL function).
 */
inline float degrees(float x)
{
    return x * (180.f / M_PI_F);
}

/**
 * Returns x converted from radians to degrees (like the GLSL function).
 */
inline double degrees(double x)
{
    return x * (180.0 / M_PI);
}

/**
 * Returns x converted from radians to degrees (like the GLSL function).
 */
inline long double degrees(long double x)
{
    return x * (180.0L / M_PI_L);
}

/**
 * Returns x converted from degrees to radians (like the GLSL function).
 */
inline float radians(float x)
{
    return x * (M_PI_F / 180.f);
}

/**
 * Returns x converted from degrees to radians (like the GLSL function).
 */
inline double radians(double x)
{
    return x * (M_PI / 180.0);
}

/**
 * Returns x converted from degrees to radians (like the GLSL function).
 */
inline long double radians(long double x)
{
    return x * (M_PI_L / 180.0L);
}

/**
 * Returns the arccosinus of x clamped to [-1,1].
 */
template<typename T>
inline T safe_acos(T x)
{
    if (x <= -1) {
        x = -1;
    } else if (x >= 1) {
        x = 1;
    }
    return acos(x);
}

/**
 * Returns the arcsinus of x clamped to [-1,1].
 */
template<typename T>
inline T safe_asin(T x)
{
    if (x <= -1) {
        x = -1;
    } else if (x >= 1) {
        x = 1;
    }
    return asin(x);
}

/**
 * Returns min if x < min, max if x > max, or x otherwise.
 */
template<typename T>
inline T clamp(T x, T min, T max)
{
    if (x <= min) {
        return min;
    } else if (x >= max) {
        return max;
    } else {
        return x;
    }
}

/**
 * Map x from the range [a, b] to the range [c, d].
 */
template <class T>
inline T remap(T value, T a, T b, T c, T d)
{
    return c + (d - c) * (value - a) / (b - a);
}

#endif
