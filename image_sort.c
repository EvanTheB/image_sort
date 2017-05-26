#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

int main(int argc, char const *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *screen = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED);
    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);

    int w,h;
    SDL_Rect dst_rect;

    SDL_Texture *image_textures[5];
    int cur_start_texture = 0;
    SDL_Surface *image_surface;

    image_surface = IMG_Load("test_image_0.jpg");
    image_textures[0] = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);

    image_surface = IMG_Load("test_image_1.jpg");
    image_textures[1] = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);

    image_surface = IMG_Load("test_image_2.jpg");
    image_textures[2] = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);

    image_surface = IMG_Load("test_image_3.jpg");
    image_textures[3] = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);

    image_surface = IMG_Load("test_image_4.jpg");
    image_textures[4] = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);


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
                case SDLK_z:
                    cur_start_texture = (cur_start_texture + 1) % 5;

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
        SDL_RenderCopy(renderer, image_textures[cur_start_texture], NULL, &dst_rect);
        
        dst_rect.x = w/2;
        dst_rect.y = 0;
        SDL_RenderCopy(renderer, image_textures[(cur_start_texture + 1) % 5], NULL, &dst_rect);
        
        dst_rect.x = 0;
        dst_rect.y = h/2;
        SDL_RenderCopy(renderer, image_textures[(cur_start_texture + 2) % 5], NULL, &dst_rect);
        
        dst_rect.x = w/2;
        dst_rect.y = h/2;
        SDL_RenderCopy(renderer, image_textures[(cur_start_texture + 3) % 5], NULL, &dst_rect);
        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(image_textures[0]);
    SDL_DestroyTexture(image_textures[1]);
    SDL_DestroyTexture(image_textures[2]);
    SDL_DestroyTexture(image_textures[3]);
    SDL_DestroyTexture(image_textures[4]);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    SDL_Quit();
}