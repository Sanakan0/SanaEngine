project(tinyxml2)
file(GLOB_RECURSE SRC *.cpp *.h)
add_library(tinyxml2 STATIC ${SRC})
target_include_directories(tinyxml2 PRIVATE ./include)
target_include_directories(tinyxml2 INTERFACE ./include)