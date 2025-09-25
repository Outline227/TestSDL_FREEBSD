#include "/usr/local/include/SDL2/SDL.h"
#include "/usr/local/include/SDL2/SDL_ttf.h"
#include <stdio.h>
#include <stdbool.h>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) || TTF_Init() == -1) return 1;

    SDL_Window *win = SDL_CreateWindow("SDL2 Desktop",
                                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font *font = TTF_OpenFont("/usr/local/share/fonts/dejavu/DejaVuSans.ttf", 24);

    bool running = true, flash = false;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q && (e.key.keysym.mod & KMOD_CTRL)) running = false;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x, y = e.button.y;
                if (x>=270 && x<=370 && y>=380 && y<=430) { flash=true; printf("Test\n"); }
            }
        }

        SDL_SetRenderDrawColor(ren, flash?255:255, flash?255:0, flash?255:0, 255);
        flash=false;
        SDL_RenderClear(ren);

        int boxX=200, boxY=100, boxW=240, boxH=100;
        int colors[6][3]={{255,0,0},{255,165,0},{255,255,0},{0,255,0},{0,0,255},{128,0,128}};
        for(int i=0;i<6;i++){ SDL_Rect r={boxX, boxY+i*boxH/6, boxW, boxH/6}; SDL_SetRenderDrawColor(ren, colors[i][0], colors[i][1], colors[i][2], 255); SDL_RenderFillRect(ren,&r); }

        SDL_Rect btn={270,380,100,50};
        SDL_SetRenderDrawColor(ren,200,200,200,255); SDL_RenderFillRect(ren,&btn); SDL_SetRenderDrawColor(ren,0,0,0,255); SDL_RenderDrawRect(ren,&btn);

        SDL_Color black={0,0,0};
        SDL_Surface *surf=TTF_RenderText_Solid(font,"Press me",black);
        SDL_Texture *tex=SDL_CreateTextureFromSurface(ren,surf);
        int tw,th; SDL_QueryTexture(tex,NULL,NULL,&tw,&th);
        SDL_Rect dst={270+(100-tw)/2,380+(50-th)/2,tw,th};
        SDL_RenderCopy(ren,tex,NULL,&dst);
        SDL_DestroyTexture(tex); SDL_FreeSurface(surf);

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    TTF_CloseFont(font); SDL_DestroyRenderer(ren); SDL_DestroyWindow(win); TTF_Quit(); SDL_Quit();
    return 0;
}
