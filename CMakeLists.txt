cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

set(PROJECT_NAME learnOpenGl)
#set(CMAKE_CXX_STANDARD 20)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Renderer/ShaderProgram.h
        src/Renderer/Camera.h
        src/Renderer/Model.h
        src/Renderer/StaticModel.h
        src/stb_image.cpp
        src/stb_image.h
        src/assets/shaders/lamp.vs
        src/assets/shaders/lamp.fs
        src/assets/shaders/shader.vs
        src/assets/shaders/shader.fs
)

target_compile_definitions(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)


configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)

add_subdirectory(external/assimp)
target_link_libraries(${PROJECT_NAME} assimp)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)