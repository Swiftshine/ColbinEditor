#include "colbin.h"
#include "editor.h"

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
        printf("Point 1:   X: %f, Y: %f\n", swapEndiannessF32(entry.point1.x), swapEndiannessF32(entry.point1.y));
        printf("Point 2:   X: %f, Y: %f\n", swapEndiannessF32(entry.point2.x), swapEndiannessF32(entry.point2.y));
        printf("Point 3:   X: %f, Y: %f\n", swapEndiannessF32(entry.point3.x), swapEndiannessF32(entry.point3.y));
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