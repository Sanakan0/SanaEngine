project(SanaEngine)
set(CMAKE_CXX_STANDARD 17)

include_directories(dependencies/assimp/include)
include_directories(dependencies/opencv/include)

add_subdirectory(dependencies)

add_subdirectory(Wnd)
add_subdirectory(GUI)
add_subdirectory(Core)
add_subdirectory(Editor)
add_subdirectory(Render)

add_executable(SanaEngine main.cpp)
target_link_libraries(SanaEngine PUBLIC Editor)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

