cmake_minimum_required (VERSION 3.6)

add_executable(steiner inputparser.cpp
                       inputparser.h
                       instance.h
                       main.cpp
                    )
include_directories("${PROJECT_SOURCE_DIR}")
#find_package(Threads)
#target_link_libraries(steiner ${CMAKE_THREAD_LIBS_INIT})
