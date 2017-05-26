#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

int main(int argc, char const *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *screen = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED);
    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);

    int w,h;
    SDL_Rect dst_rect;

    SDL_Surface *circle = IMG_Load("test_image.jpg");
    SDL_Texture *circle_sprite = SDL_CreateTextureFromSurface(renderer, circle);
    SDL_FreeSurface(circle);

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)){
            //If user closes the window
            if (e.type == SDL_QUIT){
                quit = 1;
            }
            //If user presses any key
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {
                case SDLK_q:
                    quit = 1;
                    break;
                }
            }
        }
        SDL_GetWindowSize(screen, &w, &h);

        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);

        dst_rect.x = 0;
        dst_rect.y = 0;
        dst_rect.w = w/2;
        dst_rect.h = h/2;


        SDL_RenderCopy(renderer, circle_sprite, NULL, &dst_rect);
        
        SDL_RenderPresent(renderer);

    }

    SDL_DestroyTexture(circle_sprite);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    SDL_Quit();
}