project(Editor)
file(GLOB_RECURSE SRC *.cpp *.h)
add_executable(Editor ${SRC})
target_include_directories(Editor PRIVATE include)
target_link_libraries(Editor PUBLIC GUI Wnd Render GLAD imgui Core glfw assimp spdlog opencv)

