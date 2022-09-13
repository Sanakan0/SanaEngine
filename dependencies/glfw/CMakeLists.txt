add_library(glfw STATIC IMPORTED GLOBAL)
set_property(TARGET glfw PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib)
target_include_directories(glfw INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR}/include)