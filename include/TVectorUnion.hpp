#ifndef TVECTORUNION
#define TVECTORUNION

typedef union
{
    float v[4];
#if __SSE__
    __m128 mVec128;
#endif
} TVectorUnion;

#endif // TVECTORUNION

