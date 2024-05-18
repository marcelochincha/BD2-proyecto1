cmake_minimum_required(VERSION 3.14)

project(DatabaseUISimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Incluye todos los archivos fuente de src y ui
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.hpp)
file(GLOB_RECURSE UI_FILES ui/*.cpp)

# Lista de archivos a ignorar (rutas completas)
set(IGNORE_FILES
    ${CMAKE_SOURCE_DIR}/src/Ext_Hash.cpp
    ${CMAKE_SOURCE_DIR}/src/Ext_Hash.hpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

# Filtrar los archivos ignorados
foreach(ignore_file ${IGNORE_FILES})
    list(REMOVE_ITEM SRC_FILES ${ignore_file})
endforeach()

add_executable(DatabaseUISimulation
    ${SRC_FILES}
    ${UI_FILES}
)

target_include_directories(DatabaseUISimulation PRIVATE src)

target_link_libraries(DatabaseUISimulation PRIVATE Qt6::Widgets)
