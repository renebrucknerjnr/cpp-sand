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
    bool typeIsUp = true; // up / down
    bool typeIsSolid = false; // put-delete / solid-void
    int mousePosX = 0;
    int mousePosY = 0;
    int mouseFillRadius = 0;

    unsigned int frameStart;
    unsigned int frameTime;
    unsigned int FPS = 60;
    unsigned int interval = 1000/FPS;
    unsigned int frameIndex = 0;

    unsigned char buffer1[blockCount]; // display / claims
    unsigned char buffer2[blockCount]; // copy


    // initialize the grid
    for (int i = 0; i < blockCount; i++) {
        buffer1[i] = 0;
        buffer2[i] = 0;
    }

    // text on grid on startup (suboptimal, but it works)
    for (int y = 0; y < blockHeight; y++) {
        int row = y * blockWidth;
        for (int x = 0; x < blockWidth; x++) {
            int i = x + row;

            // lmb
            if (blockWidth*0.00 <= x && x <= blockWidth*0.01 && blockHeight*0.00 <= y && y <= blockHeight*0.05) buffer1[i] = 8;
            if (blockWidth*0.00 <= x && x <= blockWidth*0.03 && blockHeight*0.04 <= y && y <= blockHeight*0.05) buffer1[i] = 8;
            
            if (blockWidth*0.04 <= x && x <= blockWidth*0.05 && blockHeight*0.00 <= y && y <= blockHeight*0.05) buffer1[i] = 8;
            if (blockWidth*0.05 <= x && x <= blockWidth*0.06 && blockHeight*0.00 <= y && y <= blockHeight*0.01) buffer1[i] = 8;
            if (blockWidth*0.06 <= x && x <= blockWidth*0.07 && blockHeight*0.01 <= y && y <= blockHeight*0.02) buffer1[i] = 8;
            if (blockWidth*0.07 <= x && x <= blockWidth*0.08 && blockHeight*0.00 <= y && y <= blockHeight*0.01) buffer1[i] = 8;
            if (blockWidth*0.08 <= x && x <= blockWidth*0.09 && blockHeight*0.00 <= y && y <= blockHeight*0.05) buffer1[i] = 8;
            
            if (blockWidth*0.10 <= x && x <= blockWidth*0.11 && blockHeight*0.00 <= y && y <= blockHeight*0.05) buffer1[i] = 8;
            if (blockWidth*0.11 <= x && x <= blockWidth*0.125 && blockHeight*0.00 <= y && y <= blockHeight*0.01) buffer1[i] = 8;
            if (blockWidth*0.11 <= x && x <= blockWidth*0.12 && blockHeight*0.02 <= y && y <= blockHeight*0.03) buffer1[i] = 8;
            if (blockWidth*0.11 <= x && x <= blockWidth*0.13 && blockHeight*0.04 <= y && y <= blockHeight*0.05) buffer1[i] = 8;
            if (blockWidth*0.12 <= x && x <= blockWidth*0.13 && blockHeight*0.01 <= y && y <= blockHeight*0.02) buffer1[i] = 8;
            if (blockWidth*0.12 <= x && x <= blockWidth*0.13 && blockHeight*0.03 <= y && y <= blockHeight*0.04) buffer1[i] = 8;

            // rmb
            if (blockWidth*0.00 <= x && x <= blockWidth*0.01 && blockHeight*0.10 <= y && y <= blockHeight*0.15) buffer1[i] = 9;
            if (blockWidth*0.01 <= x && x <= blockWidth*0.02 && blockHeight*0.10 <= y && y <= blockHeight*0.11) buffer1[i] = 9;
            if (blockWidth*0.02 <= x && x <= blockWidth*0.03 && blockHeight*0.11 <= y && y <= blockHeight*0.12) buffer1[i] = 9;
            if (blockWidth*0.01 <= x && x <= blockWidth*0.02 && blockHeight*0.12 <= y && y <= blockHeight*0.13) buffer1[i] = 9;
            if (blockWidth*0.02 <= x && x <= blockWidth*0.03 && blockHeight*0.13 <= y && y <= blockHeight*0.14) buffer1[i] = 9;
            if (blockWidth*0.02 <= x && x <= blockWidth*0.03 && blockHeight*0.14 <= y && y <= blockHeight*0.15) buffer1[i] = 9;
            
            if (blockWidth*0.04 <= x && x <= blockWidth*0.05 && blockHeight*0.10 <= y && y <= blockHeight*0.15) buffer1[i] = 9;
            if (blockWidth*0.05 <= x && x <= blockWidth*0.06 && blockHeight*0.10 <= y && y <= blockHeight*0.11) buffer1[i] = 9;
            if (blockWidth*0.06 <= x && x <= blockWidth*0.07 && blockHeight*0.11 <= y && y <= blockHeight*0.12) buffer1[i] = 9;
            if (blockWidth*0.07 <= x && x <= blockWidth*0.08 && blockHeight*0.10 <= y && y <= blockHeight*0.11) buffer1[i] = 9;
            if (blockWidth*0.08 <= x && x <= blockWidth*0.09 && blockHeight*0.10 <= y && y <= blockHeight*0.15) buffer1[i] = 9;
            
            if (blockWidth*0.10 <= x && x <= blockWidth*0.11 && blockHeight*0.10 <= y && y <= blockHeight*0.15) buffer1[i] = 9;
            if (blockWidth*0.11 <= x && x <= blockWidth*0.125 && blockHeight*0.10 <= y && y <= blockHeight*0.11) buffer1[i] = 9;
            if (blockWidth*0.11 <= x && x <= blockWidth*0.12 && blockHeight*0.12 <= y && y <= blockHeight*0.13) buffer1[i] = 9;
            if (blockWidth*0.11 <= x && x <= blockWidth*0.13 && blockHeight*0.14 <= y && y <= blockHeight*0.15) buffer1[i] = 9;
            if (blockWidth*0.12 <= x && x <= blockWidth*0.13 && blockHeight*0.11 <= y && y <= blockHeight*0.12) buffer1[i] = 9;
            if (blockWidth*0.12 <= x && x <= blockWidth*0.13 && blockHeight*0.13 <= y && y <= blockHeight*0.14) buffer1[i] = 9;
            
            // space
            if (blockWidth*0.00 <= x && x <= blockWidth*0.03 && blockHeight*0.20 <= y && y <= blockHeight*0.21) buffer1[i] = 5;
            if (blockWidth*0.00 <= x && x <= blockWidth*0.01 && blockHeight*0.20 <= y && y <= blockHeight*0.23) buffer1[i] = 5;
            if (blockWidth*0.00 <= x && x <= blockWidth*0.03 && blockHeight*0.23 <= y && y <= blockHeight*0.24) buffer1[i] = 5;
            if (blockWidth*0.02 <= x && x <= blockWidth*0.03 && blockHeight*0.24 <= y && y <= blockHeight*0.26) buffer1[i] = 5;
            if (blockWidth*0.00 <= x && x <= blockWidth*0.03 && blockHeight*0.26 <= y && y <= blockHeight*0.27) buffer1[i] = 5;
            
            if (blockWidth*0.04 <= x && x <= blockWidth*0.05 && blockHeight*0.20 <= y && y <= blockHeight*0.27) buffer1[i] = 5;
            if (blockWidth*0.05 <= x && x <= blockWidth*0.07 && blockHeight*0.20 <= y && y <= blockHeight*0.21) buffer1[i] = 5;
            if (blockWidth*0.07 <= x && x <= blockWidth*0.09 && blockHeight*0.21 <= y && y <= blockHeight*0.23) buffer1[i] = 5;
            if (blockWidth*0.05 <= x && x <= blockWidth*0.07 && blockHeight*0.23 <= y && y <= blockHeight*0.24) buffer1[i] = 5;
            
            // a
            if (blockWidth*0.00 <= x && x <= blockWidth*0.01 && blockHeight*0.31 <= y && y <= blockHeight*0.37) buffer1[i] = 1;
            if (blockWidth*0.01 <= x && x <= blockWidth*0.02 && blockHeight*0.30 <= y && y <= blockHeight*0.31) buffer1[i] = 1;
            if (blockWidth*0.02 <= x && x <= blockWidth*0.03 && blockHeight*0.31 <= y && y <= blockHeight*0.37) buffer1[i] = 1;
            if (blockWidth*0.01 <= x && x <= blockWidth*0.02 && blockHeight*0.33 <= y && y <= blockHeight*0.34) buffer1[i] = 1;

            // s
            if (blockWidth*0.00 <= x && x <= blockWidth*0.03 && blockHeight*0.40 <= y && y <= blockHeight*0.41) buffer1[i] = 1;
            if (blockWidth*0.00 <= x && x <= blockWidth*0.01 && blockHeight*0.40 <= y && y <= blockHeight*0.43) buffer1[i] = 1;
            if (blockWidth*0.00 <= x && x <= blockWidth*0.03 && blockHeight*0.43 <= y && y <= blockHeight*0.44) buffer1[i] = 1;
            if (blockWidth*0.02 <= x && x <= blockWidth*0.03 && blockHeight*0.44 <= y && y <= blockHeight*0.46) buffer1[i] = 1;
            if (blockWidth*0.00 <= x && x <= blockWidth*0.03 && blockHeight*0.46 <= y && y <= blockHeight*0.47) buffer1[i] = 1;
            
            // c
            if (blockWidth*0.01 <= x && x <= blockWidth*0.03 && blockHeight*0.50 <= y && y <= blockHeight*0.51) buffer1[i] = 5;
            if (blockWidth*0.03 <= x && x <= blockWidth*0.04 && blockHeight*0.51 <= y && y <= blockHeight*0.52) buffer1[i] = 5;
            if (blockWidth*0.00 <= x && x <= blockWidth*0.01 && blockHeight*0.51 <= y && y <= blockHeight*0.56) buffer1[i] = 5;
            if (blockWidth*0.01 <= x && x <= blockWidth*0.03 && blockHeight*0.56 <= y && y <= blockHeight*0.57) buffer1[i] = 5;
            if (blockWidth*0.03 <= x && x <= blockWidth*0.04 && blockHeight*0.55 <= y && y <= blockHeight*0.56) buffer1[i] = 5;

            // 1 - 5
            if (blockWidth*0.01 <= x && x <= blockWidth*0.02 && blockHeight*0.60 <= y && y <= blockHeight*0.61) buffer1[i] = 9;
            if (blockWidth*0.00 <= x && x <= blockWidth*0.01 && blockHeight*0.61 <= y && y <= blockHeight*0.62) buffer1[i] = 9;
            if (blockWidth*0.02 <= x && x <= blockWidth*0.03 && blockHeight*0.60 <= y && y <= blockHeight*0.67) buffer1[i] = 9;
            if (blockWidth*0.00 <= x && x <= blockWidth*0.05 && blockHeight*0.66 <= y && y <= blockHeight*0.67) buffer1[i] = 9;
            
            if (blockWidth*0.06 <= x && x <= blockWidth*0.09 && blockHeight*0.63 <= y && y <= blockHeight*0.64) buffer1[i] = 9;
            
            if (blockWidth*0.11 <= x && x <= blockWidth*0.15 && blockHeight*0.60 <= y && y <= blockHeight*0.61) buffer1[i] = 9;
            if (blockWidth*0.10 <= x && x <= blockWidth*0.11 && blockHeight*0.60 <= y && y <= blockHeight*0.63) buffer1[i] = 9;
            if (blockWidth*0.10 <= x && x <= blockWidth*0.14 && blockHeight*0.63 <= y && y <= blockHeight*0.64) buffer1[i] = 9;
            if (blockWidth*0.14 <= x && x <= blockWidth*0.15 && blockHeight*0.64 <= y && y <= blockHeight*0.66) buffer1[i] = 9;
            if (blockWidth*0.10 <= x && x <= blockWidth*0.14 && blockHeight*0.66 <= y && y <= blockHeight*0.67) buffer1[i] = 9;
            
            // t
            if (blockWidth*0.00 <= x && x <= blockWidth*0.07 && blockHeight*0.70 <= y && y <= blockHeight*0.71) buffer1[i] = 11;
            if (blockWidth*0.03 <= x && x <= blockWidth*0.04 && blockHeight*0.71 <= y && y <= blockHeight*0.77) buffer1[i] = 11;
            
            // y
            if (blockWidth*0.00 <= x && x <= blockWidth*0.01 && blockHeight*0.80 <= y && y <= blockHeight*0.81) buffer1[i] = 10;
            if (blockWidth*0.01 <= x && x <= blockWidth*0.02 && blockHeight*0.81 <= y && y <= blockHeight*0.82) buffer1[i] = 10;
            if (blockWidth*0.02 <= x && x <= blockWidth*0.03 && blockHeight*0.82 <= y && y <= blockHeight*0.87) buffer1[i] = 10;
            if (blockWidth*0.03 <= x && x <= blockWidth*0.04 && blockHeight*0.81 <= y && y <= blockHeight*0.82) buffer1[i] = 10;
            if (blockWidth*0.04 <= x && x <= blockWidth*0.05 && blockHeight*0.80 <= y && y <= blockHeight*0.81) buffer1[i] = 10;

            // q
            if (blockWidth*0.40 <= x && x <= blockWidth*0.41 && blockHeight*0.51 <= y && y <= blockHeight*0.58) buffer1[i] = 2;
            if (blockWidth*0.41 <= x && x <= blockWidth*0.48 && blockHeight*0.50 <= y && y <= blockHeight*0.51) buffer1[i] = 2;
            if (blockWidth*0.41 <= x && x <= blockWidth*0.48 && blockHeight*0.58 <= y && y <= blockHeight*0.59) buffer1[i] = 2;
            if (blockWidth*0.48 <= x && x <= blockWidth*0.49 && blockHeight*0.51 <= y && y <= blockHeight*0.58) buffer1[i] = 2;
            if (blockWidth*0.48 <= x && x <= blockWidth*0.49 && blockHeight*0.58 <= y && y <= blockHeight*0.59) buffer1[i] = 3;
            if (blockWidth*0.49 <= x && x <= blockWidth*0.50 && blockHeight*0.59 <= y && y <= blockHeight*0.60) buffer1[i] = 4;

            // w
            if (blockWidth*0.60 <= x && x <= blockWidth*0.61 && blockHeight*0.50 <= y && y <= blockHeight*0.52) buffer1[i] = 1;
            if (blockWidth*0.61 <= x && x <= blockWidth*0.62 && blockHeight*0.52 <= y && y <= blockHeight*0.54) buffer1[i] = 2;
            if (blockWidth*0.62 <= x && x <= blockWidth*0.63 && blockHeight*0.54 <= y && y <= blockHeight*0.56) buffer1[i] = 3;
            if (blockWidth*0.63 <= x && x <= blockWidth*0.64 && blockHeight*0.53 <= y && y <= blockHeight*0.54) buffer1[i] = 4;
            if (blockWidth*0.64 <= x && x <= blockWidth*0.65 && blockHeight*0.54 <= y && y <= blockHeight*0.56) buffer1[i] = 3;
            if (blockWidth*0.65 <= x && x <= blockWidth*0.66 && blockHeight*0.52 <= y && y <= blockHeight*0.54) buffer1[i] = 2;
            if (blockWidth*0.66 <= x && x <= blockWidth*0.67 && blockHeight*0.50 <= y && y <= blockHeight*0.52) buffer1[i] = 1;
            
            // e
            if (blockWidth*0.80 <= x && x <= blockWidth*0.81 && blockHeight*0.50 <= y && y <= blockHeight*0.57) buffer1[i] = 1;
            if (blockWidth*0.81 <= x && x <= blockWidth*0.83 && blockHeight*0.50 <= y && y <= blockHeight*0.51) buffer1[i] = 1;
            if (blockWidth*0.81 <= x && x <= blockWidth*0.82 && blockHeight*0.53 <= y && y <= blockHeight*0.54) buffer1[i] = 1;
            if (blockWidth*0.81 <= x && x <= blockWidth*0.83 && blockHeight*0.56 <= y && y <= blockHeight*0.57) buffer1[i] = 1;
        }
    }

    // main loop
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

                    // some AI was used to help troubleshoot here
                    if (buffer2[i] <= 4) { // sand
                        // Boundary Check
                        if (y >= blockHeight - 1) {
                            buffer1[i] = buffer2[i];
                            continue;
                        }

                        int down = i + blockWidth;
                        int dl = down - 1;
                        int dr = down + 1;
                        int d2 = i + blockWidth*3;

                        if (buffer2[down] == 9) {
                            continue;
                        }

                        bool canDown = (buffer2[down] == 0 && buffer1[down] == 0);
                        bool canLeft = (col > 0 && buffer2[dl] == 0 && buffer1[dl] == 0);
                        bool canRight = (col < blockWidth - 1 && buffer2[dr] == 0 && buffer1[dr] == 0);
                        bool canD2 = (y < blockHeight-3 && buffer2[i + blockWidth*1] == 0 && buffer1[i + blockWidth*1] == 0 && buffer2[i + blockWidth*2] == 0 && buffer1[i + blockWidth*2] == 0 && buffer2[d2] == 0 && buffer1[d2] == 0);

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
                    } else if (buffer2[i] > 4 && buffer2[i] < 8) { // smoke
                        if (y <= 0) {
                            buffer1[i] = buffer2[i];
                            continue;
                        }

                        int up = i - blockWidth;
                        int ul = up - 1;
                        int ur = up + 1;
                        int sl = i - 1;
                        int sr = i + 1;

                        if (buffer2[up] == 9) {
                            continue;
                        }

                        bool canUp = (buffer2[up] == 0 && buffer1[up] == 0);
                        bool canLeft = (col > 0 && buffer2[ul] == 0 && buffer1[ul] == 0);
                        bool canRight = (col < blockWidth - 1 && buffer2[ur] == 0 && buffer1[ur] == 0);
                        bool canLeft2 = (col > 0 && buffer2[sl] == 0 && buffer1[sl] == 0);
                        bool canRight2 = (col < blockWidth - 1 && buffer2[sr] == 0 && buffer1[sr] == 0);

                        if (canUp) {
                            buffer1[up] = buffer2[i];
                        } 
                        else {
                            // Per-pixel random choice to break up patterns
                            bool tryLeft = ((i ^ frameIndex) * 0x45d9f3b) & 1;

                            if (tryLeft) {
                                if (canLeft)        buffer1[ul] = buffer2[i];
                                else if (canRight)  buffer1[ur] = buffer2[i];
                                else if (canRight2) buffer1[sr] = buffer2[i];
                                else if (canLeft2)  buffer1[sl] = buffer2[i];
                                else                buffer1[i] = buffer2[i];
                            } else {
                                if (canRight)       buffer1[ur] = buffer2[i];
                                else if (canLeft)   buffer1[ul] = buffer2[i];
                                else if (canLeft2)  buffer1[sl] = buffer2[i];
                                else if (canRight2) buffer1[sr] = buffer2[i];
                                else                buffer1[i] = buffer2[i];
                            }
                        }
                    } else if (buffer2[i] < 10) { // solid / void
                        if (buffer2[i] == 8) buffer1[i] = 8;
                        else if (buffer2[i] == 9) buffer1[i] = 9;
                    } else {
                        if (buffer2[i] == 10) { // sand source
                            buffer1[i] = buffer2[i];
                            if (floor(fractHash(x+frameIndex, y+frameTime)*2) && y+1 < blockHeight && buffer2[i+blockWidth] == 0 && buffer1[i+blockWidth] == 0) {
                                buffer1[i+blockWidth] = (floor(blueHash(x+frameIndex, y+frameTime)*3)+1);
                            }
                        } else if (buffer2[i] == 11) { // smoke source
                            buffer1[i] = buffer2[i];
                            if (floor(fractHash(x+frameIndex, y+frameTime)*2) && y-1 >= 0 && buffer2[i-blockWidth] == 0 && buffer1[i-blockWidth] == 0) {
                                buffer1[i-blockWidth] = (floor(blueHash(x+frameIndex, y+frameTime)*3)+5);
                            }
                        }
                    }
                }
            }
        }
        if (mouseLeftDown || mouseRightDown) { // mouse click / drag
            int cx = mousePosX/blockScale;
            int cy = mousePosY/blockScale;
            for (int i = -mouseFillRadius; i <= +mouseFillRadius; ++i) {
                int x = cx + i;
                if (x < 0 || x >= blockWidth) continue;
                
                for (int j = -mouseFillRadius; j <= +mouseFillRadius; ++j) {
                    int y = cy + j;
                    if (y < 0 || y >= blockHeight) continue;
                    if (i*i + j*j > mouseFillRadius*mouseFillRadius) continue; // circle

                    if (typeIsSolid) {
                        buffer1[x + y*blockWidth] = mouseLeftDown ? 8 : 9;
                    } else if (buffer1[x + y*blockWidth] < 8 || mouseRightDown) {
                        buffer1[x + y*blockWidth] = mouseLeftDown ? (floor(blueHash(x+frameIndex, y+frameTime)*3) + (typeIsUp ? 1 : 5)) : 0;
                    }
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
                } else if (event.key.keysym.sym == SDLK_t) {
                    typeIsUp = !typeIsUp;
                } else if (event.key.keysym.sym == SDLK_y) {
                    typeIsSolid = !typeIsSolid;
                }
                break;

            case (SDL_KEYDOWN):
                if (event.key.keysym.sym == SDLK_q) {
                    for (int i = 0; i < blockWidth; i++) {
                        buffer1[i] = floor(blueHash(i+frameTime, frameIndex)*blockWidth) ? 0 : (floor(blueHash(i+frameTime, frameIndex)*3) + 1);
                        buffer1[i + blockCount-blockWidth] = floor(blueHash(i+frameTime, frameIndex)*blockWidth) ? buffer1[i + blockCount-blockWidth] : (floor(blueHash(i+frameTime, frameIndex)*3) + 5);
                    }
                } else if (event.key.keysym.sym == SDLK_w) {
                    for (int i = 0; i < blockWidth; i++) {
                        buffer1[i] = floor(fractHash(i+frameTime, frameIndex)*(blockWidth/2)) ? 0 : (floor(fractHash(i+frameTime, frameIndex)*3) + 1);
                    }
                } else if (event.key.keysym.sym == SDLK_e) {
                    for (int i = 0; i < blockWidth; i++) {
                        buffer1[i + blockCount-blockWidth] = floor(fractHash(i, frameIndex+frameTime)*(blockWidth/2)) ? 0 : (floor(fractHash(i+frameTime, frameIndex)*3) + 5);
                    }
                } else if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_s) {
                    int cx = mousePosX/blockScale;
                    int cy = mousePosY/blockScale;
                    for (int i = -mouseFillRadius; i <= +mouseFillRadius; ++i) {
                        int x = cx + i;
                        if (x < 0 || x >= blockWidth) continue;
                        
                        for (int j = -mouseFillRadius; j <= +mouseFillRadius; ++j) {
                            int y = cy + j;
                            if (y < 0 || y >= blockHeight) continue;
                            if (i*i + j*j > mouseFillRadius*mouseFillRadius) continue; // circle

                            buffer1[x + y*blockWidth] = (event.key.keysym.sym == SDLK_a ? 10 : 11);
                        }
                    }
                }
                break;

            default:
                break;
            }
        }

        // clear screen
        SDL_SetRenderDrawColor(renderer, 52, 90, 159, 255);
        SDL_RenderClear(renderer);

        // top right icon
        if (typeIsSolid) SDL_SetRenderDrawColor(renderer, 52, 70, 119, 255);
        else if (typeIsUp) SDL_SetRenderDrawColor(renderer, 52, 90, 159, 255);
        else if (!typeIsUp) SDL_SetRenderDrawColor(renderer, 72, 110, 179, 255);
        SDL_Rect r1 = {(int)(width*0.985), 0, (int)(width*0.015), (int)(width*0.015)};
        SDL_RenderFillRect(renderer, &r1);

        // draw
        for (int i = 0; i < blockCount; i++) {
            if (!buffer1[i]) continue;

            SDL_Rect r = {(i%blockWidth)*blockScale, (int)(i/blockWidth)*blockScale, blockScale, blockScale};
            // SDL_SetRenderDrawColor(renderer, rand()%128+128, rand()%128+128, rand()%128+128, 255);
            if (buffer1[i] == 1) // down
                SDL_SetRenderDrawColor(renderer, 168, 128, 55, 255);
            else if (buffer1[i] == 2)
                SDL_SetRenderDrawColor(renderer, 194, 149, 68, 255);
            else if (buffer1[i] == 3)
                SDL_SetRenderDrawColor(renderer, 203, 165, 96, 255);
            else if (buffer1[i] == 4)
                SDL_SetRenderDrawColor(renderer, 213, 182, 125, 255);
            else if (buffer1[i] == 5) // up
                SDL_SetRenderDrawColor(renderer, 182, 182, 200, 255);
            else if (buffer1[i] == 6)
                SDL_SetRenderDrawColor(renderer, 213, 213, 231, 255);
            else if (buffer1[i] == 7)
                SDL_SetRenderDrawColor(renderer, 237, 237, 255, 255);
            else if (buffer1[i] == 8) // solid
                SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
            else if (buffer1[i] == 9) // void
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            else // cloner
                SDL_SetRenderDrawColor(renderer, 55, 128, 168, 255);
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

// g++ -o run.exe Random.cpp main.cpp -static -pthread -lmingw32 -lSDL2main /c/msys64/mingw64/lib/libSDL2.a -mwindows -lsetupapi -limm32 -lwinmm -ldxguid -lole32 -loleaut32 -lversion -static-libstdc++ -static-libgcc -I/c/msys64/mingw64/include/SDL2 -L/c/msys64/mingw64/lib;
