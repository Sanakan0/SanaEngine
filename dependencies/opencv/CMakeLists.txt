add_library(opencv SHARED IMPORTED GLOBAL)
set_property(TARGET opencv PROPERTY 
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/x64/vc16/bin/opencv_world470.dll)
set_property(TARGET opencv PROPERTY
    IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/x64/vc16/lib/opencv_world470.lib)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/x64/vc16/bin/opencv_world470.dll
    DESTINATION ${PROJECT_BINARY_DIR}/Editor)

target_include_directories(opencv INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR}/include)