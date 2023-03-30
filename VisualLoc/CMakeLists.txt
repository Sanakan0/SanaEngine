project(VisualLoc)

file(GLOB_RECURSE SRC *.cpp *.h)
add_library(VisualLoc STATIC ${SRC})
target_include_directories(VisualLoc PUBLIC include)
target_link_libraries(VisualLoc PUBLIC opencv Render Core)
