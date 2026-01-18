#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <iostream>
#include "Random.h"

int main() {    
    if (!(SDL_Init(SDL_INIT_EVERYTHING) == 0)) {
        std::cout << "Failed to initialize everything" << std::endl;
        SDL_Quit();
        return 1;
    }
    const int width = 800;
    const int height = 600;
    const int blockScale = 3; // block width / height in pixels

    int blockWidth = width / blockScale; // width of board in blocks
    int blockHeight = height / blockScale; // height of board in blocks
    int blockCount = blockWidth * blockHeight; // blocks on board

    SDL_Window *window = SDL_CreateWindow(
        "Sand Simulation",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        SDL_Quit();
        return 2;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cout << "Failed to create renderer" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 3;
    }

    bool running = true;
    bool paused = true;
    SDL_Event event;
    bool mouseLeftDown = false;
    bool mouseRightDown = false;
    int mousePosX = 0;
    int mousePosY = 0;
    int mouseFillRadius = 0;

    unsigned int frameStart;
    unsigned int frameTime;
    unsigned int FPS = 60;
    unsigned int interval = 1000/FPS;
    unsigned int frameIndex = 0;

    unsigned char buffer1[blockWidth*blockHeight]; // display / claims
    unsigned char buffer2[blockWidth*blockHeight]; // copy


    for (int i = 0; i < blockCount; i++) {
        buffer1[i] = 0;
        buffer2[i] = 0;
    }

    while (running) {
        frameStart = SDL_GetTicks();

        // do cells
        if (!paused) {
            for (int i = 0; i < blockCount; i++) {
                buffer2[i] = buffer1[i];
                buffer1[i] = 0; 
            }

            for (int y = blockHeight - 1; y >= 0; y--) {                
                // Alternate between scanning left-to-right and right-to-left
                bool scanRight = (frameIndex % 2 == 0);
                
                for (int x = 0; x < blockWidth; x++) {
                    int col = scanRight ? x : (blockWidth - 1 - x); // current column based on scan direction
                    int i = y * blockWidth + col;

                    if (buffer2[i] == 0) continue;

                    // Boundary Check
                    if (y >= blockHeight - 1) {
                        buffer1[i] = buffer2[i];
                        continue;
                    }

                    int down = i + blockWidth;
                    int dl = down - 1;
                    int dr = down + 1;
                    int d2 = i + blockWidth*3;

                    bool canDown = (buffer2[down] == 0 && buffer1[down] == 0);
                    bool canLeft = (col > 0 && buffer2[dl] == 0 && buffer1[dl] == 0);
                    bool canRight = (col < blockWidth - 1 && buffer2[dr] == 0 && buffer1[dr] == 0);
                    bool canD2 = (y < blockHeight-3 && buffer2[d2] == 0 && buffer1[d2] == 0);

                    if (canD2) {
                        buffer1[d2] = buffer2[i];
                    }
                    else if (canDown) {
                        buffer1[down] = buffer2[i];
                    } 
                    else {
                        // Per-pixel random choice to break up patterns
                        bool tryLeft = ((i ^ frameIndex) * 0x45d9f3b) & 1;

                        if (tryLeft) {
                            if (canLeft)        buffer1[dl] = buffer2[i];
                            else if (canRight)  buffer1[dr] = buffer2[i];
                            else                buffer1[i] = buffer2[i];
                        } else {
                            if (canRight)       buffer1[dr] = buffer2[i];
                            else if (canLeft)   buffer1[dl] = buffer2[i];
                            else                buffer1[i] = buffer2[i];
                        }
                    }
                }
            }
        }
        if (mouseLeftDown || mouseRightDown) {
            int cx = mousePosX/blockScale;
            int cy = mousePosY/blockScale;
            for (int i = -mouseFillRadius; i <= +mouseFillRadius; ++i) {
                int x = cx + i;
                if (x < 0 || x >= blockWidth) continue;
                
                for (int j = -mouseFillRadius; j <= +mouseFillRadius; ++j) {
                    int y = cy + j;
                    if (y < 0 || y >= blockHeight) continue;
                    if (i*i + j*j > mouseFillRadius*mouseFillRadius) continue; // circle
                    buffer1[x + y*blockWidth] = mouseLeftDown ? (floor(blueHash(x+frameIndex, y+frameTime)*3) + 1) : 0;
                }
            }
        }

        // handle events
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case (SDL_QUIT):
                running = false;
                break;

            case (SDL_MOUSEBUTTONDOWN):
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseLeftDown = true;
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    mouseRightDown = true;
                }
                break;
            case (SDL_MOUSEBUTTONUP):
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseLeftDown = false;
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    mouseRightDown = false;
                }
                break;
            
            case (SDL_MOUSEMOTION):
                mousePosX = event.motion.x;
                mousePosY = event.motion.y;
                break;

            case (SDL_KEYUP):
                if (event.key.keysym.sym == SDLK_SPACE) {
                    paused = !paused;
                } else if (event.key.keysym.sym == SDLK_c) {
                    for (int i = 0; i < blockCount; i++) {
                        buffer1[i] = 0;
                        buffer2[i] = 0;
                    }
                } else if (event.key.keysym.sym == SDLK_1) {
                    mouseFillRadius = 0*(10/blockScale);
                } else if (event.key.keysym.sym == SDLK_2) {
                    mouseFillRadius = 1*(10/blockScale);
                } else if (event.key.keysym.sym == SDLK_3) {
                    mouseFillRadius = 2*(10/blockScale);
                } else if (event.key.keysym.sym == SDLK_4) {
                    mouseFillRadius = 4*(10/blockScale);
                } else if (event.key.keysym.sym == SDLK_5) {
                    mouseFillRadius = 8*(10/blockScale);
                } else if (event.key.keysym.sym == SDLK_f) {
                    for (int j = 0; j < blockHeight; j++) {
                        for (int i = 0; i < blockWidth; i++) {
                            buffer1[i + j*blockWidth] = floor(fractHash(i+frameTime, j+frameIndex)*4);
                        }
                    }
                }
                break;

            case (SDL_KEYDOWN):
                if (event.key.keysym.sym == SDLK_q) {
                    for (int i = 0; i < blockWidth; i++) {
                        buffer1[i] = rand() % blockWidth ? 0 : (rand()%3 + 1);
                        buffer1[i + blockCount-blockWidth] = rand() % blockWidth ? buffer1[i + blockCount-blockWidth] : 0;
                    }
                } else if (event.key.keysym.sym == SDLK_w) {
                    for (int i = 0; i < blockWidth; i++) {
                        buffer1[i] = rand() % (blockWidth/2) ? 0 : (rand()%3 + 1);
                    }
                } else if (event.key.keysym.sym == SDLK_e) {
                    for (int i = 0; i < blockWidth; i++) {
                        buffer1[i] = floor(fractHash(i, frameIndex+frameTime)*(blockWidth/2)) ? 0 : (floor(fractHash(i+frameTime, frameIndex)*3) + 1);
                    }
                } else if (event.key.keysym.sym == SDLK_r) {
                    for (int i = 0; i < blockWidth; i++) {
                        buffer1[i] = floor(blueHash(i, frameIndex+frameTime)*(blockWidth/2)) ? 0 : (floor(blueHash(i+frameTime, frameIndex)*3) + 1);
                    }
                }
                break;

            default:
                break;
            }
        }


        // clear
        SDL_SetRenderDrawColor(renderer, 52, 90, 159, 255);
        SDL_RenderClear(renderer);

        // draw
        for (int i = 0; i < blockCount; i++) {
            if (!buffer1[i]) continue;

            SDL_Rect r = {(i%blockWidth)*blockScale, (int)(i/blockWidth)*blockScale, blockScale, blockScale};
            // SDL_SetRenderDrawColor(renderer, rand()%128+128, rand()%128+128, rand()%128+128, 255);
            if (buffer1[i] == 1)
                SDL_SetRenderDrawColor(renderer, 168, 128, 55, 255);
            else if (buffer1[i] == 2)
                SDL_SetRenderDrawColor(renderer, 194, 149, 68, 255);
            else if (buffer1[i] == 3)
                SDL_SetRenderDrawColor(renderer, 203, 165, 96, 255);
            else
                SDL_SetRenderDrawColor(renderer, 213, 182, 125, 255);
            SDL_RenderFillRect(renderer, &r);
            // SDL_SetRenderDrawColor(renderer, 191, 191, 191, 255);
            // SDL_RenderDrawRect(renderer, &r);
        }

        frameIndex = (frameIndex + 1) % (2 * blockCount);
        // show
        SDL_RenderPresent(renderer);

        // slow down
        frameTime = SDL_GetTicks() - frameStart;
        if (interval > frameTime) {
            SDL_Delay(interval - frameTime);
        }
    }

    // cleanup
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}


// pacman -Syu
// pacman -S mingw-w64-x86_64-SDL2

// cd "/c/users/administrator/desktop/glfw sandsim"; clear; g++ -o run.exe main.cpp -static -pthread -lmingw32 -lSDL2main /c/msys64/mingw64/lib/libSDL2.a -mwindows -lsetupapi -limm32 -lwinmm -ldxguid -lole32 -loleaut32 -lversion -static-libstdc++ -static-libgcc -I/c/msys64/mingw64/include/SDL2 -L/c/msys64/mingw64/lib; ./run.exe
