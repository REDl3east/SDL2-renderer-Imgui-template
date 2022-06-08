#include <cstdio>
#include <memory>

#include "SDL.h"
#include "SDL_image.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

std::shared_ptr<SDL_Window> SDL_CreateSharedWindow(const char* title, int x, int y, int w, int h, Uint32 flags);
std::shared_ptr<SDL_Renderer> SDL_CreateSharedRenderer(SDL_Window* window, int index, Uint32 flags);
std::shared_ptr<SDL_Texture> IMG_LoadSharedTexture(SDL_Renderer* renderer, const char* file);

static constexpr const char* APP_NAME = "Dear ImGui SDL2 & SDL_Renderer example";
static constexpr int INITIAL_WIDTH    = 1280;
static constexpr int INITIAL_HEIGHT   = 720;
static constexpr int SCREEN_NUM       = 1; // first monitor

static bool fullscreen = false;
bool show_demo_window  = false;

int main(int argc, char** argv) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
    printf("[ERROR] %s\n", SDL_GetError());
    return -1;
  }

  Uint32 flags = (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
  auto window  = SDL_CreateSharedWindow(APP_NAME, SDL_WINDOWPOS_CENTERED_DISPLAY(SCREEN_NUM), SDL_WINDOWPOS_CENTERED_DISPLAY(SCREEN_NUM), INITIAL_WIDTH, INITIAL_HEIGHT, flags);

  if (!window) {
    printf("[ERROR] SDL_CreateWindow: %s\n", SDL_GetError());
    return -1;
  }

  auto renderer = SDL_CreateSharedRenderer(window.get(), -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    printf("[ERROR] SDL_CreateRenderer: %s\n", SDL_GetError());
    return -1;
  }

  auto img = IMG_LoadSharedTexture(renderer.get(), "../assets/10_club.png");

  if (!img) {
    printf("[ERROR] IMG_LoadTexture: %s\n", SDL_GetError());
    return -1;
  }

  int w, h;
  SDL_QueryTexture(img.get(), NULL, NULL, &w, &h); // get the width and height of the texture

  SDL_Rect texr;
  texr.x = INITIAL_WIDTH / 2 - w / 2;
  texr.y = INITIAL_HEIGHT / 2 - h / 2;
  texr.w = w;
  texr.h = h;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
  ImGui_ImplSDLRenderer_Init(renderer.get());

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

    SDL_RenderCopy(renderer.get(), img.get(), NULL, &texr);

    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer.get());
  }

  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_Quit();

  return 0;
}

std::shared_ptr<SDL_Window> SDL_CreateSharedWindow(const char* title, int x, int y, int w, int h, Uint32 flags) {
  return std::shared_ptr<SDL_Window>(SDL_CreateWindow(title, x, y, w, h, flags), [](auto* p) { SDL_DestroyWindow(p); });
}

std::shared_ptr<SDL_Renderer> SDL_CreateSharedRenderer(SDL_Window* window, int index, Uint32 flags) {
  return std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(window, index, flags), [](auto* p) { SDL_DestroyRenderer(p); });
}

std::shared_ptr<SDL_Texture> IMG_LoadSharedTexture(SDL_Renderer* renderer, const char* file) {
  return std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, file), [](auto* p) { SDL_DestroyTexture(p); });
}