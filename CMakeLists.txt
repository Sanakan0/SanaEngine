project(SanaEngine)
set(CMAKE_CXX_STANDARD 17)



add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/imgui)
add_subdirectory(dependencies/GLAD)
add_subdirectory(dependencies/glm)
add_subdirectory(dependencies/assimp)

add_subdirectory(Wnd)
add_subdirectory(GUI)
add_subdirectory(Core)
add_subdirectory(Editor)
add_subdirectory(Render)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

