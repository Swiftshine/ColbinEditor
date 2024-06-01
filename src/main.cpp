#include <cstdio>
#include "colbin.h"

int main(int argc, char** argv) {
    if (argc != 2) { printf("bad argument count\n"); return 1; }

    ColbinEditor::printColbinData(argv[1]);
    return 0;
}