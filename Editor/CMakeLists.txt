project(Editor)

file(GLOB_RECURSE SRC *.cpp *.h)
add_library(Editor STATIC ${SRC})
target_include_directories(Editor PUBLIC include)
target_link_libraries(Editor PUBLIC GUI Wnd Render GLAD imgui Core glfw assimp spdlog opencv nativefiledialog VisualLoc iconv)

