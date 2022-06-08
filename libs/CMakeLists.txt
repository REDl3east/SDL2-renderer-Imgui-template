include(FetchContent)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG 53dea9830964eee8b5c2a7ee0a65d6e268dc78a1
  SOURCE_DIR ${SDL2_SORUCE_DIR}
)

# TODO: Build static libs
FetchContent_Declare(
  SDL2_IMAGE
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG 97405e74e952f51b16c315ed5715b6b9de5a8a50
  SOURCE_DIR ${SDL2_IMAGE_SORUCE_DIR}
)

FetchContent_Declare(
  IMGUI
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG c71a50deb5ddf1ea386b91e60fa2e4a26d080074
  SOURCE_DIR ${IMGUI_SORUCE_DIR}
)

FetchContent_MakeAvailable(SDL2 SDL2_IMAGE IMGUI)

target_compile_options(jpeg PUBLIC -fPIC)

add_library(imgui
    ${IMGUI_SORUCE_DIR}/imgui.cpp
    ${IMGUI_SORUCE_DIR}/imgui_widgets.cpp
    ${IMGUI_SORUCE_DIR}/imgui_demo.cpp
    ${IMGUI_SORUCE_DIR}/imgui_draw.cpp
    ${IMGUI_SORUCE_DIR}/imgui_tables.cpp
    ${IMGUI_SORUCE_DIR}/backends/imgui_impl_sdl.cpp
    ${IMGUI_SORUCE_DIR}/backends/imgui_impl_sdlrenderer.cpp
)

target_include_directories(imgui PUBLIC ${IMGUI_SORUCE_DIR})
target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIR})