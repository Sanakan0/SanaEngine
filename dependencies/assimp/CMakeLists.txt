
add_library(assimp SHARED IMPORTED GLOBAL)
set_property(TARGET assimp PROPERTY 
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/bin/assimp-vc142-mtd.dll)
set_property(TARGET assimp PROPERTY
    IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp-vc142-mtd.lib)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/assimp-vc142-mtd.dll 
    DESTINATION ${PROJECT_BINARY_DIR}/Editor)

target_include_directories(assimp INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR}/include)