cmake_minimum_required(VERSION 3.10)
project(RayTracingDemos)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(RayTracingDemos
        common.cpp
        common.h
        main.cpp
        ppm.cpp
        ppm.h
        stdafx.h)
