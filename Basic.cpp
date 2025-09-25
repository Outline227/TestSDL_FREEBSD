#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("No Xorg Demo",
                                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       640, 480, SDL_WINDOW_FULLSCREEN);
    if (!win) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!ren) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 1;
    }

    // Load font
    TTF_Font *font = TTF_OpenFont("/usr/local/share/fonts/dejavu/DejaVuSans.ttf", 24);
    if (!font) {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        return 1;
    }

    bool running = true;
    bool flash = false;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_q && (e.key.keysym.mod & KMOD_CTRL)) {
                    running = false;
                }
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x;
                int my = e.button.y;
                // Button bounds (centered at 270, 380, size 100x50)
                if (mx >= 270 && mx <= 370 && my >= 380 && my <= 430) {
                    flash = true;
                    printf("Test\n");
                }
            }
        }

        // Background
        if (flash) {
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // White flash
            flash = false;
        } else {
            SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); // Red
        }
        SDL_RenderClear(ren);

        // Rainbow box
        int boxX = 200, boxY = 100, boxW = 240, boxH = 100;
        int colors[6][3] = {
            {255, 0, 0}, {255, 165, 0}, {255, 255, 0},
            {0, 255, 0}, {0, 0, 255}, {128, 0, 128}
        };
        int stripeH = boxH / 6;
        for (int i = 0; i < 6; i++) {
            SDL_Rect r = {boxX, boxY + i * stripeH, boxW, stripeH};
            SDL_SetRenderDrawColor(ren, colors[i][0], colors[i][1], colors[i][2], 255);
            SDL_RenderFillRect(ren, &r);
        }

        // Button
        SDL_Rect button = {270, 380, 100, 50};
        SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
        SDL_RenderFillRect(ren, &button);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderDrawRect(ren, &button);

        // Button text
        SDL_Color black = {0, 0, 0};
        SDL_Surface *surf = TTF_RenderText_Solid(font, "Press me", black);
        SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);
        int tw, th;
        SDL_QueryTexture(tex, NULL, NULL, &tw, &th);
        SDL_Rect dst = {270 + (100 - tw) / 2, 380 + (50 - th) / 2, tw, th};
        SDL_RenderCopy(ren, tex, NULL, &dst);
        SDL_DestroyTexture(tex);
        SDL_FreeSurface(surf);

        SDL_RenderPresent(ren);
        SDL_Delay(16); // ~60 FPS
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
