project(Render)

file(GLOB_RECURSE SRC *.cpp *.h)
add_library(Render STATIC ${SRC})
target_include_directories(Render PUBLIC include)
target_link_libraries(Render PUBLIC GLAD glm)
