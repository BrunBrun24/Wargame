cmake_minimum_required(VERSION 3.10)
project(Wargame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

qt_add_executable(Wargame WIN32
    ${SOURCES}
    ${HEADERS}
)

set_target_properties(Wargame PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Ajout des dossiers d’inclusion
file(GLOB_RECURSE ALL_DIRS LIST_DIRECTORIES true "src/*")
foreach(dir ${ALL_DIRS})
    if(IS_DIRECTORY ${dir})
        target_include_directories(Wargame PRIVATE ${dir})
    endif()
endforeach()

target_link_libraries(Wargame PRIVATE
    Qt6::Widgets
    Qt6::OpenGLWidgets
    opengl32
)

# Ajoute le fichier de ressources
qt6_add_resources(RESOURCES resources/resources.qrc)

# Ajoute le fichier compilé à ton exécutable
target_sources(Wargame PRIVATE ${RESOURCES})