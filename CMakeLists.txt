cmake_minimum_required(VERSION 3.10)
project(Wargame)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.h"
)

add_executable(Wargame ${SOURCES})

file(GLOB_RECURSE ALL_DIRS LIST_DIRECTORIES true "src/*")
foreach(dir ${ALL_DIRS})
    if(IS_DIRECTORY ${dir})
        target_include_directories(Wargame PRIVATE ${dir})
    endif()
endforeach()
target_include_directories(Wargame PRIVATE src)