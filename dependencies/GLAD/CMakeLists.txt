project(GLAD)
file(GLOB_RECURSE SRC *.c *.h)
add_library(GLAD STATIC ${SRC})
target_include_directories(GLAD PUBLIC ./include)