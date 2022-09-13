project(Editor)

file(GLOB_RECURSE SRC *.cpp *.h)
add_executable(Editor ${SRC})
target_include_directories(Editor PUBLIC include)
# target_include_directories(Editor PRIVATE ../Wnd/include)
# target_include_directories(Editor PRIVATE ../Render/include)
# target_include_directories(Editor PRIVATE ../GUI/include)
# target_include_directories(Editor PRIVATE ../Core/include)
target_link_libraries(Editor PUBLIC GUI Wnd Render GLAD imgui Core glfw)

