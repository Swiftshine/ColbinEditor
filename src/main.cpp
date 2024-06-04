#include <cstdio>
#include "editor.h"

enum Usage {
    PROGRAM = 0,
    DISPLAY_TYPE, // "print" or "graph"
    FILENAME,
    
    REQUIRED_ARGUMENT_COUNT,
};


int main(int argc, char** argv) {
    if (argc != REQUIRED_ARGUMENT_COUNT) {
        printf("bad argument count\n");
        return 1;
    }

    if ("print" == std::string(argv[DISPLAY_TYPE])) {
        ColbinEditor::printColbinData(argv[FILENAME]);
    } else if ("graph" == std::string(argv[DISPLAY_TYPE])) {
        ColbinEditor::graphColbinData(argv[FILENAME]);
    } else {
        printf("invalid display type. must be either 'print' or 'graph'.\n");
    }
    return 0;
}