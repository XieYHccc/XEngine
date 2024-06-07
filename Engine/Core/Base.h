#pragma once
#include <memory>
#include "Platform/Detection.h"

// Causes a debug breakpoint to be hit.
#if defined(QK_PLATFORM_WINDOWS)
    #include <intrin.h>
    #define QK_DEBUGBREAK() __debugbreak()
#elif defined(QK_PLATFORM_MACOS)
    #define QK_DEBUGBREAK() __builtin_trap()
#else
    #error "Platform doesn't support debugbreak yet!"
#endif

// Properly define static assertions.
#define QK_STATIC_ASSERT static_assert

// Unsigned int types.
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

// Signed int types.
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

// Floating point types
using f32 = float;
using f64 = double;

// Ensure all types are of the correct size.
QK_STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
QK_STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
QK_STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
QK_STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");
QK_STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
QK_STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
QK_STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
QK_STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");
QK_STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
QK_STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");
QK_STATIC_ASSERT(sizeof(bool) == 1, "Expected bool to be 1 byte.");

// Import/export qualifier
#ifdef QK_EXPORT
// Exports
    #ifdef _MSC_VER
        #define QK_API __declspec(dllexport)
    #else
        #define QK_API __attribute__((visibility("default")))
    #endif
#else
// Imports
    #ifdef _MSC_VER
        #define QK_API __declspec(dllimport)
    #else
        #define QK_API
    #endif
#endif

// Inlining
#if defined(__clang__) || defined(__gcc__)
    #define QK_FORCE_INLINE __attribute__((always_inline)) inline
    #define QK_NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
    #define QK_FORCE_INLINE __forceinline
    #define QK_NOINLINE __declspec(noinline)
#endif


// Aligning
#define QK_SAFE_ALIGNMENT 16

// Reference and Scope
template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#define USE_VULKAN_DRIVER

#include "Core/Logger.h"
#include "Core/Assert.h"