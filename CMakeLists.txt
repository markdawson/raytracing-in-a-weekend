cmake_minimum_required(VERSION 3.17)
project(raytracing_weekend)

set(CMAKE_CXX_STANDARD 17)

add_executable(raytracing_weekend main.cpp vec3.h color.h ray.h utils.h hittable.h)