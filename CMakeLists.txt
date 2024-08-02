cmake_minimum_required(VERSION 3.8)

set(SDL2_DIR "vcpkg_installed/x64-windows/share/sdl2")

find_package(SDL2 CONFIG REQUIRED)

project("tracer")

add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_DECLARE_NONSTDC_NAMES=0)

add_executable(tracer "free.c" "image.c" "load.c" "math.c" "platformstub.c" "tracer.c" "write.c")

target_link_libraries(tracer
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
