cmake_minimum_required(VERSION 3.8)

project("tracer")

add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_DECLARE_NONSTDC_NAMES=0)

add_executable(tracer "free.c" "image.c" "load.c" "math.c" "platformstub.c" "tracer.c" "write.c")