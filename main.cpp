#include <cstdio>
#include <memory>

#include <SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

static constexpr const char* APP_NAME = "Dear ImGui SDL2 & SDL_Renderer example";
static constexpr int INITIAL_WIDTH    = 1280;
static constexpr int INITIAL_HEIGHT   = 720;
static constexpr int SCREEN_NUM       = 1; // first monitor

static bool fullscreen = false;

int main(int argc, char** argv) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
    printf("[ERROR] %s\n", SDL_GetError());
    return -1;
  }

  Uint32 flags = (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
  auto window  = std::shared_ptr<SDL_Window>(SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED_DISPLAY(SCREEN_NUM), SDL_WINDOWPOS_CENTERED_DISPLAY(SCREEN_NUM), INITIAL_WIDTH, INITIAL_HEIGHT, flags), [](auto* p) { SDL_DestroyWindow(p); });

  if (!window) {
    printf("[ERROR] SDL_CreateWindow: %s\n", SDL_GetError());
    return -1;
  }

  auto renderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED), [](auto* p) { SDL_DestroyRenderer(p); });
  if (!renderer) {
    printf("[ERROR] SDL_CreateRenderer: %s\n", SDL_GetError());
    return -1;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
  ImGui_ImplSDLRenderer_Init(renderer.get());

  bool show_demo_window = true;

  // Main loop
  bool done = false;
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if ((event.type == SDL_QUIT) ||
          (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) ||
          (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window.get()))) {
        done = true;
      }
      switch (event.type) {
        case SDL_QUIT: {
          done = true;
          break;
        }
        case SDL_KEYDOWN: {
          if (event.key.keysym.sym == SDLK_F11) {
            fullscreen = !fullscreen;
            if (fullscreen) {
              SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
            } else {
              SDL_SetWindowFullscreen(window.get(), 0);
            }
          }
          break;
        }
      }
    }

    if (done) break;

    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window) {
      ImGui::ShowDemoWindow(&show_demo_window);
    }

    ImGui::Render();

    SDL_SetRenderDrawColor(renderer.get(), 255, 200, 100, 255);
    SDL_RenderClear(renderer.get());

    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer.get());
  }

  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_Quit();

  return 0;
}
