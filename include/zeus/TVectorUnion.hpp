#ifndef TVECTORUNION
#define TVECTORUNION

namespace zeus
{
typedef union {
    float v[4];
#if __SSE__
    __m128 mVec128;
#endif
} TVectorUnion;

typedef union {
    double v[4];
#if __SSE__
    __m128d mVec128[2];
#endif
} TDblVectorUnion;
}

#endif // TVECTORUNION
