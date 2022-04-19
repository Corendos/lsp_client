/*
 * MIT Licence
 * Copyright (c) 2022 Corentin Godeau (@Corendos)
 * More details at https://github.com/Corendos/Reborn
 */

#ifndef REBORN_TYPES_H
#define REBORN_TYPES_H

#if defined(_MSC_VER)

#define COMPILER_CL 1

#if defined(_WIN32)
#define OS_WINDOWS 1
#else
#error This compiler/platform combo is not supported yet
#endif

#if defined(_M_AMD64)
#define ARCH_X64 1
#elif defined(_M_IX86)
#define ARCH_X86 1
#elif defined(_M_ARM64)
#define ARCH_ARM64 1
#elif defined(_M_ARM)
#define ARCH_ARM 1
#else
#error architecture not supported yet
#endif

#elif defined(__clang__)

#define COMPILER_CLANG 1

#if defined(__APPLE__) && defined(__MACH__)
#define OS_MAC 1
#else
#error This compiler/platform combo is not supported yet
#endif

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#define ARCH_X64 1
#elif defined(i386) || defined(__i386) || defined(__i386__)
#define ARCH_X86 1
#elif defined(__aarch64__)
#define ARCH_ARM64 1
#elif defined(__arm__)
#define ARCH_ARM 1
#else
#error architecture not supported yet
#endif

#elif defined(__GNUC__) || defined(__GNUG__)

#define COMPILER_GCC 1

#if defined(__gnu_linux__)
#define OS_LINUX 1
#else
#error This compiler/platform combo is not supported yet
#endif

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#define ARCH_X64 1
#elif defined(i386) || defined(__i386) || defined(__i386__)
#define ARCH_X86 1
#elif defined(__aarch64__)
#define ARCH_ARM64 1
#elif defined(__arm__)
#define ARCH_ARM 1
#else
#error architecture not supported yet
#endif

#else

#error This compiler is not supported yet

#endif

#if defined(ARCH_X64) || defined(ARCH_ARM64)
#define ARCH_64BIT 1
#elif defined(ARCH_X86) || defined(ARCH_ARM)
#define ARCH_32BIT 1
#endif

#if defined(OS_WINDOWS)
#define EXPORT __declspec(dllexport)

#if defined(ARCH_X86)
typedef unsigned long long int u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef long long int i64;
typedef int i32;
typedef short i16;
typedef signed char i8;

typedef u32 usize;

typedef float f32;
typedef double f64;
#elif defined(ARCH_X64)
typedef unsigned long long int u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef long long int i64;
typedef int i32;
typedef short i16;
typedef signed char i8;

typedef u64 usize;

typedef float f32;
typedef double f64;
#elif defined(ARCH_ARM)
typedef unsigned long long int u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef long long int i64;
typedef int i32;
typedef short i16;
typedef signed char i8;

typedef u32 usize;

typedef float f32;
typedef double f64;
#elif defined(ARCH_ARM64)
typedef unsigned long long int u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef long long int i64;
typedef int i32;
typedef short i16;
typedef signed char i8;

typedef u64 usize;

typedef float f32;
typedef double f64;
#endif

#else
#error "Unsupported platform"
#endif

#define single_linked_list_stack_push(last_node, new_node) (new_node)->next = (last_node), (last_node) = (new_node)
#define single_linked_list_stack_pop(last_node)            (last_node) = (last_node)->next

#define doubly_linked_list_push(list, new_node)                                                                        \
    do {                                                                                                               \
        if ((list)->first == 0) {                                                                                      \
            (list)->first = (new_node);                                                                                \
        }                                                                                                              \
        if ((list)->last == 0) {                                                                                       \
            (list)->last = (new_node);                                                                                 \
        } else {                                                                                                       \
            (new_node)->prev = (list)->last;                                                                           \
            (list)->last->next = (new_node);                                                                           \
            (list)->last = (new_node);                                                                                 \
        }                                                                                                              \
        (list)->count++;                                                                                               \
    } while (0)

#define internal static
#if defined(COMPILER_GCC) || defined(COMPILER_CLANG)
#define inl __attribute__((__always_inline__))
#elif defined(COMPILER_CL)
#define inl __forceinline
#else
#endif
#define internal static

inl bool is_power_of_two(u64 number) { return (number & (number - 1)) == 0; }

inl u64 align_to(u64 value, u16 alignment) {
    if (value != 0) {
        u64 temp = value - 1;
        return temp - (temp % alignment) + alignment;
    }
    return 0;
}

inl bool is_aligned_to(u64 value, u16 alignment) { return value % alignment == 0; }

#define _STRINGIFY(s)     #s
#define STRINGIFY(s)      _STRINGIFY(s)
#define GLUE_(x, y)       x##y
#define CONCAT(x, y)      GLUE_(x, y)
#define __FILE_AND_LINE__ CONCAT(__FILE__, CONCAT(STRINGIFY( : L), STRINGIFY(__LINE__)))

#define STATEMENT(s)                                                                                                   \
    do {                                                                                                               \
        s                                                                                                              \
    } while (0)

#define ASSERT_BREAK(message)    (*((i32*)0) = 0xA11E)
#define ASSERT_ALWAYS(c)         STATEMENT(if (!(c)) { ASSERT_BREAK(c); })
#define ASSERT_MESSAGE_ALWAYS(m) ASSERT_BREAK(m)

#if defined(_DEBUG)
#define ASSERT(c)         ASSERT_ALWAYS(c)
#define ASSERT_MESSAGE(m) ASSERT_MESSAGE_ALWAYS(m)
#else
#define ASSERT(c)         (void)(c)
#define ASSERT_MESSAGE(m) (void)(m)
#endif

#define CHECK_UNREACHABLE() ASSERT_MESSAGE("REACHED UNREACHABLE CODE")

#define ENABLE_CUSTOM_MEMORY_FUNCTION 0

#define DECLARE_ARRAY(name, type)                                                                                      \
    struct name {                                                                                                      \
        type* items;                                                                                                   \
        u64 count;                                                                                                     \
    }

#define DECLARE_LIST(name, type)                                                                                       \
    struct CONCAT(name, Node) {                                                                                        \
        CONCAT(name, Node) * next;                                                                                     \
        CONCAT(name, Node) * prev;                                                                                     \
        type value;                                                                                                    \
    };                                                                                                                 \
                                                                                                                       \
    struct name {                                                                                                      \
        CONCAT(name, Node) * first;                                                                                    \
        CONCAT(name, Node) * last;                                                                                     \
        u64 count;                                                                                                     \
    }

DECLARE_ARRAY(ArrayU8, u8);
DECLARE_ARRAY(ArrayU16, u16);
DECLARE_ARRAY(ArrayU32, u32);
DECLARE_ARRAY(ArrayU64, u64);
DECLARE_ARRAY(ArrayI8, i8);
DECLARE_ARRAY(ArrayI16, i16);
DECLARE_ARRAY(ArrayI32, i32);
DECLARE_ARRAY(ArrayI64, i64);

DECLARE_LIST(ListU64, u64);
DECLARE_LIST(ListI64, i64);

#define DECLARE_RESULT(name, type, value_name)                                                                         \
    struct CONCAT(name, Result) {                                                                                      \
        bool valid;                                                                                                    \
        type value_name;                                                                                               \
    }
#endif // REBORN_TYPES_H
