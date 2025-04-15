#ifndef SYS_C_SYS_C_H
#define SYS_C_SYS_C_H

#ifdef __cplusplus
extern "C" {
#endif

static inline long sys_c_0(long n);
static inline long sys_c_1(long n, long a1);
static inline long sys_c_2(long n, long a1, long a2);
static inline long sys_c_3(long n, long a1, long a2, long a3);
static inline long sys_c_4(long n, long a1, long a2, long a3, long a4);
static inline long sys_c_5(long n, long a1, long a2, long a3, long a4, long a5);
static inline long sys_c_6(long n, long a1, long a2, long a3, long a4, long a5, long a6);

#if defined(__x86_64__)
#include "sys_c_x86_64.inl"
#elif defined(__aarch64__)
#include "sys_c_aarch64.inl"
#else
#error "Unsupported architecture"
#endif

#ifdef __cplusplus
}
#endif

#endif  // SYS_C_SYS_C_H