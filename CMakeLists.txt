cmake_minimum_required(VERSION 3.14)

project("SDL2 Renderer & Imgui Template" LANGUAGES CXX)

set(SDL2_SORUCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/SDL2)
set(SDL2_INCLUDE_DIR ${SDL2_SORUCE_DIR}/include)
set(IMGUI_SORUCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/imgui)
set(IMGUI_SORUCE_BACKEND_DIR ${IMGUI_SORUCE_DIR}/backends)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)

add_executable(sdlrenderer-imgui-template main.cpp)
target_link_libraries(sdlrenderer-imgui-template PRIVATE SDL2::SDL2 imgui)
target_include_directories(sdlrenderer-imgui-template PUBLIC ${SDL2_INCLUDE_DIR} ${IMGUI_SORUCE_DIR} ${IMGUI_SORUCE_BACKEND_DIR})