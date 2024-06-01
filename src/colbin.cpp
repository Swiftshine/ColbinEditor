#include "colbin.h"

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

void ColbinEditor::printColbinData(std::string input) {
    if (input.empty()) {
        printf("Error - input is empty\n");
        return;
    }

    Colbin::Header header;
    std::fstream file(input, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        printf("Error - failed to open file %s\n", input.c_str());
        return;
    }

    file.read((char*)&header, sizeof(Colbin::Header));

    printf("\n- Header Information - \n");
    printf("Multiplier (?): %f\n", swapEndiannessF32(header.multiplier));
    printf("Number of entries: %d\n\n", swapEndianness32(header.numEntries));

    for (int i = 0; i < swapEndianness32(header.numEntries); i++) {
        Colbin::Entry entry;
        file.read((char*)&entry, sizeof(Colbin::Entry));

        printf("- Entry %d Information -\n", i);
        printf("Start: X: %f, Y: %f, Z: %f\n", swapEndiannessF32(entry.start.x), swapEndiannessF32(entry.start.y), swapEndiannessF32(entry.start.z));
        printf("End:   X: %f, Y: %f, Z: %f\n", swapEndiannessF32(entry.end.x), swapEndiannessF32(entry.end.y), swapEndiannessF32(entry.end.z));
        printf("Index: %d\n", swapEndianness32(entry.index));
        printf("Collision type index: %d\n", swapEndianness32(entry.collisionTypeIndex));
        printf("\n");
    }

    Colbin::Footer footer;
    file.read((char*)&footer, sizeof(Colbin::Footer));

    printf("- Footer Information - \n");
    printf("Number of collision types: %d\n", swapEndianness32(footer.numCollisionTypes));

    for (int i = 0; i < swapEndianness32(footer.numCollisionTypes); i++) {
        Colbin::Descriptor descriptor;
        file.read(descriptor, sizeof(descriptor));

        printf("Type %d: %s\n", i, descriptor);
    }
    printf("\n");
}