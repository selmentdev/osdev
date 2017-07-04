#pragma once
#define UNUSED(a) (void)(a)

typedef unsigned long long size_t;

#ifdef __cplusplus
#define STATIC_ASSERT(_Expression)      static_assert(_Expression, #_Expression)
#else
#if __STDC_VERSION__ == 201112L
#define STATIC_ASSERT(_Expression)      _Static_assert(_Expression, #_Expression)
#else
#define STATIC_ASSERT(_Expression) typedef char __STATIC_ASSERT__[(_Expression) ? 1 : -1]
#endif
#endif