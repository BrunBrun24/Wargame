cmake_minimum_required(VERSION 3.10)
project(Wargame)

# --- STANDARDS ET RIGUEUR ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuration des alertes (strict mode)
if(MSVC)
    # /W4 : Niveau d'alerte élevé
    # /permissive- : Force la conformité standard (détecte les headers manquants)
    add_compile_options(/W4 /permissive-)
else()
    # -Wall -Wextra : Alertes maximales sous Linux
    add_compile_options(-Wall -Wextra)
endif()

# --- DÉPENDANCES ---
find_package(OpenGL REQUIRED)

# On suppose que glfw-3.4 est un sous-dossier de ton projet
add_subdirectory(glfw-3.4) 

# --- SOURCES ---
# On récupère récursivement tous les .cpp et .h
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.h"
)

# On ajoute glad.c (qui est du C pur, c'est pour ça qu'il n'était pas dans SOURCES)
list(APPEND SOURCES "src/glad.c")

# Création de l'exécutable
add_executable(Wargame ${SOURCES})

# --- INCLUSIONS (Le mode PRIVATE) ---
# Le mode PRIVATE signifie que ces dossiers d'en-tête sont utilisés 
# uniquement pour compiler Wargame, ils ne sont pas "exportés" ailleurs.
target_include_directories(Wargame PRIVATE 
    src
    include
    glfw-3.4/include
)

# Ta boucle pour ajouter tous les sous-dossiers de src est une bonne idée
# pour éviter de faire des #include "game/player.h" et faire juste "player.h"
file(GLOB_RECURSE ALL_DIRS LIST_DIRECTORIES true "src/*")
foreach(dir ${ALL_DIRS})
    if(IS_DIRECTORY ${dir})
        target_include_directories(Wargame PRIVATE ${dir})
    endif()
endforeach()

# --- LIENS (LIBRARIES) ---
target_link_libraries(Wargame
    glfw
    OpenGL::GL
)