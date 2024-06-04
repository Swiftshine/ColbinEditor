#include "colbin.h"
#include "editor.h"
#include <SDL.h>

const int   WINDOW_WIDTH    = 800;
const int   WINDOW_HEIGHT   = 600;
const float ZOOM            = 50.0f;
const int   FRAMERATE       = 60;
const int   DELAY_TIME      = 1000 / FRAMERATE;
const float OFFSETX         = 8.0f;
const float OFFSETY         = 4.5f;

void ColbinEditor::graphColbinData(std::string input) {
    if (input.empty()) {
        printf("Error - input filename is empty\n");
        return;
    }

    Colbin::Header header;
    std::vector<Colbin::Entry> entries;
    std::vector<std::string> collisionTypes;

    std::fstream file(input, std::ios::in | std::ios::binary);
    file.read((char*)&header, sizeof(Colbin::Header));

    for (int i = 0; i < swapEndianness32(header.numEntries); i++) {
        Colbin::Entry entry;
        file.read((char*)&entry, sizeof(Colbin::Entry));
        entries.push_back(entry);
    }

    file.seekg(swapEndianness32(header.footerOffset));
    u32 numCollisionTypes = 0;
    file.read((char*)&numCollisionTypes, 4);
    numCollisionTypes = swapEndianness32(numCollisionTypes);

    for (int i = 0; i < numCollisionTypes; i++) {
        std::string type = "";
        file >> type;
        collisionTypes.push_back(type);
    }

    file.close();

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;

    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error - failed to initialise SDL. Error: %s\n", SDL_GetError());
        goto cleanup;
    }



    window = SDL_CreateWindow(std::string("ColbinEditor - " + input).c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Error - failed to create window. Error: %s\n", SDL_GetError());
        goto cleanup;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        printf("Error - failed to create renderer. Error: %s\n", SDL_GetError());
        goto cleanup;
        return;
    }

    running = true;

    while (running) {

        unsigned int frameStart = SDL_GetTicks();

        SDL_RenderPresent(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int i = 0; i < entries.size(); i++) {
            float x1 = (swapEndiannessF32(entries[i].point1.x) + OFFSETX) * ZOOM;
            float y1 = WINDOW_HEIGHT - (swapEndiannessF32(entries[i].point1.y) + OFFSETY) * ZOOM;

            float x2 = (swapEndiannessF32(entries[i].point2.x) + OFFSETX) * ZOOM;
            float y2 = WINDOW_HEIGHT - (swapEndiannessF32(entries[i].point2.y) + OFFSETY) * ZOOM;

            float x3 = (swapEndiannessF32(entries[i].point3.x) + OFFSETX) * ZOOM;
            float y3 = WINDOW_HEIGHT - (swapEndiannessF32(entries[i].point3.y) + OFFSETY) * ZOOM;

            SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
            SDL_RenderDrawLineF(renderer, x2, y2, x3, y3);
            SDL_RenderDrawLineF(renderer, x3, y3, x1, y1);       
        }

        SDL_RenderPresent(renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (SDL_QUIT == event.type) {
                running = false;
            }
        }

        unsigned int frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < DELAY_TIME) {
            SDL_Delay(DELAY_TIME - frameTime);
        }
    }


cleanup:
    entries.clear();
    collisionTypes.clear();

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);

    SDL_Quit();
}