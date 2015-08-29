#ifndef ZEUS_GLOBAL_HPP
#define ZEUS_GLOBAL_HPP

#if _M_IX86_FP >= 1 || _M_X64
#   define __SSE__ 1
#endif

#if __SSE__
#   include <immintrin.h>
#   ifdef _MSC_VER
#       define ZE_ALIGN(x) __declspec(align(x))
#   else
#       include <mm_malloc.h>
#       define ZE_ALIGN(x) alignas(16)
#   endif
#   define ZE_SHUFFLE(x,y,z,w) ((w)<<6 | (z)<<4 | (y)<<2 | (x))
#   define zeAlloc(sz, align) _mm_malloc(sz, align)
#   define zeFree(ptr) _mm_free(ptr)
#else
#   define ZE_ALIGN(x)
#endif

#if __SSE__
#   define ZE_DECLARE_ALIGNED_ALLOCATOR() \
        inline void* operator new(size_t sizeInBytes)    { return zeAlloc(sizeInBytes,16); } \
        inline void  operator delete(void* ptr)          { zeFree(ptr); } \
        inline void* operator new(size_t, void* ptr)     { return ptr; } \
        inline void  operator delete(void*, void*)       { }   \
        inline void* operator new[](size_t sizeInBytes)  { return zeAlloc(sizeInBytes,16); } \
        inline void  operator delete[](void* ptr)        { zeFree(ptr); } \
        inline void* operator new[](size_t, void* ptr)   { return ptr; } \
        inline void  operator delete[](void*, void*)     { } \
        void __unused__()
#else
#   define ZE_DECLARE_ALIGNED_ALLOCATOR() void __unused__()
#endif

#if __SSE__
#   define ZE_SHUFFLE(x,y,z,w) ((w)<<6 | (z)<<4 | (y)<<2 | (x))
#   define ze_pshufd_ps( _a, _mask ) _mm_shuffle_ps((_a), (_a), (_mask) )
#   define ze_splat3_ps( _a, _i ) ze_pshufd_ps((_a), ZE_SHUFFLE(_i,_i,_i, 3) )
#   define ze_splat_ps( _a, _i )  ze_pshufd_ps((_a), ZE_SHUFFLE(_i,_i,_i,_i) )
#   if _WIN32
#       define zeCastiTo128f(a) (_mm_castsi128_ps(a))
#   else
#       define zeCastiTo128f(a)  ((__m128) (a))
#   endif
#endif


#endif //ZEUS_GLOBAL_HPP

