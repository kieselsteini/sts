////////////////////////////////////////////////////////////////////////////////
/*
 sts_game.h - v0.01 - public domain
 written 2016 by Sebastian Steinhauer

  VERSION HISTORY
    0.01 (2016-05-05) initial version

  LICENSE
    Public domain. See "unlicense" statement at the end of this file.

  ABOUT
    Create a standard SDL2 mainloop.

  DEPENDENCIES
    SDL2
    stb_image
    stb_vorbis
    sts_mixer
    sts_net

  REMARKS
    The packet API is still work in progress.

*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __INCLUDED__STS_GAME_H__
#define __INCLUDED__STS_GAME_H__


#include "SDL.h"
#include "sts_mixer.h"
#include "sts_net.h"


#ifndef STS_GAME_WINDOW_WIDTH
#define STS_GAME_WINDOW_WIDTH       1280
#endif // STS_GAME_WINDOW_WIDTH
#ifndef STS_GAME_WINDOW_HEIGHT
#define STS_GAME_WINDOW_HEIGHT      720
#endif // STS_GAME_WINDOW_HEIGHT
#ifndef STS_GAME_WINDOW_FLAGS
#define STS_GAME_WINDOW_FLAGS       (SDL_WINDOW_RESIZABLE)
#endif // STS_GAME_WINDOW_FLAGS
#ifndef STS_GAME_WINDOW_TITLE
#define STS_GAME_WINDOW_TITLE       "sts-game Window"
#endif // STS_GAME_WINDOW_TITLE


extern SDL_Window*                  sts_game_window;
extern SDL_Renderer*                sts_game_renderer;
extern sts_mixer_t                  sts_game_mixer;


////////////////////////////////////////////////////////////////////////////////
//
//    General functions
//

// call this to stop the main loop
void sts_game_quit();

// call this to throw an error
void sts_game_error(const char* fmt, ...);


////////////////////////////////////////////////////////////////////////////////
//
//    Events
//

// called once on startup, so you can load images / sounds
void sts_game_event_init();

// called before everything is going to shutdown
void sts_game_event_quit();

// called for every frame
void sts_game_event_update(const float dt);

// called to draw stuff on the screen
void sts_game_event_draw();

// ...
void sts_game_event_key_pressed();

void sts_game_event_key_released();

void sts_game_event_window_close();

void sts_game_event_window_resized(int w, int h);

void sts_game_event_window_focus(int focus);


#endif // __INCLUDED__STS_GAME_H__
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////
////    IMPLEMENTATION
////
////
#ifdef STS_GAME_IMPLEMENTATION
#include <stdlib.h>

SDL_Window*                         sts_game_window = NULL;
SDL_Renderer*                       sts_game_renderer = NULL;
sts_mixer_t                         sts_game_mixer;

static int                          sts_game__was_init = 0;
static int                          sts_game__running = 1;


static void sts_game__panic_SDL(const char* func) {
  sts_game_error("%s() failed: %s", func, SDL_GetError());
}


static void sts_game__shutdown(void) {
  if (sts_game__was_init) sts_game_event_quit();
  if (sts_game_renderer) SDL_DestroyRenderer(sts_game_renderer);
  if (sts_game_window) SDL_DestroyWindow(sts_game_window);
  SDL_Quit();
}


void sts_game_quit() {
  sts_game__running = 0;
}


void sts_game_error(const char* fmt, ...) {
  va_list   va;
  char      buffer[1024];

  va_start(va, fmt);
  SDL_vsnprintf(buffer, sizeof(buffer), fmt, va);
  va_end(va);
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", buffer, sts_game_window);
  exit(-1);
}


int main(int argc, char *argv[]) {
  SDL_Event     ev;
  Uint32        last_tick, current_tick, delta_tick;

  (void)(argc); (void)(argv);
  atexit(sts_game__shutdown);
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0) sts_game__panic_SDL("SDL_Init");
  sts_game_window = SDL_CreateWindow(STS_GAME_WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, STS_GAME_WINDOW_WIDTH, STS_GAME_WINDOW_HEIGHT, STS_GAME_WINDOW_FLAGS);
  if (!sts_game_window) sts_game__panic_SDL("SDL_CreateWindow");
  sts_game_renderer = SDL_CreateRenderer(sts_game_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!sts_game_renderer) sts_game__panic_SDL("SDL_CreateRenderer");

  last_tick = SDL_GetTicks();
  while (sts_game__running) {
    // handle events
    while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
      case SDL_QUIT:
        sts_game_event_window_close();
        break;
      case SDL_WINDOWEVENT:
        switch (ev.window.type) {
        case SDL_WINDOWEVENT_RESIZED:
          sts_game_event_window_resized(ev.window.data1, ev.window.data2);
          break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
          sts_game_event_window_focus(1);
          break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
          sts_game_event_window_focus(0);
          break;
        }
        break;
      }
    }

    // handle time
    current_tick = SDL_GetTicks();
    delta_tick = current_tick - last_tick;
    last_tick = current_tick;
    sts_game_event_update((float)delta_tick / 1000.0f);

    // draw stuff
    SDL_SetRenderDrawColor(sts_game_renderer, 16, 16, 16, 0);
    SDL_RenderClear(sts_game_renderer);
    sts_game_event_draw();
    SDL_RenderPresent(sts_game_renderer);
  }
  return 0;
}


#define STS_MIXER_IMPLEMENTATION
#include "sts_mixer.h"
#define STS_NET_IMPLEMENTATION
#include "sts_net.h"

#endif // STS_GAME_IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////
//
//  Skeleton
//
#if 0

#include "sts_game.h"

void sts_game_event_init() {
}

void sts_game_event_quit() {
}

void sts_game_event_update(const float dt) {
}

void sts_game_event_draw() {
}

void sts_game_event_window_close() {
  sts_game_quit();
}

void sts_game_event_window_resized(int w, int h) {
}

void sts_game_event_window_focus(int focus) {
}

#define STS_GAME_IMPLEMENTATION
#include "sts_game.h"

#endif // 0
/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/
