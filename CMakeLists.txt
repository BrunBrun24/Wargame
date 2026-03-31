cmake_minimum_required(VERSION 3.10)
project(Wargame)

set(CMAKE_CXX_STANDARD 17)


find_package(OpenGL REQUIRED)
add_subdirectory(glfw-3.4) #Ajoute glfw 


file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.h"
)

set(SOURCES ${SOURCES} src/glad.c) # Ajoute glad.c?

add_executable(Wargame ${SOURCES})

target_include_directories(Wargame PRIVATE 
    src
    include
    glfw-3.4/include
)# Franchement aucune idée? pour ajouter en mode privé les 3 trucs?

file(GLOB_RECURSE ALL_DIRS LIST_DIRECTORIES true "src/*")
foreach(dir ${ALL_DIRS})
    if(IS_DIRECTORY ${dir})
        target_include_directories(Wargame PRIVATE ${dir})
    endif()
endforeach()
target_include_directories(Wargame PRIVATE src)


target_link_libraries(Wargame
    glfw
    OpenGL::GL
)# Link OpenGL et GLFW