#ifndef CCOLOR_HPP
#define CCOLOR_HPP

#include "MathLib.hpp"
#include <iostream>

class CColor
{
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();
    
    CColor() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    CColor(float r, float g, float b, float a = 1.0f)
        : r(r), g(g), b(b), a(a)
    {
    }

    CColor(Athena::io::IStreamReader& reader) {readRGBA(reader);}
    
    inline void readRGBA(Athena::io::IStreamReader& reader)
    {
        r = reader.readFloat();
        g = reader.readFloat();
        b = reader.readFloat();
        a = reader.readFloat();
    }
    
    inline void readBGRA(Athena::io::IStreamReader& reader)
    {
        b = reader.readFloat();
        g = reader.readFloat();
        r = reader.readFloat();
        a = reader.readFloat();
    }

    inline const float& operator[](const int& idx) { return (&r)[idx]; }
    
    union
    {
        struct
        {
            float r, g, b, a;
        };
#if __SSE__
        __m128 mVec128;
#endif
    };

    void fromRGBA32(unsigned int rgba)
    {
        union
        {
            struct
            {
                unsigned char r, g, b, a;
            };
            unsigned int rgba;
        } tmp =
        {
            .rgba = rgba
        };

        r = tmp.r / 255.f;
        g = tmp.g / 255.f;
        b = tmp.b / 255.f;
        a = tmp.a / 255.f;
    }
};

#endif // CCOLOR_HPP
