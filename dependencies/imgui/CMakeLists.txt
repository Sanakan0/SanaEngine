project(imgui)
file(GLOB_RECURSE SRC *.cpp *.h)
add_library(imgui STATIC ${SRC})
target_include_directories(imgui PRIVATE ./include/imgui)
target_include_directories(imgui INTERFACE ./include)
target_link_libraries(imgui PUBLIC glfw)