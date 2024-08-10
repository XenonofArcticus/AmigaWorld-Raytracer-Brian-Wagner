cmake_minimum_required(VERSION 3.27)

project("tracer")

find_package(SDL2 CONFIG REQUIRED)

find_package(SPNG CONFIG REQUIRED)

set(SOURCE_FILES 
    free.c
    image.c
    load.c
    math.c
    platformstub.c
    tracer.c
    write.c
)

add_executable(tracer ${SOURCE_FILES})

target_compile_definitions(tracer
    PUBLIC
    _CRT_SECURE_NO_WARNINGS
    _CRT_DECLARE_NONSTDC_NAMES=0
    WINDOWED_UI
    OUTPUT_PNG
)

target_link_libraries(tracer
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

target_link_libraries(tracer PRIVATE $<IF:$<TARGET_EXISTS:spng::spng>,spng::spng,spng::spng_static>) 

