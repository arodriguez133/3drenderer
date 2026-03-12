#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int window_width = 800;
int window_height = 600;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

bool initialize_window(void) {
  // Tell Windows to use the actual native DPI instead of virtualized/scaled
  // coordinates. Without this, SDL_GetCurrentDisplayMode returns scaled
  // dimensions and the window is blurry on 4K displays with DPI scaling enabled.
  SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "ERROR initializing SDL.\n");
    return false;
  };

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  window_width = display_mode.w;
  window_height = display_mode.h;

   window = SDL_CreateWindow
    (
      NULL,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      window_width,
      window_height,
      SDL_WINDOW_FULLSCREEN_DESKTOP
    );
    
   if(!window) {
     fprintf(stderr, "Error creating SDL window\n");
     return false;
   }
  // Use the display index of the window rather than -1 (auto) so SDL picks
  // the GPU that physically drives the monitor. On multi-GPU systems (e.g.
  // Intel integrated + discrete), -1 often selects the wrong adapter and
  // forces a cross-adapter copy through the Windows compositor, causing flicker.
  int display_index = SDL_GetWindowDisplayIndex(window);
  renderer = SDL_CreateRenderer(window, display_index, SDL_RENDERER_PRESENTVSYNC);
  if(!renderer) {
    fprintf(stderr, "Error creating SDL renderer\n");
    return false;
  }

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

  return true; 
}

//There is most likely a formula for this to help
//I need to know the pixel density within a specific area to know when I'd like to start
//drawing a line. This is for later though. We will begin naively.
//
// we don't want to start for the first (y * 10) rows
// after that we begin to place n pixels down. how do we solve for n here?
// remember, the pixels are drawn in sequence, so, each position is 'evaluated'
// the boundary will be the window_width so for the total n pixles creates r lengh 0 < n < window_width
//
// in plain english: "place a pixel until you have reached the window_width." this should already
// be accounted for in the second loop
void draw_grid() {
  for (int y = 0; y < window_height; y+=10) {
    for (int x = 0; x < window_width; x+=10) {
        if (y % 10 == 0 || x % 10 == 0){
          color_buffer[(window_width * y) + x] = 0xFF444444;
        }
      }
    }
  }

void draw_pixel(int x, int y, uint32_t color){
  if (x >= 0 && x < window_width && y >= 0 && y < window_height)
    color_buffer[(window_width * y) + x] = color;
}

// draw_rect works entirely in screen space: loop variables (row, col) are absolute
// pixel coordinates, starting at (x, y) and ending at (x+width, y+height).
//
// Previously the loops ran from 0 and used offset variables (current_x = x + i,
// current_y = y + j) to translate back to screen coordinates on every iteration —
// two coordinate spaces (loop-relative and screen-absolute) in the same function.
//
// By initializing the loops at the target position instead of at 0, the translation
// step disappears entirely. The buffer index is computed directly from the loop
// variables, which already represent the correct screen location.
//
// color_buffer is a flat 1D array: index = (window_width * row) + col
// This maps each (col, row) screen coordinate to a unique element in the buffer.
void draw_rect(int x, int y, int width, int height, uint32_t color) {
  for (int row = y; row < y + height; row++) {
    for (int col = x; col < x + width; col++) {
      //color_buffer[(window_width * row) + col] = color;
      draw_pixel(col, row, color);
    }
  }
}


void render_color_buffer(void) {
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(window_width * sizeof(uint32_t)));
  SDL_RenderCopy(renderer, color_buffer_texture,NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++) {
      color_buffer[(window_width * y) + x] = color;
    }
  }
  
}

void destroy_window(void) {
  free(color_buffer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}



