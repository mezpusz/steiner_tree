cmake_minimum_required (VERSION 3.6)

add_executable(steiner inputparser.cpp
                       inputparser.h
                       instance.h
                       main.cpp
                       solutionfinder.cpp
                       solutionfinder.h
                    )
include_directories("${PROJECT_SOURCE_DIR}")
