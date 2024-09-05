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

include(CTest)
enable_testing()

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

# Define the command that generates the output file
add_test(NAME pyrs256k-generate
         COMMAND ${CMAKE_BINARY_DIR}/tracer ${CMAKE_SOURCE_DIR}/pyrs ${CMAKE_SOURCE_DIR}/pyrsopts 640 400
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
         )

# Compare testdata output and unverified recent output
add_test(NAME pyrs256k-compare
         COMMAND ${CMAKE_COMMAND} -E compare_files
         ${CMAKE_SOURCE_DIR}/pyrs.png
         ${CMAKE_SOURCE_DIR}/testdata/pyrs.png
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Ensure that the comparison test only runs after the output is generated
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS pyrs256k-generate pyrs256k-compare
)
