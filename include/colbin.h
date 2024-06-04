#pragma once
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>

#pragma pack(push, 1)

using u32   = uint32_t;
using u8    = uint8_t;
struct Vec2f { float x; float y; };

namespace Colbin {
    struct Header {
        float multiplier; // i think? i know it's a float though
        u32 numEntries;
        u32 entryOffset;
        u32 footerOffset;
    };

    static_assert(sizeof(Colbin::Header) == 0x10, "size of Colbin::Header must be 0x10");

    struct Entry {
        Vec2f point1;
        Vec2f point2;
        Vec2f point3;
        u32 index;
        u32 collisionTypeIndex;
    };

    static_assert(sizeof(Colbin::Entry) == 0x20, "size of Colbin::Entry must be 0x20");

    struct Footer {
        u32 numCollisionTypes;
        /* present is a Colbin::Descriptor[numCollisionTypes] */

        /* padding until next multiple of 0x10 (0x20?)*/
    };

    using Descriptor = char[0x20];
}

#pragma pack(pop)
