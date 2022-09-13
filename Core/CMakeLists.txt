project(Core)

file(GLOB_RECURSE SRC *.cpp *.h)
add_library(Core STATIC ${SRC})
target_include_directories(Core PUBLIC include)

#target_link_libraries(Core PUBLIC Render)