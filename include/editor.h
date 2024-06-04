#pragma once
#include "colbin.h"
#include <cstring>
#include <string>
#include <fstream>
#include <vector>

namespace ColbinEditor {
    void printColbinData(std::string input);
    void graphColbinData(std::string input);
}


static u32 swapEndianness32(u32 value) {
    return ((value >> 24) & 0x000000FF) |
        ((value >> 8) & 0x0000FF00) |
        ((value << 8) & 0x00FF0000) |
        ((value << 24) & 0xFF000000);
}

static float swapEndiannessF32(float value) {
    u32 intval;
    std::memcpy(&intval, &value, sizeof(float));
    intval = swapEndianness32(intval);
    float result;
    std::memcpy(&result, &intval, sizeof(float));
    return result;
}