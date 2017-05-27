#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>

#include "libgen.h"
#include "sys/stat.h"

SDL_Texture* load_texture(const char *fname, SDL_Renderer *renderer)
{
    printf("load %s\n", fname);
    SDL_Surface *image_surface = IMG_Load(fname);
    SDL_Texture *ret = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);
    return ret;
}

void move_image(const char *fname, const char *dest)
{
    char *dir = malloc(strlen(fname) + 1);
    strcpy(dir, fname);
    char *base = malloc(strlen(fname) + 1);
    strcpy(base, fname);
    char *new_loc = malloc(strlen(fname) + strlen(dest) + 2 + 1);

    sprintf(new_loc, "%s/%s", dirname(dir), dest);
    printf("mkdir %s\n", new_loc);
    mkdir(new_loc, 0777);

    sprintf(new_loc, "%s/%s/%s", dirname(dir), dest, basename(base));
    printf("move %s\nto %s\n", fname, new_loc);
    rename(fname, new_loc);

    // move RAF
    strcpy(dir, fname);
    int j = strlen(dir);
    int i = strlen(new_loc);
    new_loc[i-3] = dir[j-3] = 'R';
    new_loc[i-2] = dir[j-2] = 'A';
    new_loc[i-1] = dir[j-1] = 'F';
    rename(dir, new_loc);

    free(dir);
    free(base);
    free(new_loc);
}

int main(int argc, char const *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *screen = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED);
    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);

    int w,h;
    SDL_Rect dst_rect;

    SDL_Texture *image_textures[5];
    const char *image_fnames[5];

    int cur_image = 1;
    for (cur_image = 1; cur_image < argc && cur_image <= 4; ++cur_image)
    {
        image_textures[cur_image-1] = load_texture(argv[cur_image], renderer);
        image_fnames[cur_image-1] = argv[cur_image];
    }


    SDL_Event e;
    int quit = 0;
    int b_load_image = 1;
    int cur_start_texture = 0;
    while (!quit && image_fnames[cur_start_texture]) {
        // buffer the next image
        if (b_load_image){
            b_load_image = 0;
            if (cur_image < argc) {
                image_textures[(cur_start_texture + 4) % 5] = load_texture(argv[cur_image], renderer);
                image_fnames[(cur_start_texture + 4) % 5] = argv[cur_image];
            }
            cur_image++;
        }
        while (SDL_PollEvent(&e)){
            //If user closes the window
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
            //If user presses any key
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {
                case SDLK_q:
                    quit = 1;
                    break;
                case SDLK_a:
                    move_image(image_fnames[cur_start_texture], "delete");
                    goto next_image;
                case SDLK_s:
                    move_image(image_fnames[cur_start_texture], "bad");
                    goto next_image;
                case SDLK_d:
                    move_image(image_fnames[cur_start_texture], "good");
                    goto next_image;
                case SDLK_z:
                next_image:
                    SDL_DestroyTexture(image_textures[cur_start_texture]);
                    image_textures[cur_start_texture] = NULL;
                    image_fnames[cur_start_texture] = NULL;

                    cur_start_texture = (cur_start_texture + 1) % 5;
                    b_load_image = 1;
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