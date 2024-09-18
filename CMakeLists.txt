cmake_minimum_required(VERSION 3.27)

project("tracer")

find_package(SDL2 CONFIG REQUIRED)

find_package(SPNG CONFIG REQUIRED)

set(SOURCE_FILES 
    free.c
    image.c
    load.c
    math.c
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

# Define a list of source files to format
file(GLOB_RECURSE SOURCE_FILES 
    "${CMAKE_SOURCE_DIR}/*.c"
    "${CMAKE_SOURCE_DIR}/*.cpp"
    "${CMAKE_SOURCE_DIR}/*.h"
    "${CMAKE_SOURCE_DIR}/*.hpp"
)

# Set the uncrustify command
set(UNCRUSTIFY_COMMAND uncrustify -c ${CMAKE_SOURCE_DIR}/xen.cfg --replace)

# Create a custom target to format all source files
add_custom_target(uncrustify_format
    COMMAND ${UNCRUSTIFY_COMMAND} ${SOURCE_FILES}
    COMMENT ${UNCRUSTIFY_COMMAND}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Optionally, make uncrustify_format run before build
# add_dependencies(tracer uncrustify_format)


# Define the command that generates the output file
add_test(NAME pyrs256k-generate
         COMMAND ${CMAKE_BINARY_DIR}/tracer ${CMAKE_SOURCE_DIR}/pyrs ${CMAKE_SOURCE_DIR}/pyrsopts ${CMAKE_SOURCE_DIR}/pyrsopts 640 400 1
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
         )

# Compare testdata output and unverified recent output
add_test(NAME pyrs256k-compare
         COMMAND ${CMAKE_COMMAND} -E compare_files
         ${CMAKE_SOURCE_DIR}/pyrs.png
         ${CMAKE_SOURCE_DIR}/testdata/pyrs.png
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Define the command that generates the gingham output file
add_test(NAME pyrs256k_gingham-generate
         COMMAND ${CMAKE_BINARY_DIR}/tracer ${CMAKE_SOURCE_DIR}/pyrs ${CMAKE_SOURCE_DIR}/pyrsopts_gingham ${CMAKE_SOURCE_DIR}/pyrsopts_gingham 640 400 1
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
         )

         # Compare testdata gingham output and unverified recent output
add_test(NAME pyrs256k_gingham-compare
         COMMAND ${CMAKE_COMMAND} -E compare_files
         ${CMAKE_SOURCE_DIR}/pyrs.png
         ${CMAKE_SOURCE_DIR}/testdata/pyrs_gingham.png
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)


# Ensure that the comparison test only runs after the output is generated
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS pyrs256k-generate pyrs256k-compare
    # first generate and compare must be complete before starting second generate
    DEPENDS pyrs256k_gingham-generate pyrs256k_gingham-compare pyrs256k-generate pyrs256k-compare
)
